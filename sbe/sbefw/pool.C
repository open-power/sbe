#include <stdint.h>
#include <sbetrace.H>
#include <stddef.h>
#include<pool.H>

namespace SBEVECTORPOOL
{

vectorMemPool_t g_pool[G_POOLSIZE];

vectorMemPool_t * allocMem()
{
    vectorMemPool_t *pool = NULL;
    for( size_t idx = 0; idx < G_POOLSIZE; idx++ )
    {
        if( 0 == g_pool[idx].refCount )
        {
            pool = g_pool + idx;
            g_pool[idx].refCount++;
            break;
        }
    }
    SBE_TRACE(" Giving pool 0x%08X", pool);
    return pool;
}

void releaseMem( vectorMemPool_t * i_pool )
{
    do
    {
        if ( NULL == i_pool )   break;

        if( 0 == i_pool->refCount )
        {
            //TODO via RTC 129166
            // Assert here.  This pool was not supposed to be in use.
            // Currenty just keeping it as is as we can not do much till the
            // time support for assert is in.
            break;
        }
        SBE_TRACE(" Releasing pool 0x%08X", i_pool);
        i_pool->refCount--;
        SBE_TRACE(" In releaseMem() RefCount:%u", i_pool->refCount);
    }while(0);
}

} // namesspace SBEVECTORPOOL
