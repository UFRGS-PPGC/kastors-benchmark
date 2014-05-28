/**
 *
 * @file core_dsblas.h
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @author Hatem Ltaief
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @generated ds Tue Jan  7 11:44:39 2014
 *
 **/
#ifndef _PLASMA_CORE_DSBLAS_H_
#define _PLASMA_CORE_DSBLAS_H_

#ifdef __cplusplus
extern "C" {
#endif

/** ****************************************************************************
 *  Declarations of serial kernels - alphabetical order
 **/
void CORE_slag2d(int m, int n,
                 const float *A, int lda,
                 double *B, int ldb);
void CORE_dlag2s(int m, int n,
                 const double *A, int lda,
                 float *B, int ldb, int *info);

#ifdef __cplusplus
}
#endif

#endif
