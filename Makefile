-include ./config/config
-include ./config/config.default

DIRS = jacobi strassen sparselu plasma

.SECONDARY:

all:
	@echo "Kaapi Starpu OpenMP Runtime Task suite"
	@echo ""
	@echo "Do make [rule] where rule can be one of:"
	@echo "    [project name]:"
	@echo "        To build specified benchmark"
	@echo "    test :"
	@echo "        To compile all project then run them"
	@echo "    test-only :"
	@echo "        To run test already compiled"
	@echo "    clean :"
	@echo "        Clean the whole benchmark project"
	@echo ""

# We want a target per test to compile them in parallel
jacobi:
	@make -C jacobi

plasma:
	@make -C plasma

strassen:
	@make -C strassen

sparselu:
	@make -C sparselu

test: $(DIRS)
	@make test-only

test-only:
	@echo "=== Run all benchmarks === "
	@for folder in $(DIRS);\
	do\
		make -C $$folder $@;\
	done
	@echo "=== All benchmarks runned === "

.PHONY: clean test test-only $(DIRS)

clean:
	@echo "=== Clean up benchmarks === "
	@for folder in $(DIRS);\
	do\
		make -C $$folder clean;\
	done
	rm -rf ./obj/*
