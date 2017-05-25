/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeSecurity.H $                                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017                             */
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
#ifndef __SBE_SECURITY_H
#define __SBE_SECURITY_H

#include <stdint.h>

namespace SBE_SECURITY
{
    typedef enum
    {
        READ = 0,
        WRITE
    } accessType;

    // type to define a range
    template <typename T>
    struct
    {
        T start;
        T end;
    } range;

    // key-value map used for the tables
    template <typename T, typename U>
    struct map_t
    {
        T key;
        U value;
    };

    // table data structure
    template <typename T>
    struct table
    {
        uint8_t size;
        uint32_t mask;
        T *table;
    };

    /* @brief isAllowed       - Public function used for address verification
     *                          for a given type of access
     *
     * @param[in] i_addr      - given address to verify
     * @param[in] accessType  - access type - READ/WRITE
     *
     * @return                - boolean to denote if the access on the address
     *                          is allowed or not
     */
    bool isAllowed(const uint32_t i_addr, accessType type);

    /* @brief _is_present     - Look up tables to find if the given
     *                          address is present
     * @param[in] table1      - table 1 - map with a range and running count
     * @param[in] table2      - table 2 - map with value and running count
     * @param[in] table3      - table 3 - array with values
     * @param[in] i_addr      - given address to look up
     *
     * @return                - boolean to denote if the address in present
     *                          in the list of tables
     */
    template <typename M1_T, typename M1_U,
              typename M2_T, typename M2_U,
              typename T3>
    bool _is_present(const table< map_t< range_t<M1_T>, M1_U > > &table1,
                     const table< map_t<M2_T, M2_U> > &table2,
                     const table< T3 > &table3,
                     const uint32_t i_addr);
}
#endif //__SBE_SECURITY_H
