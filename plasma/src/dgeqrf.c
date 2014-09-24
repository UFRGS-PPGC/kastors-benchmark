/**
 *
 * @file dgeqrf.c
 *
 *  PLASMA computational routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @date 2010-11-15
 * @generated d Tue Jan  7 11:45:07 2014
 *
 **/
#include "common.h"

/***************************************************************************//**
 *
 * @ingroup double_Tile_Async
 *
 *  PLASMA_dgeqrf_Tile_Async - Computes the tile QR factorization of a matrix.
 *  Non-blocking equivalent of PLASMA_dgeqrf_Tile().
 *  May return before the computation is finished.
 *  Allows for pipelining of operations at runtime.
 *
 *******************************************************************************
 *
 * @param[in] sequence
 *          Identifies the sequence of function calls that this call belongs to
 *          (for completion checks and exception handling purposes).
 *
 * @param[out] request
 *          Identifies this function call (for exception handling purposes).
 *
 *******************************************************************************
 *
 * @sa PLASMA_dgeqrf
 * @sa PLASMA_dgeqrf_Tile
 * @sa PLASMA_cgeqrf_Tile_Async
 * @sa PLASMA_dgeqrf_Tile_Async
 * @sa PLASMA_sgeqrf_Tile_Async
 * @sa PLASMA_dgeqrs_Tile_Async
 *
 ******************************************************************************/
int PLASMA_dgeqrf_Tile_Async(PLASMA_desc *A, PLASMA_desc *T, int ib)
{
    PLASMA_desc descA;
    PLASMA_desc descT;

    descA = *A;
    descT = *T;

    plasma_pdgeqrf_quark(descA, descT, ib);

    return PLASMA_SUCCESS;
}
