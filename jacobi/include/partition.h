
#if defined(USE_OMPEXT)

#define NGX 8
#define NGY 16
static inline int GET_PARTITION(int i, int j, int block_size, int nx, int ny) 
{
  int max_blocks_x = (nx / block_size);
  int max_blocks_y = (ny / block_size);
  return ((i/block_size)/(max_blocks_x/NGY)%NGY)*NGX + ((j/block_size)/(max_blocks_y/NGX)%NGX);
}
#endif
