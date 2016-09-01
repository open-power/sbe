/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/perv/p9_sbe_gptr_time_repr_initf.C $ */
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
//------------------------------------------------------------------------------
/// @file  p9_sbe_gptr_time_repr_initf.C
///
/// @brief Scan0 and Load repair, time and GPTR rings for all enabled chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Brian Silver <bsilver@us.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_gptr_time_repr_initf.H"

#include "p9_perv_scom_addresses.H"
#include "p9_perv_sbe_cmn.H"


enum P9_SBE_GPTR_TIME_REPR_INITF_Private_Constants
{
    REGIONS_EXCEPT_VITAL = 0x7FF,
    SCAN_TYPES_TIME_GPTR_REPR = 0x230
};

static fapi2::ReturnCode
p9_sbe_gptr_time_repr_initf_scan0_and_ring_module_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_gptr_time_repr_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    for (auto l_chplt_trgt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        FAPI_INF("Call sbe_gptr_time_repr_initf_scan0_and_ring_module_function");
        FAPI_TRY(p9_sbe_gptr_time_repr_initf_scan0_and_ring_module_function(
                     l_chplt_trgt));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Scan 0 on time, repair, gptr on all enabled chiplets
/// scan initialize GPTR,TIME and REPR Rings
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode
p9_sbe_gptr_time_repr_initf_scan0_and_ring_module_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    bool l_read_reg = false;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    FAPI_INF("Check for chiplet enable");
    //Getting NET_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL0, l_data64));
    l_read_reg = l_data64.getBit<0>();  //l_read_reg = NET_CTRL0.CHIPLET_ENABLE

    if ( l_read_reg )
    {
        FAPI_INF("run scan0 module for regions except vital scan types GPTR, TIME, REPR");
        FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chiplet, REGIONS_EXCEPT_VITAL,
                                              SCAN_TYPES_TIME_GPTR_REPR));


        //TODO:Load Ring Module : Scan initialize PLL BNDY chain
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
