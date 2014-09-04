-include ../config/config
-include ../config/config.default
-include ../config/config.user
-include ../config/strassen.config

ifeq ($(SEQ), true)
CFLAGS += -DSEQ
CC=$(CC_SEQ)
else
CFLAGS+=$(OMP_CCFLAGS)
LDFLAGS+=$(OMP_LDFLAGS)
endif

CFLAGS += -DMSIZE -DCUTOFF_SIZE -DCUTOFF_DEPTH -I./include

.SECONDARY:

all:
	@echo "=== Compile strassen benchmarks ==="
	@for kind in $(KINDS);\
	do\
		if [ "$${kind}" = "seq" ] || [ "$${kind}" = "block" ];\
		then\
			make SEQ="true" ./bin/$(CC_NAME)-$$kind;\
		else\
			make ./bin/$(CC_NAME)-$$kind;\
		fi;\
	done
	@echo "=== Strassen Compilation Done ==="

test: all
	@make test-only

test-only:
	@echo "=== Run strassen benchmarks ==="
	@for kind in $(KINDS);\
	do\
		echo "./bin/$(CC_NAME)-$$kind";\
		./bin/$(CC_NAME)-$$kind -c;\
	done
	@echo "=== Strassen Done ==="

./bin/$(CC_NAME)-%: ./obj/strassen-%.o ./obj/strassen.o ./obj/main.o ./obj/strassen-seq.o
	$(CC) $(LDFLAGS) $^ -o $@;

./obj/strassen-seq.o: ./src/strassen-seq.c ./include/strassen.h ../config/config ../config/config.default ../config/strassen.config
	$(CC) -c -DSEQ $(CFLAGS) $< -o $@;

./obj/%.o: ./src/%.c ./include/strassen.h ../config/config ../config/config.default ../config/strassen.config
	$(CC) -c $(CFLAGS) $< -o $@;

# Force rebuilt because of sequential possibility
./obj/strassen.o: ./src/strassen.c ./include/strassen.h ../config/config ../config/config.default ../config/strassen.config
	$(CC) -c $(CFLAGS) $< -o $@;

./obj/main.o: ../common/main.c ../config/config ../config/config.default
	$(CC) -c $(CFLAGS) $< -o $@;


.PHONY: info clean ./obj/strassen.o ./obj/main.o

info:
	@echo "Compiler:\t $(CC)"
	@echo "Flags:\t\t $(CFLAGS)"

clean:
	rm -rf ./obj/* ./bin/*
