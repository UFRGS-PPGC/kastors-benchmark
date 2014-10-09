AC_INIT([KaStORS],[1.0],[kastors-user@lists.gforge.inria.fr])
dnl AC_CONFIG_AUX_DIR: not before, not after
dnl http://www.mail-archive.com/autoconf@gnu.org/msg07824.html
AC_CONFIG_AUX_DIR([build-aux])
AC_CANONICAL_TARGET
AC_CONFIG_SRCDIR([sparselu/src/sparselu.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_LINKS([run.sh:run.sh])
AM_INIT_AUTOMAKE([subdir-objects foreign 1.9 tar-pax])
dnl Require Autoconf v2.62 for the AC_OPENMP macro
AC_PREREQ([2.62])

AC_PROG_CC(gcc icc clang llvm cc)
AC_PROG_CC_C99
AC_PROG_RANLIB

dnl Optimistic scenario!
KASTORS_COMPILE_STRASSEN=yes
KASTORS_COMPILE_SPARSELU=yes
KASTORS_COMPILE_PLASMA=yes

AC_LANG([C])

dnl Check for OpenMP support
AC_OPENMP
if test "$ac_cv_prog_c_openmp" = "unsupported";
then
        AC_MSG_ERROR([it appears your $CC compiler does not support OpenMP. Aborting.])
fi

saved_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $OPENMP_CFLAGS"
dnl Check for the support of OpenMP task dependencies
AC_MSG_CHECKING([for $CC to support OpenMP task dependencies])
AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM([[int x;]],[[#pragma omp task depend(out: x)
  x = 10;]])],
        [OMP_DEP=yes],
        [OMP_DEP=no]
)

if test "$OMP_DEP" = "yes";
then
        AC_MSG_RESULT([yes])
else
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([it appears your $CC compiler does not support OpenMP task dependencies. Aborting.])
fi

CFLAGS="$saved_CFLAGS -O2 -g"

dnl Add possibility to disable jacobi
AC_ARG_ENABLE([jacobi],
              AS_HELP_STRING([--disable-jacobi], [Disable jacobi benchs]))
KASTORS_COMPILE_JACOBI=no
AS_IF([test "x$enable_jacobi" != "xno"], [
KASTORS_COMPILE_JACOBI=yes
     ])

dnl Add possibility to disable strassen
AC_ARG_ENABLE([strassen],
              AS_HELP_STRING([--disable-strassen], [Disable strassen benchs]))
KASTORS_COMPILE_STRASSEN=no
AS_IF([test "x$enable_strassen" != "xno"], [
KASTORS_COMPILE_STRASSEN=yes
     ])

dnl Add possibility to disable sparselu
AC_ARG_ENABLE([sparselu],
              AS_HELP_STRING([--disable-sparselu], [Disable sparselu benchs]))
KASTORS_COMPILE_SPARSELU=no
AS_IF([test "x$enable_sparselu" != "xno"], [
KASTORS_COMPILE_SPARSELU=yes
     ])

dnl Add possibility to disable plasma
AC_ARG_ENABLE([plasma],
              AS_HELP_STRING([--disable-plasma], [Disable plasma benchs]))
KASTORS_COMPILE_PLASMA=no
AS_IF([test "x$enable_plasma" != "xno"], [
KASTORS_COMPILE_PLASMA=yes
     ])

dnl Check math support
AC_CHECK_LIB([m], [sqrt],
             [],
             [
                AC_MSG_ERROR([couldn't find sqrt in -lm])
dnl '
	     ]
)

dnl Check blas availability
dnl PLASMA-related checks

AC_CHECK_HEADER([lapacke.h],
                [],
                [
        		AC_MSG_WARN([couldn't find lapacke.h header. Deactivating compilation of the PLASMA benchmarks.])
dnl '
        		KASTORS_COMPILE_PLASMA=no
        		KASTORS_MISSING_DEPS=lapacke.h
		]
)


dnl Add possibility to choose blas
AC_ARG_WITH([blas],
            [AS_HELP_STRING([--with-blas],
                            [specifity blas you want to use(def=best)])],
            [],
            [with_blas=best])

AS_IF( [test "x$with_blas" == "xbest" ],
      [
        saved_LIBS=$LIBS
        LIBS=
        AC_SEARCH_LIBS([cblas_dgemm], [openblas cblas],
                       [
                        echo "Use ${ac_cv_search_cblas_dgemm}"
                        PLASMA_comLIBS=$LIBS
                        ],
                       [
                        AC_MSG_WARN([couldn't find cblas_dgemm in openblas or cblas. Deactivating compilation of the PLASMA benchmarks.])
dnl '
                        KASTORS_COMPILE_PLASMA=no
                        KASTORS_MISSING_DEPS="$KASTORS_MISSING_DEPS -lcblas"
                        ])
        LIBS=$saved_LIBS
      ], [
AS_IF( [test "x$with_blas" == "xcblas" ],
      [
AC_CHECK_LIB([cblas], [cblas_dgemm],
             [
                PLASMA_comLIBS="-lcblas"
              ],
             [
            AC_MSG_WARN([couldn't find cblas_dgemm in -cblas. Deactivating compilation of the PLASMA benchmarks.])
dnl '
            KASTORS_COMPILE_PLASMA=no
            KASTORS_MISSING_DEPS="$KASTORS_MISSING_DEPS -lcblas"
         ])
       ],[
AS_IF( [test "x$with_blas" == "xopenblas" ],
        [
AC_CHECK_LIB([openblas], [cblas_dgemm],
           [
              PLASMA_comLIBS="-lopenblas"
            ],
           [
              AC_MSG_WARN([couldn't find cblas_dgemm in -openblas. Deactivating compilation of the PLASMA benchmarks.])
dnl '
              KASTORS_COMPILE_PLASMA=no
              KASTORS_MISSING_DEPS="$KASTORS_MISSING_DEPS -lopenblas"
           ])
          ],[
          PLASMA_comLIBS=$with_blas
          ])
        ])
      ])

AC_SUBST([plasma_comLIBS], ["$PLASMA_comLIBS"])

dnl Check clock_gettime exist on unix systems
case $host in
    *linux*)
AC_CHECK_LIB([rt], [clock_gettime],
             [],
             [
                AC_MSG_ERROR([couldn't find clock_gettime in -lrt.])
dnl '
                KASTORS_MISSING_DEPS="$KASTORS_MISSING_DEPS -lrt"
	     ]
);;
esac

AM_CONDITIONAL([COMPILE_JACOBI],[test "$KASTORS_COMPILE_JACOBI" != "no"])
AM_CONDITIONAL([COMPILE_STRASSEN],[test "$KASTORS_COMPILE_STRASSEN" != "no"])
AM_CONDITIONAL([COMPILE_SPARSELU],[test "$KASTORS_COMPILE_SPARSELU" != "no"])
AM_CONDITIONAL([COMPILE_PLASMA],[test "$KASTORS_COMPILE_PLASMA" != "no"])

echo -e ""
echo -e "===== KaStORS configuration summary ====="
echo -e "Compiler: ""$CC"
echo -e "Flags:    ""$CFLAGS"
echo -e "\twill compile JACOBI?\t$KASTORS_COMPILE_JACOBI"
echo -e "\twill compile STRASSEN?\t$KASTORS_COMPILE_STRASSEN"
echo -e "\twill compile SPARSELU?\t$KASTORS_COMPILE_SPARSELU"
echo -e "\twill compile PLASMA?\t$KASTORS_COMPILE_PLASMA"
echo "========================================="

AC_CONFIG_FILES([
 Makefile
 strassen/Makefile
 jacobi/Makefile
 sparselu/Makefile
 plasma/Makefile
])
AC_OUTPUT

