/**
 *
 * @file pdpltmg.c
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @generated d Tue Jan  7 11:45:12 2014
 *
 **/
#include "common.h"
#if defined(USE_OMPEXT)
#include <omp_ext.h>
#include "omp.h"
#include <stdlib.h>
#endif

#define A(m, n) BLKADDR(A, double, m, n)

/***************************************************************************//**
 *  Parallel tile matrix generation - dynamic scheduling
 **/
void plasma_pdpltmg_quark(PLASMA_desc A, unsigned long long int seed)
{
#if defined(_OPENMP)
    int cnt= 0;
#endif
    int m, n;
    int ldam;
    int tempmm, tempnn;

    for (m = 0; m < A.mt; m++) {
        tempmm = m == A.mt-1 ? A.m-m*A.mb : A.mb;
        ldam = BLKLDD(A, m);

        for (n = 0; n < A.nt; n++) {
            tempnn = n == A.nt-1 ? A.n-n*A.nb : A.nb;
            double *dA = A(m, n);
#pragma omp task depend(out:dA[0:tempnn*ldam]) affinity( node: cnt++, 1)
            CORE_dplrnt(tempmm, tempnn, dA, ldam, A.m, m*A.mb, n*A.nb, seed);
        }
    }
}

/***************************************************************************//**
 *  Parallel tile matrix generation - dynamic scheduling
 *  Same as previous function, without OpenMP pragma. Used to check solution.
 **/
void plasma_pdpltmg_seq(PLASMA_desc A, unsigned long long int seed)
{
    int m, n;
    int ldam;
    int tempmm, tempnn;

    for (m = 0; m < A.mt; m++) {
        tempmm = m == A.mt-1 ? A.m-m*A.mb : A.mb;
        ldam = BLKLDD(A, m);

        for (n = 0; n < A.nt; n++) {
            tempnn = n == A.nt-1 ? A.n-n*A.nb : A.nb;
            double *dA = A(m, n);
            CORE_dplrnt(tempmm, tempnn, dA, ldam, A.m, m*A.mb, n*A.nb, seed);
        }
    }
}

