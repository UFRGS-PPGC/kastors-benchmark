/**
 *
 * @file control/auxiliary.c
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
#include "common.h"
#include "auxiliary.h"

#include <stdio.h>
#include <stdlib.h>

/***************************************************************************//**
 *
 *  Indicates a recoverable problem.
 *  User's erroneous action without severe consequences.
 *  Problems occuring while PLASMA is being used correctly.
 *  Context aware.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 ******************************************************************************/
void plasma_warning(const char *func_name, char* msg_text)
{
    plasma_context_t *plasma;

    plasma = plasma_context_self();
    if (plasma == NULL)
        plasma_fatal_error("plasma_warning", "PLASMA not initialized");
    if (plasma->warnings_enabled)
        fprintf(stderr, "PLASMA WARNING: %s(): %s\n", func_name, msg_text);
}

/***************************************************************************//**
 *
 *  Indicates a recoverable problem.
 *  User's erroneous action with potentially severe consequences.
 *  Problems occuring due to incorrect use of PLASMA.
 *  Context aware.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 ******************************************************************************/
void plasma_error(const char *func_name, char* msg_text)
{
    plasma_context_t *plasma;

    plasma = plasma_context_self();
    if (plasma == NULL)
        plasma_fatal_error("plasma_error", "PLASMA not initialized");
    if (plasma->errors_enabled)
        fprintf(stderr, "PLASMA ERROR: %s(): %s\n", func_name, msg_text);

}

/***************************************************************************//**
 *
 *  Unexpected behavior within the library.
 *  Unrecoverable user errors.
 *  Context oblivious.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 ******************************************************************************/
void plasma_fatal_error(const char *func_name, char* msg_text)
{
    fprintf(stderr, "PLASMA FATAL ERROR: %s(): %s\n", func_name, msg_text);
    exit(0);
}

/***************************************************************************//**
 *
 **/
void plasma_memset_int(int *mem, int size, int value)
{
    int i;

    for (i = 0; i < size; i++)
        mem[i] = value;
}

int   format[6]= { PlasmaCM, PlasmaCCRB, PlasmaCRRB, PlasmaRCRB, PlasmaRRRB, PlasmaRM };
int   side[2]  = { PlasmaLeft,    PlasmaRight };
int   uplo[2]  = { PlasmaUpper,   PlasmaLower };
int   diag[2]  = { PlasmaNonUnit, PlasmaUnit  };
int   trans[3] = { PlasmaNoTrans, PlasmaTrans, PlasmaConjTrans };

char *formatstr[6]= { "CM", "RM", "CCRB", "CRRB", "RCRB", "RRRB"};
char *sidestr[2]  = { "Left ", "Right" };
char *uplostr[2]  = { "Upper", "Lower" };
char *diagstr[2]  = { "NonUnit", "Unit   " };
char *transstr[3] = { "N", "T", "H" };

/*-------------------------------------------------------------------
 * Mapping for the different layouts
 */

#define map_cm(m, n, i, j)   ((i) + (j) * (m))
#define map_rm(m, n, i, j)   ((i) * (n) + (j))

int map_CM(  int m, int n, int mb, int nb, int i, int j) { return map_cm(m, n, i, j); }
int map_RM(  int m, int n, int mb, int nb, int i, int j) { return map_rm(m, n, i, j); }
int map_CCRB(int m, int n, int mb, int nb, int i, int j) {
    int m0 = m - m%mb;
    int n0 = n - n%nb;
    if ( j < n0 )
        if (i < m0)
            /* Case in A11 */
            return ( map_cm( m/mb, n/nb, i/mb, j/nb )*mb*nb + map_cm( mb,   nb,   i%mb, j%nb) );
        else
            /* Case in A21 */
            return ( m0*n0 + ( (j/nb) * (nb*(m%mb)) )       + map_cm( m%mb, nb,   i%mb, j%nb) );
    else
        if (i < m0)
            /* Case in A12 */
            return ( m*n0  + ( (i/mb) * (mb*(n%nb)) )       + map_cm( mb,   n%nb, i%mb, j%nb) );
        else
            /* Case in A22 */
            return ( m*n0  + (n-n0)*m0                      + map_cm( m%mb, n%nb, i%mb, j%nb) );
}

int map_CRRB(int m, int n, int mb, int nb, int i, int j) {
    int m0 = m - m%mb;
    int n0 = n - n%nb;
    if ( j < n0 )
        if (i < m0)
            /* Case in A11 */
            return ( map_cm( m/mb, n/nb, i/mb, j/nb )*mb*nb + map_rm( mb,   nb,   i%mb, j%nb) );
        else
            /* Case in A21 */
            return ( m0*n0 + ( (j/nb) * (nb*(m%mb)) )       + map_rm( m%mb, nb,   i%mb, j%nb) );
    else
        if (i < m0)
            /* Case in A12 */
            return ( m*n0  + ( (i/mb) * (mb*(n%nb)) )       + map_rm( mb,   n%nb, i%mb, j%nb) );
        else
            /* Case in A22 */
            return ( m*n0  + (n-n0)*m0                      + map_rm( m%mb, n%nb, i%mb, j%nb) );
}

int map_RCRB(int m, int n, int mb, int nb, int i, int j) {
    int m0 = m - m%mb;
    int n0 = n - n%nb;
    if ( j < n0 )
        if (i < m0)
            /* Case in A11 */
            return ( map_rm( m/mb, n/nb, i/mb, j/nb )*mb*nb + map_cm( mb,   nb,   i%mb, j%nb) );
        else
            /* Case in A21 */
            return ( m0*n  + ( (j/nb) * (nb*(m%mb)) )       + map_cm( m%mb, nb,   i%mb, j%nb) );
    else
        if (i < m0)
            /* Case in A12 */
            return ( m0*n0 + ( (i/mb) * (mb*(n%nb)) )       + map_cm( mb,   n%nb, i%mb, j%nb) );
        else
            /* Case in A22 */
            return ( m*n0  + (n-n0)*m0                      + map_cm( m%mb, n%nb, i%mb, j%nb) );
}

int map_RRRB(int m, int n, int mb, int nb, int i, int j) {
    int m0 = m - m%mb;
    int n0 = n - n%nb;
    if ( j < n0 )
        if (i < m0)
            /* Case in A11 */
            return ( map_rm( m/mb, n/nb, i/mb, j/nb )*mb*nb + map_rm( mb,   nb,   i%mb, j%nb) );
        else
            /* Case in A21 */
            return ( m0*n  + ( (j/nb) * (nb*(m%mb)) )       + map_rm( m%mb, nb,   i%mb, j%nb) );
    else
        if (i < m0)
            /* Case in A12 */
            return ( m0*n0 + ( (i/mb) * (mb*(n%nb)) )       + map_rm( mb,   n%nb, i%mb, j%nb) );
        else
            /* Case in A22 */
            return ( m*n0  + (n-n0)*m0                      + map_rm( m%mb, n%nb, i%mb, j%nb) );
}

void *formatmap[6] = {  map_CM, map_RM, map_CCRB, map_CRRB, map_RCRB, map_RRRB };

