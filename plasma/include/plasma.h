/**
 *
 * @file plasma.h
 *
 *  PLASMA main header
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @date 2010-11-15
 *
 **/
#ifndef _PLASMA_H_
#define _PLASMA_H_

#include <stdio.h>
#include "plasmatypes.h"


/** ****************************************************************************
 *  Tile matrix descriptor
 *
 *  Matrices are stored in a contiguous data chunk containing in order
 *  A11, A21, A12, A22 with :
 *
 *           n1      n2
 *      +----------+---+
 *      |          |   |    With m1 = lm - (lm%mb)
 *      |          |   |         m2 = lm%mb
 *  m1  |    A11   |A12|         n1 = ln - (ln%nb)
 *      |          |   |         n2 = ln%nb
 *      |          |   |
 *      +----------+---+
 *  m2  |    A21   |A22|
 *      +----------+---+
 *
 */
typedef struct plasma_desc_t {
    void *mat;          /**< pointer to the beginning of the matrix                           */
    size_t A21;         /**< pointer to the beginning of the matrix A21                       */
    size_t A12;         /**< pointer to the beginning of the matrix A12                       */
    size_t A22;         /**< pointer to the beginning of the matrix A22                       */
    PLASMA_enum dtyp;   /**< precision of the matrix                                          */
    int mb;             /**< number of rows in a tile                                         */
    int nb;             /**< number of columns in a tile                                      */
    int bsiz;           /**< size in elements including padding                               */
    int lm;             /**< number of rows of the entire matrix                              */
    int ln;             /**< number of columns of the entire matrix                           */
    int lm1;            /**< number of tile rows of the A11 matrix - derived parameter        */
    int ln1;            /**< number of tile columns of the A11 matrix - derived parameter     */
    int lmt;            /**< number of tile rows of the entire matrix - derived parameter     */
    int lnt;            /**< number of tile columns of the entire matrix - derived parameter  */
    int i;              /**< row index to the beginning of the submatrix                      */
    int j;              /**< column index to the beginning of the submatrix                   */
    int m;              /**< number of rows of the submatrix                                  */
    int n;              /**< number of columns of the submatrix                               */
    int mt;             /**< number of tile rows of the submatrix - derived parameter         */
    int nt;             /**< number of tile columns of the submatrix - derived parameter      */
} PLASMA_desc;

#define plasma_const_neg(const) (((const-1)^0x01)+1)

/** ****************************************************************************
 *  PLASMA constants - boolean
 **/
#define PLASMA_FALSE 0
#define PLASMA_TRUE  1

/** ****************************************************************************
 *  State machine switches
 **/
#define PLASMA_WARNINGS   1
#define PLASMA_ERRORS     2
#define PLASMA_DAG        4

/** ****************************************************************************
 *  PLASMA constants - configuration parameters
 **/
#define PLASMA_CONCURRENCY      1
#define PLASMA_TILE_SIZE        2
#define PLASMA_INNER_BLOCK_SIZE 3
#define PLASMA_TNTPIVOTING_SIZE 9

/** ****************************************************************************
 *  Math function prototypes
 **/
/*#include <plasma_z.h>*/
#include "plasma_d.h"
/*#include <plasma_c.h>*/
/*#include <plasma_s.h>*/
/*#include <plasma_zc.h>*/
#include "plasma_ds.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ****************************************************************************
 *  Auxiliary function prototypes
 **/
int PLASMA_Version(int *ver_major, int *ver_minor, int *ver_micro);
int PLASMA_Enable(PLASMA_enum lever);
int PLASMA_Disable(PLASMA_enum lever);
int PLASMA_Set(PLASMA_enum param, int value);
int PLASMA_Get(PLASMA_enum param, int *value);
int PLASMA_Init(int cores);
int PLASMA_Finalize();
int PLASMA_Desc_Create(PLASMA_desc **desc, void *mat, PLASMA_enum dtyp, int mb, int nb, int bsiz, int lm, int ln, int i, int j, int m, int n);
int PLASMA_Desc_Destroy(PLASMA_desc **desc);
int PLASMA_Lapack_to_Tile(void *Af77, int LDA, PLASMA_desc *A);
int PLASMA_Tile_to_Lapack(PLASMA_desc *A, void *Af77, int LDA);

/** ****************************************************************************
 *  Workspace deallocation
 **/
int PLASMA_Dealloc_Handle(void **handle);
int PLASMA_Dealloc_Handle_Tile(PLASMA_desc **desc);

#ifdef __cplusplus
}
#endif

#endif
