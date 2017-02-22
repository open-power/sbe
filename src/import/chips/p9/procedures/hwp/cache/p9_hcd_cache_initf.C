/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/// @file  p9_hcd_cache_initf.C
/// @brief EX (non-core) scan init
///
/// Procedure Summary:
///   Initfiles in procedure defined on VBU ENGD wiki
///   Check for the presence of cache FUNC override rings from image;
///   if found, apply;  if not, apply cache base FUNC rings from image
///   Note:  FASTINIT ring (eg CMSK ring) is setup at this point to limit the
///   stumps that participate in FUNC ring scanning (this is new for P9).
///   (need to make sure the image build support is in place)
///   Note: all caches that are in the Cache Multicast group will be
///   initialized to the same values via multicast scans

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p9_hcd_cache_initf.H"
#include <p9_hcd_common.H>
#include <p9_quad_scom_addresses.H>
#include <p9_ring_id.h>

static const uint64_t RING_INDEX[10] =
{
    0, 5039, 5100, 5664, 5725, 5973, 6034, 6282, 6343, 17871
};

//------------------------------------------------------------------------------
// Procedure: EX (non-core) scan init
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target,
    const uint64_t* i_ring_save_data)
{
    FAPI_INF(">>p9_hcd_cache_initf");
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint64_t> l_data64_2;
    uint8_t                 l_isMpipl = 0;
    uint8_t                 l_isRingSaveMpipl = 0;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

#ifndef __PPE__
    uint8_t l_attr_system_ipl_phase;
    uint8_t l_attr_runn_mode;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, l_sys,
                           l_attr_system_ipl_phase));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_MODE,        l_sys,
                           l_attr_runn_mode));

#endif

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_MPIPL, l_sys, l_isMpipl),
             "fapiGetAttribute of ATTR_IS_MPIPL failed!");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_RING_SAVE_MPIPL, l_chip, l_isRingSaveMpipl),
             "fapiGetAttribute of ATTR_CHIP_EC_FEATURE_RING_SAVE_MPIPL failed");

    FAPI_DBG("Scan eq_fure ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_fure),
             "Error from putRing (eq_fure)");
    FAPI_DBG("Scan eq_ana_func ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_ana_func),
             "Error from putRing (eq_ana_func)");

    for (auto& l_ex_target : i_target.getChildren<fapi2::TARGET_TYPE_EX>())
    {
        FAPI_DBG("Scan ex_l2_fure ring");
        FAPI_TRY(fapi2::putRing(l_ex_target, ex_l2_fure),
                 "Error from putRing (ex_l2_fure)");
        FAPI_DBG("Scan ex_l2_mode ring");
        FAPI_TRY(fapi2::putRing(l_ex_target, ex_l2_mode),
                 "Error from putRing (ex_l2_mode)");
        FAPI_DBG("Scan ex_l3_fure ring");
        FAPI_TRY(fapi2::putRing(l_ex_target, ex_l3_fure),
                 "Error from putRing (ex_l3_fure)");

#ifndef __PPE__

        if (l_attr_system_ipl_phase ==
            fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED)
        {
            FAPI_DBG("Cache contained: Skipping ex_l3_refr ring scan");
        }
        else
        {
#endif
            FAPI_DBG("Scan ex_l3_refr_fure ring");
            FAPI_TRY(fapi2::putRing(l_ex_target, ex_l3_refr_fure),
                     "Error from putRing (ex_l3_refr_fure)");
#ifndef __PPE__
        }

#endif
    }

#ifndef __PPE__

    if (l_attr_system_ipl_phase ==
        fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED)
    {
        if (l_attr_runn_mode)
        {
            FAPI_DBG("RUN-N mode drop fences for clock sync");
            l_data64.flush<0>();
            l_data64.setBit<3>();
            l_data64.setBit<4>();
            l_data64.setBit<5>();
            l_data64.setBit<10>();
            l_data64.setBit<11>();
            l_data64.setBit<14>();

            FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, l_data64));
        }
    }

#endif

    if (l_isMpipl && l_isRingSaveMpipl)
    {
        l_data64.flush<0>();
        l_data64.setBit<4>();
        l_data64.setBit<5>();
        l_data64.setBit<11>();
        l_data64.setBit<59>();

        FAPI_TRY(fapi2::putScom(i_target,
                                EQ_SCAN_REGION_TYPE,
                                l_data64));

        l_data64.flush<0>().set(0xa5a5a5a5a5a5a5a5);
        FAPI_TRY(fapi2::putScom(i_target,
                                EQ_SCAN64,
                                l_data64));

        for(uint32_t l_spin = 1; l_spin < 10; l_spin++)
        {
            /***********RING_INDEX***********/
            //  {0,    0},
            //  {5039, 0xE000000000000000}, //3
            //  {5100, 0xC1E061FFED5F0000}, //29
            //  {5664, 0xE000000000000000}, //3
            //  {5725, 0xC1E061FFED5F0000}, //29
            //  {5973, 0xE000000000000000}, //3
            //  {6034, 0xC1E061FFED5F0000}, //29
            //  {6282, 0xE000000000000000}, //3
            //  {6343, 0xC1E061FFED5F0000}, //29
            //  {17871, 0}
            /**********************************/
            uint64_t l_scandata = ((l_spin == 0) || (l_spin == 9)) ? 0x0 : (l_spin & 0x1) ?
                                  0xE000000000000000 : 0xC1E061FFED5F0000;
            l_data64.flush<0>().set((RING_INDEX[l_spin] - RING_INDEX[l_spin - 1]) << 32);

            FAPI_TRY(fapi2::putScom(i_target,
                                    EQ_SCAN_LONG_ROTATE,
                                    l_data64));
            l_data64.flush<0>();

            do
            {
                FAPI_TRY(fapi2::getScom(i_target,
                                        EQ_CPLT_STAT0,
                                        l_data64));
            }
            while (l_data64.getBit<8>() == 0);

            l_data64.flush<0>();

            if (l_spin == 9)
            {
                FAPI_TRY(fapi2::getScom(i_target,
                                        EQ_SCAN64,
                                        l_data64));

                if(l_data64 != 0xa5a5a5a5a5a5a5a5)
                {
                    FAPI_ASSERT(false,
                                fapi2::P9_HCD_CACHE_INITF_INCORRECT_EQ_SCAN64_VAL()
                                .set_EQ_SCAN64_VAL(l_data64),
                                "Incorrect Value from EQ_SCAN64, Expected Value [0xa5a5a5a5a5a5a5a5]");
                }
            }
            else
            {
                l_data64.flush<0>();
                FAPI_TRY(fapi2::getScom(i_target,
                                        EQ_SCAN64,
                                        l_data64));

                l_data64_2.set( (l_data64 & ~l_scandata) | i_ring_save_data[l_spin - 1]);
                FAPI_TRY(fapi2::putScom(i_target,
                                        EQ_SCAN64,
                                        l_data64_2));
            }
        }

        l_data64.flush<0>();
        FAPI_TRY(fapi2::putScom(i_target,
                                EQ_SCAN_REGION_TYPE,
                                l_data64));
    }

fapi_try_exit:
    FAPI_INF("<<p9_hcd_cache_initf");
    return fapi2::current_err;
}
