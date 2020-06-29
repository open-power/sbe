/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_skew_adjust_setup.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
/// @file  p10_sbe_skew_adjust_setup.C
///
/// @brief  Drop Dcadj reset and bypass, Drop skewadjust reset and bypass
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE:SGPE
//------------------------------------------------------------------------------

#include "p10_sbe_skew_adjust_setup.H"
#include "p10_scom_perv_5.H"
#include <target_filters.H>
#include <multicast_group_defs.H>

enum P10_SBE_SKEW_ADJUST_SETUP_Private_Constants
{
    DCADJ_COMP_DLY = 0x0040,
    DCADJ_DAC_DLY = 0x0010,
    DCADJ_LOW_PASS_DLY = 0x2000,
    SKEWADJ_IGNORE_CNT = 0x2,
    SKEWADJ_WAIT_CNT = 0x8,
};

fapi2::ReturnCode p10_sbe_skew_adjust_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint16_t> l_attr_skewadj_core_override, l_attr_skewadj_cache_override, l_attr_dcadj_override ,
          l_attr_dcadj_target_override;
    uint8_t l_attr_skewadj_bypass, l_attr_dcadj_bypass;

    FAPI_DBG("p10_sbe_skew_adjust_setup : Entering ...");

    auto l_mc_core = i_target_chip.getMulticast(fapi2::MCGROUP_ALL_EQ, fapi2::MCCORE_ALL);
    auto l_mc_eq = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_ALL_EQ);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_DCADJ_BYPASS, i_target_chip, l_attr_dcadj_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SKEWADJ_BYPASS, i_target_chip, l_attr_skewadj_bypass));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_DCADJ_DCC_OVERRIDE, i_target_chip, l_attr_dcadj_override));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_DCADJ_TARGET_OVERRIDE, i_target_chip, l_attr_dcadj_target_override));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SKEWADJ_CORE_PDLY_OVERRIDE, i_target_chip, l_attr_skewadj_core_override));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SKEWADJ_CACHE_PDLY_OVERRIDE, i_target_chip, l_attr_skewadj_cache_override));

    FAPI_DBG("Drop Dcadj / Skewadj resets");
    l_data64.flush <1>().clearBit< 16, 8 >();
    FAPI_TRY(fapi2::putScom(l_mc_eq, NET_CTRL1_RW_WAND, l_data64));

    // SkewAdj
    FAPI_DBG("Put SkewAdj into INIT state");
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010320, 0x0));

    FAPI_DBG("Put SkewAdj into HOLD state");
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010321, 0x0));

    FAPI_DBG("Set up wait cycles between adjust and next measurement");
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 6 >(SKEWADJ_WAIT_CNT);
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010328, l_data64));

    FAPI_DBG("Set up number of sensor toggles to ignore");
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 4 >(SKEWADJ_IGNORE_CNT);
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010329, l_data64));

    FAPI_DBG("Set up core PDLY override");
    l_data64.flush<0>().writeBit<0>(l_attr_skewadj_core_override.getBit<0>());
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010332, l_data64));
    l_data64.insert< 4, 4, 12 >(l_attr_skewadj_core_override);
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010326, l_data64));

    FAPI_DBG("Set up cache PDLY override");
    l_data64.flush<0>().writeBit<0>(l_attr_skewadj_cache_override.getBit<0>());
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010333, l_data64));
    l_data64.insert< 4, 4, 12 >(l_attr_skewadj_cache_override);
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010327, l_data64));

    // DcAdj
    FAPI_DBG("Put DCAdj into INIT state");
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010300, 0x0));

    FAPI_DBG("Put DCAdj into HOLD state");
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010301, 0x0));

    FAPI_DBG("Set up low pass filter delay");
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 16 >(DCADJ_LOW_PASS_DLY);
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010309, l_data64));

    FAPI_DBG("Set up DAC settlement delay");
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 16 >(DCADJ_DAC_DLY);
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x2001030A, l_data64));

    FAPI_DBG("Set up comparator settlement delay");
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 16 >(DCADJ_COMP_DLY);
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x2001030B, l_data64));

    FAPI_DBG("Set up DCC override");
    l_data64.flush<0>().writeBit<0>(l_attr_dcadj_override.getBit<0>());
    l_data64.insert< 4, 8, 8 >(l_attr_dcadj_override);
    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010306, l_data64));

    FAPI_DBG("Set up duty cycle target");
    l_data64.flush<0>();

    if(l_attr_dcadj_target_override.getBit<0>())
    {
        uint8_t dcadj_target_override;
        l_attr_dcadj_target_override.extract< 8, 8 >(dcadj_target_override);
        l_data64.insertFromRight< 0, 7 >(0x40 + dcadj_target_override);
    }
    else
    {
        l_data64.insertFromRight< 0, 7 >(0x40);
    }

    FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010308, l_data64));


    l_data64.flush <1>();

    if(!l_attr_dcadj_bypass)
    {
        l_data64.clearBit< 0, 8 >();
    }

    if(!l_attr_skewadj_bypass)
    {
        l_data64.clearBit< 8, 8 >();
    }

    FAPI_DBG("Drop SkewAdj and DCAdj bypass");
    FAPI_TRY(fapi2::putScom(l_mc_eq, NET_CTRL1_RW_WAND, l_data64));


    if(!l_attr_dcadj_bypass && !(l_attr_dcadj_override.getBit<0>()))
    {
        FAPI_DBG("Put DCAdj into ADJUST state");
        FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010302, 0x0));
    }

    if(!l_attr_skewadj_bypass && !(l_attr_skewadj_core_override.getBit<0>()))
    {
        FAPI_DBG("Put SkewAdj into ADJUST state");
        FAPI_TRY(fapi2::putScom(l_mc_core, 0x20010322, 0x0));
    }

    FAPI_DBG("p10_sbe_skew_adjust_setup : Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
