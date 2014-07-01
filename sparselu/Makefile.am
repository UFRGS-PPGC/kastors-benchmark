bin_PROGRAMS = sparselu_seq sparselu_task sparselu_taskdep

sparselu_seq_SOURCES     = ./src/sparselu.c ./src/sparselu-seq.c ../common/main.c
sparselu_task_SOURCES    = ./src/sparselu.c ./src/sparselu-seq.c ../common/main.c ./src/sparselu-task.c
sparselu_taskdep_SOURCES = ./src/sparselu.c ./src/sparselu-seq.c ../common/main.c ./src/sparselu-task-dep.c

sparselu_seq_CFLAGS     = -I$(srcdir)/include -I$(srcdir)/../common -DMSIZE -DSMSIZE -DSEQ
sparselu_task_CFLAGS    = -I$(srcdir)/include -I$(srcdir)/../common -DMSIZE -DSMSIZE
sparselu_taskdep_CFLAGS = -I$(srcdir)/include -I$(srcdir)/../common -DMSIZE -DSMSIZE
