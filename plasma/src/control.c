/**
 *
 * @file control.c
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @date 2010-11-15
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "auxiliary.h"
#include "allocate.h"

/***************************************************************************//**
 *
 * @ingroup Auxiliary
 *
 *  PLASMA_Init - Initialize PLASMA.
 *
 *******************************************************************************
 *
 * @param[in] cores
 *          Number of cores to use (threads to launch).
 *          If cores = 0, cores = PLASMA_NUM_THREADS if it is set, the
 *          system number of core otherwise.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *
 ******************************************************************************/
int PLASMA_Init(int cores)
{
    plasma_context_insert(plasma_context_create());
    return PLASMA_SUCCESS;
}

/***************************************************************************//**
 *
 * @ingroup Auxiliary
 *
 *  PLASMA_Finalize - Finalize PLASMA.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *
 ******************************************************************************/
int PLASMA_Finalize()
{
    plasma_context_remove(plasma_context_self());
    return PLASMA_SUCCESS;
}
