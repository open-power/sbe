/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_sbe_fabricinit.C $ */
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
/// @file p9_sbe_fabricinit.C
/// @brief Initialize island-mode fabric configuration (FAPI2)
///
/// @author Joe McGill <jmcgill@us.ibm.com>
/// @author Christy Graves <clgraves@us.ibm.com>
///

//
// *HWP HWP Owner: Joe McGill <jmcgill@us.ibm.com>
// *HWP FW Owner: Thi Tran <thi@us.ibm.com>
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
//

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p9_sbe_fabricinit.H>
#include <p9_fbc_utils.H>
#include <p9_misc_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// FBC SCOM register address definitions
// TODO: these are currently not present in the generated SCOM adddress header
//       including locally defined address constants here for testing purposes
const uint64_t PU_FBC_MODE_REG = 0x05011C0A;

// ADU delay/polling constants
const uint64_t FABRICINIT_DELAY_HW_NS = 1000; // 1us
const uint64_t FABRICINIT_DELAY_SIM_CYCLES = 200;

// ADU Command Register field/bit definitions
const uint32_t ALTD_CMD_START_OP_BIT = 2;
const uint32_t ALTD_CMD_CLEAR_STATUS_BIT = 3;
const uint32_t ALTD_CMD_RESET_FSM_BIT = 4;
const uint32_t ALTD_CMD_ADDRESS_ONLY_BIT = 6;
const uint32_t ALTD_CMD_LOCK_BIT = 11;
const uint32_t ALTD_CMD_SCOPE_START_BIT = 16;
const uint32_t ALTD_CMD_SCOPE_END_BIT = 18;
const uint32_t ALTD_CMD_DROP_PRIORITY_BIT = 20;
const uint32_t ALTD_CMD_OVERWRITE_PBINIT_BIT = 22;
const uint32_t ALTD_CMD_TTYPE_START_BIT = 25;
const uint32_t ALTD_CMD_TTYPE_END_BIT = 31;
const uint32_t ALTD_CMD_TSIZE_START_BIT = 32;
const uint32_t ALTD_CMD_TSIZE_END_BIT = 39;

const uint32_t ALTD_CMD_TTYPE_NUM_BITS = (ALTD_CMD_TTYPE_END_BIT - ALTD_CMD_TTYPE_START_BIT + 1);
const uint32_t ALTD_CMD_TSIZE_NUM_BITS = (ALTD_CMD_TSIZE_END_BIT - ALTD_CMD_TSIZE_START_BIT + 1);
const uint32_t ALTD_CMD_SCOPE_NUM_BITS = (ALTD_CMD_SCOPE_END_BIT - ALTD_CMD_SCOPE_START_BIT + 1);

const uint32_t ALTD_CMD_TTYPE_PBOP_EN_ALL = 0x3F;
const uint32_t ALTD_CMD_TSIZE_PBOP_EN_ALL = 0x0B;
const uint32_t ALTD_CMD_SCOPE_GROUP = 0x3;

// ADU Status Register field/bit definitions
const uint32_t ALTD_STATUS_ADDR_DONE_BIT = 2;
const uint32_t ALTD_STATUS_PBINIT_MISSING_BIT = 18;
const uint32_t ALTD_STATUS_CRESP_START_BIT = 59;
const uint32_t ALTD_STATUS_CRESP_END_BIT = 63;

const uint32_t ALTD_STATUS_CRESP_NUM_BITS = (ALTD_STATUS_CRESP_END_BIT - ALTD_STATUS_CRESP_START_BIT + 1);

const uint32_t ALTD_STATUS_CRESP_ACK_DONE = 0x04;


//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------


fapi2::ReturnCode
p9_sbe_fabricinit(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_INF("Start");

    fapi2::buffer<uint64_t> l_cmd_data;
    fapi2::buffer<uint64_t> l_status_data_act;
    fapi2::buffer<uint64_t> l_status_data_exp;
    fapi2::buffer<uint64_t> l_hp_mode_data;
    bool l_fbc_is_initialized, l_fbc_is_running;
    fapi2::ATTR_PROC_FABRIC_PUMP_MODE_Type l_pump_mode;

    // before fabric is initialized, configure resources which live in hotplug registers
    // but which themselves are not hotpluggable
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_PUMP_MODE, fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(), l_pump_mode),
             "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_PUMP_MODE)");
    FAPI_TRY(fapi2::getScom(i_target, PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR, l_hp_mode_data),
             "Error from getScom (PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR)");

    l_hp_mode_data.clearBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_PHYP_IS_GROUP>()    // PHYP_IS_GROUP
    .clearBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_ADDR_BAR>()                   // Large System Map
    .clearBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_DCACHE_CAPP>();               // dsable Dcache CAPP mode

    if (l_pump_mode == fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_NODE)
    {
        l_hp_mode_data.clearBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_PUMP>();
    }
    else
    {
        l_hp_mode_data.setBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_PUMP>();
    }

    // write back to all hotplug registers (EAST/CENTER/WEST, NEXT & CURR)
    FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR, l_hp_mode_data),
             "Error from putScom (PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR)");
    FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM0_PB_CENT_HP_MODE_NEXT, l_hp_mode_data),
             "Error from putScom (PU_PB_CENT_SM0_PB_CENT_HP_MODE_NEXT)");

    FAPI_TRY(fapi2::putScom(i_target, PU_PB_EAST_HP_MODE_CURR, l_hp_mode_data),
             "Error from putScom (PU_PB_EAST_HP_MODE_CURR)");
    FAPI_TRY(fapi2::putScom(i_target, PU_PB_EAST_HP_MODE_NEXT, l_hp_mode_data),
             "Error from putScom (PU_PB_EAST_HP_MODE_NEXT)");

    FAPI_TRY(fapi2::putScom(i_target, PU_PB_WEST_SM0_PB_WEST_HP_MODE_CURR, l_hp_mode_data),
             "Error from putScom (PU_PB_WEST_SM0_PB_WEST_HP_MODE_CURR)");
    FAPI_TRY(fapi2::putScom(i_target, PU_PB_WEST_SM0_PB_WEST_HP_MODE_NEXT, l_hp_mode_data),
             "Error from putScom (PU_PB_WEST_SM0_PB_WEST_HP_MODE_NEXT)");

    // check state of fabric pervasive stop control signal
    // if set, this would prohibit all fabric commands from being broadcast
    FAPI_DBG("Checking status of FBC stop ...");
    FAPI_TRY(p9_fbc_utils_get_fbc_state(i_target, l_fbc_is_initialized, l_fbc_is_running),
             "Error from p9_fbc_utils_get_fbc_state");
    FAPI_ASSERT(l_fbc_is_running,
                fapi2::P9_SBE_FABRICINIT_FBC_STOPPED_ERR().
                set_TARGET(i_target).
                set_FBC_RUNNING(l_fbc_is_running),
                "Pervasive stop control is asserted, so fabricinit will not run!");

    // write ADU Command Register to attempt lock acquisition
    // hold lock until finished with sequence
    FAPI_DBG("Lock and reset ADU ...");
    l_cmd_data.setBit<ALTD_CMD_LOCK_BIT>();
    FAPI_TRY(fapi2::putScom(i_target, PU_ALTD_CMD_REG, l_cmd_data),
             "Error writing ADU Command Register to acquire lock");

    // clear ADU status/reset state machine
    l_cmd_data.setBit<ALTD_CMD_CLEAR_STATUS_BIT>()
    .setBit<ALTD_CMD_RESET_FSM_BIT>();
    FAPI_TRY(fapi2::putScom(i_target, PU_ALTD_CMD_REG, l_cmd_data),
             "Error writing ADU Command Register to clear status and reset state machine");

    // launch init command
    FAPI_INF("Launching fabric init command via ADU ...");
    l_cmd_data.setBit<ALTD_CMD_START_OP_BIT>()
    .clearBit<ALTD_CMD_CLEAR_STATUS_BIT>()
    .clearBit<ALTD_CMD_RESET_FSM_BIT>()
    .setBit<ALTD_CMD_ADDRESS_ONLY_BIT>()
    .setBit<ALTD_CMD_DROP_PRIORITY_BIT>()
    .setBit<ALTD_CMD_OVERWRITE_PBINIT_BIT>();
    l_cmd_data.insertFromRight<ALTD_CMD_SCOPE_START_BIT, ALTD_CMD_SCOPE_NUM_BITS>(ALTD_CMD_SCOPE_GROUP);
    l_cmd_data.insertFromRight<ALTD_CMD_TTYPE_START_BIT, ALTD_CMD_TTYPE_NUM_BITS>(ALTD_CMD_TTYPE_PBOP_EN_ALL);
    l_cmd_data.insertFromRight<ALTD_CMD_TSIZE_START_BIT, ALTD_CMD_TSIZE_NUM_BITS>(ALTD_CMD_TSIZE_PBOP_EN_ALL);
    FAPI_TRY(fapi2::putScom(i_target, PU_ALTD_CMD_REG, l_cmd_data),
             "Error writing ADU Command Register to launch init operation");

    // delay prior to checking for completion
    FAPI_TRY(fapi2::delay(FABRICINIT_DELAY_HW_NS, FABRICINIT_DELAY_SIM_CYCLES),
             "Error from delay");

    // read ADU Status Register and check for expected pattern
    FAPI_DBG("Checking status of ADU operation ...");
    FAPI_TRY(fapi2::getScom(i_target, PU_ALTD_STATUS_REG, l_status_data_act),
             "Error polling ADU Status Register");

    l_status_data_exp.setBit<ALTD_STATUS_ADDR_DONE_BIT>();
    l_status_data_exp.insertFromRight<ALTD_STATUS_CRESP_START_BIT, ALTD_STATUS_CRESP_NUM_BITS>(ALTD_STATUS_CRESP_ACK_DONE);

    FAPI_ASSERT(l_status_data_exp == l_status_data_act,
                fapi2::P9_SBE_FABRICINIT_FAILED_ERR().set_TARGET(i_target).
                set_ADU_STATUS_EXP(l_status_data_act).
                set_ADU_STATUS_ACT(l_status_data_act),
                "Fabric init failed, or mismatch in expected ADU status!");

    // clear ADU Command Register to release lock
    FAPI_DBG("Success! Releasing ADU lock ...");
    l_cmd_data = 0;
    FAPI_TRY(fapi2::putScom(i_target, PU_ALTD_CMD_REG, l_cmd_data),
             "Error writing ADU Command Register to release lock");

    // confirm that fabric was successfully initialized
    FAPI_DBG("Verifying status of FBC init/stop ...");
    FAPI_TRY(p9_fbc_utils_get_fbc_state(i_target, l_fbc_is_initialized, l_fbc_is_running),
             "Error from p9_fbc_utils_get_fbc_state");
    FAPI_ASSERT(l_fbc_is_initialized && l_fbc_is_running,
                fapi2::P9_SBE_FABRICINIT_NO_INIT_ERR().
                set_TARGET(i_target).
                set_FBC_INITIALIZED(l_fbc_is_initialized).
                set_FBC_RUNNING(l_fbc_is_running),
                "ADU command succeded, but fabric was not cleanly initialized!");

fapi_try_exit:
    FAPI_INF("End");
    return fapi2::current_err;
}
