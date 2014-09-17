/**
 *
 * @file context.h
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @date 2010-11-15
 *
 **/
#ifndef _PLASMA_CONTEXT_H_
#define _PLASMA_CONTEXT_H_

#include <stdio.h>
#include "global.h"

struct plasma_context_struct;

/***************************************************************************//**
 *  PLASMA context
 **/
typedef struct plasma_context_struct {
    /* Initialization flag */
    PLASMA_bool initialized;

    /* Master-worker communication */
    volatile PLASMA_enum action;
    void (*parallel_func_ptr)(struct plasma_context_struct*);
    unsigned char args_buff[ARGS_BUFF_SIZE];

    /* Boolean flags */
    PLASMA_bool errors_enabled;
    PLASMA_bool warnings_enabled;
    PLASMA_bool dynamic_section;

    /* Matrix tile attributes */
    int nb;
    int ib;
    int nbnbsize;   // tile size in elements (possibly padded)
    int ibnbsize;   // T or L tile size in elements (---''---)
    int tntsize;    // Tournament pivoting grouping size

    /* Barrier implementation */
    /* Busy waiting version */
    volatile int barrier_in[CONTEXT_THREADS_MAX];
    volatile int barrier_out[CONTEXT_THREADS_MAX];

    /* Static scheduler implementation */
    int ss_ld;                  // static scheduler progress table leading dimension
    volatile int ss_abort;      // static scheduler abort flag
    volatile int *ss_progress;  // static scheduler progress table

} plasma_context_t;

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 *  Internal routines
 **/
plasma_context_t *plasma_context_create();
int plasma_context_insert(plasma_context_t *context);
int plasma_context_remove(plasma_context_t *context);
plasma_context_t *plasma_context_self();

#ifdef __cplusplus
}
#endif

#endif
