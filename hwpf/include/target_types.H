/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: hwpf/include/target_types.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2016                        */
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
/**
 * @file target_types.H
 * @brief definitions for fapi2 target types
 */

#ifndef __FAPI2_TARGET_TYPES__
#define __FAPI2_TARGET_TYPES__

#ifndef __ASSEMBLER__

/// FAPI namespace
namespace fapi2
{

    ///
    /// @enum fapi::TargetType
    /// @brief Types, kinds, of targets
    /// @note TYPE_NONE is used to represent empty/NULL targets in lists
    /// or tables. TYPE_ALL is used to pass targets to methods which
    /// can act generally on any type of target
    ///
    /// Target Kind
    enum TargetType
    {
        TARGET_TYPE_NONE              = 0x00,  ///< No type
        TARGET_TYPE_PROC_CHIP         = 0x01,  ///< Processor type
        TARGET_TYPE_EX                = 0x02,  ///< Ex type
        TARGET_TYPE_CORE              = 0x04,  ///< Core type
        TARGET_TYPE_EQ                = 0x08,  ///< EQ type
        TARGET_TYPE_MCS               = 0x10,  ///< MCS type
        TARGET_TYPE_PERV              = 0x20,  ///< Pervasive type
        TARGET_TYPE_MCAST             = 0x40,  ///< Multicast type
        TARGET_TYPE_MI                = 0x80,  ///< MI type
        TARGET_TYPE_MCBIST            = 0x100, ///< MCBIST type
        TARGET_TYPE_SYSTEM            = 0x400, ///< System type
        TARGET_TYPE_ALL               = 0xFFF, ///< Any/All types

        // The following are actually illegal targets on PPE platforms
//         TARGET_TYPE_SYSTEM            = 0xFE, ///< System type
//         TARGET_TYPE_DIMM              = 0xFD, ///< DIMM type
//         TARGET_TYPE_MEMBUF_CHIP       = 0xFC, ///< Membuf type
//         TARGET_TYPE_MBA               = 0xFB, ///< MBA type
//         TARGET_TYPE_XBUS              = 0xFA, ///< XBUS type
//         TARGET_TYPE_ABUS              = 0xF9, ///< ABUS type
//         TARGET_TYPE_L4                = 0xF8, ///< L4 type
//         TARGET_TYPE_MCA               = 0xF7, ///< MCA type
//         TARGET_TYPE_MCBIST            = 0xF6, ///< MCBIST type
//         TARGET_TYPE_MIA               = 0xF5, ///< MIA type
//         TARGET_TYPE_MIS               = 0xF4, ///< MIS type
//         TARGET_TYPE_DMI               = 0xF3, ///< DMI type
//         TARGET_TYPE_OBUS              = 0xF2, ///< OBUS type
//         TARGET_TYPE_NV                = 0xF1, ///< NV bus type
//         TARGET_TYPE_SBE               = 0xF0, ///< SBE type
//         TARGET_TYPE_PPE               = 0xEF, ///< PPE type
//         TARGET_TYPE_PEC               = 0xEE, ///< PEC type
//         TARGET_TYPE_PHB               = 0xED, ///< PHB type
//         TARGET_TYPE_MI                = 0xEC, ///< MI type

        // Mappings to target types found in the error xml files
        TARGET_TYPE_EX_CHIPLET    = TARGET_TYPE_EX,
//        TARGET_TYPE_MBA_CHIPLET   = TARGET_TYPE_MBA,
        TARGET_TYPE_MCS_CHIPLET   = TARGET_TYPE_MCS,
//        TARGET_TYPE_XBUS_ENDPOINT = TARGET_TYPE_XBUS,
//        TARGET_TYPE_ABUS_ENDPOINT = TARGET_TYPE_ABUS,
    };

    ///
    /// @brief Typedef used when passing multiple TargetType values
    ///
    typedef uint16_t TargetTypes_t;

    ///
    /// @brief Enumeration of chiplet filters
    ///
    enum TargetFilter
    {
        TARGET_FILTER_TP          = 0x8000000000000000,  // Pervasive 1
        TARGET_FILTER_NEST_NORTH  = 0x4000000000000000,  // Pervasive 2
        TARGET_FILTER_NEST_SOUTH  = 0x2000000000000000,  // Pervasive 3
        TARGET_FILTER_NEST_EAST   = 0x1000000000000000,  // Pervasive 4
        TARGET_FILTER_NEST_WEST   = 0x0800000000000000,  // Pervasive 5
        TARGET_FILTER_XBUS        = 0x0400000000000000,  // Pervasive 6
        TARGET_FILTER_MC_WEST     = 0x0200000000000000,  // Pervasive 7
        TARGET_FILTER_MC_EAST     = 0x0100000000000000,  // Pervasive 8
        TARGET_FILTER_OBUS0       = 0x0080000000000000,  // Pervasive 9
        TARGET_FILTER_OBUS1       = 0x0040000000000000,  // Pervasive 10
        TARGET_FILTER_OBUS2       = 0x0020000000000000,  // Pervasive 11
        TARGET_FILTER_OBUS3       = 0x0010000000000000,  // Pervasive 12
        TARGET_FILTER_PCI0        = 0x0008000000000000,  // Pervasive 13
        TARGET_FILTER_PCI1        = 0x0004000000000000,  // Pervasive 14
        TARGET_FILTER_PCI2        = 0x0002000000000000,  // Pervasive 15
        TARGET_FILTER_CACHE0      = 0x0001000000000000,  // Pervasive 16
        TARGET_FILTER_CACHE1      = 0x0000800000000000,  // Pervasive 17
        TARGET_FILTER_CACHE2      = 0x0000400000000000,  // Pervasive 18
        TARGET_FILTER_CACHE3      = 0x0000200000000000,  // Pervasive 19
        TARGET_FILTER_CACHE4      = 0x0000100000000000,  // Pervasive 20
        TARGET_FILTER_CACHE5      = 0x0000080000000000,  // Pervasive 21
        TARGET_FILTER_CORE0       = 0x0000040000000000,  // Pervasive 32
        TARGET_FILTER_CORE1       = 0x0000020000000000,  // Pervasive 33
        TARGET_FILTER_CORE2       = 0x0000010000000000,  // Pervasive 34
        TARGET_FILTER_CORE3       = 0x0000008000000000,  // Pervasive 35
        TARGET_FILTER_CORE4       = 0x0000004000000000,  // Pervasive 36
        TARGET_FILTER_CORE5       = 0x0000002000000000,  // Pervasive 37
        TARGET_FILTER_CORE6       = 0x0000001000000000,  // Pervasive 38
        TARGET_FILTER_CORE7       = 0x0000000800000000,  // Pervasive 39
        TARGET_FILTER_CORE8       = 0x0000000400000000,  // Pervasive 40
        TARGET_FILTER_CORE9       = 0x0000000200000000,  // Pervasive 41
        TARGET_FILTER_CORE10      = 0x0000000100000000,  // Pervasive 42
        TARGET_FILTER_CORE11      = 0x0000000080000000,  // Pervasive 43
        TARGET_FILTER_CORE12      = 0x0000000040000000,  // Pervasive 44
        TARGET_FILTER_CORE13      = 0x0000000020000000,  // Pervasive 45
        TARGET_FILTER_CORE14      = 0x0000000010000000,  // Pervasive 46
        TARGET_FILTER_CORE15      = 0x0000000008000000,  // Pervasive 47
        TARGET_FILTER_CORE16      = 0x0000000004000000,  // Pervasive 48
        TARGET_FILTER_CORE17      = 0x0000000002000000,  // Pervasive 49
        TARGET_FILTER_CORE18      = 0x0000000001000000,  // Pervasive 50
        TARGET_FILTER_CORE19      = 0x0000000000800000,  // Pervasive 51
        TARGET_FILTER_CORE20      = 0x0000000000400000,  // Pervasive 52
        TARGET_FILTER_CORE21      = 0x0000000000200000,  // Pervasive 53
        TARGET_FILTER_CORE22      = 0x0000000000100000,  // Pervasive 54
        TARGET_FILTER_CORE23      = 0x0000000000080000,  // Pervasive 55

        // Composite filters follow

        // Pervasive 32-55 (all cores)
        TARGET_FILTER_ALL_CORES   = (TARGET_FILTER_CORE0 |
                TARGET_FILTER_CORE1 | TARGET_FILTER_CORE2 |
                TARGET_FILTER_CORE3 | TARGET_FILTER_CORE4 |
                TARGET_FILTER_CORE5 | TARGET_FILTER_CORE6 |
                TARGET_FILTER_CORE7 | TARGET_FILTER_CORE8 |
                TARGET_FILTER_CORE9 | TARGET_FILTER_CORE10 |
                TARGET_FILTER_CORE11 | TARGET_FILTER_CORE12 |
                TARGET_FILTER_CORE13 | TARGET_FILTER_CORE14 |
                TARGET_FILTER_CORE15 | TARGET_FILTER_CORE16 |
                TARGET_FILTER_CORE17 | TARGET_FILTER_CORE18 |
                TARGET_FILTER_CORE19 | TARGET_FILTER_CORE20 |
                TARGET_FILTER_CORE21 | TARGET_FILTER_CORE22 |
                TARGET_FILTER_CORE23),

        // Pervasive 16-21 (all caches)
        TARGET_FILTER_ALL_CACHES  = (TARGET_FILTER_CACHE0 |
                TARGET_FILTER_CACHE1 | TARGET_FILTER_CACHE2 |
                TARGET_FILTER_CACHE3 | TARGET_FILTER_CACHE4 |
                TARGET_FILTER_CACHE5),

        // Pervasive 2-5 (eg N0-N3) < req'd
        TARGET_FILTER_ALL_NEST    = (TARGET_FILTER_NEST_NORTH |
                TARGET_FILTER_NEST_SOUTH | TARGET_FILTER_NEST_EAST |
                TARGET_FILTER_NEST_WEST),

        // Pervasive 2-4 (eg N0-N2) < req'd
        TARGET_FILTER_NEST_SLAVES =
            (TARGET_FILTER_NEST_NORTH | TARGET_FILTER_NEST_SOUTH |
             TARGET_FILTER_NEST_EAST),

        // Pervasive 5 (eg N32) < req'd
        TARGET_FILTER_NEST_MASTER = TARGET_FILTER_NEST_WEST,

        // Pervasive 7-8 (eg MC0-MC1)
        TARGET_FILTER_ALL_MC      =
            (TARGET_FILTER_MC_WEST | TARGET_FILTER_MC_EAST),

        // Pervasive 9-12 (OB0-OB3)
        TARGET_FILTER_ALL_OBUS    =
            (TARGET_FILTER_OBUS0 | TARGET_FILTER_OBUS1 | TARGET_FILTER_OBUS2 |
             TARGET_FILTER_OBUS3),

        // Pervasive 13-15 (PCI0-PCI2)
        TARGET_FILTER_ALL_PCI =
            (TARGET_FILTER_PCI0 | TARGET_FILTER_PCI1 | TARGET_FILTER_PCI2),

        // Sync mode filter = All NEST + All MCS
        TARGET_FILTER_SYNC_MODE_NEST =
            (TARGET_FILTER_ALL_NEST | TARGET_FILTER_ALL_MC),

        // All IO Targets except NEST
        TARGET_FILTER_ALL_IO_EXCEPT_NEST =
            (TARGET_FILTER_XBUS | TARGET_FILTER_ALL_PCI | TARGET_FILTER_ALL_OBUS),

        // All sync mode IO except NEST
        TARGET_FILTER_SYNC_MODE_ALL_IO_EXCEPT_NEST =
            (TARGET_FILTER_ALL_MC | TARGET_FILTER_XBUS | TARGET_FILTER_ALL_PCI |
             TARGET_FILTER_ALL_OBUS),

        // All sync mode NEST slaves
        TARGET_FILTER_SYNC_MODE_NEST_SLAVES =
            (TARGET_FILTER_ALL_MC | TARGET_FILTER_NEST_SLAVES),

        // All sync mode IO
        TARGET_FILTER_SYNC_MODE_ALL_IO =
            (TARGET_FILTER_ALL_MC | TARGET_FILTER_ALL_NEST |
             TARGET_FILTER_ALL_OBUS | TARGET_FILTER_ALL_PCI |
             TARGET_FILTER_XBUS),

        // All IO
        TARGET_FILTER_ALL_IO = (TARGET_FILTER_ALL_NEST |
                TARGET_FILTER_ALL_OBUS | TARGET_FILTER_ALL_PCI |
                TARGET_FILTER_XBUS),

        // All sync mode except TP
        TARGET_FILTER_SYNC_MODE_ALL_EXCEPT_TP =
            (TARGET_FILTER_ALL_MC | TARGET_FILTER_ALL_NEST |
             TARGET_FILTER_ALL_OBUS | TARGET_FILTER_ALL_PCI |
             TARGET_FILTER_XBUS | TARGET_FILTER_ALL_CORES |
             TARGET_FILTER_ALL_CACHES),
    };

    /// @cond
    constexpr TargetType operator|(TargetType x, TargetType y)
    {
        return static_cast<TargetType>(static_cast<int>(x) |
                                       static_cast<int>(y));
    }

    template<uint64_t V>
    class bitCount {
    public:
        // Don't use enums, too hard to compare
        static const uint8_t count = bitCount<(V >> 1)>::count + (V & 1);
    };

    template<>
    class bitCount<0> {
    public:
        static const uint8_t count = 0;
    };
    /// @endcond
};

#endif  // __ASSEMBLER__
#endif  // __FAPI2_TARGET_TYPES__
