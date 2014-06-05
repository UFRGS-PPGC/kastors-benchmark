-include ../config/config
-include ../config/config.default
-include ../config/config.user

ifeq ($(SEQ), true)
CFLAGS += -DSEQ
CC=$(CC_SEQ)
else
CFLAGS+=$(OMP_CCFLAGS)
LDFLAGS+=$(OMP_LDFLAGS)
endif

CFLAGS += -I./include -DADD_ -DMSIZE -DBSIZE

LDFLAGS += -lcblas -llapacke -llapack -lblas -lpthread -lm -lgfortran

KINDS = seq task

# Use Hwloc
HWLOC = $(shell pkg-config --libs hwloc 2> /dev/null )
ifeq (1, $(words $(findstring hwloc, $(HWLOC))))
        #CFLAGS  := $(CFLAGS) -DPLASMA_HWLOC -DQUARK_HWLOC
        CFLAGS  += -DPLASMA_HWLOC
        INCEXT  := $(INCEXT) $(shell pkg-config --cflags-only-I hwloc)
        LIBEXT  := $(LIBEXT) $(shell pkg-config --libs hwloc)
        require := hwloc
		CFLAGS += $(INCEXT)
		LDFLAGS += $(LIBEXT)
endif


ALLPROG = $(wildcard src/time_d*.c)
ALLPROGOBJ = $(ALLPROG:src/%.c=obj/%.o)
ALLSRC = $(filter-out $(ALLPROG), $(wildcard src/*.c))
ALLOBJ = $(ALLSRC:src/%.c=obj/%.o)
EXE = $(ALLPROG:.c=)

.SECONDARY:

all:
	@echo "=== Compile plasma benchmarks === "
	@for kind in $(KINDS);\
	do\
		if [ "$${kind}" = "seq" ] || [ "$${kind}" = "block" ];\
		then\
			make cleanomp;\
			make SEQ="true" ./bin/$(CC_NAME)-time_dpotrf-$$kind;\
			make SEQ="true" ./bin/$(CC_NAME)-time_dgetrf-$$kind;\
			make SEQ="true" ./bin/$(CC_NAME)-time_dgeqrf-$$kind;\
		else\
			make cleanomp;\
			make ./bin/$(CC_NAME)-time_dpotrf-$$kind;\
			make ./bin/$(CC_NAME)-time_dgetrf-$$kind;\
			make ./bin/$(CC_NAME)-time_dgeqrf-$$kind;\
		fi;\
	done
	@echo "=== Plasma Compilation Done === "

test: all
	@make test-only

test-only:
	@echo "=== Run Plasma benchmarks === "
	@for kind in $(KINDS);\
	do\
		echo "./bin/$(CC_NAME)-time_dpotrf-$$kind";\
		./bin/$(CC_NAME)-time_dpotrf-$$kind;\
		echo "./bin/$(CC_NAME)-time_dgetrf-$$kind";\
		./bin/$(CC_NAME)-time_dgetrf-$$kind;\
		echo "./bin/$(CC_NAME)-time_dgeqrf-$$kind";\
		./bin/$(CC_NAME)-time_dgeqrf-$$kind;\
	done
	@echo "=== Plasma Done === "

./bin/$(CC_NAME)-%: ./obj/%.o $(ALLOBJ) ./src/timing.inc ./obj/main.o
	$(CC) $(filter-out src/timing.inc, $^) -o $@ $(LDFLAGS)

./obj/%.o: ./src/%.c ../config/config ../config/config.default
	$(CC) $(CFLAGS) -c $< -o $@

./obj/time_d%.o: ./src/time_d%.c
	$(CC) $(CFLAGS) -c $< -o $@ -DCOMPILER='"$(CC) $(CFLAGS)"'


./obj/main.o: ../common/main.c ../config/config ../config/config.default
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: info clean cleanomp

info:
	@echo "Compiler:\t $(CC)"
	@echo "Flags:\t\t $(CFLAGS)"

cleanomp:
	rm -f ./obj/time_d*.o ./obj/pd*.o ./obj/dpotrf.o ./obj/dgetrf.o ./obj/dgeqrf.o ./obj/main.o

clean:
	rm -rf ./obj/* ./bin/*
