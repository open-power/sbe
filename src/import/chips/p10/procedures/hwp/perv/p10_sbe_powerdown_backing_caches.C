/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_powerdown_backing_caches.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
/// @file  p10_sbe_powerdown_backing_caches.H
/// @brief  Determine the powered on caches and intimates QME to put the cores
///         to stop 11
// *HWP HWP Owner: Greg Still <stillgs@us.ibm.com>
// *HWP FW Owner : Prasad Bg Ranganath <prasadbgr@in.ibm.com>
// *HWP Team: PM
// *HWP Level: 2
// *HWP Consumed by: SBE
///
//------------------------------------------------------------------------------


// ----------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------

#include <p10_sbe_powerdown_backing_caches.H>
#include <p10_hcd_common.H>
#include <p10_pm_hcd_flags.h>
#include <p10_scom_proc.H>
#include <p10_scom_eq.H>
#include <p10_scom_c_0.H>
#include <multicast_group_defs.H>

using namespace scomt;
using namespace proc;
using namespace eq;
using namespace c;
using namespace p10hcd;


// 1000000 nanosecond = 1 millisecond
// total timeout = 10 milliseconds
static const uint64_t POLLTIME_NS          = 1000000;
static const uint64_t POLLTIME_MCYCLES     = 4000;
static const uint32_t TRIES_BEFORE_TIMEOUT = 500;

#define EQ_CORE_MASK   0xF

#define IS_BACKING_CACHE_CONFIG(vec, num) (vec & (EQ_CORE_MASK << SHIFT32(num-1)))

// ----------------------------------------------------------------------
// Procedure Function
// ----------------------------------------------------------------------
fapi2::ReturnCode
p10_sbe_powerdown_backing_caches(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_proc_target)
{
    FAPI_INF("> p10_sbe_powerdown_backing_caches...");

    fapi2::buffer<uint64_t> l_data64 = 0;
    uint32_t l_relative_core_pos = 0;
    uint64_t l_scrb_data = 0;
    uint64_t l_ssh_data = 0;
    uint8_t  l_eq_pos = 0;


    uint8_t l_attr_chip_unit_core_pos = 0;
    uint8_t l_attr_chip_unit_eq_pos = 0;
    fapi2::ATTR_BACKING_CACHES_VEC_Type l_attr_backing_vec = 0;
    auto l_eq_mc =
        i_proc_target.getMulticast<fapi2::TARGET_TYPE_EQ>(fapi2::MCGROUP_GOOD_EQ);

    auto eq_list =
        i_proc_target.getChildren<fapi2::TARGET_TYPE_EQ>(fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BACKING_CACHES_VEC,
                           i_proc_target,
                           l_attr_backing_vec),
             "fapiGetAttribute of ATTR_BACKING_CACHES_VEC failed");

    for( auto eq : eq_list )
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               eq,
                               l_attr_chip_unit_eq_pos),
                 "fapiGetAttribute of ATTR_CHIP_UNIT_POS for eq failed");

        l_eq_pos = (l_attr_chip_unit_eq_pos + 1) << 2;
        l_relative_core_pos = (IS_BACKING_CACHE_CONFIG(l_attr_backing_vec,
                               l_eq_pos)) >> SHIFT32(l_eq_pos - 1);

        auto coreList =
            eq.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL);

        for( auto core : coreList )
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                                   core,
                                   l_attr_chip_unit_core_pos),
                     "fapiGetAttribute of ATTR_CHIP_UNIT_POS for core failed");

            l_attr_chip_unit_core_pos = l_attr_chip_unit_core_pos % 4;

            if (l_relative_core_pos & (BIT32(l_attr_chip_unit_core_pos) >> 28))
            {
                //STOP 11 entry request enable[0:3]
                l_scrb_data = BIT64(l_attr_chip_unit_core_pos) >> 24;
                PUT_QME_SCRB_WO_OR(eq, l_scrb_data);
            }
        }
    }

    //Multicast this QME_FLAGS_STOP11_ENTRY_REQUESTED thru QME_FLAG
    FAPI_TRY(PUT_QME_FLAGS_WO_OR(l_eq_mc, BIT64(QME_FLAGS_STOP11_ENTRY_REQUESTED)));

    //Verify backing cache cores are entered stop 11
    for( auto eq : eq_list )
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               eq,
                               l_attr_chip_unit_eq_pos),
                 "fapiGetAttribute of ATTR_CHIP_UNIT_POS for eq failed");

        l_eq_pos = (l_attr_chip_unit_eq_pos + 1) << 2;

        l_relative_core_pos = (IS_BACKING_CACHE_CONFIG(l_attr_backing_vec,
                               l_eq_pos)) >> SHIFT32(l_eq_pos - 1);

        if (l_relative_core_pos)
        {
            auto coreList =
                eq.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL);

            for( auto core : coreList )
            {
                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                                       core,
                                       l_attr_chip_unit_core_pos),
                         "fapiGetAttribute of ATTR_CHIP_UNIT_POS for core failed");

                l_attr_chip_unit_core_pos = l_attr_chip_unit_core_pos % 4;

                if (l_relative_core_pos & (BIT32(l_attr_chip_unit_core_pos) >> 28))
                {
                    FAPI_IMP("Waiting for stop 11 to enter for core %d of quad %d", l_relative_core_pos, l_attr_chip_unit_eq_pos);
                    uint32_t l_stop11_state_entered = false;

                    //RTC 247535: need to revisit again
                    // for (uint32_t i = 0; i < TRIES_BEFORE_TIMEOUT; i++)
                    do
                    {
                        FAPI_TRY(GET_QME_SSH_OTR(core, l_data64));
                        GET_QME_SSH_OTR_ACT_STOP_LEVEL(l_data64, l_ssh_data);

                        if (l_ssh_data == 0xB ||
                            l_ssh_data == 0xF)
                        {
                            FAPI_IMP(" stop 11  entered for core %d of quad %d", l_relative_core_pos, l_attr_chip_unit_eq_pos);
                            l_stop11_state_entered = true;
                            break;
                        }

                        fapi2::delay(POLLTIME_NS, POLLTIME_MCYCLES * 1000 * 1000);
                    }
                    while(1);

                    if (!l_stop11_state_entered)
                    {
                        FAPI_ERR("STOP 11 enter operation failed for backing core %d of quad %d",
                                 l_attr_chip_unit_core_pos, l_attr_chip_unit_eq_pos);
                        FAPI_ASSERT(false,
                                    fapi2::PM_BACKING_CACHEPOWER_DOWN_FAILED()
                                    .set_CHIP_TARGET(i_proc_target)
                                    .set_SCRB_DATA(l_scrb_data)
                                    .set_SSH_OTR_DATA(l_data64)
                                    .set_CORE_TARGET(core)
                                    .set_CORE_POSITION(l_attr_chip_unit_core_pos)
                                    .set_EQ_TARGET(eq)
                                    .set_EQ_POSITION(l_attr_chip_unit_eq_pos),
                                    "Backing cache power down procedure failed");
                    }
                }
            }
        }
    }

fapi_try_exit:
    FAPI_INF("< p10_sbe_powerdown_backing_caches...");
    return fapi2::current_err;
}
