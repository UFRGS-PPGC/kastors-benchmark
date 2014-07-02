bin_PROGRAMS = dpotrf_seq dpotrf_taskdep dgetrf_seq dgetrf_taskdep dgeqrf_seq dgeqrf_taskdep

# Get all source files from ./src except the one called time_xxx
plasma_SOURCES = ./src/allocate.c \
./src/core_dlaswp.c 		  \
./src/core_dsyrk.c 		  \
./src/dgeqrs.c 			  \
./src/pdgeqrf.c 		  \
./src/pdtile.c 			  \
./src/async.c 			  \
./src/core_dormqr.c 		  \
./src/core_dtrsm.c 		  \
./src/dgetrf.c 			  \
./src/pdgetmi2.c 		  \
./src/pdtrsm.c 			  \
./src/auxiliary.c 		  \
./src/core_dpamm.c 		  \
./src/core_dtrtri.c 		  \
./src/dgetrs.c 			  \
./src/pdgetrf_rectil.c 		  \
./src/plasmaos.c 		  \
./src/context.c 		  \
./src/core_dparfb.c 		  \
./src/core_dtsmqr.c 		  \
./src/dplgsy.c 			  \
./src/pdlaswp.c 		  \
./src/plasma_threadsetting.c 	  \
./src/control.c 		  \
./src/core_dplgsy.c		  \
./src/core_dtsqrt.c		  \
./src/dplrnt.c			  \
./src/pdormqr.c			  \
./src/primes.c			  \
./src/core_dgeqrt.c		  \
./src/core_dplrnt.c		  \
./src/core_pivot_update.c	  \
./src/dpotrf.c			  \
./src/pdormqrrh.c		  \
./src/workspace.c		  \
./src/core_dgetrf.c		  \
./src/core_dpltmg.c		  \
./src/dauxiliary.c		  \
./src/dpotrs.c			  \
./src/pdplgsy.c			  \
./src/workspace_d.c		  \
./src/core_dgetrf_rectil.c	  \
./src/core_dpotrf.c		  \
./src/descriptor.c		  \
./src/dtile.c			  \
./src/pdpltmg.c			  \
./src/core_dgetrip.c		  \
./src/core_dshift.c		  \
./src/dgecfi.c			  \
./src/global.c			  \
./src/pdpotrf.c			  \
./src/core_dlacpy.c		  \
./src/core_dsyr2k.c		  \
./src/dgeqrf.c			  \
./src/pdbarrier.c		  \
./src/pdshift.c			  \
./src/tile.c

dpotrf_seq_SOURCES      = ../common/main.c $(plasma_SOURCES) ./src/time_dpotrf-seq.c
dpotrf_taskdep_SOURCES  = ../common/main.c $(plasma_SOURCES) ./src/time_dpotrf-task.c
dgetrf_seq_SOURCES      = ../common/main.c $(plasma_SOURCES) ./src/time_dgetrf-seq.c
dgetrf_taskdep_SOURCES  = ../common/main.c $(plasma_SOURCES) ./src/time_dgetrf-task.c
dgeqrf_seq_SOURCES      = ../common/main.c $(plasma_SOURCES) ./src/time_dgeqrf-seq.c
dgeqrf_taskdep_SOURCES  = ../common/main.c $(plasma_SOURCES) ./src/time_dgeqrf-task.c

dpotrf_seq_CFLAGS       = -I$(srcdir)/include -I$(srcdir)/../common -DADD_ -DMSIZE -DBSIZE -DSEQ
dpotrf_taskdep_CFLAGS   = -I$(srcdir)/include -I$(srcdir)/../common -DADD_ -DMSIZE -DBSIZE
dgetrf_seq_CFLAGS       = -I$(srcdir)/include -I$(srcdir)/../common -DADD_ -DMSIZE -DBSIZE -DSEQ
dgetrf_taskdep_CFLAGS   = -I$(srcdir)/include -I$(srcdir)/../common -DADD_ -DMSIZE -DBSIZE
dgeqrf_seq_CFLAGS       = -I$(srcdir)/include -I$(srcdir)/../common -DADD_ -DMSIZE -DBSIZE -DSEQ
dgeqrf_taskdep_CFLAGS   = -I$(srcdir)/include -I$(srcdir)/../common -DADD_ -DMSIZE -DBSIZE

dpotrf_seq_LDFLAGS      = -lcblas -llapacke -llapack -lblas -lpthread -lm
dpotrf_taskdep_LDFLAGS  = $(dpotrf_seq_LDFLAGS)
dgetrf_seq_LDFLAGS      = $(dpotrf_seq_LDFLAGS)
dgetrf_taskdep_LDFLAGS  = $(dpotrf_seq_LDFLAGS)
dgeqrf_seq_LDFLAGS      = $(dpotrf_seq_LDFLAGS)
dgeqrf_taskdep_LDFLAGS  = $(dpotrf_seq_LDFLAGS)
