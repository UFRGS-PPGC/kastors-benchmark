# include "poisson.h"

#include  "partition.h"
#include "omp.h"
# include <numaif.h>
# include <stdio.h>

/* #pragma omp task/taskwait version of SWEEP. */
void sweep (int nx, int ny, double dx, double dy, double *f_,
        int itold, int itnew, double *u_, double *unew_, int block_size)
{
#ifdef _OPENMP
    double (*f)[nx][ny] = (double (*)[nx][ny])f_;
    double (*u)[nx][ny] = (double (*)[nx][ny])u_;
    double (*unew)[nx][ny] = (double (*)[nx][ny])unew_;
#endif
    if (block_size == 0)
        block_size = nx;

#pragma omp parallel \
    shared(u_, unew_, f, nx, ny, dx, dy, itold, itnew, block_size) 
#pragma omp master
    {
       int num_thread = omp_get_num_threads();
        for (int it = itold + 1; it <= itnew; it++) {
            // Save the current estimate.
            for (int j = 0; j < ny; j += block_size) {
              for (int i = 0; i < nx; i += block_size) {
#pragma omp task shared(u_, unew_)  firstprivate(i, j, block_size, nx, ny) \
                    depend(in: unew[i: block_size][j: block_size]) \
                    depend(out: u[i: block_size][j: block_size]) affinity(thread:GET_PARTITION(i, j, block_size, nx, ny, num_thread), 1)
                    copy_block(nx, ny, i/block_size, j/block_size, u_, unew_, block_size);
                }
            }

            // Compute a new estimate.
              for (int i = 0; i < nx; i += block_size) {
            for (int j = 0; j < ny; j += block_size) {
                    int num_node = -1;
                    /*printf("unew[i][j] : %p, unew : %p\n", &((*unew)[i][j]), &unew_[i*nx + j]);*/
                    if (get_mempolicy(&num_node, (void*)0, 0, &((*unew)[i][j]), MPOL_F_NODE | MPOL_F_ADDR))
                        perror("Error get_mempolicy\n");
                    /*int expected = GET_PARTITION(i, j, block_size, nx, ny, num_thread)/8;*/
                    /*printf("Block %i,%i, on node %i vs %i\n", i, j, num_node, expected);*/
                    int xdm1 = i == 0 ? 0 : block_size;
                    int xdp1 = i == nx-block_size ? 0 : block_size;
                    int ydp1 = j == ny-block_size ? 0 : block_size;
                    int ydm1 = j == 0 ? 0 : block_size;
#pragma omp task shared(u_, unew_) firstprivate(dx, dy, nx, ny, block_size, i, j, xdm1, xdp1, ydp1, ydm1) \
                    depend(out: unew[i: block_size][j: block_size]) \
                    depend(in: f[i: block_size][j: block_size], \
                               u[i: block_size][j: block_size], \
                               u[(i - xdm1): block_size][j: block_size], \
                               u[i: block_size][(j + ydp1): block_size], \
                               u[i: block_size][(j - ydm1): block_size], \
                               u[(i + xdp1): block_size][j: block_size]) affinity(thread:GET_PARTITION(i, j, block_size, nx, ny, num_thread), 1)
                    compute_estimate(i/block_size, j/block_size, u_, unew_, f_, dx, dy,
                                     nx, ny, block_size);
                }
            }
        }
    }
}
