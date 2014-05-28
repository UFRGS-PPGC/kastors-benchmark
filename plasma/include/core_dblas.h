/**
 *
 * @file core_dblas.h
 *
 *  PLASMA auxiliary routines
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
#ifndef _PLASMA_CORE_DBLAS_H_
#define _PLASMA_CORE_DBLAS_H_

#define REAL

#ifdef __cplusplus
extern "C" {
#endif
#include "plasma.h"

/** ****************************************************************************
 *  Declarations of serial kernels - alphabetical order
 **/
void CORE_dasum(int storev, PLASMA_enum uplo, int M, int N,
                 const double *A, int lda, double *work);
void CORE_dbrdalg1(     PLASMA_enum uplo,
                        int n,
                        int nb,
                        double *A,
                        int lda,
                        double *VQ,
                        double *TAUQ,
                        double *VP,
                        double *TAUP,
                        int Vblksiz, int wantz,
                        int i, int sweepid, int m, int grsiz,
                        double *work);
int CORE_dgbelr(PLASMA_enum uplo, int N,
                PLASMA_desc *A, double *V, double *TAU,
                int st, int ed, int eltsize);
int CORE_dgbrce(PLASMA_enum uplo, int N,
                PLASMA_desc *A, double *V, double *TAU,
                int st, int ed, int eltsize);
int CORE_dgblrx(PLASMA_enum uplo, int N,
                PLASMA_desc *A, double *V, double *TAU,
                int st, int ed, int eltsize);
int CORE_dgeadd(int M, int N, double alpha,
                const double *A, int LDA,
                      double *B, int LDB);
int  CORE_dgelqt(int M, int N, int IB,
                 double *A, int LDA,
                 double *T, int LDT,
                 double *TAU,
                 double *WORK);
void CORE_dgemm(PLASMA_enum transA, PLASMA_enum transB,
                int M, int N, int K,
                double alpha, const double *A, int LDA,
                                          const double *B, int LDB,
                double beta,        double *C, int LDC);
void CORE_dgemv(PLASMA_enum trans, int M, int N,
                double alpha, const double *A, int LDA,
                                          const double *x, int incx,
                double beta,        double *y, int incy);
void CORE_dgeqp3_init( int n, int *jpvt );
void CORE_dgeqp3_larfg( PLASMA_desc A, int ii, int jj, int i, int j,
                        double *tau, double *beta );
void CORE_dgeqp3_norms( PLASMA_desc A, int ioff, int joff, double *norms1, double *norms2 );
void CORE_dgeqp3_pivot( PLASMA_desc A, double *F, int ldf,
                        int jj, int k, int *jpvt,
                        double *norms1, double *norms2, int *info );
int  CORE_dgeqp3_tntpiv(int m, int n,
                        double *A, int lda,
                        int *IPIV, double *tau,
                        int *iwork);
void CORE_dgeqp3_update( const double *Ajj, int lda1,
                         double       *Ajk, int lda2,
                         const double *Fk,  int ldf,
                         int joff, int k, int koff, int nb,
                         double *norms1, double *norms2,
                         int *info );
int  CORE_dgeqrt(int M, int N, int IB,
                 double *A, int LDA,
                 double *T, int LDT,
                 double *TAU, double *WORK);
int  CORE_dgessm(int M, int N, int K, int IB,
                 const int *IPIV,
                 const double *L, int LDL,
                 double *A, int LDA);
int  CORE_dgessq(int M, int N,
                 const double *A, int LDA,
                 double *scale, double *sumsq);
int  CORE_dgetf2_nopiv(int m, int n,
                      double *A, int lda);
int  CORE_dgetrf(int M, int N,
                 double *A, int LDA,
                 int *IPIV, int *INFO);
int  CORE_dgetrf_incpiv(int M, int N, int IB,
                        double *A, int LDA,
                        int *IPIV, int *INFO);
int  CORE_dgetrf_nopiv(int m, int n, int ib,
                      double *A, int lda);
int  CORE_dgetrf_reclap(int M, int N,
                        double *A, int LDA,
                        int *IPIV, int *info);
void CORE_dgetrf_reclap_init(void);
int  CORE_dgetrf_rectil(const PLASMA_desc A, int *IPIV, int *info);
void CORE_dgetrf_rectil_init(void);
void CORE_dgetrip(int m, int n, double *A,
                  double *work);
int CORE_dhbelr(PLASMA_enum uplo, int N,
                PLASMA_desc *A, double *V, double *TAU,
                int st, int ed, int eltsize);
int CORE_dhblrx(PLASMA_enum uplo, int N,
                PLASMA_desc *A, double *V, double *TAU,
                int st, int ed, int eltsize);
int CORE_dhbrce(PLASMA_enum uplo, int N,
                PLASMA_desc *A, double *V, double *TAU,
                int st, int ed, int eltsize);
void CORE_dsbtype1cb(int N, int NB,
                     double *A, int LDA,
                     double *V, double *TAU,
                     int st, int ed, int sweep, int Vblksiz, int WANTZ,
                     double *WORK);
void CORE_dsbtype2cb(int N, int NB,
                     double *A, int LDA,
                     double *V, double *TAU,
                     int st, int ed, int sweep, int Vblksiz, int WANTZ,
                     double *WORK);
void CORE_dsbtype3cb(int N, int NB,
                     double *A, int LDA,
                     const double *V, const double *TAU,
                     int st, int ed, int sweep, int Vblksiz, int WANTZ,
                     double *WORK);
void CORE_dgbtype1cb(PLASMA_enum uplo, int N, int NB,
                double *A, int LDA,
                double *VQ, double *TAUQ,
                double *VP, double *TAUP,
                int st, int ed, int sweep, int Vblksiz, int WANTZ,
                double *WORK);
void CORE_dgbtype2cb(PLASMA_enum uplo, int N, int NB,
                double *A, int LDA,
                double *VQ, double *TAUQ,
                double *VP, double *TAUP,
                int st, int ed, int sweep, int Vblksiz, int WANTZ,
                double *WORK);
void CORE_dgbtype3cb(PLASMA_enum uplo, int N, int NB,
                double *A, int LDA,
                double *VQ, double *TAUQ,
                double *VP, double *TAUP,
                int st, int ed, int sweep, int Vblksiz, int WANTZ,
                double *WORK);
void CORE_dsygst(int itype, PLASMA_enum uplo, int N,
                 double *A, int LDA,
                 double *B, int LDB, int *INFO);
#ifdef COMPLEX
void CORE_dsymm(PLASMA_enum side, PLASMA_enum uplo,
                int M, int N,
                double alpha, const double *A, int LDA,
                                          const double *B, int LDB,
                double beta,        double *C, int LDC);
void CORE_dsyrk(PLASMA_enum uplo, PLASMA_enum trans,
                int N, int K,
                double alpha, const double *A, int LDA,
                double beta,        double *C, int LDC);
void CORE_dsyr2k(PLASMA_enum uplo, PLASMA_enum trans,
                 int N, int K,
                 double alpha, const double *A, int LDA,
                                           const double *B, int LDB,
                 double beta,                    double *C, int LDC);
int  CORE_dhessq(PLASMA_enum uplo, int N,
                 const double *A, int LDA,
                 double *scale, double *sumsq);
#endif
int  CORE_dsyrfb(PLASMA_enum uplo, int N, int K, int IB, int NB,
                 const double *A,    int LDA,
                 const double *T,    int LDT,
                       double *C,    int LDC,
                       double *WORK, int LDWORK);
void CORE_dlacpy(PLASMA_enum uplo, int M, int N,
                 const double *A, int LDA,
                       double *B, int LDB);
int CORE_dlacpy_pivot( const PLASMA_desc descA,
                       PLASMA_enum direct,
                       int k1, int k2, const int *ipiv,
                       int *rankin, int *rankout,
                       double *A, int lda,
                       int init);
void CORE_dlange(int norm, int M, int N,
                 const double *A, int LDA,
                 double *work, double *normA);
#ifdef COMPLEX
void CORE_dlansy(int norm, PLASMA_enum uplo, int N,
                 const double *A, int LDA,
                 double *work, double *normA);
#endif
void CORE_dlansy(int norm, PLASMA_enum uplo, int N,
                 const double *A, int LDA,
                 double *work, double *normA);
void CORE_dlantr(PLASMA_enum norm, PLASMA_enum uplo, PLASMA_enum diag,
                 int M, int N,
                 const double *A, int LDA,
                 double *work, double *normA);
int CORE_dlarfb_gemm(PLASMA_enum side, PLASMA_enum trans, PLASMA_enum direct, PLASMA_enum storev,
                     int M, int N, int K,
                     const double *V, int LDV,
                     const double *T, int LDT,
                           double *C, int LDC,
                           double *WORK, int LDWORK);
int CORE_dlarfx2(PLASMA_enum side, int N,
                 double V,
                 double TAU,
                 double *C1, int LDC1,
                 double *C2, int LDC2);
int CORE_dlarfx2c(PLASMA_enum uplo,
                  double V,
                  double TAU,
                  double *C1,
                  double *C2,
                  double *C3);
int CORE_dlarfx2ce(PLASMA_enum uplo,
                   double *V,
                   double *TAU,
                   double *C1,
                   double *C2,
                   double *C3);
void CORE_dlarfy(int N,
                 double *A, int LDA,
                 const double *V,
                 const double *TAU,
                 double *WORK);
void CORE_dlaset(PLASMA_enum uplo, int n1, int n2,
                 double alpha, double beta,
                 double *tileA, int ldtilea);
void CORE_dlaset2(PLASMA_enum uplo, int n1, int n2, double alpha,
                  double *tileA, int ldtilea);
void CORE_dlaswp(int N, double *A, int LDA,
                 int I1,  int I2, const int *IPIV, int INC);
int  CORE_dlaswp_ontile( PLASMA_desc descA, int i1, int i2, const int *ipiv, int inc);
int  CORE_dlaswpc_ontile(PLASMA_desc descA, int i1, int i2, const int *ipiv, int inc);
int  CORE_dlatro(PLASMA_enum uplo, PLASMA_enum trans,
                 int M, int N,
                 const double *A, int LDA,
                       double *B, int LDB);
void CORE_dlauum(PLASMA_enum uplo, int N, double *A, int LDA);
int CORE_dpamm(int op, PLASMA_enum side, PLASMA_enum storev,
               int M, int N, int K, int L,
               const double *A1, int LDA1,
                     double *A2, int LDA2,
               const double *V, int LDV,
                     double *W, int LDW);
int  CORE_dparfb(PLASMA_enum side, PLASMA_enum trans, PLASMA_enum direct, PLASMA_enum storev,
                 int M1, int N1, int M2, int N2, int K, int L,
                       double *A1, int LDA1,
                       double *A2, int LDA2,
                 const double *V, int LDV,
                 const double *T, int LDT,
                       double *WORK, int LDWORK);
int CORE_dpemv(PLASMA_enum trans, PLASMA_enum storev,
               int M, int N, int L,
               double ALPHA,
               const double *A, int LDA,
               const double *X, int INCX,
               double BETA,
               double *Y, int INCY,
               double *WORK);
void CORE_dplgsy(double bump, int m, int n, double *A, int lda,
                 int bigM, int m0, int n0, unsigned long long int seed );
void CORE_dplgsy(double bump, int m, int n, double *A, int lda,
                 int bigM, int m0, int n0, unsigned long long int seed );
void CORE_dplrnt(int m, int n, double *A, int lda,
                 int bigM, int m0, int n0, unsigned long long int seed );
int  CORE_dpltmg(PLASMA_enum mtxtype, int m, int n, double *A, int lda,
                  int gM, int gN, int m0, int n0, unsigned long long int seed );
int  CORE_dpltmg_chebvand( int M, int N, double *A, int LDA,
                           int gN, int m0, int n0,
                           double *W );
int  CORE_dpltmg_circul( int M, int N, double *A, int LDA,
                         int gM, int m0, int n0,
                         const double *V );
void CORE_dpltmg_condexq( int M, int N, double *Q, int LDQ );
void CORE_dpltmg_fiedler(int m, int n,
                         const double *X, int incX,
                         const double *Y, int incY,
                         double *A, int lda);
int  CORE_dpltmg_hankel( PLASMA_enum uplo, int M, int N, double *A, int LDA,
                         int m0, int n0, int nb,
                         const double *V1,
                         const double *V2 );
void CORE_dpltmg_toeppd1( int gM, int m0, int M, double *W,
                          unsigned long long int seed );
void CORE_dpltmg_toeppd2( int M, int N, int K, int m0, int n0,
                          const double *W,
                          double *A, int LDA );
void CORE_dpotrf(PLASMA_enum uplo, int N, double *A, int LDA, int *INFO);
void CORE_dsetvar(const double *alpha, double *x);
void CORE_dshift(int s, int m, int n, int L,
                 double *A);
void CORE_dshiftw(int s, int cl, int m, int n, int L,
                  double *A, double *W);
int  CORE_dssssm(int M1, int N1, int M2, int N2, int K, int IB,
                       double *A1, int LDA1,
                       double *A2, int LDA2,
                 const double *L1, int LDL1,
                 const double *L2, int LDL2,
                 const int *IPIV);
void CORE_dsymm(PLASMA_enum side, PLASMA_enum uplo,
                int M, int N,
                double alpha, const double *A, int LDA,
                                          const double *B, int LDB,
                double beta,        double *C, int LDC);
void CORE_dsyrk(PLASMA_enum uplo, PLASMA_enum trans,
                int N, int K,
                double alpha, const double *A, int LDA,
                double beta,        double *C, int LDC);
void CORE_dsyr2k(PLASMA_enum uplo, PLASMA_enum trans,
                 int N, int K,
                 double alpha, const double *A, int LDA,
                                           const double *B, int LDB,
                 double beta,        double *C, int LDC);
int  CORE_dsyssq(PLASMA_enum uplo, int N,
                 const double *A, int LDA,
                 double *scale, double *sumsq);
void CORE_dswpab(int i, int n1, int n2,
                 double *A, double *work);
int  CORE_dswptr_ontile(PLASMA_desc descA, int i1, int i2, const int *ipiv, int inc,
                        const double *Akk, int ldak);
void CORE_dtrasm(PLASMA_enum storev, PLASMA_enum uplo, PLASMA_enum diag,
                 int M, int N, const double *A, int lda, double *work);
void CORE_dtrdalg1(int n,
                        int nb,
                        double *A,
                        int lda,
                        double *V,
                        double *TAU,
                        int Vblksiz, int wantz,
                        int i, int sweepid, int m, int grsiz,
                        double *work);
void CORE_dtrmm(PLASMA_enum side, PLASMA_enum uplo,
                PLASMA_enum transA, PLASMA_enum diag,
                int M, int N,
                double alpha, const double *A, int LDA,
                                                double *B, int LDB);
void CORE_dtrsm(PLASMA_enum side, PLASMA_enum uplo,
                PLASMA_enum transA, PLASMA_enum diag,
                int M, int N,
                double alpha, const double *A, int LDA,
                                                double *B, int LDB);
int  CORE_dtrssq(PLASMA_enum uplo, PLASMA_enum diag, int M, int N,
                 const double *A, int LDA,
                 double *scale, double *sumsq);
void CORE_dtrtri(PLASMA_enum uplo, PLASMA_enum diag, int N,
                 double *A, int LDA, int *info);
int  CORE_dtslqt(int M, int N, int IB,
                 double *A1, int LDA1,
                 double *A2, int LDA2,
                 double *T, int LDT,
                 double *TAU, double *WORK);
int  CORE_dtsmlq(PLASMA_enum side, PLASMA_enum trans,
                 int M1, int N1, int M2, int N2, int K, int IB,
                 double *A1, int LDA1,
                 double *A2, int LDA2,
                 const double *V, int LDV,
                 const double *T, int LDT,
                 double *WORK, int LDWORK);
int CORE_dtsmlq_corner( int m1, int n1, int m2, int n2, int m3, int n3,
                        int k, int ib, int nb,
                        double *A1, int lda1,
                        double *A2, int lda2,
                        double *A3, int lda3,
                        const double *V, int ldv,
                        const double *T, int ldt,
                        double *WORK, int ldwork);
int CORE_dtsmlq_sytra1( PLASMA_enum side, PLASMA_enum trans,
                        int m1, int n1, int m2, int n2,
                        int k, int ib,
                        double *A1, int lda1,
                        double *A2, int lda2,
                        const double *V, int ldv,
                        const double *T, int ldt,
                        double *WORK, int ldwork);
int  CORE_dtsmqr(PLASMA_enum side, PLASMA_enum trans,
                 int M1, int N1, int M2, int N2, int K, int IB,
                 double *A1, int LDA1,
                 double *A2, int LDA2,
                 const double *V, int LDV,
                 const double *T, int LDT,
                 double *WORK, int LDWORK);
int CORE_dtsmqr_corner( int m1, int n1, int m2, int n2, int m3, int n3,
                        int k, int ib, int nb,
                        double *A1, int lda1,
                        double *A2, int lda2,
                        double *A3, int lda3,
                        const double *V, int ldv,
                        const double *T, int ldt,
                        double *WORK, int ldwork);
int CORE_dtsmqr_sytra1( PLASMA_enum side, PLASMA_enum trans,
                        int m1, int n1, int m2, int n2,
                        int k, int ib,
                        double *A1, int lda1,
                        double *A2, int lda2,
                        const double *V, int ldv,
                        const double *T, int ldt,
                        double *WORK, int ldwork);
int  CORE_dtsqrt(int M, int N, int IB,
                 double *A1, int LDA1,
                 double *A2, int LDA2,
                 double *T, int LDT,
                 double *TAU, double *WORK);
int  CORE_dtstrf(int M, int N, int IB, int NB,
                 double *U, int LDU,
                 double *A, int LDA,
                 double *L, int LDL,
                 int *IPIV, double *WORK,
                 int LDWORK, int *INFO);
int  CORE_dttmqr(PLASMA_enum side, PLASMA_enum trans,
                 int M1, int N1, int M2, int N2, int K, int IB,
                 double *A1, int LDA1,
                 double *A2, int LDA2,
                 const double *V, int LDV,
                 const double *T, int LDT,
                 double *WORK, int LDWORK);
int  CORE_dttqrt(int M, int N, int IB,
                 double *A1, int LDA1,
                 double *A2, int LDA2,
                 double *T, int LDT,
                 double *TAU,
                 double *WORK);
int  CORE_dttmlq(PLASMA_enum side, PLASMA_enum trans,
                 int M1, int N1, int M2, int N2, int K, int IB,
                 double *A1, int LDA1,
                 double *A2, int LDA2,
                 const double *V, int LDV,
                 const double *T, int LDT,
                 double *WORK, int LDWORK);
int  CORE_dttlqt(int M, int N, int IB,
                 double *A1, int LDA1,
                 double *A2, int LDA2,
                 double *T, int LDT,
                 double *TAU,
                 double *WORK);
int  CORE_dormlq(PLASMA_enum side, PLASMA_enum trans,
                 int M, int N, int IB, int K,
                 const double *V, int LDV,
                 const double *T, int LDT,
                 double *C, int LDC,
                 double *WORK, int LDWORK);
int  CORE_dormqr(PLASMA_enum side, PLASMA_enum trans,
                 int M, int N, int K, int IB,
                 const double *V, int LDV,
                 const double *T, int LDT,
                 double *C, int LDC,
                 double *WORK, int LDWORK);


#ifdef __cplusplus
}
#endif

#undef COMPLEX

#endif
