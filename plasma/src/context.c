/**
 *
 * @file context.c
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
#include <stdlib.h>

#include "common.h"
#include "auxiliary.h"
#include "context.h"

/***************************************************************************//**
 *  Global data
 **/
/* master threads context lookup table */
plasma_context_t* context_map[CONTEXTS_MAX];

/***************************************************************************//**
 *  Create new context
 **/
plasma_context_t *plasma_context_create()
{
    plasma_context_t *plasma;

    plasma = (plasma_context_t*)malloc(sizeof(plasma_context_t));
    if (plasma == NULL) {
        plasma_fatal_error("plasma_context_create", "malloc() failed");
        return NULL;
    }

    plasma->action = PLASMA_ACT_STAND_BY;
    plasma->parallel_func_ptr = NULL;

    plasma->errors_enabled     = PLASMA_TRUE;
    plasma->warnings_enabled   = PLASMA_TRUE;
    plasma->dynamic_section    = PLASMA_FALSE;

    return plasma;
}

/***************************************************************************//**
 *  Insert a (context, thread_id) tuple in the context map
 **/
int plasma_context_insert(plasma_context_t *context)
{
    context_map[0] = context;
    return PLASMA_SUCCESS;
}

/***************************************************************************//**
 *  Remove a (context, thread_id) tuple from the context map
 **/
int plasma_context_remove(plasma_context_t *context)
{
    free(context_map[0]);
    context_map[0] = NULL;
    return PLASMA_SUCCESS;
}

/***************************************************************************//**
 *  Return context for a thread
 **/
plasma_context_t *plasma_context_self()
{
    return context_map[0];
}

/***************************************************************************//**
 *
 * @ingroup Auxiliary
 *
 *  PLASMA_Set - Set PLASMA parameter.
 *
 *******************************************************************************
 *
 * @param[in] param
 *          Feature to be enabled:
 *          @arg PLASMA_TILE_SIZE:        size matrix tile,
 *          @arg PLASMA_INNER_BLOCK_SIZE: size of tile inner block,
 *          @arg PLASMA_INNER_BANDWIDTH:  matrix bandwidth size
 *          @arg PLASMA_TNTPIVOTING_SIZE: size of the groups in tournament pivoting
 *
 * @param[in] value
 *          Value of the parameter.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *
 ******************************************************************************/
int PLASMA_Set(PLASMA_enum param, int value)
{
    plasma_context_t *plasma;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_error("PLASMA_Set", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    switch (param) {
        case PLASMA_TILE_SIZE:
            if (value <= 0) {
                plasma_error("PLASMA_Set", "negative tile size");
                return PLASMA_ERR_ILLEGAL_VALUE;
            }
            plasma->nb = value;
            /* Calculate A, B tile size and round up to cache line size */
            /* round up for the smallest type (float) - will hold for all */
            plasma->nbnbsize = plasma->nb * plasma->nb; // * sizeof(float);
            plasma->ibnbsize = plasma->ib * plasma->nb; // * sizeof(float);
//          plasma->nbnbsize = roundup(plasma->nbnbsize, CACHE_LINE_SIZE);
//          plasma->nbnbsize /= sizeof(float);
            /* Limit ib to nb */
            plasma->ib = min( plasma->nb, plasma->ib );
            break;
        case PLASMA_INNER_BLOCK_SIZE:
            if (value <= 0) {
                plasma_error("PLASMA_Set", "negative inner block size");
                return PLASMA_ERR_ILLEGAL_VALUE;
            }
            if (value > plasma->nb) {
                plasma_error("PLASMA_Set", "inner block larger than tile");
                return PLASMA_ERR_ILLEGAL_VALUE;
            }
            /* if (plasma->nb % value != 0) { */
            /*     plasma_error("PLASMA_Set", "inner block does not divide tile"); */
            /*     return PLASMA_ERR_ILLEGAL_VALUE; */
            /* } */
            plasma->ib = value;
            /* Calculate T, L tile size and round up to cache line size */
            /* round up for the smallest type (float) - will hold for all */
            plasma->ibnbsize = plasma->ib * plasma->nb; // * sizeof(float);
//          plasma->ibnbsize = roundup(plasma->ibnbsize, CACHE_LINE_SIZE);
//          plasma->ibnbsize /= sizeof(float);

            break;
        case PLASMA_TNTPIVOTING_SIZE:
            if (value <= 0) {
                plasma_error("PLASMA_Set", "negative tournament pivoting size");
                return PLASMA_ERR_ILLEGAL_VALUE;
            }
            plasma->tntsize = value;
            break;
        default:
            plasma_error("PLASMA_Set", "unknown parameter");
            return PLASMA_ERR_ILLEGAL_VALUE;
    }
    return PLASMA_SUCCESS;
}

/***************************************************************************//**
 *
 * @ingroup Auxiliary
 *
 *  PLASMA_Get - Get value of PLASMA parameter.
 *
 *******************************************************************************
 *
 * @param[in] param
 *          Feature to be enabled:
 *          @arg PLASMA_TILE_SIZE:        size matrix tile,
 *          @arg PLASMA_INNER_BLOCK_SIZE: size of tile inner block,
 *          @arg PLASMA_TNTPIVOTING_SIZE: size of the groups in tournament pivoting
 *
 * @param[out] value
 *          Value of the parameter.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *
 ******************************************************************************/
int PLASMA_Get(PLASMA_enum param, int *value)
{
    plasma_context_t *plasma;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_Get", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    switch (param) {
        case PLASMA_TILE_SIZE:
            *value = plasma->nb;
            return PLASMA_SUCCESS;
        case PLASMA_INNER_BLOCK_SIZE:
            *value = plasma->ib;
            return PLASMA_SUCCESS;
        case PLASMA_TNTPIVOTING_SIZE:
            *value = plasma->tntsize;
            return PLASMA_SUCCESS;
        default:
            plasma_error("PLASMA_Get", "unknown parameter");
            return PLASMA_ERR_ILLEGAL_VALUE;
    }
}
