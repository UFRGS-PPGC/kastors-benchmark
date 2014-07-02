bin_PROGRAMS = jacobi_seq jacobi_task jacobi_taskdep jacobi_block_task jacobi_block_taskdep

jacobi_seq_SOURCES           = ./src/poisson.c ./src/jacobi-seq.c ../common/main.c
jacobi_task_SOURCES          = ./src/poisson.c ./src/jacobi-seq.c ../common/main.c ./src/jacobi-task.c
jacobi_taskdep_SOURCES       = ./src/poisson.c ./src/jacobi-seq.c ../common/main.c ./src/jacobi-task-dep.c
jacobi_block_task_SOURCES    = ./src/poisson.c ./src/jacobi-seq.c ../common/main.c ./src/jacobi-block-task.c
jacobi_block_taskdep_SOURCES = ./src/poisson.c ./src/jacobi-seq.c ../common/main.c ./src/jacobi-block-task-dep.c

jacobi_seq_CFLAGS            = -I$(srcdir)/include -I$(srcdir)/../common -DMSIZE -DNITER -DBSIZE -DSEQ
jacobi_task_CFLAGS           = -I$(srcdir)/include -I$(srcdir)/../common -DMSIZE -DNITER -DBSIZE
jacobi_taskdep_CFLAGS        = $(jacobi_task_CFLAGS)
jacobi_block_task_CFLAGS     = $(jacobi_task_CFLAGS)
jacobi_block_taskdep_CFLAGS  = $(jacobi_task_CFLAGS)

jacobi_seq_LDFLAGS           = -lm
jacobi_task_LDFLAGS          = -lm
jacobi_taskdep_LDFLAGS       = -lm
jacobi_block_task_LDFLAGS    = -lm
jacobi_block_taskdep_LDFLAGS = -lm
