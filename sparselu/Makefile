-include ../config/config
-include ../config/config.default
-include ../config/config.user
-include ../config/sparselu.config

ifeq ($(SEQ), true)
CFLAGS += -DSEQ
CC:=$(CC_SEQ)
else
CFLAGS+=$(OMP_CCFLAGS)
LDFLAGS+=$(OMP_LDFLAGS)
endif

CFLAGS += -DMSIZE -DSMSIZE -I./include

.SECONDARY:

all: obj bin
	@echo "=== Compile sparselu benchmarks ==="
	@for kind in $(KINDS);\
	do\
		if [ "$${kind}" = "seq" ] || [ "$${kind}" = "block" ];\
		then\
			make SEQ="true" ./bin/$(CC_NAME)-$$kind;\
		else\
			make ./bin/$(CC_NAME)-$$kind;\
		fi;\
	done
	@echo "=== Sparselu Compilation Done ==="

bin:
	mkdir bin
obj:
	mkdir obj

test: all
	@make test-only

test-only:
	@echo "=== Run sparselu benchmarks ==="
	@for kind in $(KINDS);\
	do\
		echo "./bin/$(CC_NAME)-$$kind";\
		./bin/$(CC_NAME)-$$kind;\
	done
	@echo "=== Sparselu Done ==="

./bin/$(CC_NAME)-%: ./obj/sparselu-%.o ./obj/sparselu.o ./obj/main.o ./obj/sparselu-seq.o
	$(CC) $(LDFLAGS) $^ -o $@;

./obj/%.o: ./src/%.c ./include/sparselu.h ../config/config ../config/config.default ../config/sparselu.config
	$(CC) -c $(CFLAGS) $< -o $@;

./obj/sparselu-seq.o: ./src/sparselu-seq.c ./include/sparselu.h ../config/config ../config/config.default ../config/sparselu.config
	$(CC) -c -DSEQ $(CFLAGS) $< -o $@;

# Force rebuilt because of sequential possibility
./obj/sparselu.o: ./src/sparselu.c ./include/sparselu.h ../config/config ../config/config.default ../config/sparselu.config
	$(CC) -c $(CFLAGS) $< -o $@;

./obj/main.o: ../common/main.c ../config/config ../config/config.default
	$(CC) -c $(CFLAGS) $< -o $@;


.PHONY: info clean ./obj/sparselu.o ./obj/main.o

info:
	@echo "Compiler:\t $(CC)"
	@echo "Flags:\t\t $(CFLAGS)"

clean:
	rm -rf ./obj/* ./bin/*
