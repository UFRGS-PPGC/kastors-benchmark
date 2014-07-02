
# flags to aclocal if Makefile rules trigger it
ACLOCAL_AMFLAGS=-I m4

SUBDIRS = 

EXTRA_DIST=common/main.c common/main.h common/timer.h

if COMPILE_STRASSEN
SUBDIRS += strassen
endif

if COMPILE_JACOBI
SUBDIRS += jacobi
endif

if COMPILE_SPARSELU
SUBDIRS += sparselu
endif

if COMPILE_PLASMA
SUBDIRS += plasma
endif

