/**
 *
 * @file core_blas.h
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @author Hatem Ltaief
 * @date 2010-11-15
 *
 **/
#ifndef _PLASMA_CORE_BLAS_H_
#define _PLASMA_CORE_BLAS_H_

#include <cblas.h>
#include "plasmatypes.h"
#include "descriptor.h"

#include "core_dblas.h"
#include "core_dsblas.h"

#ifdef __cplusplus
extern "C" {
#endif

  /*
   * Coreblas Error
   */
#define coreblas_error(k, str) fprintf(stderr, "%s: Parameter %d / %s\n", __func__, k, str);

 /** ****************************************************************************
  *  LAPACK Constants
  **/
extern char *plasma_lapack_constants[];
#define lapack_const(plasma_const) plasma_lapack_constants[plasma_const][0]

  /*
   * CBlas enum
   */
#define CBLAS_TRANSPOSE enum CBLAS_TRANSPOSE
#define CBLAS_UPLO      enum CBLAS_UPLO
#define CBLAS_DIAG      enum CBLAS_DIAG
#define CBLAS_SIDE      enum CBLAS_SIDE

/* CBLAS requires for scalar arguments to be passed by address rather than by value */
#ifndef CBLAS_SADDR
#define CBLAS_SADDR( _val_ ) &(_val_)
#endif

 /** ****************************************************************************
  *  External interface of the GKK algorithm for InPlace Layout Translation
  **/
int  GKK_minloc(int n, int *T);
void GKK_BalanceLoad(int thrdnbr, int *Tp, int *leaders, int nleaders, int L);
int  GKK_getLeaderNbr(int me, int ne, int *nleaders, int **leaders);

#ifdef __cplusplus
}
#endif

#endif /* _PLASMA_CORE_BLAS_H_ */
