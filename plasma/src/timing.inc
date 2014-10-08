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
#include "plasma.h"
#include <core_blas.h>
#include "flops.h"
#include "timing.h"
#include "auxiliary.h"
#include "main.h"
#include "workspace.h"
#define EPSILON 1.0E-9

static double RunTest(double *t_, struct user_parameters*);

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


double run(struct user_parameters* params)
{
    double  *t;
    double   fmuls, fadds;
    double   sumgf, sumt, flops, gflops;
    int iter;
    int niter    = 2;
    int64_t N    = params->matrix_size;
    int64_t M    = params->matrix_size;

    params->succeed = 1;

    t = (double*)calloc(niter, sizeof(double));

    fadds = (double)(_FADDS);
    fmuls = (double)(_FMULS);
    flops = 1e-9 * (fmuls + fadds);
    gflops = 0.0;

    if (params->matrix_size <= 0) {
        params->matrix_size = 2048;
    }

    if (params->blocksize <= 0) {
        params->blocksize = 128;
    }

    printf("N : %d\n", params->matrix_size);
    printf("NB : %d\n", params->blocksize);

    sumgf  = 0.0;
    sumt   = 0.0;

    for (iter = 0; iter < niter; iter++)
    {
        if (RunTest(&(t[iter]), params) > EPSILON && params->check)
            params->succeed = 0;

        gflops = flops / t[iter];
        sumt   += t[iter];
        sumgf  += gflops;
    }

    // Average Gflops
    gflops = sumgf / niter;
    printf("Gflops : %f\n", gflops);

    free(t);

    // Average time
    return sumt / niter;
}
