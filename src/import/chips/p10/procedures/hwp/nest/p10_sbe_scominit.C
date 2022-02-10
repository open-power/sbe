/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_scominit.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2022                        */
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
#include <multicast_group_defs.H>

#include <p10_scom_perv.H>
#include <p10_scom_proc.H>
#include <p10_scom_eq.H>
#include <p10_scom_c.H>

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
    uint64_t l_tsnoop = 25;
    fapi2::ATTR_FREQ_PAU_MHZ_Type l_fpau;
    uint32_t l_fmc = 0;
    bool l_fmc_valid = false;

    auto l_mc_chiplets = i_target.getChildren<fapi2::TARGET_TYPE_MC>();
    auto l_pauc_chiplets = i_target.getChildren<fapi2::TARGET_TYPE_PAUC>();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_BROADCAST_MODE, FAPI_SYSTEM, l_broadcast_mode),
             "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_BROADCAST_MODE)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_ipl_phase),
             "Error from FAPI_ATTR_GET (ATTR_SYSTEM_IPL_PHASE)");

    //
    // STATION MODE register
    //

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

    //
    // STATION CFG3 register
    //

    FAPI_TRY(GET_PB_COM_SCOM_EQ0_STATION_CFG3(i_target, l_pb_cfg3_data),
             "Error from getScom (PB_COM_SCOM_EQ0_STATION_CFG3)");

    // select to apply mode D on switch_cd signal
    SET_PB_COM_SCOM_EQ0_STATION_CFG3_PB_CFG_PBIASY_UNIT0_SELCD(l_pb_cfg3_data);
    SET_PB_COM_SCOM_EQ0_STATION_CFG3_PB_CFG_PBIASY_UNIT1_SELCD(l_pb_cfg3_data);
    SET_PB_COM_SCOM_EQ0_STATION_CFG3_PB_CFG_PBIASY_LINK0_SELCD(l_pb_cfg3_data);
    SET_PB_COM_SCOM_EQ0_STATION_CFG3_PB_CFG_PBIASY_LINK1_SELCD(l_pb_cfg3_data);

    // calculate tsnoop based on memory controller/PAU frequencies
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_fpau),
             "Error from FAPI_ATTR_GET (ATTR_FREQ_PAU_MHZ)");

    // find the slowest MC frequency on the chip for the purposes of tsnoop calculations
    {
        fapi2::ATTR_MC_PLL_BUCKET_Type l_attr_mc_pll_bucket;
        fapi2::ATTR_CHIP_UNIT_POS_Type l_mc_pos;
        uint32_t l_fmc_curr = 0;

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_PLL_BUCKET, i_target, l_attr_mc_pll_bucket));

        for (auto& l_mc_target : i_target.getChildren<fapi2::TARGET_TYPE_MC>())
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_mc_target, l_mc_pos),
                     "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS_Type)");

            switch (l_attr_mc_pll_bucket[l_mc_pos])
            {
                case 0:
                    l_fmc_curr = 1600;
                    break;

                case 1:
                    l_fmc_curr = 1466;
                    break;

                case 2:
                    l_fmc_curr = 1333;
                    break;

                case 4:
                    l_fmc_curr = 2000;
                    break;

                default:
                    l_fmc_curr = 1333;
                    break;
            }

            FAPI_DBG("  MC pos: %d, freq: %d MHz, bucket: %d",
                     l_mc_pos, l_fmc_curr, l_attr_mc_pll_bucket[l_mc_pos]);

            // first valid MC found, start tracking slowest by its current frequency
            if (!l_fmc_valid)
            {
                l_fmc = l_fmc_curr;
                l_fmc_valid = true;
                FAPI_DBG("  Set slowest MC freq: %d MHz",
                         l_fmc);
            }
            // not the first found, update slowest frequency only if this MC is
            // running slower than the current saved value
            else
            {
                if (l_fmc_curr < l_fmc)
                {
                    l_fmc = l_fmc_curr;
                    FAPI_DBG("  Updated slowest MC freq: %d MHz",
                             l_fmc);
                }
            }
        }
    }

    // backoff to safest settings if not at POR DD2 frequency setpoint
    if (l_fpau != 2250)
    {
        FAPI_DBG("Tsnoop: set by non-POR PAU frequency (%d MHz)",
                 l_fpau);
        l_tsnoop = 27;
    }
    else
    {
        // set tsnoop based on memory controller setpoint if any functional
        // MCs are present on this chip
        if (l_fmc_valid)
        {
            FAPI_DBG("Tsnoop: set by MC freqeuncy (%d MHz)",
                     l_fmc);

            switch (l_fmc)
            {
                case 1333:
                    l_tsnoop = 27;
                    break;

                case 1466:
                    l_tsnoop = 26;
                    break;

                case 1600:
                case 2000:
                    l_tsnoop = 25;
                    break;

                default:
                    l_tsnoop = 27;
                    break;
            }
        }
        // no MC snoopers, keep default tsnoop based on PAU requirement
        else
        {
            FAPI_DBG("Tsnoop: set by POR PAU frequency (%d MHz)",
                     l_fpau);
            l_tsnoop = 25;
        }
    }

    // convert tsnoop cycle count into register programming
    // 0xC = 25
    // 0xD = 26
    // 0xE = 27
    FAPI_DBG("Final tnsoop: %d", l_tsnoop);
    l_tsnoop = 0xC + (l_tsnoop - 25);
    SET_PB_COM_SCOM_EQ0_STATION_CFG3_PB_CFG_CHIP_TSNOOP_DELAY_EQ0(l_tsnoop, l_pb_cfg3_data);

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

/// @brief Configures interrupt controller mode registers
/// @param[in] i_target       Reference to processor chip target
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_scominit_int(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt;
    using namespace scomt::proc;

    fapi2::buffer<uint64_t> l_tctxt_cfg = 0;

    FAPI_DBG("Entering ...");

    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_FUSED_CORE_MODE_Type l_fused_core_mode;
    fapi2::ATTR_CORE_LPAR_MODE_Type l_core_lpar_mode;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE, FAPI_SYSTEM, l_fused_core_mode));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CORE_LPAR_MODE, FAPI_SYSTEM, l_core_lpar_mode));

    FAPI_TRY(GET_INT_PC_REGS_TCTXT_CFG(i_target, l_tctxt_cfg),
             "Error from getScom (INT_PC_REGS_TCTXT_CFG)");

    if (l_fused_core_mode == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_FUSED)
    {
        SET_INT_PC_REGS_TCTXT_CFG_CFG_FUSE_CORE_EN(l_tctxt_cfg);
    }

    if (l_core_lpar_mode == fapi2::ENUM_ATTR_CORE_LPAR_MODE_LPAR_PER_CORE)
    {
        SET_INT_PC_REGS_TCTXT_CFG_CFG_PHYP_CORE_MODE(l_tctxt_cfg);
    }

    FAPI_TRY(PUT_INT_PC_REGS_TCTXT_CFG(i_target, l_tctxt_cfg),
             "Error from getScom (INT_PC_REGS_TCTXT_CFG)");

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

/// @brief Configures the core LPAR mode prior to powering on the cores
/// @param[in] i_target       Reference to processor chip target
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_scominit_core_lpar(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt;
    using namespace scomt::proc;
    using namespace scomt::eq;
    using namespace scomt::c;

    FAPI_DBG("Entering ...");

    fapi2::buffer<uint64_t> l_qme_scrb;
    fapi2::buffer<uint64_t> l_scsr;
    auto l_eq_mc  = i_target.getMulticast<fapi2::TARGET_TYPE_EQ>(fapi2::MCGROUP_ALL_EQ);
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_CORE_LPAR_MODE_Type l_core_lpar_mode;
    fapi2::ATTR_CHIP_EC_FEATURE_DYN_CORE_LPAR_Type l_ec_dyn_core_lpar;
    fapi2::buffer<uint64_t> l_perv_ctrl0;
    fapi2::buffer<uint64_t> l_export_regl_status;
    bool l_smt8_req = false;
    bool l_smt8_act = false;

    // confirm that requested core mode (fused/normal) aligns with feedback reported from export regulation
    // status registers
    // - HWP code in p10_setup_sbe_config will alwasy place request in PERV_CTRL0[22], based on HWSV/Cronus platform
    //   state of ATTR_FUSED_CORE_MODE
    // - HW will reflect actual state (considering OTPROM programmed restrictions) in EXPORT_REGL_STATUS
    // - cross-checking HW state here should work across different platform implementations, to ensure that
    //   ATTR_FUSED_CORE mode attribute matches actual HW state going forward
    //   (PPE platform currently updates ATTR_FUSED_CORE_MODE based on EXPORT_REGL_STATUS to reflect true
    //    state, Cronus platform currently does not)
    FAPI_TRY(fapi2::getScom(i_target, TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_PERV_CTRL0_RW, l_perv_ctrl0))
    l_smt8_req = l_perv_ctrl0.getBit<TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_PERV_CTRL0_TP_OTP_SCOM_FUSED_CORE_MODE>();

    FAPI_DBG("Read EXPORT_REGL_STATUS to confirm core fused state");
    FAPI_TRY(fapi2::getScom(i_target, TP_TPCHIP_PIB_OTP_OTPC_M_EXPORT_REGL_STATUS, l_export_regl_status));
    l_smt8_act = l_export_regl_status.getBit<TP_TPCHIP_PIB_OTP_OTPC_M_EXPORT_REGL_STATUS_TP_EX_FUSE_SMT8_CTYPE_EN_DC>();

    FAPI_ASSERT(l_smt8_req == l_smt8_act,
                fapi2::P10_SBE_SCOMINIT_FUSED_CORE_MISMATCH_ERR()
                .set_TARGET_CHIP(i_target)
                .set_FUSED_CORE_REQ(l_smt8_req)
                .set_FUSED_CORE_ACT(l_smt8_act),
#ifdef __PPE__
                "HW export regulation status fused core state (%d) does not match mailbox request (%d)!",
                ((l_smt8_act) ? 1 : 0),
                ((l_smt8_req) ? 1 : 0));
#else
                "HW export regulation status fused core state (%s) does not match mailbox request (%s)!",
                ((l_smt8_act) ? ("fused") : ("normal")),
                ((l_smt8_req) ? ("fused") : ("normal")));
#endif

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_DYN_CORE_LPAR, i_target, l_ec_dyn_core_lpar));

    //Skip if we are in DD1 hw
    if (!l_ec_dyn_core_lpar)
    {
        return fapi2::current_err;
    }

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CORE_LPAR_MODE, FAPI_SYSTEM, l_core_lpar_mode));

    if (l_core_lpar_mode == fapi2::ENUM_ATTR_CORE_LPAR_MODE_LPAR_PER_CORE)
    {

        FAPI_TRY(PREP_QME_SCRB_WO_OR(l_eq_mc),
                 "Error from prep (QME_SCRB_WO_OR)");
        l_qme_scrb.setBit<20, 4>();
        FAPI_TRY(PUT_QME_SCRB_WO_OR(l_eq_mc, l_qme_scrb),
                 "Error from putScom (QME_SCRB_WO_OR)");
    }

    for ( auto l_core_target : i_target.getChildren<fapi2::TARGET_TYPE_CORE>( fapi2::TARGET_STATE_FUNCTIONAL ) )
    {
        if (l_core_lpar_mode == fapi2::ENUM_ATTR_CORE_LPAR_MODE_LPAR_PER_CORE)
        {
            FAPI_TRY(PREP_QME_SCSR_SCOM2(l_core_target),
                     "Error from prep (QME_SCSR_SCOM2)");
            SET_P10_20_QME_SCSR_SINGLE_LPAR_MODE(l_scsr);
            FAPI_TRY(PUT_QME_SCSR_SCOM2(l_core_target, l_scsr),
                     "Error from putScom (QME_SCSR_SCOM2)");
        }
    }

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}

/// @brief Remove access to cl2/l3/mma regions in all EQs
/// @param[in] i_target       Reference to processor chip target
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_scominit_prep_eqs(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt::perv;

    // EQ region enablement in SBE:
    // - p10_sbe_chiplet_reset -> +PERV/+QME/+CLKADJ       (all good regions)
    // - p10_sbe_startclocks   -> +PG CL2/L3/MMA regions   (HW519203, to permit skew adjust function)
    // - p10_sbe_scominit      -> -ALL CL2/L3/MMA regions  (match powered-off state of logic)
    // - p10_sbe_select_ex     -> +USED CL2/L3/MMA regions (active cores, backing caches used for boot, primary chip only)
    //
    // istep15,16 and HCODE manage runtime enablement for cl2/l3/mma

    FAPI_DBG("Entering ...");

    auto l_mc_eq = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_ALL_EQ);
    fapi2::buffer<uint64_t> l_data64;

    // revert to only enable perv, qme, clkadj region PGOOD/PSCOM_EN controls
    l_data64.flush<0>();
    l_data64.setBit<4>().setBit<13>().setBit<14>();

    FAPI_TRY(fapi2::putScom(l_mc_eq, CPLT_CTRL2_RW, l_data64));
    FAPI_TRY(fapi2::putScom(l_mc_eq, CPLT_CTRL3_RW, l_data64));

    // set DFT fence for all cl2/l3/mma regions
    l_data64.flush<0>();
    l_data64.setBit<5, 8>().setBit<15, 4>();
    FAPI_TRY(fapi2::putScom(l_mc_eq, CPLT_CTRL5_RW, l_data64));

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}

/// Main function, see description in header
fapi2::ReturnCode p10_sbe_scominit(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("Entering ...");

    FAPI_TRY(p10_sbe_scominit_fbc(i_target), "Error from p10_sbe_scominit_fbc");
    FAPI_TRY(p10_sbe_scominit_int(i_target), "Error from p10_sbe_scominit_int");
    FAPI_TRY(p10_sbe_scominit_bars(i_target), "Error from p10_sbe_scominit_bars");
    FAPI_TRY(p10_sbe_scominit_trace(i_target), "Error from p10_sbe_scominit_trace");
    FAPI_TRY(p10_sbe_scominit_core_lpar(i_target), "Error from p10_sbe_scominit_core_lpar");
    FAPI_TRY(p10_sbe_scominit_prep_eqs(i_target), "Error from p10_sbe_scominit_prep_eqs");

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}
