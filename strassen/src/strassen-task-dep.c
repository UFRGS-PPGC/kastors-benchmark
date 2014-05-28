/**********************************************************************************************/
/*  This program is part of the Barcelona OpenMP Tasks Suite                                  */
/*  Copyright (C) 2009 Barcelona Supercomputing Center - Centro Nacional de Supercomputacion  */
/*  Copyright (C) 2009 Universitat Politecnica de Catalunya                                   */
/*                                                                                            */
/**********************************************************************************************/

/*
 * Copyright (c) 1996 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to use, copy, modify, and distribute the Software without
 * restriction, provided the Software, including any modified copies made
 * under this license, is not distributed for a fee, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE MASSACHUSETTS INSTITUTE OF TECHNOLOGY BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * /WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the Massachusetts
 * Institute of Technology shall not be used in advertising or otherwise
 * to promote the sale, use or other dealings in this Software without
 * prior written authorization from the Massachusetts Institute of
 * Technology.
 *
 */

#include <stdlib.h>
#include "strassen.h"

static void MY_SSUB(REAL* out, REAL* a, REAL* b, int RowIncr, unsigned QuadrantSize)
{
  int Row;
  int Column;
PTR TempMatrixOffset = 0;
PTR MatrixOffset = 0;

  for (Row = 0; Row < QuadrantSize; Row++) {
    for (Column = 0; Column < QuadrantSize; Column++) {
      /* Element of Global Matrix, such as A, B, C */
      #define E(Matrix)   (* (REAL*) ( ((PTR) Matrix) + TempMatrixOffset ) )
      #define EM(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffset ) )

      E(out) = *a - EM(b);
      TempMatrixOffset += sizeof(REAL);
      MatrixOffset += sizeof(REAL);
    } /* end row loop*/
    MatrixOffset += RowIncr;
  } /* end column loop */
}

static void MY_SUBS(REAL* out, REAL* a, REAL* b, int RowIncr, unsigned QuadrantSize)
{
  int Row;
  int Column;
PTR TempMatrixOffset = 0;
PTR MatrixOffset = 0;

  for (Row = 0; Row < QuadrantSize; Row++) {

    /*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
    for (Column = 0; Column < QuadrantSize; Column++) {

      /***********************************************************
      ** Within this loop, the following holds for MatrixOffset:
      ** MatrixOffset = (Row * RowWidth) + Column
      ** (note: that the unit of the offset is number of reals)
      ***********************************************************/
      /* Element of Global Matrix, such as A, B, C */
      #define E(Matrix)   (* (REAL*) ( ((PTR) Matrix) + TempMatrixOffset ) )
      #define EM(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffset ) )

      E(out) = EM(a) - *b;
      TempMatrixOffset += sizeof(REAL);
      MatrixOffset += sizeof(REAL);
    } /* end row loop*/

    MatrixOffset += RowIncr;
  } /* end column loop */
}

static void MY_SUB(REAL* out, REAL* a, REAL* b, int RowIncr, unsigned QuadrantSize)
{
  int Row;
  int Column;
PTR TempMatrixOffset = 0;
PTR MatrixOffset = 0;

  for (Row = 0; Row < QuadrantSize; Row++) {

    /*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
    for (Column = 0; Column < QuadrantSize; Column++) {

      /***********************************************************
      ** Within this loop, the following holds for MatrixOffset:
      ** MatrixOffset = (Row * RowWidth) + Column
      ** (note: that the unit of the offset is number of reals)
      ***********************************************************/
      /* Element of Global Matrix, such as A, B, C */
      #define E(Matrix)   (* (REAL*) ( ((PTR) Matrix) + TempMatrixOffset ) )
      #define EM(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffset ) )

      E(out) = EM(a) - EM(b);
      TempMatrixOffset += sizeof(REAL);
      MatrixOffset += sizeof(REAL);
    } /* end row loop*/

    MatrixOffset += RowIncr;
  } /* end column loop */
}

static void MY_SUM(REAL* out, REAL* a, REAL* b, int RowIncr, unsigned QuadrantSize)
{
  int Row;
  int Column;
PTR TempMatrixOffset = 0;
PTR MatrixOffset = 0;

  for (Row = 0; Row < QuadrantSize; Row++) {

    /*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
    for (Column = 0; Column < QuadrantSize; Column++) {

      /***********************************************************
      ** Within this loop, the following holds for MatrixOffset:
      ** MatrixOffset = (Row * RowWidth) + Column
      ** (note: that the unit of the offset is number of reals)
      ***********************************************************/
      /* Element of Global Matrix, such as A, B, C */
      #define E(Matrix)   (* (REAL*) ( ((PTR) Matrix) + TempMatrixOffset ) )
      #define EM(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffset ) )

      E(out) = EM(a) + EM(b);
      TempMatrixOffset += sizeof(REAL);
      MatrixOffset += sizeof(REAL);
    } /* end row loop*/

    MatrixOffset += RowIncr;
  } /* end column loop */
}

static void OptimizedStrassenMultiply_par(REAL *C, REAL *A, REAL *B, unsigned MatrixSize,
     unsigned RowWidthC, unsigned RowWidthA, unsigned RowWidthB, int Depth,
     unsigned int cutoff_size, unsigned int cutoff_depth)
{
  unsigned QuadrantSize = MatrixSize >> 1; /* MatixSize / 2 */
  unsigned QuadrantSizeInBytes = sizeof(REAL) * QuadrantSize * QuadrantSize
                                 + 32;
  unsigned Column, Row;

  /************************************************************************
  ** For each matrix A, B, and C, we'll want pointers to each quandrant
  ** in the matrix. These quandrants will be addressed as follows:
  **  --        --
  **  | A11  A12 |
  **  |          |
  **  | A21  A22 |
  **  --        --
  ************************************************************************/
  REAL /* *A11, *B11, *C11, */ *A12, *B12, *C12,
       *A21, *B21, *C21, *A22, *B22, *C22;

  REAL *S1,*S2,*S3,*S4,*S5,*S6,*S7,*S8,*M2,*M5,*T1sMULT;
  #define T2sMULT C22
  #define NumberOfVariables 11

  char *Heap;
  void *StartHeap;

  /* Distance between the end of a matrix row and the start of the next row */
  PTR RowIncrementA = ( RowWidthA - QuadrantSize ) << 3;
  PTR RowIncrementB = ( RowWidthB - QuadrantSize ) << 3;
  PTR RowIncrementC = ( RowWidthC - QuadrantSize ) << 3;

  if (MatrixSize <= cutoff_size) {
    MultiplyByDivideAndConquer(C, A, B, MatrixSize, RowWidthC, RowWidthA, RowWidthB, 0);
    return;
  }

  /* Initialize quandrant matrices */
  #define A11 A
  #define B11 B
  #define C11 C
  A12 = A11 + QuadrantSize;
  B12 = B11 + QuadrantSize;
  C12 = C11 + QuadrantSize;
  A21 = A + (RowWidthA * QuadrantSize);
  B21 = B + (RowWidthB * QuadrantSize);
  C21 = C + (RowWidthC * QuadrantSize);
  A22 = A21 + QuadrantSize;
  B22 = B21 + QuadrantSize;
  C22 = C21 + QuadrantSize;

  /* Allocate Heap Space Here */
  StartHeap = Heap = malloc(QuadrantSizeInBytes * NumberOfVariables);
  /* ensure that heap is on cache boundary */
  if ( ((PTR) Heap) & 31)
     Heap = (char*) ( ((PTR) Heap) + 32 - ( ((PTR) Heap) & 31) );

  /* Distribute the heap space over the variables */
  S1 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  S2 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  S3 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  S4 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  S5 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  S6 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  S7 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  S8 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  M2 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  M5 = (REAL*) Heap; Heap += QuadrantSizeInBytes;
  T1sMULT = (REAL*) Heap; Heap += QuadrantSizeInBytes;


  if (Depth < cutoff_depth)
    {
// gcc and icc have internal error on these dependencies
//    #pragma omp task shared(S1, A21, A22, RowIncrementA, QuadrantSize) depend(in: A21, A22) depend(out: S1)
//    MY_SUM(S1, A21, A22, RowIncrementA, QuadrantSize);
//    #pragma omp task shared(S2, S1, A11, RowIncrementA, QuadrantSize) depend(in: S1, A11) depend(out: S2)
//    MY_SSUB(S2, S1, A11, RowIncrementA, QuadrantSize);
//    #pragma omp task shared(S4, A12, S2, RowIncrementA, QuadrantSize) depend(in: A12, S2) depend(out: S4)
//    MY_SUBS(S4, A12, S2, RowIncrementA, QuadrantSize);
#pragma omp task depend(out: S1, S2, S4)
        {
        MY_SUM(S1, A21, A22, RowIncrementA, QuadrantSize);
        MY_SSUB(S2, S1, A11, RowIncrementA, QuadrantSize);
        MY_SUBS(S4, A12, S2, RowIncrementA, QuadrantSize);
    }
// gcc and icc have internal error on these dependencies
//#pragma omp task shared(S5, B12, B11, RowIncrementB, QuadrantSize) depend(in: B11, B12) depend(out: S5)
//MY_SUB(S5, B12, B11, RowIncrementB, QuadrantSize);
//#pragma omp task shared(S5, S6, B22, RowIncrementB, QuadrantSize) depend(in: B22, S5) depend(out: S6)
//MY_SUBS(S6, B22, S5, RowIncrementB, QuadrantSize);
//#pragma omp task shared(S8, S6, B21, RowIncrementB, QuadrantSize) depend(in: S6, B21) depend(out: S8)
//MY_SSUB(S8, S6, B21, RowIncrementB, QuadrantSize);
#pragma omp task depend(out: S5, S6, S8)
    {
        MY_SUB(S5, B12, B11, RowIncrementB, QuadrantSize);
        MY_SUBS(S6, B22, S5, RowIncrementB, QuadrantSize);
        MY_SSUB(S8, S6, B21, RowIncrementB, QuadrantSize);
    }
#pragma omp task depend(out: S3)
    MY_SUB(S3, A11, A21, RowIncrementA, QuadrantSize);
#pragma omp task depend(out: S7)
    MY_SUB(S7, B22, B12, RowIncrementB, QuadrantSize);
    /* M2 = A11 x B11 */
#pragma omp task untied depend(in: A11, B11)
    OptimizedStrassenMultiply_par(M2, A11, B11, QuadrantSize, QuadrantSize, RowWidthA, RowWidthB, Depth+1, cutoff_size, cutoff_depth);

    /* M5 = S1 * S5 */
#pragma omp task untied depend(in: S1, S5)
    OptimizedStrassenMultiply_par(M5, S1, S5, QuadrantSize, QuadrantSize, QuadrantSize, QuadrantSize, Depth+1, cutoff_size, cutoff_depth);

    /* Step 1 of T1 = S2 x S6 + M2 */
#pragma omp task untied depend(in: S2, S6)
    OptimizedStrassenMultiply_par(T1sMULT, S2, S6,  QuadrantSize, QuadrantSize, QuadrantSize, QuadrantSize, Depth+1, cutoff_size, cutoff_depth);

    /* Step 1 of T2 = T1 + S3 x S7 */
#pragma omp task untied depend(in: S3, S7)
    OptimizedStrassenMultiply_par(C22, S3, S7, QuadrantSize, RowWidthC /*FIXME*/, QuadrantSize, QuadrantSize, Depth+1, cutoff_size, cutoff_depth);

    /* Step 1 of C11 = M2 + A12 * B21 */
#pragma omp task untied depend(in: A12, B21)
    OptimizedStrassenMultiply_par(C11, A12, B21, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, Depth+1, cutoff_size, cutoff_depth);

    /* Step 1 of C12 = S4 x B22 + T1 + M5 */
#pragma omp task untied depend(in: S4, B22)
    OptimizedStrassenMultiply_par(C12, S4, B22, QuadrantSize, RowWidthC, QuadrantSize, RowWidthB, Depth+1, cutoff_size, cutoff_depth);

    /* Step 1 of C21 = T2 - A22 * S8 */
#pragma omp task untied depend(in: A22, S8)
    OptimizedStrassenMultiply_par(C21, A22, S8, QuadrantSize, RowWidthC, RowWidthA, QuadrantSize, Depth+1, cutoff_size, cutoff_depth);

    /**********************************************
     ** Synchronization Point
     **********************************************/
#pragma omp taskwait
}
  else
  {
    MY_SUM(S1, A21, A22, RowIncrementA, QuadrantSize);
    MY_SSUB(S2, S1, A11, RowIncrementA, QuadrantSize);
    MY_SUBS(S4, A12, S2, RowIncrementA, QuadrantSize);
    MY_SUB(S5, B12, B11, RowIncrementB, QuadrantSize);
    MY_SUBS(S6, B22, S5, RowIncrementB, QuadrantSize);
    MY_SSUB(S8, S6, B21, RowIncrementB, QuadrantSize);
    MY_SUB(S3, A11, A21, RowIncrementA, QuadrantSize);
    MY_SUB(S7, B22, B12, RowIncrementB, QuadrantSize);
    /* M2 = A11 x B11 */
    OptimizedStrassenMultiply_par(M2, A11, B11, QuadrantSize, QuadrantSize, RowWidthA, RowWidthB, Depth+1, cutoff_size, cutoff_depth);
    /* M5 = S1 * S5 */
    OptimizedStrassenMultiply_par(M5, S1, S5, QuadrantSize, QuadrantSize, QuadrantSize, QuadrantSize, Depth+1, cutoff_size, cutoff_depth);
    /* Step 1 of T1 = S2 x S6 + M2 */
    OptimizedStrassenMultiply_par(T1sMULT, S2, S6,  QuadrantSize, QuadrantSize, QuadrantSize, QuadrantSize, Depth+1, cutoff_size, cutoff_depth);
    /* Step 1 of T2 = T1 + S3 x S7 */
    OptimizedStrassenMultiply_par(C22, S3, S7, QuadrantSize, RowWidthC /*FIXME*/, QuadrantSize, QuadrantSize, Depth+1, cutoff_size, cutoff_depth);
    /* Step 1 of C11 = M2 + A12 * B21 */
    OptimizedStrassenMultiply_par(C11, A12, B21, QuadrantSize, RowWidthC, RowWidthA, RowWidthB, Depth+1, cutoff_size, cutoff_depth);
    /* Step 1 of C12 = S4 x B22 + T1 + M5 */
    OptimizedStrassenMultiply_par(C12, S4, B22, QuadrantSize, RowWidthC, QuadrantSize, RowWidthB, Depth+1, cutoff_size, cutoff_depth);
    /* Step 1 of C21 = T2 - A22 * S8 */
    OptimizedStrassenMultiply_par(C21, A22, S8, QuadrantSize, RowWidthC, RowWidthA, QuadrantSize, Depth+1, cutoff_size, cutoff_depth);
  }

  /***************************************************************************
  ** Step through all columns row by row (vertically)
  ** (jumps in memory by RowWidth => bad locality)
  ** (but we want the best locality on the innermost loop)
  ***************************************************************************/
  for (Row = 0; Row < QuadrantSize; Row++) {
    /*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
    for (Column = 0; Column < QuadrantSize; Column += 4) {
      REAL LocalM5_0 = *(M5);
      REAL LocalM5_1 = *(M5+1);
      REAL LocalM5_2 = *(M5+2);
      REAL LocalM5_3 = *(M5+3);
      REAL LocalM2_0 = *(M2);
      REAL LocalM2_1 = *(M2+1);
      REAL LocalM2_2 = *(M2+2);
      REAL LocalM2_3 = *(M2+3);
      REAL T1_0 = *(T1sMULT) + LocalM2_0;
      REAL T1_1 = *(T1sMULT+1) + LocalM2_1;
      REAL T1_2 = *(T1sMULT+2) + LocalM2_2;
      REAL T1_3 = *(T1sMULT+3) + LocalM2_3;
      REAL T2_0 = *(C22) + T1_0;
      REAL T2_1 = *(C22+1) + T1_1;
      REAL T2_2 = *(C22+2) + T1_2;
      REAL T2_3 = *(C22+3) + T1_3;
      (*(C11))   += LocalM2_0;
      (*(C11+1)) += LocalM2_1;
      (*(C11+2)) += LocalM2_2;
      (*(C11+3)) += LocalM2_3;
      (*(C12))   += LocalM5_0 + T1_0;
      (*(C12+1)) += LocalM5_1 + T1_1;
      (*(C12+2)) += LocalM5_2 + T1_2;
      (*(C12+3)) += LocalM5_3 + T1_3;
      (*(C22))   = LocalM5_0 + T2_0;
      (*(C22+1)) = LocalM5_1 + T2_1;
      (*(C22+2)) = LocalM5_2 + T2_2;
      (*(C22+3)) = LocalM5_3 + T2_3;
      (*(C21  )) = (- *(C21  )) + T2_0;
      (*(C21+1)) = (- *(C21+1)) + T2_1;
      (*(C21+2)) = (- *(C21+2)) + T2_2;
      (*(C21+3)) = (- *(C21+3)) + T2_3;
      M5 += 4;
      M2 += 4;
      T1sMULT += 4;
      C11 += 4;
      C12 += 4;
      C21 += 4;
      C22 += 4;
    }
    C11 = (REAL*) ( ((PTR) C11 ) + RowIncrementC);
    C12 = (REAL*) ( ((PTR) C12 ) + RowIncrementC);
    C21 = (REAL*) ( ((PTR) C21 ) + RowIncrementC);
    C22 = (REAL*) ( ((PTR) C22 ) + RowIncrementC);
  }
  free(StartHeap);
}


void strassen_main_par(REAL *A, REAL *B, REAL *C, int n,
                       unsigned int cutoff_size, unsigned int cutoff_depth)
{
	#pragma omp parallel
	#pragma omp master
		OptimizedStrassenMultiply_par(C, A, B, n, n, n, n, 1, cutoff_size, cutoff_depth);
}
