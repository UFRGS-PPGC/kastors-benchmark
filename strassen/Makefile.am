
include ../common/common.am

bin_PROGRAMS = strassen_seq strassen_task strassen_taskdep

strassen_common_HDRS = include/strassen.h $(COMMON_HDRS)
strassen_common_SRCS = src/strassen.c src/strassen-seq.c $(COMMON_SRCS) \
	$(strassen_common_HDRS)

strassen_seq_SOURCES     = $(strassen_common_SRCS)
strassen_task_SOURCES    = $(strassen_common_SRCS) src/strassen-task.c
strassen_taskdep_SOURCES = $(strassen_common_SRCS) src/strassen-task-dep.c

strassen_comCFLAGS = -I$(srcdir)/include $(COMMON_comCFLAGS) \
		-DMSIZE -DCUTOFF_SIZE -DCUTOFF_DEPTH
strassen_seq_CFLAGS = $(strassen_comCFLAGS) $(COMMON_seqCFLAGS)
strassen_parCFLAGS = $(strassen_comCFLAGS) $(COMMON_parCFLAGS)

strassen_task_CFLAGS    = $(strassen_parCFLAGS)
strassen_taskdep_CFLAGS = $(strassen_parCFLAGS)

strassen_comLDFLAGS = $(COMMON_comLDFLAGS)
strassen_seq_LDFLAGS = $(strassen_comLDFLAGS) $(COMMON_seqLDFLAGS)
strassen_parLDFLAGS = $(strassen_comLDFLAGS) $(COMMON_parLDFLAGS)

strassen_task_LDFLAGS    = $(strassen_parLDFLAGS)
strassen_taskdep_LDFLAGS = $(strassen_parLDFLAGS)

