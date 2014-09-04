/**
 *
 * @generated d Tue Jan  7 11:45:24 2014
 *
 **/
#define _TYPE  double
#define _PREC  double
#define _LAMCH LAPACKE_dlamch_work

#include "omp.h"

#define _NAME  "PLASMA_dpotrf_Tile"
/* See Lawn 41 page 120 */
#define _FMULS FMULS_POTRF( N )
#define _FADDS FADDS_POTRF( N )

#include "./timing.inc"

static int
RunTest(int *iparam, double *dparam, real_Double_t *t_)
{
#if 0
double t0;
printf("%f : ------------- BEGIN INIT\n", t0 =omp_get_wtime());
#endif
    PASTE_CODE_IPARAM_LOCALS( iparam );
    int uplo = PlasmaUpper;

    LDA = max(LDA, N);

    /* Allocate Data */
    PASTE_CODE_ALLOCATE_MATRIX_TILE( descA, 1,     double, PlasmaRealDouble, LDA, N, N );
    PASTE_CODE_ALLOCATE_MATRIX_TILE( descB, check, double, PlasmaRealDouble, LDB, N, NRHS );
#pragma omp parallel
#pragma omp master
    {
    PLASMA_dplgsy_Tile( (double)N, descA, 51 );
    }

    /* Save A for check */
    PASTE_TILE_TO_LAPACK( descA, A, check, double, LDA, N );
#if 0
printf("%f : ------------- END INIT: %f s\n", omp_get_wtime(), omp_get_wtime()-t0);
#endif

    /* PLASMA DPOSV */
#if 0
printf("%f : ------------- BEGIN\n", t0 =omp_get_wtime());
#endif
    START_TIMING();
#pragma omp parallel
#pragma omp master
    {
    PLASMA_dpotrf_Tile(uplo, descA);
    }
    STOP_TIMING();
#if 0
printf("%f : ------------- END COMPUTE: %f s\n", omp_get_wtime(), omp_get_wtime()-t0);
#endif

    /* Check the solution */
    if ( check )
    {
        PLASMA_dplrnt_Tile( descB, 7672 );
        PASTE_TILE_TO_LAPACK( descB, B, check, double, LDB, NRHS );

        PLASMA_dpotrs_Tile( uplo, descA, descB );

        PASTE_TILE_TO_LAPACK( descB, X, check, double, LDB, NRHS );

        dparam[IPARAM_RES] = d_check_solution(N, N, NRHS, A, LDA, B, X, LDB,
                                              &(dparam[IPARAM_ANORM]),
                                              &(dparam[IPARAM_BNORM]),
                                              &(dparam[IPARAM_XNORM]));

        PASTE_CODE_FREE_MATRIX( descB );
        free( A );
        free( B );
        free( X );
      }

    PASTE_CODE_FREE_MATRIX( descA );

    return 0;
}
