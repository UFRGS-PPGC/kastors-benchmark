

//#define NGX 8
//#define NGY 8
static inline int GET_PARTITION(int i, int j, int block_size, int nx, int ny, int square_len)
{
//nx : taille sur x
//ny : taille sur y
  int ngy = square_len/2;
  int ngx = square_len-ngy;
  ngx = 1 << ngx;
  ngy = 1 << ngy;
  int max_blocks_x = (nx / block_size);
  int max_blocks_y = (ny / block_size);
  return (((i/block_size)/(max_blocks_x/ngy))%ngy)*ngx + (((j/block_size)/(max_blocks_y/ngx))%ngx);
  //return (i/(block_size*6))*8 + j/(6*block_size);
}
