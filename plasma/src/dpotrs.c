/**
 *
 * @file dpotrs.c
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
 * @ingroup double
 *
 *  PLASMA_dpotrs - Solves a system of linear equations A * X = B with
 *  a symmetric positive definite (or Hermitian positive definite in
 *  the complex case) matrix A using the Cholesky factorization A =
 *  U**T*U or A = L*L**T computed by PLASMA_dpotrf.
 *
 *******************************************************************************
 *
 * @param[in] uplo
 *          = PlasmaUpper: Upper triangle of A is stored;
 *          = PlasmaLower: Lower triangle of A is stored.
 *
 * @param[in] N
 *          The order of the matrix A. N >= 0.
 *
 * @param[in] NRHS
 *          The number of right hand sides, i.e., the number of
 *          columns of the matrix B. NRHS >= 0.
 *
 * @param[in,out] A
 *          The triangular factor U or L from the Cholesky
 *          factorization A = U**T*U or A = L*L**T, computed by
 *          PLASMA_dpotrf.
 *          Remark: If out-of-place layout translation is used, the
 *          matrix A can be considered as input, however if inplace
 *          layout translation is enabled, the content of A will be
 *          reordered for computation and restored before exiting the
 *          function.
 *
 * @param[in] LDA
 *          The leading dimension of the array A. LDA >= max(1,N).
 *
 * @param[in,out] B
 *          On entry, the N-by-NRHS right hand side matrix B.
 *          On exit, if return value = 0, the N-by-NRHS solution matrix X.
 *
 * @param[in] LDB
 *          The leading dimension of the array B. LDB >= max(1,N).
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *          \retval <0 if -i, the i-th argument had an illegal value
 *
 *******************************************************************************
 *
 * @sa PLASMA_dpotrs_Tile
 * @sa PLASMA_dpotrs_Tile_Async
 * @sa PLASMA_cpotrs
 * @sa PLASMA_dpotrs
 * @sa PLASMA_spotrs
 * @sa PLASMA_dpotrf
 *
 ******************************************************************************/
int PLASMA_dpotrs(PLASMA_enum uplo, int N, int NRHS,
                  double *A, int LDA,
                  double *B, int LDB)
{
    int NB;
    int status;
    printf("dpotrs\n");
    abort();
#if 0
    plasma_context_t *plasma;
    PLASMA_sequence *sequence = NULL;
    PLASMA_request request = PLASMA_REQUEST_INITIALIZER;
    PLASMA_desc descA, descB;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_dpotrs", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    /* Check input arguments */
    if (uplo != PlasmaUpper && uplo != PlasmaLower) {
        plasma_error("PLASMA_dpotrs", "illegal value of uplo");
        return -1;
    }
    if (N < 0) {
        plasma_error("PLASMA_dpotrs", "illegal value of N");
        return -2;
    }
    if (NRHS < 0) {
        plasma_error("PLASMA_dpotrs", "illegal value of NRHS");
        return -3;
    }
    if (LDA < max(1, N)) {
        plasma_error("PLASMA_dpotrs", "illegal value of LDA");
        return -5;
    }
    if (LDB < max(1, N)) {
        plasma_error("PLASMA_dpotrs", "illegal value of LDB");
        return -7;
    }
    /* Quick return */
    if (min(N, NRHS) == 0)
        return PLASMA_SUCCESS;

    /* Tune NB depending on M, N & NRHS; Set NBNB */
    status = plasma_tune(PLASMA_FUNC_DPOSV, N, N, NRHS);
    if (status != PLASMA_SUCCESS) {
        plasma_error("PLASMA_dpotrs", "plasma_tune() failed");
        return status;
    }

    /* Set NT & NTRHS */
    NB    = PLASMA_NB;

    plasma_sequence_create(plasma, &sequence);

    if ( PLASMA_TRANSLATION == PLASMA_OUTOFPLACE ) {
        plasma_dooplap2tile( descA, A, NB, NB, LDA, N,    0, 0, N, N,    sequence, &request,
                             plasma_desc_mat_free(&(descA)) );
        plasma_dooplap2tile( descB, B, NB, NB, LDB, NRHS, 0, 0, N, NRHS, sequence, &request,
                             plasma_desc_mat_free(&(descA)); plasma_desc_mat_free(&(descB)));
    } else {
        plasma_diplap2tile( descA, A, NB, NB, LDA, N,    0, 0, N, N,   
                            sequence, &request);
        plasma_diplap2tile( descB, B, NB, NB, LDB, NRHS, 0, 0, N, NRHS,
                            sequence, &request);
    }

    /* Call the tile interface */
    PLASMA_dpotrs_Tile_Async(uplo, &descA, &descB, sequence, &request);

    if ( PLASMA_TRANSLATION == PLASMA_OUTOFPLACE ) {
        plasma_dooptile2lap( descB, B, NB, NB, LDB, NRHS,  sequence, &request);
        plasma_dynamic_sync();
        plasma_desc_mat_free(&descA);
        plasma_desc_mat_free(&descB);
    } else {
        plasma_diptile2lap( descA, A, NB, NB, LDA, N,     sequence, &request);
        plasma_diptile2lap( descB, B, NB, NB, LDB, NRHS,  sequence, &request);
        plasma_dynamic_sync();
    }
    
    status = sequence->status;
    plasma_sequence_destroy(plasma, sequence);
#endif
    return status;
}

/***************************************************************************//**
 *
 * @ingroup double_Tile
 *
 *  PLASMA_dpotrs_Tile - Solves a system of linear equations using previously
 *  computed Cholesky factorization.
 *  Tile equivalent of PLASMA_dpotrs().
 *  Operates on matrices stored by tiles.
 *  All matrices are passed through descriptors.
 *  All dimensions are taken from the descriptors.
 *
 *******************************************************************************
 *
 * @param[in] uplo
 *          = PlasmaUpper: Upper triangle of A is stored;
 *          = PlasmaLower: Lower triangle of A is stored.
 *
 * @param[in] A
 *          The triangular factor U or L from the Cholesky factorization A = U**T*U or A = L*L**T,
 *          computed by PLASMA_dpotrf.
 *
 * @param[in,out] B
 *          On entry, the N-by-NRHS right hand side matrix B.
 *          On exit, if return value = 0, the N-by-NRHS solution matrix X.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *
 *******************************************************************************
 *
 * @sa PLASMA_dpotrs
 * @sa PLASMA_dpotrs_Tile_Async
 * @sa PLASMA_cpotrs_Tile
 * @sa PLASMA_dpotrs_Tile
 * @sa PLASMA_spotrs_Tile
 * @sa PLASMA_dpotrf_Tile
 *
 ******************************************************************************/
int PLASMA_dpotrs_Tile(PLASMA_enum uplo, PLASMA_desc *A, PLASMA_desc *B)
{
    plasma_context_t *plasma;
    PLASMA_sequence *sequence = NULL;
    PLASMA_request request = PLASMA_REQUEST_INITIALIZER;
    int status;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_dpotrs_Tile", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    plasma_sequence_create(plasma, &sequence);
    PLASMA_dpotrs_Tile_Async(uplo, A, B, sequence, &request);
    plasma_dynamic_sync();
    status = sequence->status;
    plasma_sequence_destroy(plasma, sequence);
    return status;
}

/***************************************************************************//**
 *
 * @ingroup double_Tile_Async
 *
 *  PLASMA_dpotrs_Tile_Async - Solves a system of linear equations using previously
 *  computed Cholesky factorization.
 *  Non-blocking equivalent of PLASMA_dpotrs_Tile().
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
 * @sa PLASMA_dpotrs
 * @sa PLASMA_dpotrs_Tile
 * @sa PLASMA_cpotrs_Tile_Async
 * @sa PLASMA_dpotrs_Tile_Async
 * @sa PLASMA_spotrs_Tile_Async
 * @sa PLASMA_dpotrf_Tile_Async
 *
 ******************************************************************************/
int PLASMA_dpotrs_Tile_Async(PLASMA_enum uplo, PLASMA_desc *A, PLASMA_desc *B,
                             PLASMA_sequence *sequence, PLASMA_request *request)
{
    PLASMA_desc descA;
    PLASMA_desc descB;
    plasma_context_t *plasma;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_dpotrs_Tile", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        plasma_fatal_error("PLASMA_dpotrs_Tile", "NULL sequence");
        return PLASMA_ERR_UNALLOCATED;
    }
    if (request == NULL) {
        plasma_fatal_error("PLASMA_dpotrs_Tile", "NULL request");
        return PLASMA_ERR_UNALLOCATED;
    }
    /* Check sequence status */
    if (sequence->status == PLASMA_SUCCESS)
        request->status = PLASMA_SUCCESS;
    else
        return plasma_request_fail(sequence, request, PLASMA_ERR_SEQUENCE_FLUSHED);

    /* Check descriptors for correctness */
    if (plasma_desc_check(A) != PLASMA_SUCCESS) {
        plasma_error("PLASMA_dpotrs_Tile", "invalid first descriptor");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    } else {
        descA = *A;
    }
    if (plasma_desc_check(B) != PLASMA_SUCCESS) {
        plasma_error("PLASMA_dpotrs_Tile", "invalid second descriptor");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    } else {
        descB = *B;
    }
    /* Check input arguments */
    if (descA.nb != descA.mb || descB.nb != descB.mb) {
        plasma_error("PLASMA_dpotrs_Tile", "only square tiles supported");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    }
    if (uplo != PlasmaUpper && uplo != PlasmaLower) {
        plasma_error("PLASMA_dpotrs_Tile", "illegal value of uplo");
        return plasma_request_fail(sequence, request, -1);
    }
    /* Quick return */
/*
    if (min(N, NRHS) == 0)
        return PLASMA_SUCCESS;
*/
    plasma_parallel_call_9(plasma_pdtrsm,
        PLASMA_enum, PlasmaLeft,
        PLASMA_enum, uplo,
        PLASMA_enum, uplo == PlasmaUpper ? PlasmaTrans : PlasmaNoTrans,
        PLASMA_enum, PlasmaNonUnit,
        double, 1.0,
        PLASMA_desc, descA,
        PLASMA_desc, descB,
        PLASMA_sequence*, sequence,
        PLASMA_request*, request);

    plasma_parallel_call_9(plasma_pdtrsm,
        PLASMA_enum, PlasmaLeft,
        PLASMA_enum, uplo,
        PLASMA_enum, uplo == PlasmaUpper ? PlasmaNoTrans : PlasmaTrans,
        PLASMA_enum, PlasmaNonUnit,
        double, 1.0,
        PLASMA_desc, descA,
        PLASMA_desc, descB,
        PLASMA_sequence*, sequence,
        PLASMA_request*, request);

    return PLASMA_SUCCESS;
}
