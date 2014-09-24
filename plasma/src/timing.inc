/**
 *
 * @file timing.c
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Mathieu Faverge
 * @author Dulceneia Becker
 * @date 2010-11-15
 *
 **/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#ifdef PLASMA_EZTRACE
#include <eztrace.h>
#endif

#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <cblas.h>
#include <lapacke.h>
#include <plasma.h>
#include <core_blas.h>
#include "flops.h"
#include "timing.h"
#include "auxiliary.h"
#include "main.h"
#include "workspace.h"
#define EPSILON 1.0E-9

static int RunTest(int *iparam, _PREC *dparam, double *t_);

double cWtime(void);

int ISEED[4] = {0,0,0,1};   /* initial seed for zlarnv() */

/*
 * struct timeval {time_t tv_sec; suseconds_t tv_usec;};
 */
double cWtime(void)
{
    struct timeval tp;
    gettimeofday( &tp, NULL );
    return tp.tv_sec + 1e-6 * tp.tv_usec;
}

static double
Test(int *iparam, struct user_parameters* params) {
    int      iter;
    int      niter;
    int64_t M, N, K, NRHS;
    double  *t;
    _PREC    dparam[IPARAM_DNBPARAM];
    double   fmuls, fadds, fp_per_mul, fp_per_add;
    double   sumgf, sumt, flops, gflops;

    memset( &dparam, 0, IPARAM_DNBPARAM * sizeof(_PREC) );

    niter   = iparam[IPARAM_NITER];

    M    = iparam[IPARAM_M];
    N    = iparam[IPARAM_N];
    K    = iparam[IPARAM_K];
    NRHS = K;

    t = (double*)calloc(niter, sizeof(double));

    if (sizeof(_TYPE) == sizeof(_PREC)) {
        fp_per_mul = 1;
        fp_per_add = 1;
    } else {
        fp_per_mul = 6;
        fp_per_add = 2;
    }

    fadds = (double)(_FADDS);
    fmuls = (double)(_FMULS);
    flops = 1e-9 * (fmuls * fp_per_mul + fadds * fp_per_add);
    gflops = 0.0;

    if ( iparam[IPARAM_WARMUP] ) {
        RunTest( iparam, dparam, &(t[0]));
    }

    sumgf  = 0.0;
    sumt   = 0.0;

    for (iter = 0; iter < niter; iter++)
    {
        if( iter == 0 ) {
          if ( iparam[IPARAM_TRACE] )
            iparam[IPARAM_TRACE] = 2;
          if ( iparam[IPARAM_DAG] )
            iparam[IPARAM_DAG] = 2;

          RunTest( iparam, dparam, &(t[iter]));

          iparam[IPARAM_TRACE] = 0;
          iparam[IPARAM_DAG] = 0;
        }
        else
            RunTest( iparam, dparam, &(t[iter]));

        gflops = flops / t[iter];

        sumt   += t[iter];
        sumgf  += gflops;
    }

    gflops = sumgf / niter;
    printf("Gflops : %f\n", gflops);

    free(t);

    if (params->check && dparam[IPARAM_RES] < EPSILON)
        params->succeed = 1;

    return sumt/niter;
}

double run(struct user_parameters* params)
{
    params->succeed = 0;
    int start =  2048;
    int stop  = 2048;
    int step  =  1;
    int iparam[IPARAM_SIZEOF];


    memset(iparam, 0, IPARAM_SIZEOF*sizeof(int));

    iparam[IPARAM_THRDNBR       ] = 1;
    iparam[IPARAM_THRDNBR_SUBGRP] = 1;
    iparam[IPARAM_M             ] = -1;
    iparam[IPARAM_N             ] = 500;
    iparam[IPARAM_K             ] = 1;
    iparam[IPARAM_LDA           ] = -1;
    iparam[IPARAM_LDB           ] = -1;
    iparam[IPARAM_LDC           ] = -1;
    iparam[IPARAM_MB            ] = 128;
    iparam[IPARAM_NB            ] = 128;
    iparam[IPARAM_IB            ] = 32;
    iparam[IPARAM_NITER         ] = 2;
    iparam[IPARAM_WARMUP        ] = 1;
    iparam[IPARAM_CHECK         ] = params->check;
    iparam[IPARAM_VERBOSE       ] = 0;
    iparam[IPARAM_INPUTFMT      ] = 0;
    iparam[IPARAM_OUTPUTFMT     ] = 0;
    iparam[IPARAM_TRACE         ] = 0;
    iparam[IPARAM_DAG           ] = 0;
    iparam[IPARAM_ASYNC         ] = 1;
    iparam[IPARAM_MX            ] = -1;
    iparam[IPARAM_NX            ] = -1;
    iparam[IPARAM_TNTPIV_SIZE   ] = 4;


    if (params->matrix_size > 0) {
        start = params->matrix_size;
        stop = start;
    } else {
        params->matrix_size = start;
    }

    if (params->blocksize > 0) {
        iparam[IPARAM_MB] = params->blocksize;
        iparam[IPARAM_NB] = params->blocksize;
    } else {
        params->blocksize = iparam[IPARAM_NB];
    }

    /* Initialize Plasma */
    PLASMA_Init( iparam[IPARAM_THRDNBR] );

    PLASMA_Set(PLASMA_TILE_SIZE,        iparam[IPARAM_NB] );
    PLASMA_Set(PLASMA_INNER_BLOCK_SIZE, iparam[IPARAM_IB] );

    /* Tournament pivoting */
    PLASMA_Set(PLASMA_TNTPIVOTING_SIZE, iparam[IPARAM_TNTPIV_SIZE]);

    if (step < 1) step = 1;
    double timing = 0;
    iparam[IPARAM_M] = start;
    iparam[IPARAM_N] = start;
    printf("M : %d\n", iparam[IPARAM_M]);
    printf("N : %d\n", iparam[IPARAM_N]);
    printf("NB : %d\n", iparam[IPARAM_NB]);
    timing = Test(iparam, params);

    PLASMA_Finalize();

    return timing;
}
