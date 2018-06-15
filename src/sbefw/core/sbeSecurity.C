/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeSecurity.C $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2018                        */
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
#include "sbeSecurity.H"
#include "sbetrace.H"
#include "sbeglobals.H"

#include "sbeSecurityGen.H"

namespace SBE_SECURITY
{

// Figure out at compile time, the number of shifts required for the mask
constexpr uint32_t get_shift_len(uint32_t mask, uint8_t shifts = 0)
{
    return ((mask>>shifts) & 0x01) ? (shifts) : (get_shift_len(mask, ++shifts));
}

template <typename Func>
map_t<bool, int32_t> binary_search(
                    const uint32_t search_key,
                    range_t<int32_t> x_range,
                    Func get_element)
{
    map_t<bool, int32_t> ret = {false, 0}; // found=false

    while((x_range.start <= x_range.end) &&
          (ret.key == false))
    {
        int32_t midpoint = (x_range.start + x_range.end) / 2;
        SBE_DEBUG("binary_search : midpoint[0x%08x]",
                midpoint);
        uint32_t ele = get_element(midpoint);
        SBE_DEBUG("binary_search : search_key[0x%08x] ele[0x%08x]",
                search_key,
                ele);
        if(search_key == ele)
        {
            ret.key = true;
            ret.value = midpoint;
        }
        else if(search_key < ele)
        {
            x_range.end = midpoint - 1;
        }
        else
        {
            x_range.start = midpoint + 1;
        }
        SBE_DEBUG("binary_search : x_range.start[0x%08x] x_range.end[0x%08x]",
                                   x_range.start,
                                   x_range.end);
    }
    SBE_DEBUG("binary_search : ret[%d]",ret.key);
    return ret;
}

template <typename M1_T, typename M1_U,
          typename M2_T, typename M2_U,
          typename T3>
bool _is_present(const table< map_t< range_t<M1_T>, M1_U > > &table1,
                 const table< map_t<M2_T, M2_U> > &table2,
                 const table< T3 > &table3,
                 const uint32_t i_addr)
{
#define SBE_FUNC "SBE_SECURITY::_is_present"
    SBE_ENTER(SBE_FUNC);
    for(size_t i = 0; i < table1.size; i++)
    {
        uint32_t search_key = (i_addr & table1.mask) >> get_shift_len(table1.mask);
        if((table1.table[i].key.start <= search_key) &&
           (table1.table[i].key.end >= search_key))
        {
            SBE_DEBUG(SBE_FUNC" table1:found key[0x%x] table index[%d]",
                                                        search_key, i);
            // Found the range where key might belong to
            search_key = (i_addr & table2.mask) >> get_shift_len(table2.mask);
            range_t<int32_t> search_range = {};
            search_range.start = i ? table1.table[i-1].value : 0;
            search_range.end = table1.table[i].value - 1;
            map_t<bool, int32_t> search_result =
                binary_search(
                        search_key,
                        search_range,
                        [&table2](int32_t midpoint) -> uint32_t {
                            return table2.table[midpoint].key;
                        });
            if(search_result.key == true)
            {
                SBE_DEBUG(SBE_FUNC" table2:found key[0x%x] table index[%d]",
                                                search_key,
                                                search_result.value);
                // Found the key
                search_range.start = (search_result.value ?
                                table2.table[search_result.value-1].value : 0);
                search_range.end =
                                table2.table[search_result.value].value - 1;
                search_key = (i_addr & table3.mask) >>
                                               get_shift_len(table3.mask);
                search_result = binary_search(
                                            search_key,
                                            search_range,
                                            [&table3](int32_t midpoint) -> uint32_t {
                                                return table3.table[midpoint];
                                            });
                if(search_result.key == true)
                {
                    SBE_DEBUG(SBE_FUNC" table3:found key[0x%x] table index[%d]",
                                                search_key,
                                                search_result.value);
                    // Found the number
                    return true;
                }
            }
        }
    }
    SBE_EXIT(SBE_FUNC);
    return false;
#undef SBE_FUNC
}

template <typename T1, typename T2 >
bool _is_present(const table< map_t< T1, T2 > > &table1,
                 const T1 i_addr,
                 const T2 i_mask)
{
#define SBE_FUNC "SBE_SECURITY::_is_present "
    SBE_ENTER(SBE_FUNC"Searching address/mask table");
    bool ret = false;
    for(size_t i = 0; i < table1.size; i++)
    {
        // Not using mask in table for search
        if((table1.table[i].key ==  i_addr) &&
           (( i_mask & (~table1.table[i].value)) == 0 ))
        {
            SBE_DEBUG(SBE_FUNC" table1:found addr[0x%x] table index[%d]",
                                                        i_addr, i);
            ret = true;
            break;
        }
    }
    SBE_EXIT(SBE_FUNC);
    return ret;
#undef SBE_FUNC
}
bool isAllowed(const uint32_t i_addr, accessType type)
{
    bool ret = true;
    if(SBE_GLOBAL->sbeFWSecurityEnabled)
    {
        if(type == WRITE)
            ret =  WHITELIST::isPresent(i_addr);
        else if(type == READ)
            ret =  !BLACKLIST::isPresent(i_addr);
        SBE_INFO("SBE_SECURITY access[%d] allowed[%d] addr[0x%08x]",
                                        type, ret, i_addr);
    }
    return ret;
}

} // namespace SBE_SECURITY
