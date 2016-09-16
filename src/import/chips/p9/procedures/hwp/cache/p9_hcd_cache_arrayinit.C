/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_arrayinit.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
///
/// @file  p9_hcd_cache_arrayinit.C
/// @brief EX Initialize arrays
///
/// Procedure Summary:
///   Use ABIST engine to zero out all arrays
///   Upon completion, scan0 flush all rings
///     except Vital, Repair, GPTR, TIME and DPLL

// *HWP HWP Owner          : David Du      <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still    <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_misc_scom_addresses.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_cache_arrayinit.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

enum P9_HCD_CACHE_ARRAYINIT_Private_Constants
{
    LOOP_COUNTER = 0x0000000000042FFF,
    SELECT_SRAM = 0x1,
    SELECT_EDRAM = 0x0,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

//-----------------------------------------------------------------------------
// Procedure: Initialize Cache Arrays
//-----------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_arrayinit(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_arrayinit");
    fapi2::buffer<uint64_t>                     l_data64;
    uint16_t                                    l_region_array;
    uint16_t                                    l_region_scan0;
    uint8_t                                     l_attr_chip_unit_pos = 0;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::Target<fapi2::TARGET_TYPE_PERV>      l_perv =
        i_target.getParent<fapi2::TARGET_TYPE_PERV>();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_perv,
                           l_attr_chip_unit_pos));
    l_attr_chip_unit_pos = l_attr_chip_unit_pos - p9hcd::PERV_TO_EQ_POS_OFFSET;

    FAPI_TRY(getScom(l_chip, PU_OCB_OCI_QCSR_SCOM, l_data64));
    FAPI_DBG("Working on cache[%d] good EXs in QCSR[%016llX]",
             l_attr_chip_unit_pos, l_data64);

    l_region_array = p9hcd::SCAN0_REGION_ALL_BUT_EX_DPLL;
    l_region_scan0 = p9hcd::SCAN0_REGION_ALL_BUT_EX_ANEP_DPLL;

    if (l_data64 & BIT64(l_attr_chip_unit_pos << 1))
    {
        l_region_array |= p9hcd::SCAN0_REGION_EX0_L2_L3_REFR;
        l_region_scan0 |= p9hcd::SCAN0_REGION_EX0_L2_L3_REFR;
    }

    if (l_data64 & BIT64((l_attr_chip_unit_pos << 1) + 1))
    {
        l_region_array |= p9hcd::SCAN0_REGION_EX1_L2_L3_REFR;
        l_region_scan0 |= p9hcd::SCAN0_REGION_EX1_L2_L3_REFR;
    }

    /// @todo add DD1 attribute control
    FAPI_DBG("DD1 only: set sdis_n(flushing LCBES condition workaround");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CONF0_OR, MASK_SET(34)));

#ifndef P9_HCD_STOP_SKIP_ARRAYINIT

    FAPI_DBG("Arrayinit all regions except vital/DPLL");
    FAPI_TRY(p9_perv_sbe_cmn_array_init_module(l_perv,
             l_region_array,
             LOOP_COUNTER,
             SELECT_SRAM,
             SELECT_EDRAM,
             START_ABIST_MATCH_VALUE));

#endif

#ifndef P9_HCD_STOP_SKIP_FLUSH

    //--------------------------------------------
    // perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the loop can be removed.

    FAPI_DBG("Scan0 region:all_but_anep_dpll type:all_but_gptr_repr_time rings");

    for(uint32_t l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
        FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                              l_region_scan0,
                                              p9hcd::SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

#endif

    /// @todo add DD1 attribute control
    FAPI_DBG("DD1 only: reset sdis_n(flushing LCBES condition workaround");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CONF0_CLEAR, MASK_SET(34)));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_arrayinit");
    return fapi2::current_err;
}

