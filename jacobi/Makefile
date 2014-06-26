-include ../config/config
-include ../config/config.default
-include ../config/config.user
-include ../config/poisson.config

ifeq ($(SEQ), true)
CFLAGS += -DSEQ
CC=$(CC_SEQ)
else
CFLAGS+=$(OMP_CCFLAGS)
LDFLAGS+=$(OMP_LDFLAGS)
endif

CFLAGS += -DMSIZE -DNITER -DBSIZE -I./include -g -std=gnu99

.SECONDARY:

all:
	@echo "=== Compile jacobi benchmarks === "
	@for kind in $(KINDS);\
	do\
		if [ "$${kind}" = "seq" ] || [ "$${kind}" = "block" ];\
		then\
			make SEQ="true" ./bin/$(CC_NAME)-$$kind;\
		else\
			make ./bin/$(CC_NAME)-$$kind;\
		fi;\
	done
	@echo "=== Jacobi Compilation Done === "

test: all
	@make test-only

test-only:
	@echo "=== Run jacobi benchmarks === "
	@for kind in $(KINDS);\
	do\
		echo "./bin/$(CC_NAME)-$$kind";\
		./bin/$(CC_NAME)-$$kind;\
	done
	@echo "=== Jacobi Done === "

./bin/$(CC_NAME)-%: ./obj/jacobi-%.o ./obj/poisson.o ./obj/main.o ./obj/jacobi-seq.o
	$(CC) $(LDFLAGS) $^ -o $@

./obj/jacobi-seq.o: ./src/jacobi-seq.c ./include/poisson.h ../config/config ../config/config.default ../config/poisson.config
	$(CC_SEQ) -DSEQ -c $(CFLAGS) $< -o $@

./obj/%.o: ./src/%.c ./include/poisson.h ../config/config ../config/config.default ../config/poisson.config
	$(CC) -c $(CFLAGS) $< -o $@

# Force rebuilt because of sequential possibility
./obj/poisson.o: ./src/poisson.c ./include/poisson.h ../config/config ../config/config.default ../config/poisson.config
	$(CC) -c $(CFLAGS) $< -o $@

./obj/main.o: ../common/main.c ../config/config ../config/config.default
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: info clean ./obj/poisson.o ./obj/main.o

info:
	@echo "Compiler:\t $(CC)"
	@echo "Flags:\t\t $(CFLAGS)"

clean:
	rm -rf ./obj/* ./bin/*
