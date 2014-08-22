
include ../common/common.am

bin_PROGRAMS = jacobi_seq jacobi_task jacobi_taskdep jacobi_block_task jacobi_block_taskdep jacobi_block_for

jacobi_common_HDRS = include/poisson.h $(COMMON_HDRS)
jacobi_common_SRCS = src/poisson.c src/jacobi-seq.c $(COMMON_SRCS) \
	$(jacobi_common_HDRS)

jacobi_seq_SOURCES           = $(jacobi_common_SRCS)
jacobi_task_SOURCES          = $(jacobi_common_SRCS) src/jacobi-task.c
jacobi_taskdep_SOURCES       = $(jacobi_common_SRCS) src/jacobi-task-dep.c
jacobi_block_task_SOURCES    = $(jacobi_common_SRCS) src/jacobi-block-task.c
jacobi_block_taskdep_SOURCES = $(jacobi_common_SRCS) src/jacobi-block-task-dep.c
jacobi_block_for_SOURCES     = $(jacobi_common_SRCS) src/jacobi-block-for.c

jacobi_comCFLAGS = -I$(srcdir)/include $(COMMON_comCFLAGS) \
		-DMSIZE -DNITER -DBSIZE
jacobi_seq_CFLAGS = $(jacobi_comCFLAGS) $(COMMON_seqCFLAGS)
jacobi_parCFLAGS = $(jacobi_comCFLAGS) $(COMMON_parCFLAGS)

jacobi_task_CFLAGS           = $(jacobi_parCFLAGS)
jacobi_taskdep_CFLAGS        = $(jacobi_parCFLAGS)
jacobi_block_task_CFLAGS     = $(jacobi_parCFLAGS)
jacobi_block_taskdep_CFLAGS  = $(jacobi_parCFLAGS)
jacobi_block_for_CFLAGS      = $(jacobi_parCFLAGS)

jacobi_comLDFLAGS = -lm $(COMMON_comLDFLAGS)
jacobi_seq_LDFLAGS = $(jacobi_comLDFLAGS) $(COMMON_seqLDFLAGS)
jacobi_parLDFLAGS = $(jacobi_comLDFLAGS) $(COMMON_seqLDFLAGS)

jacobi_task_LDFLAGS          = $(jacobi_parLDFLAGS)
jacobi_taskdep_LDFLAGS       = $(jacobi_parLDFLAGS)
jacobi_block_task_LDFLAGS    = $(jacobi_parLDFLAGS)
jacobi_block_taskdep_LDFLAGS = $(jacobi_parLDFLAGS)
jacobi_block_for_LDFLAGS     = $(jacobi_parLDFLAGS)