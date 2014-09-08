#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _OPENMP
#  include <omp.h>
#endif

#include "main.h"

void parse(int argc, char* argv[], struct user_parameters* params)
{
    int i;
    for(i=1; i<argc; i++) {
        if(!strcmp(argv[i], "-c"))
            params->check = 1;
        else if(!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            printf("----------------------------------------------\n");
            printf("-                KaStORS                     -\n");
            printf("-   Kaapi Starpu OpenMP Runtime task Suite   -\n");
            printf("----------------------------------------------\n");
            printf("-h, --help : Show help information\n");
            printf("-c : Ask to check result\n");
#ifdef MSIZE
            printf("-n : Matrix size\n");
#endif
#ifdef SMSIZE
            printf("-m : SubMatrix size\n");
#endif
#ifdef BSIZE
            printf("-b : Block size\n");
#endif
#ifdef NITER
            printf("-i : Number of iterations\n");
#endif
#ifdef CUTOFF_SIZE
            printf("-s : Cutoff (Size of the matrix)\n");
#endif
#ifdef CUTOFF_DEPTH
            printf("-d : Cutoff (depth)\n");
#endif
            exit(EXIT_SUCCESS);
#ifdef MSIZE
        } else if(!strcmp(argv[i], "-n")) {
            if (++i < argc)
                params->matrix_size = atoi(argv[i]);
            else {
                fprintf(stderr, "-n requires a number\n");
                exit(EXIT_FAILURE);
            }
#endif
#ifdef SMSIZE
        } else if(!strcmp(argv[i], "-m")) {
            if (++i < argc)
                params->submatrix_size = atoi(argv[i]);
            else {
                fprintf(stderr, "-m requires a number\n");
                exit(EXIT_FAILURE);
            }
#endif
#ifdef BSIZE
        } else if(!strcmp(argv[i], "-b")) {
            if (++i < argc)
                params->blocksize = atoi(argv[i]);
            else {
                fprintf(stderr, "-b requires a number\n");
                exit(EXIT_FAILURE);
            }
#endif
#ifdef NITER
        } else if(!strcmp(argv[i], "-i")) {
            if (++i < argc)
                params->niter = atoi(argv[i]);
            else {
                fprintf(stderr, "-i requires a number\n");
                exit(EXIT_FAILURE);
            }
#endif
#ifdef CUTOFF_SIZE
        } else if(!strcmp(argv[i], "-s")) {
            if (++i < argc)
                params->cutoff_size = atoi(argv[i]);
            else {
                fprintf(stderr, "-s requires a number\n");
                exit(EXIT_FAILURE);
            }
#endif
#ifdef CUTOFF_DEPTH
        } else if(!strcmp(argv[i], "-d")) {
            if (++i < argc)
                params->cutoff_depth = atoi(argv[i]);
            else {
                fprintf(stderr, "-d requires a number\n");
                exit(EXIT_FAILURE);
            }
#endif
        } else
            fprintf(stderr, "Unknown parameter : %s\n", argv[i]);
    }
}

int main(int argc, char* argv[])
{
    int num_threads = 1;
    struct user_parameters params;
    memset(&params, 0, sizeof(params));
    parse(argc, argv, &params);

// get Number of thread if OpenMP is activated
#ifdef _OPENMP
    #pragma omp parallel
    #pragma omp master
    num_threads = omp_get_num_threads();
#endif

    double timing = run(&params);

    printf("Program : %s\n", argv[0]);
#ifdef MSIZE
    printf("Size : %d\n", params.matrix_size);
#endif
#ifdef SMSIZE
    printf("Submatrix size : %d\n", params.submatrix_size);
#endif
#ifdef BSIZE
    printf("Blocksize : %d\n", params.blocksize);
#endif
#ifdef NITER
    printf("Iterations : %d\n", params.niter);
#endif
#ifdef CUTOFF_SIZE
    printf("Cutoff Size : %d\n", params.cutoff_size);
#endif
#ifdef CUTOFF_DEPTH
    printf("Cutoff depth : %d\n", params.cutoff_depth);
#endif
    printf("Threads : %d\n", num_threads);
    printf("Time : %lf sec\n", timing);
    if(params.check)
        printf("Check : %s\n", (params.succeed)?
                ((params.succeed > 1)?"not implemented":"success")
                :"fail");
    if (params.string2display !=0)
      printf("%s", params.string2display);
    printf("\n");

    return 0;
}
