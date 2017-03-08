#ifndef IPARAM_H
#define IPARAM_H

typedef double real_Double_t;

#include <time.h>
#include <stdint.h>


typedef struct timespec struct_time;
#  define gettime(t) clock_gettime( CLOCK_REALTIME, t)
#  define get_sub_seconde(t) (1e-9*(double)t.tv_nsec)
#  define get_sub_seconde_ns(t) ((uint64_t)t.tv_nsec)

uint64_t kaapi_get_elapsedns(void)
{
  uint64_t retval;
  struct_time st;
  int err = gettime(&st);
  if (err != 0) return (uint64_t)0UL;
  retval = (uint64_t)st.tv_sec * 1000000000ULL;
  retval += get_sub_seconde_ns(st);
  return retval;
}

#define PASTE_CODE_FREE_MATRIX(_desc_)                                  \
    if ( _desc_ != NULL ) {                                             \
        free(_desc_->mat);                                              \
    }                                                                   \
    PLASMA_Desc_Destroy( &_desc_ );

/*********************
 *
 * General Macros for timing
 *
 */
#define START_TIMING()                          \
  t = -cWtime();\
  *startTime = kaapi_get_elapsedns();


#define STOP_TIMING()                           \
  t += cWtime();                                \
  *t_ = t;\
  *endTime = kaapi_get_elapsedns();

#endif /* IPARAM_H */
