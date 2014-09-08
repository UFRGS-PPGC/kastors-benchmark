/**
 *
 * @file dgetrf.c
 *
 *  PLASMA computational routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Hatem Ltaief
 * @author Mathieu Faverge
 * @date 2009-11-15
 *
 * @generated d Tue Jan  7 11:45:07 2014
 *
 **/
#include "common.h"

/***************************************************************************//**
 *
 * @ingroup double_Tile_Async
 *
 *  PLASMA_dgetrf_Tile_Async - Computes the tile LU factorization of a matrix.
 *  Non-blocking equivalent of PLASMA_dgetrf_Tile().
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
 * @sa PLASMA_dgetrf
 * @sa PLASMA_dgetrf_Tile
 * @sa PLASMA_cgetrf_Tile_Async
 * @sa PLASMA_dgetrf_Tile_Async
 * @sa PLASMA_sgetrf_Tile_Async
 * @sa PLASMA_dgetrs_Tile_Async
 *
 ******************************************************************************/
int PLASMA_dgetrf_Tile_Async(PLASMA_desc *A, int *IPIV)
{
    PLASMA_desc descA;
        descA = *A;

        plasma_pdbarrier_tl2pnl_quark(descA);
        plasma_pdgetrf_rectil_quark(descA, IPIV);
        plasma_pdbarrier_pnl2tl_quark(descA);

    return PLASMA_SUCCESS;
}
