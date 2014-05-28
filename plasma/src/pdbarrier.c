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
void plasma_pdbarrier_tl2pnl_quark(PLASMA_desc A, PLASMA_sequence *sequence, PLASMA_request *request)
{
    int m, n;
    plasma_context_t *plasma;
    /*Quark_Task_Flags task_flags = Quark_Task_Flags_Initializer;*/

    plasma = plasma_context_self();
    if (sequence->status != PLASMA_SUCCESS)
        return;
    /*QUARK_Task_Flag_Set(&task_flags, TASK_SEQUENCE, (intptr_t)sequence->quark_sequence);*/

    for (n = 0; n < A.nt; n++)
    {
        /* Protection from previous GATHERV */
#if 0
        QUARK_Insert_Task(plasma->quark, CORE_foo_quark, &task_flags,
                          sizeof(double)*A.mb*A.nb, A(0, n), INOUT,
                          0);
#endif
        double *base = A(0,n);
        double *bloc;
#pragma omp task depend(inout:base[0:A.mb*A.nb])
        {
            ;
        /*CORE_foo_quark(plasma->quark);*/
        }

        for (m = 1; m < A.mt; m++)
        {
#if 0
            QUARK_Insert_Task(plasma->quark, CORE_foo2_quark, &task_flags,
                              sizeof(double)*A.mb*A.nb, A(0, n), INOUT | GATHERV,
                              sizeof(double)*A.mb*A.nb, A(m, n), INOUT,
                              0);
#endif
            base = A(0,n);
            bloc = A(m,n);
#pragma omp task depend(inout:base[0:A.mb*A.nb], bloc[0:A.mb*A.nb])
            {
                ;
        /*CORE_foo_quark(plasma->quark);*/
            }
        }

        /* Protection to next GATHERV */
#if 0
        QUARK_Insert_Task(plasma->quark, CORE_foo_quark, &task_flags,
                          sizeof(double)*A.mb*A.nb, A(0, n), INOUT,
                          0);
#endif
        base = A(0,n);
#pragma omp task depend(inout:base[0:A.mb*A.nb])
        {
        /*CORE_foo_quark(plasma->quark);*/
            ;
        }
    }
}

/***************************************************************************//**
 *  Barrier from panels to tiles
 **/
void plasma_pdbarrier_pnl2tl_quark(PLASMA_desc A, PLASMA_sequence *sequence, PLASMA_request *request)
{
    int m, n;
    plasma_context_t *plasma;
    /*Quark_Task_Flags task_flags = Quark_Task_Flags_Initializer;*/

    plasma = plasma_context_self();
    if (sequence->status != PLASMA_SUCCESS)
        return;
    /*QUARK_Task_Flag_Set(&task_flags, TASK_SEQUENCE, (intptr_t)sequence->quark_sequence);*/

    for (n = 0; n < A.nt; n++)
    {
        /* Protection from previous GATHERV */
#if 0
        QUARK_Insert_Task(plasma->quark, CORE_foo_quark, &task_flags,
                          sizeof(double)*A.mb*A.nb, A(0, n), INOUT,
                          0);
#endif
        double *base = A(0,n);
        double *bloc;
#pragma omp task depend(inout:base[0:A.mb*A.nb])
        {
            ;
        /*CORE_foo_quark(plasma->quark);*/
        }

        for (m = 1; m < A.mt; m++)
        {
            base = A(0,n);
            bloc = A(m,n);
#if 0
            QUARK_Insert_Task(plasma->quark, CORE_foo2_quark, &task_flags,
                              sizeof(double)*A.mb*A.nb, A(0, n), INPUT,
                              sizeof(double)*A.mb*A.nb, A(m, n), INOUT,
                              0);
#endif
#pragma omp task depend(in:base[0:A.mb*A.nb]) depend(inout:bloc[0:A.mb*A.nb])
            {
                ;
                /*CORE_foo_quark(plasma->quark);*/
            }
        }
    }
}

/***************************************************************************//**
 *  Barrier from tiles to panels
 **/
void plasma_pdbarrier_tl2row_quark(PLASMA_desc A, PLASMA_sequence *sequence, PLASMA_request *request)
{
    fprintf(stderr, "Warning, you are not using the actual OpenMP plasma version\n");
    abort();
#if 0
    int m, n;
    plasma_context_t *plasma;
    Quark_Task_Flags task_flags = Quark_Task_Flags_Initializer;

    plasma = plasma_context_self();
    if (sequence->status != PLASMA_SUCCESS)
        return;
    QUARK_Task_Flag_Set(&task_flags, TASK_SEQUENCE, (intptr_t)sequence->quark_sequence);
 
    for (m = 0; m < A.mt; m++)
    {
        /* Protection from previous GATHERV */
        QUARK_Insert_Task(plasma->quark, CORE_foo_quark, &task_flags,
                          sizeof(double)*A.mb*A.nb, A(m, 0), INOUT,
                          0);

        for (n = 1; n < A.nt; n++)
        {
            QUARK_Insert_Task(plasma->quark, CORE_foo2_quark, &task_flags,
                              sizeof(double)*A.mb*A.nb, A(m, 0), INOUT | GATHERV,
                              sizeof(double)*A.mb*A.nb, A(m, n), INOUT,
                              0);
        }

        /* Protection to next GATHERV */
        QUARK_Insert_Task(plasma->quark, CORE_foo_quark, &task_flags,
                          sizeof(double)*A.mb*A.nb, A(m, 0), INOUT,
                          0);
    }
#endif
}

/***************************************************************************//**
 *  Barrier from panels to tiles
 **/
void plasma_pdbarrier_row2tl_quark(PLASMA_desc A, PLASMA_sequence *sequence, PLASMA_request *request)
{
    fprintf(stderr, "Warning, you are not using the actual OpenMP plasma version\n");
    abort();
#if 0
    int m, n;
    plasma_context_t *plasma;
    Quark_Task_Flags task_flags = Quark_Task_Flags_Initializer;

    plasma = plasma_context_self();
    if (sequence->status != PLASMA_SUCCESS)
        return;
    QUARK_Task_Flag_Set(&task_flags, TASK_SEQUENCE, (intptr_t)sequence->quark_sequence);
 
    for (m = 0; m < A.mt; m++)
    {
        /* Protection from previous GATHERV */
        QUARK_Insert_Task(plasma->quark, CORE_foo_quark, &task_flags,
                          sizeof(double)*A.mb*A.nb, A(m, 0), INOUT,
                          0);

        for (n = 1; n < A.nt; n++)
        {
            QUARK_Insert_Task(plasma->quark, CORE_foo2_quark, &task_flags,
                              sizeof(double)*A.mb*A.nb, A(m, 0), INPUT,
                              sizeof(double)*A.mb*A.nb, A(m, n), INOUT,
                              0);
        }
    }
#endif
}

