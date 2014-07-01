bin_PROGRAMS = strassen_seq strassen_task strassen_taskdep

strassen_seq_SOURCES     = ./src/strassen.c ./src/strassen-seq.c ../common/main.c
strassen_task_SOURCES    = ./src/strassen.c ./src/strassen-seq.c ./src/strassen-task.c ../common/main.c
strassen_taskdep_SOURCES = ./src/strassen.c ./src/strassen-seq.c ./src/strassen-task-dep.c ../common/main.c

strassen_seq_CFLAGS     = -I$(srcdir)/include -I$(srcdir)/../common -DMSIZE -DCUTOFF_SIZE -DCUTOFF_DEPTH -DSEQ
strassen_task_CFLAGS    = -I$(srcdir)/include -I$(srcdir)/../common -DMSIZE -DCUTOFF_SIZE -DCUTOFF_DEPTH
strassen_taskdep_CFLAGS = -I$(srcdir)/include -I$(srcdir)/../common -DMSIZE -DCUTOFF_SIZE -DCUTOFF_DEPTH
