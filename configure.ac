AC_INIT(KaStORS, 1.0)
AM_INIT_AUTOMAKE([subdir-objects])
dnl Require Autoconf v2.62 for the AC_OPENMP macro
AC_PREREQ([2.62])
AC_CONFIG_FILES([
 Makefile
 strassen/Makefile
 jacobi/Makefile
 sparselu/Makefile
])
AC_PROG_CC(gcc icc clang llvm cc)
AC_PROG_CC_C99

dnl Check for OpenMP support
AC_OPENMP
if test "$ac_cv_prog_c_openmp" = "unsupported";
then
        AC_MSG_ERROR([it appears your $CC compiler does not support OpenMP. Aborting.])
fi

CFLAGS="$CFLAGS $OPENMP_CFLAGS"
AC_LANG([C])

dnl Check for the support of OpenMP task dependencies

AC_MSG_CHECKING([for $CC to support OpenMP task dependencies])
AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM([[int x;]],[[#pragma omp task depend(out: x)
  x = 10;]])],
        [AC_SUBST([OMP_DEP], [yes])],
        [AC_SUBST([OMP_DEP], [no])]
)

if test "$OMP_DEP" = "yes";
then
        AC_SUBST([KASTORS_COMPILE_JACOBI], [yes])
        AC_SUBST([KASTORS_COMPILE_STRASSEN], [yes])
        AC_SUBST([KASTORS_COMPILE_SPARSELU], [yes])
        AC_MSG_RESULT([yes])
else
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([it appears your $CC compiler does not support OpenMP task dependencies. Aborting.])
fi

dnl PLASMA-related checks

AC_CHECK_HEADER([lapacke.h],
                [AC_SUBST([HAVE_LAPACKE_HEADER], [yes])],
                [AC_SUBST([HAVE_LAPACKE_HEADER], [no])]
)

if test "$HAVE_LAPACKE_HEADER" = "no";
then
        AC_MSG_WARN([couldn't find lapacke.h header. Deactivating compilation of the PLASMA benchmarks.])
        AC_SUBST([KASTORS_COMPILE_PLASMA], [no])
fi

dnl AC_SEARCH_LIBS(pthread_create, pthread)
dnl AC_SEARCH_LIBS(cblas_dgemm, cblas lapacke lapack blas m)

dnl -lcblas -llapacke -llapack -lblas -lpthread -lm -lgfortran

#AC_SUBST([abs_srcdir], [$(cd $srcdir && pwd)])

AC_OUTPUT

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
        echo -e "\twill compile PLASMA?\tno"
fi
echo "========================================="
