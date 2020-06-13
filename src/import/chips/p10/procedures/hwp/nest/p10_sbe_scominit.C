/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_scominit.C $ */
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
///
/// @file p10_sbe_scominit.C
/// @brief Perform SCOM initialization required for fabric & HBI operation
///
/// *HWP HW Maintainer: Jenny Huynh <jhuynh@us.ibm.com>
/// *HWP FW Maintainer: Raja Das <rajadas2@in.ibm.com>
/// *HWP Consumed by: SBE
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p10_sbe_scominit.H>
#include <p10_fbc_utils.H>

#include <p10_scom_perv.H>
#include <p10_scom_proc.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// XSCOM/LPC BAR constants
const uint64_t XSCOM_BAR_MASK   = 0xFF000003FFFFFFFFULL;
const uint64_t LPC_BAR_MASK     = 0xFF000000FFFFFFFFULL;

// FBC Mode constants
const uint8_t PB_CFG_MCA_RATIO_OVERRIDE = 0x01;

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

/// @brief Configures fabric mode registers
/// @param[in] i_target       Reference to processor chip target
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_scominit_fbc(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt;
    using namespace scomt::proc;

    FAPI_DBG("Entering ...");

    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_PROC_FABRIC_BROADCAST_MODE_Type l_broadcast_mode;
    fapi2::ATTR_SYSTEM_IPL_PHASE_Type l_ipl_phase;
    fapi2::ATTR_CHIP_UNIT_POS_Type l_chiplet_pos;
    fapi2::buffer<uint64_t> l_pb_mode_data;
    fapi2::buffer<uint64_t> l_pb_cfg3_data;

    auto l_mc_chiplets = i_target.getChildren<fapi2::TARGET_TYPE_MC>();
    auto l_pauc_chiplets = i_target.getChildren<fapi2::TARGET_TYPE_PAUC>();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_BROADCAST_MODE, FAPI_SYSTEM, l_broadcast_mode),
             "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_BROADCAST_MODE)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_ipl_phase),
             "Error from FAPI_ATTR_GET (ATTR_SYSTEM_IPL_PHASE)");

    FAPI_TRY(GET_PB_COM_SCOM_EQ0_STATION_MODE(i_target, l_pb_mode_data),
             "Error from getScom (PB_COM_SCOM_EQ0_STATION_MODE)");

    // configure broadcast mode
    switch(l_broadcast_mode)
    {
        case fapi2::ENUM_ATTR_PROC_FABRIC_BROADCAST_MODE_1HOP_CHIP_IS_GROUP:
            CLEAR_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_HOP_MODE_EQ0(l_pb_mode_data);
            SET_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_PUMP_MODE_EQ0(l_pb_mode_data);
            break;

        case fapi2::ENUM_ATTR_PROC_FABRIC_BROADCAST_MODE_1HOP_CHIP_IS_NODE:
            CLEAR_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_HOP_MODE_EQ0(l_pb_mode_data);
            CLEAR_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_PUMP_MODE_EQ0(l_pb_mode_data);
            break;

        case fapi2::ENUM_ATTR_PROC_FABRIC_BROADCAST_MODE_2HOP_CHIP_IS_NODE:
            SET_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_HOP_MODE_EQ0(l_pb_mode_data);
            CLEAR_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_PUMP_MODE_EQ0(l_pb_mode_data);
            break;

        default:
            FAPI_ASSERT(false,
                        fapi2::P10_SBE_SCOMINIT_FABRIC_BROADCAST_ATTR_ERR()
                        .set_TARGET(i_target)
                        .set_BROADCAST_MODE(l_broadcast_mode),
                        "Invalid fabric broadcast mode!");
            break;
    }

    // configure sl chip domain for all ipl types
    SET_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_SL_DOMAIN_SIZE_EQ0(l_pb_mode_data);

    // configure first mc as source for mca ratio if HBI and a valid mc exists, else override mc async
    if((l_ipl_phase == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL) && l_mc_chiplets.size())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_mc_chiplets.front(), l_chiplet_pos),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

        SET_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_MCA_RATIO_SEL_EQ0(l_chiplet_pos, l_pb_mode_data);
    }
    else
    {
        SET_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_MCA_RATIO_OVERRIDE_EQ0(PB_CFG_MCA_RATIO_OVERRIDE, l_pb_mode_data);
    }

    // configure first pau as source for pau ratio if HBI and a valid pau exists, else override pau async
    if((l_ipl_phase == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL) && l_pauc_chiplets.size())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_pauc_chiplets.front(), l_chiplet_pos),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

        SET_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_PAU_STEP_SEL_EQ0(l_chiplet_pos, l_pb_mode_data);
    }
    else
    {
        SET_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_PAU_STEP_OVERRIDE_EQ0(l_pb_mode_data);
    }

    // configure fbc to abide by switch_cd signal
    SET_PB_COM_SCOM_EQ0_STATION_MODE_PB_CFG_SWITCH_CD_GATE_ENABLE_EQ0(l_pb_mode_data);

    // apply pb mode configuration to all racetrack station registers
    FAPI_TRY(p10_fbc_utils_set_racetrack_regs(i_target, PB_COM_SCOM_EQ0_STATION_MODE, l_pb_mode_data),
             "Error from p10_fbc_utils_set_racetrack_regs (PB_COM_SCOM_EQ0_STATION_MODE)");

    FAPI_TRY(GET_PB_COM_SCOM_EQ0_STATION_CFG3(i_target, l_pb_cfg3_data),
             "Error from getScom (PB_COM_SCOM_EQ0_STATION_CFG3)");

    // select to apply mode D on switch_cd signal
    SET_PB_COM_SCOM_EQ0_STATION_CFG3_PB_CFG_PBIASY_UNIT0_SELCD(l_pb_cfg3_data);
    SET_PB_COM_SCOM_EQ0_STATION_CFG3_PB_CFG_PBIASY_UNIT1_SELCD(l_pb_cfg3_data);
    SET_PB_COM_SCOM_EQ0_STATION_CFG3_PB_CFG_PBIASY_LINK0_SELCD(l_pb_cfg3_data);
    SET_PB_COM_SCOM_EQ0_STATION_CFG3_PB_CFG_PBIASY_LINK1_SELCD(l_pb_cfg3_data);

    // apply pb cfg3 configuration to all racetrack station registers
    FAPI_TRY(p10_fbc_utils_set_racetrack_regs(i_target, PB_COM_SCOM_EQ0_STATION_CFG3, l_pb_cfg3_data),
             "Error from p10_fbc_utils_set_racetrack_regs (PB_COM_SCOM_EQ0_STATION_CFG3)");

    // initialize topology table attribute for SBE platform
    FAPI_TRY(topo::init_topology_id_table(i_target));
    FAPI_TRY(topo::set_topology_id_tables(i_target));

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}

/// @brief Performs BAR setup needed for HBI (XSCOM/LPC)
/// @param[in] i_target       Reference to processor chip target
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_scominit_bars(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt;
    using namespace scomt::proc;

    FAPI_DBG("Entering ...");

    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    uint64_t l_base_addr_nm0;
    uint64_t l_base_addr_nm1;
    uint64_t l_base_addr_m;
    uint64_t l_base_addr_mmio;

    // determine base address of chip nm/m/mmio regions in real address space
    FAPI_TRY(p10_fbc_utils_get_chip_base_address(
                 i_target,
                 EFF_TOPOLOGY_ID,
                 l_base_addr_nm0,
                 l_base_addr_nm1,
                 l_base_addr_m,
                 l_base_addr_mmio),
             "Error from p10_fbc_utils_get_chip_base_address");

    // set XSCOM BAR (always set smf bit, anyone can access the bar if smf is disabled)
    {
        fapi2::buffer<uint64_t> l_xscom_bar;
        fapi2::ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET_Type l_xscom_bar_offset;

        FAPI_DBG("Configuring XSCOM BAR");

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET, FAPI_SYSTEM, l_xscom_bar_offset),
                 "Error from FAPI_ATTR_GET (ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET)");

        l_xscom_bar = l_base_addr_mmio + l_xscom_bar_offset;
        l_xscom_bar.setBit(FABRIC_ADDR_SMF_BIT);

        FAPI_ASSERT((l_xscom_bar & XSCOM_BAR_MASK) == 0,
                    fapi2::P10_SBE_SCOMINIT_XSCOM_BAR_ATTR_ERR()
                    .set_TARGET(i_target)
                    .set_BAR(l_xscom_bar)
                    .set_BAR_MASK(XSCOM_BAR_MASK)
                    .set_BAR_OVERLAP(l_xscom_bar & XSCOM_BAR_MASK)
                    .set_BAR_OFFSET(l_xscom_bar_offset)
                    .set_BASE_ADDR_MMIO(l_base_addr_mmio),
                    "Invalid XSCOM BAR configuration!");

        FAPI_TRY(PREP_TP_TPBR_AD_XSCOM_BASE_REG(i_target),
                 "Error from prep (TP_TPBR_AD_XSCOM_BASE_REG)");
        FAPI_TRY(PUT_TP_TPBR_AD_XSCOM_BASE_REG(i_target, l_xscom_bar),
                 "Error from putScom (TP_TPBR_AD_XSCOM_BASE_REG)");
    }

    // set LPC BAR
    {
        fapi2::buffer<uint64_t> l_lpc_bar;
        fapi2::ATTR_PROC_LPC_BAR_BASE_ADDR_OFFSET_Type l_lpc_bar_offset;

        FAPI_DBG("Configuring LPC BAR");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_LPC_BAR_BASE_ADDR_OFFSET, FAPI_SYSTEM, l_lpc_bar_offset),
                 "Error from FAPI_ATTR_GET (ATRR_PROC_LPC_BAR_BASE_ADDR_OFFSET");

        l_lpc_bar = l_base_addr_mmio + l_lpc_bar_offset;

        FAPI_ASSERT((l_lpc_bar & LPC_BAR_MASK) == 0,
                    fapi2::P10_SBE_SCOMINIT_LPC_BAR_ATTR_ERR()
                    .set_TARGET(i_target)
                    .set_BAR(l_lpc_bar)
                    .set_BAR_MASK(LPC_BAR_MASK)
                    .set_BAR_OVERLAP(l_lpc_bar & LPC_BAR_MASK)
                    .set_BAR_OFFSET(l_lpc_bar_offset)
                    .set_BASE_ADDR_MMIO(l_base_addr_mmio),
                    "Invalid LPC BAR configuration!");

        FAPI_TRY(PREP_TP_TPBR_AD_LPC_BASE_REG(i_target),
                 "Error from prep (TP_TPBR_AD_LPC_BASE_REG)");
        FAPI_TRY(PUT_TP_TPBR_AD_LPC_BASE_REG(i_target, l_lpc_bar),
                 "Error from putScom (TP_TPBR_AD_LPC_BASE_REG)");
    }

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}

/// @brief Configures any trace array setup that cannot be done by scan init
/// @param[in] i_target       Reference to processor chip target
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_scominit_trace(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("Entering ...");

    // Nothing for P10 (yet!) -- placeholder function

    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}

/// Main function, see description in header
fapi2::ReturnCode p10_sbe_scominit(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("Entering ...");

    FAPI_TRY(p10_sbe_scominit_fbc(i_target), "Error from p10_sbe_scominit_fbc\n");
    FAPI_TRY(p10_sbe_scominit_bars(i_target), "Error from p10_sbe_scominit_bars\n");
    // FIR inits have been moved to scan
    FAPI_TRY(p10_sbe_scominit_trace(i_target), "Error from p10_sbe_scominit_trace\n");

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}
