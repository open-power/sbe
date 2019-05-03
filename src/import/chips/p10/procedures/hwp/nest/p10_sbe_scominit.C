/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_scominit.C $ */
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
#include <p10_scom_nmmu_4.H>
#include <p10_scom_proc_3.H>
#include <p10_scom_proc_6.H>
#include <p10_scom_proc_7.H>
#include <p10_scom_proc_9.H>
#include <p10_scom_proc_a.H>
#include <p10_scom_proc_b.H>
#include <p10_scom_proc_c.H>
#include <p10_scom_proc_f.H>
#include <target_filters.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// XSCOM/LPC BAR constants
const uint64_t XSCOM_BAR_MASK   = 0xFF000003FFFFFFFFULL;
const uint64_t LPC_BAR_MASK     = 0xFF000000FFFFFFFFULL;

// @FIXME TODO RTC208222 Fill in actual FIR mask/action values
// FBC FIR constants
const uint64_t PU_PB_ES3_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t PU_PB_ES3_FIR_ACTION1 = 0x0000000000000000ULL;
const uint64_t PU_PB_ES3_FIR_MASK    = 0xFFFFFFFFFFFFFFFFULL;

// LPC FIR constants
const uint64_t LPC_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t LPC_FIR_ACTION1 = 0x0000000000000000ULL;
const uint64_t LPC_FIR_MASK    = 0xFFFFFFFFFFFFFFFFULL;

// PBA FIR constants
const uint64_t PBA_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t PBA_FIR_ACTION1 = 0x0000000000000000ULL;
const uint64_t PBA_FIR_MASK    = 0xFFFFFFFFFFFFFFFFULL;

// FBC Mode constants
const uint8_t PB_CFG_MCA_RATIO_OVERRIDE = 0x01;

// NMMU LCO constants
const uint8_t MAX_L3_TARGETS = 32;

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

/// @brief Configures fabric topology IDs in each pervasive chiplet
/// @param[in] i_target       Reference to processor chip target
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_scominit_topology(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt;
    using namespace scomt::perv;

    FAPI_DBG("Entering ...");

    fapi2::buffer<uint64_t> l_cplt_conf0;
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_PROC_FABRIC_TOPOLOGY_ID_Type l_fbc_topology_id;
    fapi2::TargetFilter l_target_filter = static_cast<fapi2::TargetFilter>(
            fapi2::TARGET_FILTER_TP |
            fapi2::TARGET_FILTER_ALL_EQ |
            fapi2::TARGET_FILTER_ALL_NEST |
            fapi2::TARGET_FILTER_ALL_IOHS |
            fapi2::TARGET_FILTER_ALL_PAU |
            fapi2::TARGET_FILTER_ALL_MC |
            fapi2::TARGET_FILTER_ALL_PCI);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_TOPOLOGY_ID, i_target, l_fbc_topology_id),
             "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_TOPOLOGY_ID)");

    for (auto& l_chplt_target : i_target.getChildren<fapi2::TARGET_TYPE_PERV>(l_target_filter,
            fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(GET_CPLT_CONF0_RW(l_chplt_target, l_cplt_conf0), "Error from getScom (PERV_CPLT_CONF0)");

        SET_CPLT_CONF0_TC_TOPOLOGY_ID_DC_48G(l_fbc_topology_id >> 3, l_cplt_conf0);
        SET_CPLT_CONF0_TC_TOPOLOGY_ID_DC_49G(l_fbc_topology_id >> 2, l_cplt_conf0);
        SET_CPLT_CONF0_TC_TOPOLOGY_ID_DC_50G(l_fbc_topology_id >> 1, l_cplt_conf0);
        SET_CPLT_CONF0_TC_TOPOLOGY_ID_DC_51G(l_fbc_topology_id, l_cplt_conf0);

        FAPI_TRY(PUT_CPLT_CONF0_RW(l_chplt_target, l_cplt_conf0), "Error from putScom (PERV_CPLT_CONF0)");
    }

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}

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

    auto l_mc_chiplets = i_target.getChildren<fapi2::TARGET_TYPE_MC>();
    auto l_pau_chiplets = i_target.getChildren<fapi2::TARGET_TYPE_PAU>();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_BROADCAST_MODE, FAPI_SYSTEM, l_broadcast_mode),
             "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_BROADCAST_MODE)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_ipl_phase),
             "Error from FAPI_ATTR_GET (ATTR_SYSTEM_IPL_PHASE)");

    FAPI_TRY(GET_PB_COM_ES3_MODE(i_target, l_pb_mode_data), "Error from getScom (PB_COM_ES3_MODE)");

    // configure broadcast mode
    switch(l_broadcast_mode)
    {
        case fapi2::ENUM_ATTR_PROC_FABRIC_BROADCAST_MODE_1HOP_CHIP_IS_GROUP:
            CLEAR_PB_COM_ES3_MODE_CFG_HOP_MODE_ES3(l_pb_mode_data);
            SET_PB_COM_ES3_MODE_CFG_PUMP_MODE_ES3(l_pb_mode_data);
            break;

        case fapi2::ENUM_ATTR_PROC_FABRIC_BROADCAST_MODE_1HOP_CHIP_IS_NODE:
            CLEAR_PB_COM_ES3_MODE_CFG_HOP_MODE_ES3(l_pb_mode_data);
            CLEAR_PB_COM_ES3_MODE_CFG_PUMP_MODE_ES3(l_pb_mode_data);
            break;

        case fapi2::ENUM_ATTR_PROC_FABRIC_BROADCAST_MODE_2HOP_CHIP_IS_NODE:
            SET_PB_COM_ES3_MODE_CFG_HOP_MODE_ES3(l_pb_mode_data);
            CLEAR_PB_COM_ES3_MODE_CFG_PUMP_MODE_ES3(l_pb_mode_data);
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
    SET_PB_COM_ES3_MODE_CFG_SL_DOMAIN_SIZE_ES3(l_pb_mode_data);

    // configure first mc as source for mca ratio if HBI and a valid mc exists, else override mc async
    if((l_ipl_phase == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL) && l_mc_chiplets.size())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_mc_chiplets.front(), l_chiplet_pos),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

        SET_PB_COM_ES3_MODE_CFG_MCA_RATIO_SEL_ES3(l_chiplet_pos, l_pb_mode_data);
    }
    else
    {
        SET_PB_COM_ES3_MODE_CFG_MCA_RATIO_OVERRIDE_ES3(PB_CFG_MCA_RATIO_OVERRIDE, l_pb_mode_data);
    }

    // configure first pau as source for pau ratio if HBI and a valid pau exists, else override pau async
    if((l_ipl_phase == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL) && l_pau_chiplets.size())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_pau_chiplets.front(), l_chiplet_pos),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

        // @FIXME TODO RTC209723 Replace with iopau target type when available
        SET_PB_COM_ES3_MODE_CFG_PAU_STEP_SEL_ES3(l_chiplet_pos / 2, l_pb_mode_data);
    }
    else
    {
        SET_PB_COM_ES3_MODE_CFG_PAU_STEP_OVERRIDE_ES3(l_pb_mode_data);
    }

    FAPI_TRY(PUT_PB_COM_ES3_MODE(i_target, l_pb_mode_data), "Error from putScom (PB_COM_ES3_MODE)");

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

    // set XSCOM BAR
    {
        fapi2::buffer<uint64_t> l_xscom_bar;
        fapi2::ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET_Type l_xscom_bar_offset;
        fapi2::ATTR_SMF_CONFIG_Type l_smf_config;

        FAPI_DBG("Configuring XSCOM BAR");

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET, FAPI_SYSTEM, l_xscom_bar_offset),
                 "Error from FAPI_ATTR_GET (ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET)");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SMF_CONFIG, FAPI_SYSTEM, l_smf_config),
                 "Error from FAPI_ATTR_GET (ATTR_SMF_CONFIG)");

        l_xscom_bar = l_base_addr_mmio + l_xscom_bar_offset;

        if (l_smf_config)
        {
            l_xscom_bar.setBit(FABRIC_ADDR_SMF_BIT);
        }

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

/// @brief Configures selected FIRs in preparation for fabric init
/// @param[in] i_target       Reference to processor chip target
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_scominit_firs(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt;
    using namespace scomt::proc;

    FAPI_DBG("Entering ...");

    fapi2::buffer<uint64_t> l_zeroes(0);

    scomt::disableRegchk();

    // configure PB ES3 FIRs
    FAPI_DBG("Configuring PB ES3 FIR");
    FAPI_TRY(PUT_PB_COM_ES3_FIR_REG_RWX(i_target, l_zeroes),
             "Error from putScom (PB_COM_ES3_FIR_REG_RWX)");
    FAPI_TRY(PUT_PB_COM_ES3_FIR_ACTION0_REG(i_target, PU_PB_ES3_FIR_ACTION0),
             "Error from putScom (PB_COM_ES3_FIR_ACTION0_REG)");
    FAPI_TRY(PUT_PB_COM_ES3_FIR_ACTION1_REG(i_target, PU_PB_ES3_FIR_ACTION1),
             "Error from putScom (PB_COM_ES3_FIR_ACTION1_REG)");
    FAPI_TRY(PUT_PB_COM_ES3_FIR_MASK_REG_RWX(i_target, PU_PB_ES3_FIR_MASK),
             "Error from putScom (PB_COM_ES3_FIR_MASK_REG_RWX)");

    // configure PBA FIRs
    FAPI_DBG("Configuring PBA FIR");
    FAPI_TRY(PUT_TP_TPBR_PBA_PBAF_PBAFIR_RWX(i_target, l_zeroes),
             "Error from putScom (TP_TPBR_PBA_PBAF_PBAFIR_RWX)");
    FAPI_TRY(PUT_TP_TPBR_PBA_PBAF_PBAFIRACT0(i_target, PBA_FIR_ACTION0),
             "Error from putScom (TP_TPBR_PBA_PBAF_PBAFIRACT0)");
    FAPI_TRY(PUT_TP_TPBR_PBA_PBAF_PBAFIRACT1(i_target, PBA_FIR_ACTION1),
             "Error from putScom (TP_TPBR_PBA_PBAF_PBAFIRACT1)");
    FAPI_TRY(PUT_TP_TPBR_PBA_PBAF_PBAFIRMASK_RW(i_target, PBA_FIR_MASK),
             "Error from putScom (TP_TPBR_PBA_PBAF_PBAFIRMASK_RW)");

    // @FIXME TODO HW489793/HW491147 Enable LPCFIR scom access in future drop
    // configure LPC FIRs
    //FAPI_DBG("Configuring LPC FIR");
    //FAPI_TRY(PUT_TP_LPC_SYNC_FIR_REG_RWX(i_target, l_zeroes),
    //         "Error from putScom (TP_LPC_SYNC_FIR_REG_RWX)");
    //FAPI_TRY(PUT_TP_LPC_SYNC_FIR_ACTION0_REG(i_target, LPC_FIR_ACTION0),
    //         "Error from putScom (TP_LPC_SYNC_FIR_ACTION0_REG)");
    //FAPI_TRY(PUT_TP_LPC_SYNC_FIR_ACTION1_REG(i_target, LPC_FIR_ACTION1),
    //         "Error from putScom (TP_LPC_SYNC_FIR_ACTION1_REG)");
    //FAPI_TRY(PUT_TP_LPC_SYNC_FIR_MASK_REG_RW(i_target, LPC_FIR_MASK),
    //         "Error from putScom (TP_LPC_SYNC_FIR_MASK_REG_RW)");

    scomt::enableRegchk();

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

/// @brief Configures NMMU LCO and runtime inits
/// @param[in] i_target       Reference to processor chip target
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_scominit_nmmu(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt;
    using namespace scomt::nmmu;

    FAPI_DBG("Entering ...");

    auto l_nmmu_targets = i_target.getChildren<fapi2::TARGET_TYPE_NMMU>();

    for (auto& l_nmmu : l_nmmu_targets)
    {
        // @FIXME TODO RTC208221 Enable call to nmmu scom initfile
        // execute NMMU initfile
        {
            fapi2::ReturnCode l_rc;

            //FAPI_DBG("Executing NMMU initfile");
            //FAPI_EXEC_HWP(l_rc, p10_mmu_scom, i_target, FAPI_SYSTEM);

            //if (l_rc)
            //{
            //    FAPI_ERR("Error from p10_mmu_scom (p10.mmu.scom.initfile)");
            //    fapi2::current_err = l_rc;
            //    goto fapi_try_exit;
            //}
        }

        // setup NMMU lco config
        {
            fapi2::buffer<uint64_t> l_scom_data;
            auto l_core_targets = i_target.getChildren<fapi2::TARGET_TYPE_CORE>();
            uint8_t l_lco_min;
            uint8_t l_lco_min_threshold;

            FAPI_TRY(GET_FBC_CQ_WRAP_NXCQ_SCOM_MMCQ_LCO_CONFIG_REG(l_nmmu, l_scom_data),
                     "Error from getScom (PU_NMMU_MMCQ_PB_MODE_REG)");

            // lco_targ_config: enable only valid L3s
            for (auto& l_core : l_core_targets)
            {
                uint8_t l_coreid = l_core.get();
                FAPI_TRY(l_scom_data.setBit(FBC_CQ_WRAP_NXCQ_SCOM_MMCQ_LCO_CONFIG_REG_TARG_CONFIG + l_coreid),
                         "Error from setBit (l_scom_data, FBC_CQ_WRAP_NXCQ_SCOM_MMCQ_LCO_CONFIG_REG_TARG_CONFIG + l_coreid)");
            }

            // lco_targ_min:
            // let lco_min_threshold = 2/3 of max possible L3s in a chip
            //   if 0 L3s or 1 L3, set to zero/one respectively
            //   if lco_min_threshold or less, set to one less than number of L3s
            //   if more than lco_min_threshold, set to lco_min_threshold
            l_lco_min_threshold = floor((2 * MAX_L3_TARGETS) / 3);

            if ((l_core_targets.size() == 0) || (l_core_targets.size() == 1))
            {
                l_lco_min = l_core_targets.size();
            }
            else if (l_core_targets.size() < l_lco_min_threshold)
            {
                l_lco_min = l_core_targets.size() - 1;
            }
            else
            {
                l_lco_min = l_lco_min_threshold;
            }

            SET_FBC_CQ_WRAP_NXCQ_SCOM_MMCQ_LCO_CONFIG_REG_TARG_MIN(l_lco_min, l_scom_data);

            FAPI_TRY(PUT_FBC_CQ_WRAP_NXCQ_SCOM_MMCQ_LCO_CONFIG_REG(l_nmmu, l_scom_data),
                     "Error from putScom (PU_NMMU_MMCQ_PB_MODE_REG)");
        }
    }

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}

/// Main function, see description in header
fapi2::ReturnCode p10_sbe_scominit(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("Entering ...");

    FAPI_TRY(p10_sbe_scominit_topology(i_target), "Error from p10_sbe_scominit_topology\n");
    FAPI_TRY(p10_sbe_scominit_fbc(i_target), "Error from p10_sbe_scominit_fbc\n");
    FAPI_TRY(p10_sbe_scominit_bars(i_target), "Error from p10_sbe_scominit_bars\n");
    FAPI_TRY(p10_sbe_scominit_firs(i_target), "Error from p10_sbe_scominit_firs\n");
    FAPI_TRY(p10_sbe_scominit_trace(i_target), "Error from p10_sbe_scominit_trace\n");
    FAPI_TRY(p10_sbe_scominit_nmmu(i_target), "Error from p10_sbe_scominit_nmmu\n");

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}
