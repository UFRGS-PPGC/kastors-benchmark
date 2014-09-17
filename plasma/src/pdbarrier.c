/**
 *
 * @file pdbarrier.c
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * Barrier for algorithm mixing computation on tile/panel.
 *
 * @version 2.6.0
 * @author Mathieu Faverge
 * @date 2009-11-15
 *
 * @generated d Tue Jan  7 11:45:13 2014
 *
 **/
#include "common.h"

#define A(m,n) BLKADDR(A, double, m, n)

/***************************************************************************//**
 *  Barrier from tiles to panels
 **/
void plasma_pdbarrier_tl2pnl_quark(PLASMA_desc A)
{
    int m, n;

    for (n = 0; n < A.nt; n++)
    {
        /* Protection from previous GATHERV */
        double *base = A(0,n);
        double *bloc;
#pragma omp task depend(inout:base[0:A.mb*A.nb])
        {
            ;
        }

        for (m = 1; m < A.mt; m++)
        {
            base = A(0,n);
            bloc = A(m,n);
#pragma omp task depend(inout:base[0:A.mb*A.nb], bloc[0:A.mb*A.nb])
            {
                ;
            }
        }

        /* Protection to next GATHERV */
        base = A(0,n);
#pragma omp task depend(inout:base[0:A.mb*A.nb])
        {
            ;
        }
    }
}

/***************************************************************************//**
 *  Barrier from panels to tiles
 **/
void plasma_pdbarrier_pnl2tl_quark(PLASMA_desc A)
{
    int m, n;

    for (n = 0; n < A.nt; n++)
    {
        /* Protection from previous GATHERV */
        double *base = A(0,n);
        double *bloc;
#pragma omp task depend(inout:base[0:A.mb*A.nb])
        {
            ;
        }

        for (m = 1; m < A.mt; m++)
        {
            base = A(0,n);
            bloc = A(m,n);
#pragma omp task depend(in:base[0:A.mb*A.nb]) depend(inout:bloc[0:A.mb*A.nb])
            {
                ;
            }
        }
    }
}
