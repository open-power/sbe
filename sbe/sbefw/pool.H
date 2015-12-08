
#ifndef SBE_VECTOR_POOL_H
#define SBE_VECTOR_POOL_H

namespace SBEVECTORPOOL
{

// Size of a block  for a vector
static const size_t G_BLOCKSIZE = 512;

// TODO via RTC 143517
// Currently istep 3.1 and 3.7 take more number of vectors.
// Once getChildrenFilter support is in need to bring it back to 4.

//Pool size
static const size_t G_POOLSIZE = 8;

typedef struct
{
    size_t refCount;
    uint8_t data[G_BLOCKSIZE] __attribute__ ((aligned (8)));
}vectorMemPool_t;

/**
  * @brief Returns memory pool block.
  *
  * @return  Memory block if available, NULL otherwise.
  */
vectorMemPool_t * allocMem();

/**
  * @brief Release memory pool block.
  *
  * @param[in] i_pool pool pointer.
  */
void releaseMem( vectorMemPool_t * i_pool );

} // namespace SBEVECTORPOOL
#endif //SBE_VECTOR_POOL_H
