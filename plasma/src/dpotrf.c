/**
 *
 * @file dpotrf.c
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
#include "omp.h"

/***************************************************************************//**
 *
 * @ingroup double
 *
 *  PLASMA_dpotrf - Computes the Cholesky factorization of a symmetric positive definite
 *  (or Hermitian positive definite in the complex case) matrix A.
 *  The factorization has the form
 *
 *    \f[ A = \{_{L\times L^H, if uplo = PlasmaLower}^{U^H\times U, if uplo = PlasmaUpper} \f]
 *
 *  where U is an upper triangular matrix and L is a lower triangular matrix.
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
 * @param[in,out] A
 *          On entry, the symmetric positive definite (or Hermitian) matrix A.
 *          If uplo = PlasmaUpper, the leading N-by-N upper triangular part of A
 *          contains the upper triangular part of the matrix A, and the strictly lower triangular
 *          part of A is not referenced.
 *          If UPLO = 'L', the leading N-by-N lower triangular part of A contains the lower
 *          triangular part of the matrix A, and the strictly upper triangular part of A is not
 *          referenced.
 *          On exit, if return value = 0, the factor U or L from the Cholesky factorization
 *          A = U**T*U or A = L*L**T.
 *
 * @param[in] LDA
 *          The leading dimension of the array A. LDA >= max(1,N).
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *          \retval <0 if -i, the i-th argument had an illegal value
 *          \retval >0 if i, the leading minor of order i of A is not positive definite, so the
 *               factorization could not be completed, and the solution has not been computed.
 *
 *******************************************************************************
 *
 * @sa PLASMA_dpotrf_Tile
 * @sa PLASMA_dpotrf_Tile_Async
 * @sa PLASMA_cpotrf
 * @sa PLASMA_dpotrf
 * @sa PLASMA_spotrf
 * @sa PLASMA_dpotrs
 *
 ******************************************************************************/
int PLASMA_dpotrf(PLASMA_enum uplo, int N,
                  double *A, int LDA)
{
    printf("compute/dpotrf\n");
    abort();
    int status;
#if 0
    int NB;
    plasma_context_t *plasma;
    PLASMA_sequence *sequence = NULL;
    PLASMA_request request = PLASMA_REQUEST_INITIALIZER;
    PLASMA_desc descA;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_dpotrf", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    /* Check input arguments */
    if (uplo != PlasmaUpper && uplo != PlasmaLower) {
        plasma_error("PLASMA_dpotrf", "illegal value of uplo");
        return -1;
    }
    if (N < 0) {
        plasma_error("PLASMA_dpotrf", "illegal value of N");
        return -2;
    }
    if (LDA < max(1, N)) {
        plasma_error("PLASMA_dpotrf", "illegal value of LDA");
        return -4;
    }
    /* Quick return */
    if (max(N, 0) == 0)
        return PLASMA_SUCCESS;

    /* Tune NB depending on M, N & NRHS; Set NBNB */
    status = plasma_tune(PLASMA_FUNC_DPOSV, N, N, 0);
    if (status != PLASMA_SUCCESS) {
        plasma_error("PLASMA_dpotrf", "plasma_tune() failed");
        return status;
    }

    /* Set NT */
    NB   = PLASMA_NB;


#pragma omp parallel
    {
        fprintf(stderr, "Warning, you are not using plasma with tile, therefore not using the actual OpenMP plasma version\n");

#pragma omp master
        {
        plasma_sequence_create(plasma, &sequence);
        if ( PLASMA_TRANSLATION == PLASMA_OUTOFPLACE ) {
            plasma_dooplap2tile( descA, A, NB, NB, LDA, N, 0, 0, N, N, sequence, &request,
                                 plasma_desc_mat_free(&(descA)) );
        } else {
            plasma_diplap2tile( descA, A, NB, NB, LDA, N, 0, 0, N, N,
                                sequence, &request);
        }

        /* Call the tile interface */
        PLASMA_dpotrf_Tile_Async(uplo, &descA, sequence, &request);

        if ( PLASMA_TRANSLATION == PLASMA_OUTOFPLACE ) {
            plasma_dooptile2lap( descA, A, NB, NB, LDA, N,  sequence, &request);
            plasma_dynamic_sync();
            plasma_desc_mat_free(&descA);
        } else {
            plasma_diptile2lap( descA, A, NB, NB, LDA, N,  sequence, &request);
            plasma_dynamic_sync();
        }

        status = sequence->status;
        plasma_sequence_destroy(plasma, sequence);
        }
    }

#endif
    return status;
}

/***************************************************************************//**
 *
 * @ingroup double_Tile
 *
 *  PLASMA_dpotrf_Tile - Computes the Cholesky factorization of a symmetric positive definite
 *  or Hermitian positive definite matrix.
 *  Tile equivalent of PLASMA_dpotrf().
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
 *          On entry, the symmetric positive definite (or Hermitian) matrix A.
 *          If uplo = PlasmaUpper, the leading N-by-N upper triangular part of A
 *          contains the upper triangular part of the matrix A, and the strictly lower triangular
 *          part of A is not referenced.
 *          If UPLO = 'L', the leading N-by-N lower triangular part of A contains the lower
 *          triangular part of the matrix A, and the strictly upper triangular part of A is not
 *          referenced.
 *          On exit, if return value = 0, the factor U or L from the Cholesky factorization
 *          A = U**T*U or A = L*L**T.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *          \retval >0 if i, the leading minor of order i of A is not positive definite, so the
 *               factorization could not be completed, and the solution has not been computed.
 *
 *******************************************************************************
 *
 * @sa PLASMA_dpotrf
 * @sa PLASMA_dpotrf_Tile_Async
 * @sa PLASMA_cpotrf_Tile
 * @sa PLASMA_dpotrf_Tile
 * @sa PLASMA_spotrf_Tile
 * @sa PLASMA_dpotrs_Tile
 *
 ******************************************************************************/
int PLASMA_dpotrf_Tile(PLASMA_enum uplo, PLASMA_desc *A)
{
    plasma_context_t *plasma;
    PLASMA_sequence *sequence = NULL;
    PLASMA_request request = PLASMA_REQUEST_INITIALIZER;
    int status;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_dpotrf_Tile", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
        /*printf("Num threads : %i\n", omp_get_num_threads());*/
    plasma_sequence_create(plasma, &sequence);
        PLASMA_dpotrf_Tile_Async(uplo, A, sequence, &request);
    if (plasma->dynamic_section) {
        /*QUARK_Waitall(plasma->quark); \*/
#pragma omp taskwait
        plasma_barrier(plasma);
        plasma->dynamic_section = PLASMA_FALSE;
    }
    /*plasma_dynamic_sync();*/
    status = sequence->status;
    plasma_sequence_destroy(plasma, sequence);
    return status;
}

/***************************************************************************//**
 *
 * @ingroup double_Tile_Async
 *
 *  PLASMA_dpotrf_Tile_Async - Computes the Cholesky factorization of a symmetric
 *  positive definite or Hermitian positive definite matrix.
 *  Non-blocking equivalent of PLASMA_dpotrf_Tile().
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
 * @sa PLASMA_dpotrf
 * @sa PLASMA_dpotrf_Tile
 * @sa PLASMA_cpotrf_Tile_Async
 * @sa PLASMA_dpotrf_Tile_Async
 * @sa PLASMA_spotrf_Tile_Async
 * @sa PLASMA_dpotrs_Tile_Async
 *
 ******************************************************************************/
int PLASMA_dpotrf_Tile_Async(PLASMA_enum uplo, PLASMA_desc *A,
                             PLASMA_sequence *sequence, PLASMA_request *request)
{
    PLASMA_desc descA;
    plasma_context_t *plasma;

    plasma = plasma_context_self();
    if (plasma == NULL) {
        plasma_fatal_error("PLASMA_dpotrf_Tile_Async", "PLASMA not initialized");
        return PLASMA_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        plasma_fatal_error("PLASMA_dpotrf_Tile_Async", "NULL sequence");
        return PLASMA_ERR_UNALLOCATED;
    }
    if (request == NULL) {
        plasma_fatal_error("PLASMA_dpotrf_Tile_Async", "NULL request");
        return PLASMA_ERR_UNALLOCATED;
    }
    /* Check sequence status */
    if (sequence->status == PLASMA_SUCCESS)
        request->status = PLASMA_SUCCESS;
    else
        return plasma_request_fail(sequence, request, PLASMA_ERR_SEQUENCE_FLUSHED);

    /* Check descriptors for correctness */
    if (plasma_desc_check(A) != PLASMA_SUCCESS) {
        plasma_error("PLASMA_dpotrf_Tile_Async", "invalid descriptor");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    } else {
        descA = *A;
    }
    /* Check input arguments */
    if (descA.nb != descA.mb) {
        plasma_error("PLASMA_dpotrf_Tile_Async", "only square tiles supported");
        return plasma_request_fail(sequence, request, PLASMA_ERR_ILLEGAL_VALUE);
    }
    if (uplo != PlasmaUpper && uplo != PlasmaLower) {
        plasma_error("PLASMA_dpotrf_Tile_Async", "illegal value of uplo");
        return plasma_request_fail(sequence, request, -1);
    }
    /* Quick return */
/*
    if (max(N, 0) == 0)
        return PLASMA_SUCCESS;
*/

#pragma omp parallel
    {
#ifdef _OPENMP
        plasma_setaffinity(omp_get_thread_num());
#endif
#pragma omp master
        {
            plasma_dynamic_spawn();
    if (!plasma->dynamic_section) {
        plasma->dynamic_section = PLASMA_TRUE;
        pthread_mutex_lock(&plasma->action_mutex);
        plasma->action = PLASMA_ACT_DYNAMIC;
        pthread_mutex_unlock(&plasma->action_mutex);
        pthread_cond_broadcast(&plasma->action_condt);
        plasma_barrier(plasma);
        plasma->action = PLASMA_ACT_STAND_BY;
    }
    plasma_pdpotrf_quark(uplo, descA, sequence, request);
    /*
     *plasma_parallel_call_4(plasma_pdpotrf,
     *    PLASMA_enum, uplo,
     *    PLASMA_desc, descA,
     *    PLASMA_sequence*, sequence,
     *    PLASMA_request*, request);
     */
        }
    }

    return PLASMA_SUCCESS;
}
