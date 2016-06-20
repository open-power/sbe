/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: hwpf/include/plat/multicast.H $                               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
#ifndef __FAPI2_MULTICAST__
#define __FAPI2_MULTICAST__

#include <fapi2_multicast.H>

namespace fapi2
{

template<MulticastType M, MulticastGroup G, typename V>
template<MulticastType O, MulticastGroup N>
inline void Multicast<M, G, V>::updateHandle(V& i_value)
{
    // Update handle only if multicast bit is set in handle, else we leave the
    // handle unchanged. This enables the same procedure to work with both
    // multicast and unicast targets
    if(i_value.fields.is_multicast)
    {
        // Update the handle to reflect the new multicast type and group
        i_value.fields.chiplet_num = (0x40) | (O << 3) | N;
    }
}

template<MulticastType M, MulticastGroup G, typename V>
inline bool Multicast<M, G, V>::isMulticast() const
{
    return iv_handle.fields.is_multicast;
}

}

#endif
