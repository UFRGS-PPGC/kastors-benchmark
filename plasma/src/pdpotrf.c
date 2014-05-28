/**
 *
 * @file pdpotrf.c
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @author Hatem Ltaief
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @generated d Tue Jan  7 11:45:10 2014
 *
 **/
#include "common.h"
#include <lapacke.h>

#define A(m,n) BLKADDR(A, double, m, n)

/***************************************************************************//**
 *  Parallel tile Cholesky factorization - dynamic scheduling
 **/
void plasma_pdpotrf_quark(PLASMA_enum uplo, PLASMA_desc A,
                          PLASMA_sequence *sequence, PLASMA_request *request)
{
    plasma_context_t *plasma;

    int k, m, n;
    int ldak, ldam;
    int tempkm, tempmm;

    double zone  = (double) 1.0;
    double mzone = (double)-1.0;

    plasma = plasma_context_self();
    if (sequence->status != PLASMA_SUCCESS)
        return;
    /*
     *  PlasmaLower
     */
    if (uplo == PlasmaLower) {
        for (k = 0; k < A.mt; k++) {
            tempkm = k == A.mt-1 ? A.m-k*A.mb : A.mb;
            ldak = BLKLDD(A, k);
            double *dA = A(k, k);
#pragma omp task depend(inout:dA[0:A.mb*A.mb])
    {
    LAPACKE_dpotrf_work(LAPACK_COL_MAJOR, lapack_const(PlasmaLower), tempkm, dA, ldak);
    }

            for (m = k+1; m < A.mt; m++) {
                tempmm = m == A.mt-1 ? A.m-m*A.mb : A.mb;
                ldam = BLKLDD(A, m);
                double *dA = A(k, k);
                double *dB = A(m, k);
#pragma omp task depend(in:dA[0:A.mb*A.mb]) depend(inout:dB[0:A.mb*A.mb])
    cblas_dtrsm(
        CblasColMajor,
        (CBLAS_SIDE)PlasmaRight, (CBLAS_UPLO)PlasmaLower,
        (CBLAS_TRANSPOSE)PlasmaTrans, (CBLAS_DIAG)PlasmaNonUnit,
        tempmm, A.mb,
        zone, dA, ldak,
        dB, ldam);
            }
            for (m = k+1; m < A.mt; m++) {
                tempmm = m == A.mt-1 ? A.m-m*A.mb : A.mb;
                ldam = BLKLDD(A, m);
                double *dA = A(m, k);
                double *dB = A(m, m);
#pragma omp task depend(in:dA[0:A.mb*A.mb]) depend(inout:dB[0:A.mb*A.mb])
    {
    cblas_dsyrk(
        CblasColMajor,
        (CBLAS_UPLO)PlasmaLower, (CBLAS_TRANSPOSE)PlasmaNoTrans,
        tempmm, A.mb,
        (-1.0), dA, ldam,
        (1.0), dB, ldam);
    }
                for (n = k+1; n < m; n++) {
                    double *dA = A(m , k);
                    double *dB = A(n , k);
                    double *dC = A(m , n);
#pragma omp task depend(in:dA[0:A.mb*A.mb], dB[0:A.mb*A.mb]) depend(inout:dC[0:A.mb*A.mb])
    cblas_dgemm(CblasColMajor, (CBLAS_TRANSPOSE)PlasmaNoTrans, (CBLAS_TRANSPOSE)PlasmaTrans,
        tempmm, A.mb, A.mb,
        mzone, dA, ldam,
        dB, A.mb,
        zone, dC, ldam);
                }
            }
        }
    }
    /*
     *  PlasmaUpper
     */
    else {
        for (k = 0; k < A.nt; k++) {
            tempkm = k == A.nt-1 ? A.n-k*A.nb : A.nb;
            ldak = BLKLDD(A, k);
            double *dA = A(k, k);
#pragma omp task depend(inout:dA[0:A.mb*A.mb])
    {
    LAPACKE_dpotrf_work(LAPACK_COL_MAJOR, lapack_const(PlasmaUpper), tempkm, dA, ldak);
    }

            for (m = k+1; m < A.nt; m++) {
                tempmm = m == A.nt-1 ? A.n-m*A.nb : A.nb;
                double *dA = A(k, k);
                double *dB = A(k, m);
#pragma omp task depend(in:dA[0:A.mb*A.mb]) depend(inout:dB[0:A.mb*A.mb])
    cblas_dtrsm(
        CblasColMajor,
        (CBLAS_SIDE)PlasmaLeft, (CBLAS_UPLO)PlasmaUpper,
        (CBLAS_TRANSPOSE)PlasmaTrans, (CBLAS_DIAG)PlasmaNonUnit,
        A.mb, tempmm,
        zone, dA, ldak,
        dB, ldak);
            }
            for (m = k+1; m < A.nt; m++) {
                tempmm = m == A.nt-1 ? A.n-m*A.nb : A.nb;
                ldam = BLKLDD(A, m);
                double *dA = A(k, m);
                double *dB = A(m, m);
#pragma omp task depend(in:dA[0:A.mb*A.mb]) depend(inout:dB[0:A.mb*A.mb])
    {
    cblas_dsyrk(
        CblasColMajor,
        (CBLAS_UPLO)PlasmaUpper, (CBLAS_TRANSPOSE)PlasmaTrans,
        tempmm, A.mb,
        (-1.0), dA, ldak,
        (1.0), dB, ldam);
    }

                for (n = k+1; n < m; n++) {
                    double *dA = A(k , n);
                    double *dB = A(k , m);
                    double *dC = A(n , m);
#pragma omp task depend(in:dA[0:A.mb*A.mb], dB[0:A.mb*A.mb]) depend(inout:dC[0:A.mb*A.mb])
    cblas_dgemm(CblasColMajor, (CBLAS_TRANSPOSE)PlasmaTrans, (CBLAS_TRANSPOSE)PlasmaNoTrans,
        A.mb, tempmm, A.mb,
        mzone, dA, ldak,
        dB, ldak,
        zone, dC, A.mb);
                }
            }
        }
    }
}
