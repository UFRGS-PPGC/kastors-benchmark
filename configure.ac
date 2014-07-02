AC_INIT([KaStORS],[1.0],[kastors-user@lists.gforge.inria.fr])
dnl AC_CONFIG_AUX_DIR: not before, not after
dnl http://www.mail-archive.com/autoconf@gnu.org/msg07824.html
AC_CONFIG_AUX_DIR([build-aux])
AC_CANONICAL_TARGET
AC_CONFIG_SRCDIR([sparselu/src/sparselu.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects foreign 1.9 tar-pax])
dnl Require Autoconf v2.62 for the AC_OPENMP macro
AC_PREREQ([2.62])

AC_PROG_CC(gcc icc clang llvm cc)
AC_PROG_CC_C99
AC_PROG_RANLIB

dnl Optimistic scenario!
KASTORS_COMPILE_JACOBI=yes
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
CFLAGS="$saved_CFLAGS"

if test "$OMP_DEP" = "yes";
then
        AC_MSG_RESULT([yes])
else
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([it appears your $CC compiler does not support OpenMP task dependencies. Aborting.])
fi

dnl PLASMA-related checks

AC_CHECK_HEADER([lapacke.h],
                [],
                [
        		AC_MSG_WARN([couldn[']t find lapacke.h header. Deactivating compilation of the PLASMA benchmarks.])
dnl '
        		KASTORS_COMPILE_PLASMA=no
        		KASTORS_MISSING_DEPS=lapacke.h
		]
)

AC_CHECK_LIB([cblas], [cblas_dgemm],
             [],
             [
                AC_MSG_WARN([couldn't find cblas_dgemm in -lcblas. Deactivating compilation of the PLASMA benchmarks.])
dnl '
                KASTORS_COMPILE_PLASMA=no
                KASTORS_MISSING_DEPS="$KASTORS_MISSING_DEPS -lcblas"
	     ]
)

ACX_PTHREAD([],[
                AC_MSG_WARN([couldn't find pthread_create in -lpthread. Deactivating compilation of the PLASMA benchmarks.])
dnl '
                KASTORS_COMPILE_PLASMA=no
                KASTORS_MISSING_DEPS="$KASTORS_MISSING_DEPS -lpthread"
])

AM_CONDITIONAL([COMPILE_JACOBI],[:])
AM_CONDITIONAL([COMPILE_STRASSEN],[:])
AM_CONDITIONAL([COMPILE_SPARSELU],[:])
AM_CONDITIONAL([COMPILE_PLASMA],[test "$KASTORS_COMPILE_PLASMA" != "no"])

dnl AC_SEARCH_LIBS(pthread_create, pthread)
dnl AC_SEARCH_LIBS(cblas_dgemm, cblas lapacke lapack blas m)

dnl -lcblas -llapacke -llapack -lblas -lpthread -lm -lgfortran

#AC_SUBST([abs_srcdir], [$(cd $srcdir && pwd)])

echo -e ""
echo -e "===== KaStORS configuration summary ====="
echo -e "Compiler: ""$CC"
echo -e "Flags:    ""$CFLAGS"

if test "$KASTORS_COMPILE_JACOBI" = "yes";
then
        echo -e "\twill compile JACOBI?\tyes"
else
        echo -e "\twill compile JACOBI?\tno"
fi

if test "$KASTORS_COMPILE_STRASSEN" = "yes";
then
        echo -e "\twill compile STRASSEN?\tyes"
else
        echo -e "\twill compile STRASSEN?\tno"
fi

if test "$KASTORS_COMPILE_SPARSELU" = "yes";
then
        echo -e "\twill compile SPARSELU?\tyes"
else
        echo -e "\twill compile SPARSELU?\tno"
fi

if test "$KASTORS_COMPILE_PLASMA" = "yes";
then
        echo -e "\twill compile PLASMA?\tyes"
else
        echo -e "\twill compile PLASMA?\tno (missing $KASTORS_MISSING_DEPS)"
fi
echo "========================================="

AC_CONFIG_FILES([
 Makefile
 strassen/Makefile
 jacobi/Makefile
 sparselu/Makefile
 plasma/Makefile
])
AC_OUTPUT

