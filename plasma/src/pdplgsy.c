/**
 *
 * @file pdplgsy.c
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

#define A(m,n) BLKADDR(A, double, m, n)

/***************************************************************************//**
 *  Parallel tile Cholesky factorization - dynamic scheduling
 **/
void plasma_pdplgsy_quark( double bump, PLASMA_desc A, unsigned long long int seed)
{
#if defined(_OPENMP)
    int seed_randr = omp_get_thread_num();
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
#if defined(USE_OMPEXT)
int node_row = (int)(m / 2);
int node_col = (int)(n / 2);
omp_set_task_affinity( (node_col%4)*6+(node_row%6), 1 );
#endif
#pragma omp task depend(out:dA[0:ldam*tempnn])
            CORE_dplgsy( bump, tempmm, tempnn, dA, ldam, A.m, m*A.mb, n*A.nb, seed );
        }
    }
}
