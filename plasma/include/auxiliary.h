/**
 *
 * @file control/auxiliary.h
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @author Piotr Luszczek
 * @author Emmanuel Agullo
 * @date 2010-11-15
 *
 **/
#ifndef _PLASMA_AUXILIARY_H_
#define _PLASMA_AUXILIARY_H_

#ifndef max
#define max(a,b) ( ( (a) > (b) ) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) ( ( (a) < (b) ) ? (a) : (b))
#endif

#include "context.h"
#include "common.h"
#include "dauxiliary.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 *  Internal routines
 **/
void plasma_warning(const char *func_name, char* msg_text);
void plasma_error(const char *func_name, char* msg_text);
void plasma_fatal_error(const char *func_name, char* msg_text);
void plasma_memcpy(void *dst, void *src, PLASMA_size size, int type);
void plasma_memzero(void *memptr, PLASMA_size size, int type);
void plasma_memset_int(int *mem, int size, int value);
int  plasma_rank(plasma_context_t *plasma);
int  plasma_tune(PLASMA_enum func, int M, int N, int NRHS);


extern int IONE;
extern int ISEED[4];

extern int format[6];
extern int trans[3];
extern int uplo[2];
extern int side[2];
extern int diag[2];
extern char *formatstr[6];
extern char *transstr[3];
extern char *uplostr[2];
extern char *sidestr[2];
extern char *diagstr[2];

extern void *formatmap[6];

int map_CM  (int m, int n, int mb, int nb, int i, int j);
int map_CCRB(int m, int n, int mb, int nb, int i, int j);
int map_CRRB(int m, int n, int mb, int nb, int i, int j);
int map_RCRB(int m, int n, int mb, int nb, int i, int j);
int map_RRRB(int m, int n, int mb, int nb, int i, int j);
int map_RM  (int m, int n, int mb, int nb, int i, int j);
#ifdef __cplusplus
}
#endif

#endif
