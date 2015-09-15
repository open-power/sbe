/*
 *  @file assert.h
 *
 *  @brief This file contains the assert macro for SBE
 */
#ifndef SBE_ASSERT_H
#define SBE_ASSERT_H
#include "sbetrace.H"

//@TODO via RTC 129166
//inject exception to halt SBE. Also see if we can use some
//PK kernel API.
#ifndef NDEBUG
#define assert(expr) \
    if( !(expr ))  \
    {  \
        SBE_ERROR("assertion failed: "#expr); \
    } \

#else
#define assert(expr)
#endif  //NDEBUG

#endif // SBE_ASSERT_H
