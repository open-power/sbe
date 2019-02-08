/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_fabricinit.C $ */
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
///
/// @file p10_sbe_fabricinit.C
/// @brief Initialize island-mode fabric configuration
///
/// *HWP HW Maintainer: Jenny Huynh <jhuynh@us.ibm.com>
/// *HWP FW Maintainer: Ilya Smirnov <ismirno@us.ibm.com>
/// *HWP Consumed by: SBE
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p10_sbe_fabricinit.H>
#include <p9_misc_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// Alter/display completion timers
const uint32_t ALTD_STATUS_DELAY_HW_NS = 1000;
const uint32_t ALTD_STATUS_DELAY_SIM_CYCLES = 200;
const uint32_t ALTD_STATUS_MAX_WAIT_LOOPS = 10;

// Alter/display register field constants
const uint32_t FBC_ALTD_TTYPE_PBOPERATION = 0x3F; // pbop.enable_all
const uint32_t FBC_ALTD_TSIZE_PBOPERATION = 0x0B; // fastpath enabled
const uint32_t FBC_ALTD_SCOPE_GROUP = 0x03;
const uint32_t FBC_ALTD_CRESP_VALUE_ACK_DONE = 0x04;

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

fapi2::ReturnCode
p10_sbe_fabricinit(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_INF("Start p10_sbe_fabricinit...");

    fapi2::buffer<uint64_t> l_data;
    fapi2::buffer<uint64_t> l_data_exp;

    uint32_t altd_status_loop_count = 0;

    // check state of pb_stop; it will prohibit all fabric commands from being broadcast if asserted
    FAPI_DBG("Check that fabric is in a healthy state");
    FAPI_TRY(getScom(i_target, PU_SND_MODE_REG, l_data), "Error reading pb_stop from pMisc Mode Register");

    FAPI_ASSERT(l_data.getBit<PU_SND_MODE_REG_PB_STOP>() == 0,
                fapi2::P10_SBE_FABRICINIT_FBC_STOPPED()
                .set_TARGET(i_target),
                "Pervasive stop control (pb_stop) is asserted, so fabric init will not run!");

    // obtain lock
    FAPI_DBG("Obtain adu lock via alter/display command register");
    l_data.flush<0>().setBit<PU_ALTD_CMD_REG_FBC_LOCKED>();
    FAPI_TRY(putScom(i_target, PU_ALTD_CMD_REG, l_data), "Error obtaining adu lock via Alter/Display Command Register");

    // clear/reset
    FAPI_DBG("Clear and reset adu state machine and status registers");
    l_data.setBit<PU_ALTD_CMD_REG_FBC_CLEAR_STATUS>()
    .setBit<PU_ALTD_CMD_REG_FBC_RESET_FSM>();
    FAPI_TRY(putScom(i_target, PU_ALTD_CMD_REG, l_data), "Error resetting adu via Alter/Display Command Register");

    // launch command
    FAPI_DBG("Launch fabric init command via alter/display command register");
    l_data.flush<0>().setBit<PU_ALTD_CMD_REG_FBC_START_OP>()
    .setBit<PU_ALTD_CMD_REG_FBC_AXTYPE>()
    .setBit<PU_ALTD_CMD_REG_FBC_LOCKED>()
    .insertFromRight<PU_ALTD_CMD_REG_FBC_SCOPE, PU_ALTD_CMD_REG_FBC_SCOPE_LEN>(FBC_ALTD_SCOPE_GROUP)
    .setBit<PU_ALTD_CMD_REG_FBC_DROP_PRIORITY>()
    .setBit<PU_ALTD_CMD_REG_FBC_OVERWRITE_PBINIT>()
    .insertFromRight<PU_ALTD_CMD_REG_FBC_TTYPE, PU_ALTD_CMD_REG_FBC_TTYPE_LEN>(FBC_ALTD_TTYPE_PBOPERATION)
    .insertFromRight<PU_ALTD_CMD_REG_FBC_TSIZE, PU_ALTD_CMD_REG_FBC_TSIZE_LEN>(FBC_ALTD_TSIZE_PBOPERATION);
    FAPI_TRY(putScom(i_target, PU_ALTD_CMD_REG, l_data), "Error launching adu command via Alter/Display Command Register");

    // check status
    FAPI_DBG("Check adu status register for command completion");
    FAPI_TRY(getScom(i_target, PU_ALTD_STATUS_REG, l_data), "Error checking adu status via Alter/Display Status Register");

    l_data_exp.flush<0>().setBit<PU_ALTD_STATUS_REG_FBC_ADDR_DONE>()
    .insertFromRight<PU_ALTD_STATUS_REG_FBC_CRESP_VALUE, PU_ALTD_STATUS_REG_FBC_CRESP_VALUE_LEN>
    (FBC_ALTD_CRESP_VALUE_ACK_DONE);

    while(l_data != l_data_exp)
    {
        fapi2::delay(ALTD_STATUS_DELAY_HW_NS, ALTD_STATUS_DELAY_SIM_CYCLES);
        FAPI_TRY(getScom(i_target, PU_ALTD_STATUS_REG, l_data), "Error checking adu status via Alter/Display Status Register");

        altd_status_loop_count++;

        FAPI_ASSERT(altd_status_loop_count < ALTD_STATUS_MAX_WAIT_LOOPS,
                    fapi2::P10_SBE_FABRICINIT_BAD_ADU_STATUS()
                    .set_TARGET(i_target)
                    .set_ADU_STATUS_ACT(l_data)
                    .set_ADU_STATUS_EXP(l_data_exp)
                    .set_WAIT_TIME_NS(ALTD_STATUS_DELAY_HW_NS * ALTD_STATUS_MAX_WAIT_LOOPS),
                    "Fabric failed to initialize with the expected status within a reasonable timeframe!");
    }

    FAPI_DBG("Fabric init command from adu successfully completed!");

    // release lock
    FAPI_DBG("Release adu lock via alter/display command register");
    FAPI_TRY(putScom(i_target, PU_ALTD_CMD_REG, l_data.flush<0>()),
             "Error releasing adu lock via Alter/Display Command Register");

    // TODO: Scom access to check pb_init not working on vbu model yet, placeholder code below (RTC 205266)
    /*
    // confirm success
    FAPI_DBG("Verify fabric is in a functional state");
    FAPI_TRY(getScom(i_target, 0x301100A, l_data), "Error reading pb_init from Powerbus ES3 Mode Config Register");

    FAPI_ASSERT((l_data && 0x8000000000000000) == 1,
                fapi2::P9_SBE_FABRICINIT_FBC_NOT_RUNNING()
                .set_TARGET(i_target),
                "Init command was successfully issued, but fabric is not in an initialized state!");

    FAPI_DBG("Verify fabric is in a functional state");
    FAPI_TRY(getScom(i_target, PU_PB_ES3_MODE, l_data), "Error reading pb_init from Powerbus ES3 Mode Config Register");

    FAPI_ASSERT(l_data.getBit<PU_PB_ES3_MODE_PBIXXX_INIT>() == 1,
                fapi2::P9_SBE_FABRICINIT_FBC_NOT_RUNNING()
                .set_TARGET(i_target),
                "Init command was successfully issued, but fabric is not in an initialized state!");
    */

    FAPI_TRY(getScom(i_target, PU_SND_MODE_REG, l_data), "Error reading pb_stop from pMisc Mode Register");

    FAPI_ASSERT(l_data.getBit<PU_SND_MODE_REG_PB_STOP>() == 0,
                fapi2::P10_SBE_FABRICINIT_FBC_NOT_RUNNING()
                .set_TARGET(i_target),
                "Pervasive stop control (pb_stop) is asserted, so fabric will not function!");

    FAPI_DBG("Fabric is successfully initialized and running!");

fapi_try_exit:

    // cleanup in case we errored in the middle of fabric init process
    FAPI_DBG("Cleaning up adu by ensuring adu lock is released");
    FAPI_TRY(putScom(i_target, PU_ALTD_CMD_REG, l_data.flush<0>()),
             "Error releasing adu lock via Alter/Display Command Register");

    FAPI_INF("End p10_sbe_fabricinit...");
    return fapi2::current_err;
}
