
include ../common/common.am

check_SCRIPTS = check_dgetrf_seq.sh\
				check_dgetrf_taskdep.sh\
				check_dgeqrf_seq.sh\
				check_dgeqrf_taskdep.sh\
				check_dpotrf_seq.sh\
				check_dpotrf_taskdep.sh
TESTS= $(check_SCRIPTS)

check_%.sh:
	python -c "print './' + '$@'[6:-3] + ' -c'" > $@
	chmod +x $@


bin_PROGRAMS = dpotrf_seq dpotrf_taskdep dgetrf_seq dgetrf_taskdep dgeqrf_seq dgeqrf_taskdep

# Get all source files from ./src except the one called time_xxx
plasma_SRCS = \
	src/allocate.c 			\
	src/auxiliary.c 		\
	src/context.c 			\
	src/control.c 			\
	src/core_dgeqrt.c		\
	src/core_dgetrf_rectil.c	\
	src/core_dlaswp.c 		\
	src/core_dormqr.c 		\
	src/core_dpamm.c 		\
	src/core_dparfb.c 		\
	src/core_dplgsy.c		\
	src/core_dplrnt.c		\
	src/core_dtsmqr.c 		\
	src/core_dtsqrt.c		\
	src/dauxiliary.c		\
	src/descriptor.c		\
	src/dgeqrf.c			\
	src/dgeqrs.c 			\
	src/dgetrf.c 			\
	src/dgetrs.c 			\
	src/dpotrs.c			\
	src/global.c			\
	src/pdbarrier.c			\
	src/pdgeqrf.c 			\
	src/pdgetrf_rectil.c 		\
	src/pdlaswp.c 			\
	src/pdormqr.c			\
	src/pdplgsy.c			\
	src/pdpltmg.c			\
	src/pdpotrf.c			\
	src/pdtile.c 			\
	src/pdtrsm.c 			\
	src/workspace.c

plasma_common_HDRS = $(COMMON_HDRS)
plasma_common_SRCS = $(plasma_SRCS) $(COMMON_SRCS) $(plasma_common_HDRS)

plasma_comLIBS		= -lcblas -llapacke -llapack -lblas -lm


noinst_LIBRARIES = libplasma-seq.a libplasma-par.a
libplasma_seq_a_SOURCES = $(plasma_common_SRCS)
libplasma_par_a_SOURCES = $(plasma_common_SRCS)

dpotrf_seq_SOURCES      = src/time_dpotrf-seq.c
dpotrf_taskdep_SOURCES  = src/time_dpotrf-task.c
dgetrf_seq_SOURCES      = src/time_dgetrf-seq.c
dgetrf_taskdep_SOURCES  = src/time_dgetrf-task.c
dgeqrf_seq_SOURCES      = src/time_dgeqrf-seq.c
dgeqrf_taskdep_SOURCES  = src/time_dgeqrf-task.c

dpotrf_seq_LDADD      = libplasma-seq.a $(plasma_comLIBS)
dpotrf_taskdep_LDADD  = libplasma-par.a $(plasma_comLIBS)
dgetrf_seq_LDADD      = libplasma-seq.a $(plasma_comLIBS)
dgetrf_taskdep_LDADD  = libplasma-par.a $(plasma_comLIBS)
dgeqrf_seq_LDADD      = libplasma-seq.a $(plasma_comLIBS)
dgeqrf_taskdep_LDADD  = libplasma-par.a $(plasma_comLIBS)

plasma_comCFLAGS	= -I$(srcdir)/include $(COMMON_comCFLAGS) \
		-DADD_ -DMSIZE -DBSIZE $(PTHREAD_CFLAGS)
plasma_seqCFLAGS	= $(plasma_comCFLAGS) $(COMMON_seqCFLAGS)
plasma_parCFLAGS	= $(plasma_comCFLAGS) $(COMMON_parCFLAGS)

libplasma_seq_a_CFLAGS  = $(plasma_seqCFLAGS)
libplasma_par_a_CFLAGS  = $(plasma_parCFLAGS)

dpotrf_seq_CFLAGS       = $(plasma_seqCFLAGS)
dpotrf_taskdep_CFLAGS   = $(plasma_parCFLAGS)
dgetrf_seq_CFLAGS       = $(plasma_seqCFLAGS)
dgetrf_taskdep_CFLAGS   = $(plasma_parCFLAGS)
dgeqrf_seq_CFLAGS       = $(plasma_seqCFLAGS)
dgeqrf_taskdep_CFLAGS   = $(plasma_parCFLAGS)

plasma_comLDFLAGS	= $(COMMON_comLDFLAGS)
plasma_seqLDFLAGS	= $(plasma_comLDFLAGS) $(COMMON_seqLDFLAGS)
plasma_parLDFLAGS	= $(plasma_comLDFLAGS) $(COMMON_parLDFLAGS)

dpotrf_seq_LDFLAGS      = $(plasma_seqLDFLAGS)
dpotrf_taskdep_LDFLAGS  = $(plasma_parLDFLAGS)
dgetrf_seq_LDFLAGS      = $(plasma_seqLDFLAGS)
dgetrf_taskdep_LDFLAGS  = $(plasma_parLDFLAGS)
dgeqrf_seq_LDFLAGS      = $(plasma_seqLDFLAGS)
dgeqrf_taskdep_LDFLAGS  = $(plasma_parLDFLAGS)
