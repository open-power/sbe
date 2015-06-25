
#ifndef SBE_VECTOR_POOL_H
#define SBE_VECTOR_POOL_H

namespace SBEVECTORPOOL
{

// Size of a block  for a vector
static const size_t G_BLOCKSIZE = 512;

//Pool size
static const size_t G_POOLSIZE = 4;

typedef struct
{
    size_t refCount;
    uint8_t data[G_BLOCKSIZE];
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
