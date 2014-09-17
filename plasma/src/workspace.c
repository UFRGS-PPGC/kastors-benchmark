/**
 *
 * @file workspace.c
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
#include "common.h"
#include "allocate.h"
#include "auxiliary.h"
#include "workspace.h"

#include <stdlib.h>


/***************************************************************************//**
 *
 **/
int plasma_alloc_ibnb_tile(int M, int N, PLASMA_enum func, int type, PLASMA_desc **desc)
{
    int status;
    int IB, NB, MT, NT;
    plasma_context_t *plasma;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("plasma_alloc_ibnb_tile", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }

    /* Set MT & NT & allocate */
    NB = PLASMA_NB;
    IB = PLASMA_IB;
    MT = (M%NB==0) ? (M/NB) : (M/NB+1);
    NT = (N%NB==0) ? (N/NB) : (N/NB+1);

    /* Allocate and initialize descriptor */
    *desc = (PLASMA_desc*)malloc(sizeof(PLASMA_desc));
    if (*desc == NULL) {
        plasma_error("plasma_alloc_ibnb_tile", "malloc() failed");
        return PLASMA_ERR_OUT_OF_RESOURCES;
    }
    **desc = plasma_desc_init(type, IB, NB, IB*NB, MT*IB, NT*NB, 0, 0, MT*IB, NT*NB);

    /* Allocate matrix */
    if (plasma_desc_mat_alloc(*desc)) {
        plasma_error("plasma_alloc_ibnb_tile", "malloc() failed");
        return PLASMA_ERR_OUT_OF_RESOURCES;
    }

    return PLASMA_SUCCESS;
}

/***************************************************************************//**
 *
 * @ingroup Auxiliary
 *
 *  PLASMA_Dealloc_Handle_Tile - Deallocate Tile workspace handle allocated by any tile workspace allocation routine.
 *
 *******************************************************************************
 *
 * @param[in] desc
 *          Descriptot handle
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *
 ******************************************************************************/
int PLASMA_Dealloc_Handle_Tile(PLASMA_desc **desc)
{
    free((*desc)->mat);
    free(*desc);
    *desc = NULL;
    return PLASMA_SUCCESS;
}
