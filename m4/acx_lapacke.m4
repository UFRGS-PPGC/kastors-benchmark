AC_DEFUN([ACX_LAPACKE], [
	AC_CHECK_HEADER([lapacke.h], [],
		[
			AC_MSG_WARN([couldn't find lapacke.h header. Deactivating compilation of the PLASMA benchmarks.])
dnl '
			KASTORS_COMPILE_PLASMA=no
			KASTORS_MISSING_DEPS="$KASTORS_MISSING_DEPS lapacke.h"
		])

	AC_CHECK_LIB([lapacke], [LAPACKE_dlacpy_work],
		[
			PLASMA_comLIBS="-llapacke ${PLASMA_comLIBS}"
			LIBS="$LIBS -llapacke"
		], [
			AC_MSG_WARN([couldn't find LAPACKE_dlacpy_work in -llapacke. Deactivating compilation of the PLASMA benchmarks.])
dnl '
			KASTORS_COMPILE_PLASMA=no
			KASTORS_MISSING_DEPS="$KASTORS_MISSING_DEPS -llapacke"
		])
])
