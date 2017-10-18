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
///

//
// *HWP HWP Owner: Joe McGill <jmcgill@us.ibm.com>
// *HWP FW Owner: Thi Tran <thi@us.ibm.com>
// *HWP Team: Nest
// *HWP Level: 3
// *HWP Consumed by: SBE
//

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p9_sbe_fabricinit.H>
#include <p9_fbc_utils.H>
#include <p9_misc_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>
#include <p9n2_misc_scom_addresses.H>
#include <p9n2_misc_scom_addresses_fld.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// ADU delay/polling constants
const uint64_t FABRICINIT_DELAY_HW_NS = 1000; // 1us
const uint64_t FABRICINIT_DELAY_SIM_CYCLES = 200;

// ADU Command Register field definitions
const uint32_t ALTD_CMD_TTYPE_PBOP_EN_ALL = 0x3F;
const uint32_t ALTD_CMD_TSIZE_PBOP_EN_ALL = 0x0B;
const uint32_t ALTD_CMD_SCOPE_GROUP = 0x3;

// ADU Status Register field definitions
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
    fapi2::buffer<uint64_t> l_mode_data;
    fapi2::buffer<uint64_t> l_hp_mode_data;
    fapi2::buffer<uint64_t> l_nmmu_cqmode_data;
    bool l_fbc_is_initialized, l_fbc_is_running;
    fapi2::ATTR_PROC_FABRIC_PUMP_MODE_Type l_pump_mode;
    fapi2::ATTR_PROC_FABRIC_GROUP_ID_Type l_fbc_group_id_abs;
    fapi2::ATTR_PROC_EFF_FABRIC_GROUP_ID_Type l_fbc_group_id_eff;
    fapi2::ATTR_PROC_FABRIC_CHIP_ID_Type l_fbc_chip_id_abs;
    fapi2::ATTR_PROC_EFF_FABRIC_CHIP_ID_Type l_fbc_chip_id_eff;
    fapi2::ATTR_CHIP_EC_FEATURE_EXTENDED_ADDRESSING_MODE_Type l_extended_addressing_mode;
    uint8_t l_fbc_xlate_addr_to_id = 0;
    uint8_t l_addr_extension_group_id = 0;
    uint8_t l_addr_extension_chip_id = 0;

    // before fabric is initialized, configure resources which live in hotplug registers
    // but which themselves are not hotpluggable
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_PUMP_MODE, fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(), l_pump_mode),
             "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_PUMP_MODE)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID, i_target, l_fbc_group_id_abs),
             "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_GROUP_ID)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_EFF_FABRIC_GROUP_ID, i_target, l_fbc_group_id_eff),
             "Error from FAPI_ATTR_GET (ATTR_PROC_EFF_FABRIC_GROUP_ID)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_CHIP_ID, i_target, l_fbc_chip_id_abs),
             "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_CHIP_ID)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_EFF_FABRIC_CHIP_ID, i_target, l_fbc_chip_id_eff),
             "Error from FAPI_ATTR_GET (ATTR_PROC_EFF_FABRIC_CHIP_ID)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_EXTENDED_ADDRESSING_MODE, i_target, l_extended_addressing_mode),
             "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_EXTENDED_ADDRESSING_MODE)");

    // sample center mode and hotplug mode registers
    FAPI_TRY(fapi2::getScom(i_target, PU_PB_CENT_SM0_PB_CENT_MODE, l_mode_data),
             "Error from getScom (PU_PB_CENT_SM0_PB_CENT_MODE)");

    FAPI_TRY(fapi2::getScom(i_target, PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR, l_hp_mode_data),
             "Error from getScom (PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR)");

    // update mode register content
    if (l_extended_addressing_mode)
    {
        fapi2::ATTR_CHIP_EC_FEATURE_HW423589_OPTION2_Type l_hw423589_option2;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW423589_OPTION2, i_target, l_hw423589_option2),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_HW423589_OPTION2)");

        if (l_hw423589_option2)
        {
            l_addr_extension_group_id = CHIP_ADDRESS_EXTENSION_GROUP_ID_MASK_HW423589_OPTION2;
            l_addr_extension_chip_id = CHIP_ADDRESS_EXTENSION_CHIP_ID_MASK_HW423589_OPTION2;
        }

        // enable extended addressing mode, seed attributes from defaults
        // and use attribute values to configure fabric -- should allow for testing
        // alternate configurations via Cronus with const attribute overrides
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_FABRIC_ADDR_EXTENSION_GROUP_ID,
                               fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                               l_addr_extension_group_id),
                 "Error from FAPI_ATTR_SET (ATTR_FABRIC_ADDR_EXTENSION_GROUP_ID)");
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_FABRIC_ADDR_EXTENSION_CHIP_ID,
                               fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                               l_addr_extension_chip_id),
                 "Error from FAPI_ATTR_SET (ATTR_FABRIC_ADDR_EXTENSION_CHIP_ID)");

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FABRIC_ADDR_EXTENSION_GROUP_ID,
                               fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                               l_addr_extension_group_id),
                 "Error from FAPI_ATTR_GET (ATTR_FABRIC_ADDR_EXTENSION_GROUP_ID)");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FABRIC_ADDR_EXTENSION_CHIP_ID,
                               fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                               l_addr_extension_chip_id),
                 "Error from FAPI_ATTR_GET (ATTR_FABRIC_ADDR_EXTENSION_CHIP_ID)");

        l_mode_data.insertFromRight<P9N2_PU_PB_CENT_SM0_PB_CENT_MODE_CFG_CHIP_ADDR_EXTENSION_MASK,
                                    P9N2_PU_PB_CENT_SM0_PB_CENT_MODE_CFG_CHIP_ADDR_EXTENSION_MASK_LEN>
                                    ((l_addr_extension_group_id << 3) | l_addr_extension_chip_id);
    }

    // determine HW XOR mask based on fabric ID attributes
    l_fbc_xlate_addr_to_id =  ((l_fbc_group_id_abs << 3) | l_fbc_chip_id_abs);
    l_fbc_xlate_addr_to_id ^= ((l_fbc_group_id_eff << 3) | l_fbc_chip_id_eff);

    // update hotplug mode register content
    l_hp_mode_data.insertFromRight<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_XLATE_ADDR_TO_ID,  // XOR mask
                                   PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_XLATE_ADDR_TO_ID_LEN>(l_fbc_xlate_addr_to_id);

    l_hp_mode_data.clearBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_PHYP_IS_GROUP>()          // PHYP is group
    .clearBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_ADDR_BAR>()                             // large system map
    .clearBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_DCACHE_CAPP>();                         // disable Dcache CAPP mode

    if (l_pump_mode == fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_NODE)                   // pump mode
    {
        l_hp_mode_data.clearBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_PUMP>();
    }
    else
    {
        l_hp_mode_data.setBit<PU_PB_CENT_SM0_PB_CENT_HP_MODE_CURR_CFG_PUMP>();
    }

    // write back to all mode registers (EAST/CENTER/WEST)
    FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM0_PB_CENT_MODE, l_mode_data),
             "Error from putScom (PU_PB_CENT_SM0_PB_CENT_MODE)");

    FAPI_TRY(fapi2::putScom(i_target, PU_PB_EAST_MODE, l_mode_data),
             "Error from putScom (PU_PB_EAST_MODE)");

    FAPI_TRY(fapi2::putScom(i_target, PU_PB_WEST_SM0_PB_WEST_MODE, l_mode_data),
             "Error from putScom (PU_PB_WEST_SM0_PB_WEST_MODE)");

    // write back to all hotplug mode registers (EAST/CENTER/WEST, NEXT & CURR)
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

    // set NMMU extended addressing configuration prior to fabric init
    if (l_extended_addressing_mode)
    {
        FAPI_TRY(fapi2::getScom(i_target, P9N2_PU_NMMU_MMCQ_PB_MODE_REG, l_nmmu_cqmode_data),
                 "Error from getScom (P9N2_PU_NMMU_MMCQ_PB_MODE_REG)");
        l_nmmu_cqmode_data.insertFromRight<P9N2_PU_NMMU_MMCQ_PB_MODE_REG_ADDR_EXT_MASK,
                                           P9N2_PU_NMMU_MMCQ_PB_MODE_REG_ADDR_EXT_MASK_LEN>
                                           ((l_addr_extension_group_id << 3) | l_addr_extension_chip_id);
        FAPI_TRY(fapi2::putScom(i_target, P9N2_PU_NMMU_MMCQ_PB_MODE_REG, l_nmmu_cqmode_data),
                 "Error from getScom (P9N2_PU_NMMU_MMCQ_PB_MODE_REG)");
    }

    // check state of fabric pervasive stop control signal
    // if set, this would prohibit all fabric commands from being broadcast
    FAPI_DBG("Checking status of FBC stop ...");
    FAPI_TRY(p9_fbc_utils_get_fbc_state(i_target, l_fbc_is_initialized, l_fbc_is_running),
             "Error from p9_fbc_utils_get_fbc_state");
    FAPI_ASSERT(l_fbc_is_running,
                fapi2::P9_SBE_FABRICINIT_FBC_STOPPED_ERR().
                set_TARGET(i_target),
                "Pervasive stop control is asserted, so fabricinit will not run!");

    // write ADU Command Register to attempt lock acquisition
    // hold lock until finished with sequence
    FAPI_DBG("Lock and reset ADU ...");
    l_cmd_data.setBit<PU_ALTD_CMD_REG_FBC_LOCKED>();
    FAPI_TRY(fapi2::putScom(i_target, PU_ALTD_CMD_REG, l_cmd_data),
             "Error writing ADU Command Register to acquire lock");

    // clear ADU status/reset state machine
    l_cmd_data.setBit<PU_ALTD_CMD_REG_FBC_CLEAR_STATUS>()
    .setBit<PU_ALTD_CMD_REG_FBC_RESET_FSM>();
    FAPI_TRY(fapi2::putScom(i_target, PU_ALTD_CMD_REG, l_cmd_data),
             "Error writing ADU Command Register to clear status and reset state machine");

    // launch init command
    FAPI_INF("Launching fabric init command via ADU ...");
    l_cmd_data.setBit<PU_ALTD_CMD_REG_FBC_START_OP>()
    .clearBit<PU_ALTD_CMD_REG_FBC_CLEAR_STATUS>()
    .clearBit<PU_ALTD_CMD_REG_FBC_RESET_FSM>()
    .setBit<PU_ALTD_CMD_REG_FBC_AXTYPE>()
    .setBit<PU_ALTD_CMD_REG_FBC_DROP_PRIORITY>()
    .setBit<PU_ALTD_CMD_REG_FBC_OVERWRITE_PBINIT>();
    l_cmd_data.insertFromRight<PU_ALTD_CMD_REG_FBC_SCOPE, PU_ALTD_CMD_REG_FBC_SCOPE_LEN>(ALTD_CMD_SCOPE_GROUP);
    l_cmd_data.insertFromRight<PU_ALTD_CMD_REG_FBC_TTYPE, PU_ALTD_CMD_REG_FBC_TTYPE_LEN>(ALTD_CMD_TTYPE_PBOP_EN_ALL);
    l_cmd_data.insertFromRight<PU_ALTD_CMD_REG_FBC_TSIZE, PU_ALTD_CMD_REG_FBC_TSIZE_LEN>(ALTD_CMD_TSIZE_PBOP_EN_ALL);
    FAPI_TRY(fapi2::putScom(i_target, PU_ALTD_CMD_REG, l_cmd_data),
             "Error writing ADU Command Register to launch init operation");

    // delay prior to checking for completion
    FAPI_TRY(fapi2::delay(FABRICINIT_DELAY_HW_NS, FABRICINIT_DELAY_SIM_CYCLES),
             "Error from delay");

    // read ADU Status Register and check for expected pattern
    FAPI_DBG("Checking status of ADU operation ...");
    FAPI_TRY(fapi2::getScom(i_target, PU_ALTD_STATUS_REG, l_status_data_act),
             "Error polling ADU Status Register");

    l_status_data_exp.setBit<PU_ALTD_STATUS_REG_FBC_ADDR_DONE>();
    l_status_data_exp.insertFromRight<PU_ALTD_STATUS_REG_FBC_CRESP_VALUE, PU_ALTD_STATUS_REG_FBC_CRESP_VALUE_LEN>
    (ALTD_STATUS_CRESP_ACK_DONE);

    FAPI_ASSERT(l_status_data_exp == l_status_data_act,
                fapi2::P9_SBE_FABRICINIT_FAILED_ERR().
                set_TARGET(i_target).
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
                set_TARGET(i_target),
                "ADU command succeded, but fabric was not cleanly initialized!");

fapi_try_exit:
    FAPI_INF("End");
    return fapi2::current_err;
}
