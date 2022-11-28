/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/ppe/baselib/byteswap.h $          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2022                             */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
// EKB-Mirror-To: hw/ppe
#ifndef __BYTESWAP_H__
#define __BYTESWAP_H__

#ifndef __PPE42__
#include_next <byteswap.h>
#else
#include <stdint.h>

static inline uint16_t bswap_16( uint16_t __u16)
{
    return (uint16_t)((__u16 >> 8) | (__u16) << 8);
}

static inline uint32_t bswap_32(uint32_t __u32)
{
    uint32_t val = ((__u32 << 8) & 0xff00ff00) | ((__u32 >> 8) & 0x00ff00ff);
    return (val << 16) | (val >> 16);
}

static inline uint64_t bswap_64(uint64_t __u64)
{
    return bswap_32(__u64 >> 32) | (uint64_t(bswap_32(__u64 & 0xFFFFFFFF)) << 32);
}

#endif

#endif
