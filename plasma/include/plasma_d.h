/**
 *
 * @file plasma_d.h
 *
 *  PLASMA header file for double routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @author Hatem Ltaief
 * @author Mathieu Faverge
 * @author Azzam Haidar
 * @date 2010-11-15
 * @generated d Tue Jan  7 11:44:39 2014
 *
 **/
#ifndef _PLASMA_D_H_
#define _PLASMA_D_H_

#undef COMPLEX
#define REAL

#ifdef __cplusplus
extern "C" {
#endif

int PLASMA_dgeqrf_Tile_Async(PLASMA_desc *A, PLASMA_desc *T, int ib);
int PLASMA_dgeqrs_Tile_Async(PLASMA_desc *A, PLASMA_desc *T, PLASMA_desc *B, int ib);
int PLASMA_dgetrf_Tile_Async(  PLASMA_desc *A, int *IPIV);
int PLASMA_dgetrs_Tile_Async(PLASMA_enum trans, PLASMA_desc *A, const int *IPIV, PLASMA_desc *B);
int PLASMA_dpotrs_Tile_Async(PLASMA_enum uplo, PLASMA_desc *A, PLASMA_desc *B);

#ifdef __cplusplus
}
#endif

#undef COMPLEX

#endif
