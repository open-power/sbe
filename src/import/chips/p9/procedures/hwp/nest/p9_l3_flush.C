/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_l3_flush.C $       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2020                        */
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

///-----------------------------------------------------------------------------
///
/// @file p9_l3_flush.C
/// @brief Initiates an L3 purge request
///
/// *HWP HWP Owner: Benjamin Gass <bgass@us.ibm.com>
/// *HWP FW Owner: Thi Tran thi@us.ibm.com
/// *HWP Team: Nest
/// *HWP Level: 3
/// *HWP Consumed by: FSP and SBE
/// ----------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p9_l3_flush.H>
#include <p9_quad_scom_addresses.H>
#include <p9_quad_scom_addresses_fld.H>

//--------------------------------------------------------------------------
// Constant definitions
//--------------------------------------------------------------------------

// L3 purge operation delay times for HW/sim
const uint32_t P9_L3_FLUSH_HW_NS_DELAY     = 50000;
const uint32_t P9_L3_FLUSH_SIM_CYCLE_DELAY = 1000000;

// If the L3 purge is not completed in P9_L3_FLUSH_TIMEOUT delays, fail with error
const uint32_t P9_L3_FLUSH_TIMEOUT_COUNT = 40;

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------

// TODO: RTC 177535 - Do we support SINGLE_PURGE/DELETE?
fapi2::ReturnCode p9_l3_flush(
    const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_target,
    const uint32_t i_purge_type,
    const uint32_t i_purge_addr)
{
    // mark HWP entry
    FAPI_DBG("Entering L3 flush ...\n");

    fapi2::buffer<uint64_t> purge_reg_data(0x0);
    uint32_t purge_pending_count = 0;

    //Make sure that the inputs are acceptable
    //Make sure the purge type is full purge, single purge, single delete, full blind, or dynamic
    //Make sure that the purge address fits withing bits 17:28
    FAPI_ASSERT(!((i_purge_type != L3_FULL_PURGE) &&
                  (i_purge_type != L3_SINGLE_PURGE) &&
                  (i_purge_type != L3_FULL_BLIND_PURGE) &&
                  (i_purge_type != L3_DYNAMIC_PURGE)) &&
                (i_purge_addr < 0x1000),
                fapi2::P9_L3_FLUSH_INVALID_ARGS_ERR()
                .set_TARGET(i_target)
                .set_PURGETYPE(i_purge_type)
                .set_PURGEADDR(i_purge_addr),
                "i_purge_type is not a compatible type");

    //Make sure that another flush is not happening
    FAPI_DBG("Verifying that a previous flush is not active");
    FAPI_TRY(fapi2::getScom(i_target, EX_PRD_PURGE_REG, purge_reg_data));
    FAPI_ASSERT(!(purge_reg_data.getBit<EX_PRD_PURGE_REG_L3_REQ>()),
                fapi2::P9_L3_FLUSH_PREVIOUS_PURGE_ACTIVE_ERR()
                .set_TARGET(i_target)
                .set_PURGEREG(purge_reg_data),
                "Previous Purge request has not completed error");

    //Since there is no previous purge, build the purge request

    //Flush the data and set the req bit to initiate the purge
    purge_reg_data.flush<0>().setBit<EX_PRD_PURGE_REG_L3_REQ>();

    //set the type of the purge
    purge_reg_data.insertFromRight<EX_PRD_PURGE_REG_L3_TTYPE,
                                   EX_PRD_PURGE_REG_L3_TTYPE_LEN> (i_purge_type);

    //set the address of where to start for the address
    purge_reg_data.insertFromRight<EX_PRD_PURGE_REG_L3_DIR_ADDR,
                                   EX_PRD_PURGE_REG_L3_DIR_ADDR_LEN> (i_purge_addr);

    //Write the purge request
    FAPI_TRY(fapi2::putScom(i_target, EX_PRD_PURGE_REG, purge_reg_data));

    //Spin on PRD_PURGE_REQ until hardware clears it
    while(purge_pending_count < P9_L3_FLUSH_TIMEOUT_COUNT)
    {
        FAPI_DBG("Waiting for purge to complete...");

        FAPI_TRY(fapi2::delay(P9_L3_FLUSH_HW_NS_DELAY,
                              P9_L3_FLUSH_SIM_CYCLE_DELAY));

        FAPI_TRY(fapi2::getScom(i_target, EX_PRD_PURGE_REG, purge_reg_data));

        if (!purge_reg_data.getBit<EX_PRD_PURGE_REG_L3_REQ>())
        {
            FAPI_DBG("Purge complete!");
            break;
        }

        purge_pending_count++;
    }

    FAPI_ASSERT(purge_pending_count < P9_L3_FLUSH_TIMEOUT_COUNT,
                fapi2::P9_L3_FLUSH_PURGE_REQ_TIMEOUT_ERR()
                .set_TARGET(i_target)
                .set_PURGETYPE(i_purge_type)
                .set_PURGEADDR(i_purge_addr)
                .set_EX_PRD_PURGE_REG_DATA(purge_reg_data),
                "Purge did not complete in time.");

fapi_try_exit:
    FAPI_DBG("Exiting L3 flush...");
    return fapi2::current_err;
}
