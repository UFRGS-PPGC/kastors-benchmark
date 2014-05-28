/**
 *
 * @file pdgetrf_rectil.c
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * LU with Partial pivoting.
 *
 * @version 2.6.0
 * @author Mathieu Faverge
 * @author Hatem Ltaief
 * @date 2009-11-15
 *
 * @generated d Tue Jan  7 11:45:13 2014
 *
 **/
#include "common.h"

void CORE_dgetrf_rectil_init(void);

#define PARALLEL_KERNEL
#define A(m,n) BLKADDR(A, double, m, n)
#define IPIV(k) &(IPIV[(int64_t)A.mb*(int64_t)(k)])

#define plasma_pdgetrf_rectil_rl_quark plasma_pdgetrf_rectil_quark

/***************************************************************************//**
 *  Parallel tile LU factorization - dynamic scheduling - Right looking
 **/
void plasma_pdgetrf_rectil_rl_quark(PLASMA_desc A, int *IPIV, PLASMA_sequence *sequence, PLASMA_request *request)
{
    int k, m, n;
    plasma_context_t *plasma;
    int tempk, tempm, tempkm, tempkn, tempmm, tempnn;
    int ldak, ldam;
    /*Quark_Task_Flags task_flagsP = Quark_Task_Flags_Initializer;*/
    /*Quark_Task_Flags task_flagsU = Quark_Task_Flags_Initializer;*/

    double zone  = (double)1.0;
    double mzone = (double)-1.0;

    void * fakedep;
     /* How many threads per panel? Probably needs to be adjusted during factorization. */
    int panel_thread_count;

    plasma = plasma_context_self();
    if (sequence->status != PLASMA_SUCCESS)
        return;
    /*QUARK_Task_Flag_Set(&task_flagsP, TASK_SEQUENCE, (intptr_t)sequence->quark_sequence);*/
    /*QUARK_Task_Flag_Set(&task_flagsU, TASK_SEQUENCE, (intptr_t)sequence->quark_sequence);*/

    /* We start at PLASMA_SIZE-1, to keep the first thread adding task to the queue */
    panel_thread_count = min( max(PLASMA_SIZE, 2)-1, 48); /* kernel doesn't accept more than 48 cores */

    /*QUARK_Task_Flag_Set(&task_flagsP, TASK_THREAD_COUNT, panel_thread_count );*/

    CORE_dgetrf_rectil_init();

    for (k = 0; k < min(A.mt, A.nt); k++)
    {
        tempk  = k * A.mb;
        tempm  = A.m - tempk;
        tempkm = k == A.mt-1 ? tempm      : A.mb;
        tempkn = k == A.nt-1 ? A.n-k*A.nb : A.nb;
        ldak = BLKLDD(A, k);

        /*QUARK_Task_Flag_Set(&task_flagsU, TASK_PRIORITY, QUARK_TASK_MAX_PRIORITY - k );*/

        while ( ((panel_thread_count * 4 * A.mb) > tempm)
                && (panel_thread_count > 1) ) {
          panel_thread_count--;
          /*QUARK_Task_Flag_Set(&task_flagsP, TASK_THREAD_COUNT, panel_thread_count );*/
        }

        /*QUARK_Task_Flag_Set(&task_flagsP, TASK_PRIORITY, QUARK_TASK_MAX_PRIORITY - k );*/
        double *dA = A(k, k);
        int *dB = IPIV(k);
        PLASMA_desc pDesc = plasma_desc_submatrix(A, tempk, k*A.nb, tempm, tempkn);
#pragma omp task depend(inout:dA[0:A.mb*A.nb]) depend(out:dB[0:pDesc.n])
        {
            int info[3];
            info[1] = 0;
            info[2] = 1;

            CORE_dgetrf_rectil( pDesc, dB, info );
        }
        /*
         *QUARK_CORE_dgetrf_rectil(
         *    plasma->quark, &task_flagsP,
         *    plasma_desc_submatrix(A, tempk, k*A.nb, tempm, tempkn),
         *    A(k, k), A.mb*A.nb, IPIV(k),
         *    sequence, request, 1, tempk,
         *    panel_thread_count );
         */

        /*
         * Update the trailing submatrix
         */
        fakedep = (void *)(intptr_t)(k+1);
        for (n = k+1; n < A.nt; n++)
        {
            /*QUARK_Task_Flag_Set(&task_flagsU, TASK_PRIORITY, QUARK_TASK_MAX_PRIORITY - n );*/
            /*
             * Apply row interchange after the panel (work on the panel)
             */
            tempnn = n == A.nt-1 ? A.n-n*A.nb : A.nb;
            PLASMA_desc descA = plasma_desc_submatrix(A, tempk, n*A.nb, tempm, tempnn);
            double *dA = A(k, n);
            double *dB = A(k, k);
            int *dipiv = IPIV(k);
#pragma omp task depend(inout:dA[0:1]) depend(in:dB[0:ldak], dipiv[0:tempkm])
    CORE_dswptr_ontile(descA, 1, tempkm, dipiv, 1, dB, ldak);
            /*
             *QUARK_CORE_dswptr_ontile(
             *    plasma->quark, &task_flagsU,
             *    plasma_desc_submatrix(A, tempk, n*A.nb, tempm, tempnn),
             *    A(k, n), 1, tempkm, IPIV(k), 1,
             *    A(k, k), ldak);
             */

            m = k+1;
            if ( m < A.mt ) {
                tempmm = m == A.mt-1 ? A.m-m*A.mb : A.mb;
                ldam = BLKLDD(A, m);

                    double *dA = A(m , k);
                    double *dB = A(k , n);
                    double *dC = A(m , n);
#pragma omp task depend(in:dA[0:A.mb*A.mb], dB[0:A.mb*A.mb]) depend(inout:dC[0:A.mb*A.mb])
    cblas_dgemm(CblasColMajor, (CBLAS_TRANSPOSE)PlasmaNoTrans, (CBLAS_TRANSPOSE)PlasmaNoTrans,
        tempmm, tempnn, A.nb,
        mzone, dA, ldam,
        dB, ldak,
        zone, dC, ldam);
                /*
                 *QUARK_CORE_dgemm2(
                 *    plasma->quark, &task_flagsU,
                 *    PlasmaNoTrans, PlasmaNoTrans,
                 *    tempmm, tempnn, A.nb, A.mb,
                 *    mzone, A(m, k), ldam,
                 *           A(k, n), ldak,
                 *    zone,  A(m, n), ldam);
                 */

                for (m = k+2; m < A.mt; m++)
                {
                    tempmm = m == A.mt-1 ? A.m-m*A.mb : A.mb;
                    ldam = BLKLDD(A, m);

                    double *dA = A(m , k);
                    double *dB = A(k , n);
                    double *dC = A(m , n);
                    double *fake1 = A(k+1, n);
                    double *fake2 = (double *)fakedep;
#pragma omp task depend(in:dA[0:A.mb*A.mb], dB[0:A.mb*A.mb], fake2[0:1]) depend(inout:dC[0:A.mb*A.mb], fake1[0:A.mb*A.nb])
        {
    cblas_dgemm(CblasColMajor, (CBLAS_TRANSPOSE)PlasmaNoTrans, (CBLAS_TRANSPOSE)PlasmaNoTrans,
        tempmm, tempnn, A.nb,
        mzone, dA, ldam,
        dB, ldak,
        zone, dC, ldam);
        }
                    /*
                     *QUARK_CORE_dgemm_f2(
                     *    plasma->quark, &task_flagsU,
                     *    PlasmaNoTrans, PlasmaNoTrans,
                     *    tempmm, tempnn, A.nb, A.mb,
                     *    mzone, A(m, k), ldam,
                     *           A(k, n), ldak,
                     *    zone,  A(m, n), ldam,
                     *    [> Dependency on next swapa (gemm need to be done before) <]
                     *    A(k+1, n), A.mb*A.nb, INOUT | GATHERV,
                     *    [> Dependency on next swapb (gemm need to use panel k before it has to be swaped <]
                     *    fakedep,   1,         INPUT );
                     */
                }
            }
        }
    }

    /*QUARK_Task_Flag_Set(&task_flagsU, TASK_PRIORITY, QUARK_TASK_MIN_PRIORITY );*/
    for (k = 0; k < min(A.mt, A.nt); k++)
    {
        int mintmp;
        tempk  = k * A.mb;
        tempm  = A.m - tempk;
        tempkm = k == A.mt-1 ? tempm : A.mb;
        tempkn = k == A.nt-1 ? A.n - k * A.nb : A.nb;
        mintmp = min(tempkm, tempkn);
        ldak = BLKLDD(A, k);

        /*
         * Apply row interchange behind the panel (work on the panel)
         */
        fakedep = (void*)(intptr_t)k;
        for (n = 0; n < k; n++)
        {
            tempnn = n == A.nt-1 ? A.n-n*A.nb : A.nb;
            double *Aij = A(k, n);
            double *prevSwap = A(k-1, n);
            int *dipiv = IPIV(k);
            PLASMA_desc descA = plasma_desc_submatrix(A, tempk, n*A.nb, tempm, tempnn);
#pragma omp task depend(inout:Aij[0:1],fakedep) depend(in:dipiv[0:mintmp], prevSwap[0:A.lm*A.nb])
        CORE_dlaswp_ontile(descA, 1, mintmp, dipiv, 1);
            /*
             *QUARK_CORE_dlaswp_ontile_f2(
             *    plasma->quark, &task_flagsU,
             *    plasma_desc_submatrix(A, tempk, n*A.nb, tempm, tempnn),
             *    A(k, n), 1, mintmp, IPIV(k), 1,
             *    [> Dependency on previous swapb <]
             *    A(k-1,n), A.lm*A.nb, INPUT,
             *    [> Dependency on all GEMM from previous step <]
             *    fakedep,  1,         INOUT | GATHERV );
             */
        }
    }
}

/***************************************************************************//**
 *  Parallel tile LU factorization - dynamic scheduling - Left looking
 **/
void plasma_pdgetrf_tile_ll_quark(PLASMA_desc A, int *IPIV, PLASMA_sequence *sequence, PLASMA_request *request)
{
    fprintf(stderr, "Warning, you are not using the actual OpenMP plasma version\n");
    abort();
}
