/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/securityAlgo.H $                               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
#ifndef __SECURITY_ALGO_H
#define __SECURITY_ALGO_H

#include <stdint.h>
#include <stddef.h>

/*
    Whitelist , Blacklist and Greylist  are part of the Security validation
    of any register. Each list has been sub divided into 3 tables [t1, t2 and
    t3] as mentioned bellow. Table data will be used on security algorithm to
    validates register address is part of Whitelist, Blacklist and Greylist
    or not. 

    # -----------------------------------------------------------------#
    # Step 1:             [register list]                              #
    #                           ||                                     #
    #                           \/                                     #
    #         {base_address1 : [chiplet range1, chiplet range2]}       #
    #..................................................................#
    # Eg: [01ABCDEF, 02ABCDEF, 04ABCDEF, 05ABCDEF, 10UVWXYZ, 11UVWXYZ] #
    #                           ||                                     #
    #                           \/                                     #
    #         {ABCDEF : [0102, 0405], UVWXYZ : [1011]}                 #
    # -----------------------------------------------------------------#
    # Step 2: {base_address1 : [chiplet range1, chiplet range2]}       #
    #                           ||                                     #
    #                           \/                                     #
    #         {chiplet range1 : [base_address1, base_address2]}        #
    #..................................................................#
    # Eg:     {ABCDEF : [0102, 0405], UVWXYZ : [0405, 1020]}           #
    #                           ||                                     #
    #                           \/                                     #
    #     {0102 : [ABCDEF], 0405 : [ABCDEF, UVWXYZ], 1020 : [UVWXYZ]}  #
    # -----------------------------------------------------------------#
    # Step 3: {chiplet range1 : [base_address1, base_address2]}        #
    #                           ||                                     #
    #                           \/                                     #
    #         {chiplet range1 : {key1 : [base_addr1, base_Addr2]}}     #
    #..................................................................#
    # Eg: {0102 : [ABCDEF], 0405 : [ABCDEF, UVWXYZ], 1020 : [UVWXYZ]}  #
    #                           ||                                     #
    #                           \/                                     #
    #               {0102 : {AB : [CDEF]},                             #
    #                0405 : {AB : [CDEF], UV : [WXYZ]},                #
    #                1020 : {UV : [WXYZ]}}                             #
    # -----------------------------------------------------------------#
    # Step 4: {chiplet range1 : {key1 : [base_addr1, base_Addr2]}}     #
    #                           ||                                     #
    #                           \/                                     #
    #                  table1, table2, table3                          #
    #..................................................................#
    # Eg:           {0102 : {AB : [CD, EF]},                           #
    #                0405 : {AB : [CD, EF], UV : [WX, YZ]},            #
    #                1020 : {UV : [WX, YZ]}}                           #
    #                           ||                                     #
    #                           \/                                     #
    #         table1: {0102 : 1, 0405 : 3, 1020 : 4}                   #
    #         table2: {AB : 2, AB : 4, UV : 6, UV : 8}                 #
    #         table3: {CD, EF, CD, EF, WX, YZ, WX, YZ}                 #
    # -----------------------------------------------------------------#


    table 1:
       keys   = 2byte - (start , end) of
                the ranges in bit 0-7 of the 32-bit address
       values = running count of the paths to table2

    for example - if ranges are 0x20-0x37, 0x01-0x01, 0x10-0x17 and has
                  1, 2 and 3 paths respectively to table 2
    then table 1 will have {0x20,0x37} = 01, {0x01,0x01} = 3, {0x10,0x17} = 7

    1 byte for running count - we are good with uint8_t till the
    total paths are less than 256

    table 2
       keys   = unique 1 byte numbers having same prefix for each
                range in table 1 key - bit 8-15 from a 32-bit address
       values = running count of paths from each of the keys

    for example - if element a has 1 path, b has 0 and c has 3 paths
    then table 1 will have a = 1, b = 1, c = 4

    1 byte for key
    1 byte for number of paths
    We are good with uint8_t,
    till the number of paths to table 3 from each key is less than 256

    table 3
       values = 2 byte value bit 16-31 of the 32-bit address

 */

#define  WHITELIST_TABLE1_MASK 0xFF000000
#define  WHITELIST_TABLE2_MASK 0x00FF0000
#define  WHITELIST_TABLE3_MASK 0x0000FFFF
#define  BLACKLIST_TABLE1_MASK 0xFF000000
#define  BLACKLIST_TABLE2_MASK 0x00FF0000
#define  BLACKLIST_TABLE3_MASK 0x0000FFFF
#define  GREYLIST_TABLE1_MASK 0xFFFFFFFF

#ifndef __PPE__
    #pragma pack(1)
#endif
    // struct range_t and map_t are supporting data types in SecurityAlgo
    // type to define a range
    struct range_t
    {
        int32_t start;
        int32_t end;
    };
    // key-value map used for the tables
    struct map_t
    {
        bool key;
        int32_t value;
    };

   /*@brief Format of T1 table date */
    typedef struct
    {
        uint8_t key_start;
        uint8_t key_end;
        uint8_t value;
    } _t1_t; // Total 3 bytes

    /*@brief Format of T2 table date */
    typedef struct
    {
        uint8_t  key;
        uint16_t value;
    } _t2_t;  // Total 3 bytes

    /*@brief Format of T3 table date */
    typedef struct
    {
        uint16_t value;
    } _t3_t;  // Total 2 bytes


    /*@brief Format of Grey list table date */
    typedef struct
    {
        uint32_t key;
        uint64_t value;
    } _gl_t1_t;  // Total 12 bytes


    // table data structure
    /* @brief Format of White / Black list table-1 */
    typedef struct
    {
        uint32_t size;
        uint32_t mask;
        _t1_t *table;
    } _t1_table_t;

    /* @brief Format of White / Black list table-2 */
    typedef struct
    {
        uint32_t size;
        uint32_t mask;
        _t2_t *table;
    } _t2_table_t;

    /* @brief Format of White / Black list table-3 */
    typedef struct
    {
        uint32_t size;
        uint32_t mask;
        _t3_t *table;
    } _t3_table_t;

    /* @brief Format of Grey list table-1 */
    typedef struct
    {
        uint32_t size;
        uint32_t mask;
        _gl_t1_t *table;
    } _gl_t1_table_t;

    /* @brief Format of SecurityList dump header */
    typedef struct
    {
        uint16_t wl_t1_count;
        uint16_t wl_t2_count;
        uint16_t wl_t3_count;
        uint16_t bl_t1_count;
        uint16_t bl_t2_count;
        uint16_t bl_t3_count;
        uint16_t gl_t1_count;
    } sec_header_dump_t;

    // Security Access Types
    typedef enum
    {
        READ_ACCESS = 0,
        WRITE_ACCESS
    } secAccessType;

    typedef struct GenSecurityListTables
    {
        GenSecurityListTables(){}
        _t1_table_t  wl_t1;
        _t2_table_t  wl_t2;
        _t3_table_t  wl_t3;

        _t1_table_t  bl_t1;
        _t2_table_t  bl_t2;
        _t3_table_t  bl_t3;

        _gl_t1_table_t gl_t1;
    } GenSecurityListTables_t;
#ifndef __PPE__
    #pragma pack()
#endif

    static struct GenSecurityListTables secListTableSupport;
    // size of GenericFspMboxMessage_t header size
    constexpr size_t SEC_LIST_TABLE_HDR_SIZE = sizeof(sec_header_dump_t);

    /* @brief isAllowed       - Public function used for address verification
     *                          for a given type of access
     *
     * @param[in] i_addr      - given address to verify
     * @param[in] i_mask      - mask for data
     * @param[in] i_type      - access type - READ/WRITE
     *
     * @return                - boolean to denote if the access on the address
     *                          is allowed or not
     *
     bool _isAllowed(const uint32_t i_addr, const uint64_t i_mask,
                     accessType i_type);
 
     * @brief _is_present     - Look up tables to find if the given
     *                          address ii present
     * @param[in] table1      - table 1 - map with a range and running count
     * @param[in] table2      - table 2 - map with value and running count
     * @param[in] table3      - table 3 - array with values
     * @param[in] i_addr      - given address to look up
     *
     * @return                - boolean to denote if the address in present
     *                          in the list of tables
     */

    bool _is_present( const _t1_table_t &table1,
                      const _t2_table_t &table2,
                      const _t3_table_t &table3,
                      const uint32_t i_addr);

    /* @brief _is_present     - Look up tables to find if the given
     *                          address with mask is present
     * @param[in] table1      - table 1 - map with a range and running count
     * @param[in] i_addr      - given address to look up
     * @param[in] mask        - mask to look up
     *
     * @return                - boolean to denote if the address in present
     */
    bool _is_present(const _gl_t1_table_t &table1,
                     const uint32_t i_addr,
                     const uint64_t i_mask);

    /* @brief securityAccessTablesInit - Public function used for
     *           init all white, black and grey list table data
     * @param[in] i_i_buf      - tables data
     *
     * @return                - boolean to denote if the access on the table
     *                          data is allowed or not
     */
    bool securityAccessTablesInit(const void * i_buf );

    /* @brief isAccessAllowed - Public function used for address verification
     *                          for a given type of access
     *
     * @param[in] i_addr      - given address to verify
     * @param[in] i_mask      - mask for data
     * @param[in] i_type      - access type - READ_ACCESS / WRITE_ACCESS
     *
     * @return                - boolean to denote if the access on the address
     *                          is allowed or not
     */
    bool isAccessAllowed(const uint32_t i_addr, const uint64_t i_mask,
                   secAccessType i_type);
#endif //__SECURITY_ALGO_H
