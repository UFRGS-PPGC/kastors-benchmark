/**
 *
 * @file compute_d.h
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @generated d Tue Jan  7 11:45:15 2014
 *
 **/
#include <stdlib.h>

/***************************************************************************//**
 *  Macro for matrix conversion / Lapack interface
 **/
#define plasma_ddesc_alloc( descA, mb, nb, lm, ln, i, j, m, n, free)   \
    descA = plasma_desc_init(                                          \
        PlasmaRealDouble, (mb), (nb), ((mb)*(nb)),                  \
        (m), (n), (i), (j), (m), (n));                                 \
    if ( plasma_desc_mat_alloc( &(descA) ) ) {                         \
        plasma_error( __func__, "plasma_shared_alloc() failed");       \
        {free;};                                                       \
        abort();\
    }

#define plasma_diplap2tile( descA, A, mb, nb, lm, ln, i, j, m, n, seq, req) \
    descA = plasma_desc_init(                                         \
        PlasmaRealDouble, (mb), (nb), ((mb)*(nb)),                 \
        (lm), (ln), (i), (j), (m), (n));                              \
    descA.mat = A;                                                    \
    PLASMA_dgecfi_Async((lm), (ln), (A), PlasmaCM, (mb), (nb),        \
                        PlasmaCCRB, (mb), (nb), (seq), (req));

/***************************************************************************//**
 *  Declarations of internal sequential functions
 **/
void plasma_dpltmg_condex(PLASMA_desc A);
void plasma_dpltmg_house( PLASMA_desc A, unsigned long long int seed);

/***************************************************************************//**
 *  Declarations of parallel functions (dynamic scheduling) - alphabetical order
 **/
void plasma_pdgeadd_quark(double alpha, PLASMA_desc A, PLASMA_desc B);
void plasma_pdbarrier_tl2pnl_quark(PLASMA_desc A);
void plasma_pdbarrier_pnl2tl_quark(PLASMA_desc A);
void plasma_pdbarrier_tl2row_quark(PLASMA_desc A);
void plasma_pdbarrier_row2tl_quark(PLASMA_desc A);
void plasma_pdgbrdb_quark(PLASMA_enum uplo, PLASMA_desc A, double *D, double *E, PLASMA_desc T);
void plasma_pdgebrd_ge2gb_quark(PLASMA_desc A, PLASMA_desc T);
void plasma_pdgelqf_quark(PLASMA_desc A, PLASMA_desc T);
void plasma_pdgelqfrh_quark(PLASMA_desc A, PLASMA_desc T, int BS);
void plasma_pdgemm_quark(PLASMA_enum transA, PLASMA_enum transB, double alpha, PLASMA_desc A, PLASMA_desc B, double beta, PLASMA_desc C);
void plasma_pdgeqrf_quark(PLASMA_desc A, PLASMA_desc T);
void plasma_pdgeqrfrh_quark(PLASMA_desc A, PLASMA_desc T, int BS);
void plasma_pdgerbh_quark(PLASMA_desc A, PLASMA_desc T);
void plasma_pdgerbbrh_quark(PLASMA_desc A, PLASMA_desc T);
void plasma_pdgetmi2_quark(PLASMA_enum idep, PLASMA_enum odep, PLASMA_enum storev, int m, int n, int mb, int nb, double *A);
void plasma_pdgetrf_incpiv_quark(PLASMA_desc A, PLASMA_desc L, int *IPIV);
void plasma_pdgetrf_nopiv_quark( PLASMA_desc A);
void plasma_pdgetrf_tntpiv_quark(PLASMA_desc A, int *IPIV, PLASMA_desc W, int *Wi);
void plasma_pdgetrf_reclap_quark(PLASMA_desc A, int *IPIV);
void plasma_pdgetrf_rectil_quark(PLASMA_desc A, int *IPIV);
void plasma_pdsbcpy_t2bl_quark(PLASMA_enum uplo, PLASMA_desc A, double *AB, int LDAB);
void plasma_pdgbcpy_t2bl_quark(PLASMA_enum uplo, PLASMA_desc A, double *AB, int LDAB);
void plasma_pdsbrdt_quark(PLASMA_enum uplo, PLASMA_desc A, double *D, double *E, PLASMA_desc T);
void plasma_pdsygst_quark(PLASMA_enum itype, PLASMA_enum uplo, PLASMA_desc A, PLASMA_desc B);
#ifdef COMPLEX
void plasma_pdsymm_quark(PLASMA_enum side, PLASMA_enum uplo, double alpha, PLASMA_desc A, PLASMA_desc B, double beta, PLASMA_desc C);
void plasma_pdsyrk_quark(PLASMA_enum uplo, PLASMA_enum trans, double alpha, PLASMA_desc A, double beta, PLASMA_desc C);
void plasma_pdsyr2k_quark(PLASMA_enum uplo, PLASMA_enum trans, double alpha, PLASMA_desc A, PLASMA_desc B, double beta, PLASMA_desc C);
#endif
void plasma_pdsytrd_hb2st_v1_quark(PLASMA_enum uplo, int N, int NB, int Vblksiz, double *A, int LDA, double *V, double *TAU, double *D, double *E, int WANTZ);
void plasma_pdsytrd_he2hb_quark(PLASMA_enum uplo, PLASMA_desc A, PLASMA_desc T);
void plasma_pdlacpy_quark(PLASMA_enum uplo, PLASMA_desc A, PLASMA_desc B);
void plasma_pdlag2s_quark(PLASMA_desc A, PLASMA_desc SB);
void plasma_pdlange_quark(PLASMA_enum norm, PLASMA_desc A, double *work, double *result);
#ifdef COMPLEX
void plasma_pdlansy_quark(PLASMA_enum norm, PLASMA_enum uplo, PLASMA_desc A, double *work, double *result);
#endif
void plasma_pdlansy_quark(PLASMA_enum norm, PLASMA_enum uplo, PLASMA_desc A, double *work, double *result);
void plasma_pdlantr_quark(PLASMA_enum norm, PLASMA_enum uplo, PLASMA_enum diag, PLASMA_desc A, double *work, double *result);
void plasma_pdlaset_quark( PLASMA_enum uplo, double alpha, double beta, PLASMA_desc A);
void plasma_pdlaset2_quark(PLASMA_enum uplo, double alpha,                          PLASMA_desc A);
void plasma_pdlaswp_quark(PLASMA_desc B, const int *IPIV, int inc);
void plasma_pdlaswpc_quark(PLASMA_desc B, const int *IPIV, int inc);
void plasma_pdlauum_quark(PLASMA_enum uplo, PLASMA_desc A);
void plasma_pdplgsy_quark(double bump, PLASMA_desc A, unsigned long long int seed);
void plasma_pdplgsy_quark(double bump, PLASMA_desc A, unsigned long long int seed);
void plasma_pdpltmg_quark(PLASMA_enum mtxtype, PLASMA_desc A, unsigned long long int seed);
void plasma_pdpltmg_fiedler_quark(PLASMA_desc A, unsigned long long int seed);
void plasma_pdpltmg_toeppd_quark( PLASMA_desc A, unsigned long long int seed);
void plasma_pdpltmg_circul_quark( PLASMA_desc A, unsigned long long int seed);
void plasma_pdpltmg_chebvand_quark( PLASMA_desc A);
void plasma_pdpltmg_hankel_quark( PLASMA_desc A, unsigned long long int seed);
void plasma_pdpotrf_quark(PLASMA_enum uplo, PLASMA_desc A);
void plasma_pdshift_quark(int, int, int, double *, int *, int, int);
void plasma_pdsymm_quark(PLASMA_enum side, PLASMA_enum uplo, double alpha, PLASMA_desc A, PLASMA_desc B, double beta, PLASMA_desc C);
void plasma_pdsyrk_quark(PLASMA_enum uplo, PLASMA_enum trans, double alpha, PLASMA_desc A, double beta,  PLASMA_desc C);
void plasma_pdsyr2k_quark(PLASMA_enum uplo, PLASMA_enum trans, double alpha, PLASMA_desc A, PLASMA_desc B, double beta, PLASMA_desc C);
void plasma_pdtrmm_quark(PLASMA_enum side, PLASMA_enum uplo, PLASMA_enum transA, PLASMA_enum diag, double alpha, PLASMA_desc A, PLASMA_desc B);
void plasma_pdtrsm_quark(PLASMA_enum side, PLASMA_enum uplo, PLASMA_enum transA, PLASMA_enum diag, double alpha, PLASMA_desc A, PLASMA_desc B);
void plasma_pdtrsmpl_quark(PLASMA_desc A, PLASMA_desc B, PLASMA_desc L, const int *IPIV);
void plasma_pdtrsmrv_quark(PLASMA_enum side, PLASMA_enum uplo, PLASMA_enum transA, PLASMA_enum diag, double alpha, PLASMA_desc A, PLASMA_desc W);
void plasma_pdtrtri_quark(PLASMA_enum uplo, PLASMA_enum diag, PLASMA_desc A);
void plasma_pdorgbr_quark(PLASMA_enum side, PLASMA_desc A, PLASMA_desc O, PLASMA_desc T);
void plasma_pdorgbrrh_quark(PLASMA_enum side, PLASMA_desc A, PLASMA_desc O, PLASMA_desc T);
void plasma_pdorgqr_quark(PLASMA_desc A, PLASMA_desc Q, PLASMA_desc T);
void plasma_pdorgqrrh_quark(PLASMA_desc A, PLASMA_desc Q, PLASMA_desc T, int BS);
void plasma_pdorglq_quark(PLASMA_desc A, PLASMA_desc Q, PLASMA_desc T);
void plasma_pdorglqrh_quark(PLASMA_desc A, PLASMA_desc Q, PLASMA_desc T, int BS);
void plasma_pdorgtr_quark(PLASMA_enum uplo, PLASMA_desc A, PLASMA_desc Q, PLASMA_desc T);
void plasma_pdormqr_quark(PLASMA_enum side, PLASMA_enum trans, PLASMA_desc A, PLASMA_desc B, PLASMA_desc T);
void plasma_pdormlq_quark(PLASMA_enum side, PLASMA_enum trans, PLASMA_desc A, PLASMA_desc B, PLASMA_desc T);
void plasma_pdormlqrh_quark(PLASMA_enum side, PLASMA_enum trans, PLASMA_desc A, PLASMA_desc B, PLASMA_desc T, int BS);
