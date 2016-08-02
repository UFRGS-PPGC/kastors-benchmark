

//#define NGX 8
//#define NGY 8
static inline int GET_PARTITION(int i, int j, int block_size, int nx, int ny, int num_thread)
{
//nx : taille sur x
//ny : taille sur y
  int ngy, ngx;
  if (num_thread == 96) {
    ngx = 12;
    ngy = 8;
  } else if (num_thread == 48) {
    ngx = 8;
    ngy = 6;
  } else if (num_thread == 144) {
    ngx = 12;
    ngy = 12;
  } else if (num_thread == 192) {
    ngx = 16;
    ngy = 12;
  } else {
    int square_len = 0;
    while ((num_thread >>= 1) > 0)
      square_len++;
    ngy = square_len/2;
    ngx = square_len-ngy;
    ngx = 1 << ngx;
    ngy = 1 << ngy;
  }
  //ngx = 8;
  //ngy = 12;
  int max_blocks_x = (nx / block_size);
  int max_blocks_y = (ny / block_size);
  return (((i/block_size)/(max_blocks_x/ngy))%ngy)*ngx + (((j/block_size)/(max_blocks_y/ngx))%ngx);
  //return (i/(block_size*6))*8 + j/(6*block_size);
}
