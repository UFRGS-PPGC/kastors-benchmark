/**
 *
 * @file control.h
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
#ifndef _PLASMA_CONTROL_H_
#define _PLASMA_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 *  User routines
 **/
int PLASMA_Init(int cores);
int PLASMA_Finalize();

#ifdef __cplusplus
}
#endif

#endif
