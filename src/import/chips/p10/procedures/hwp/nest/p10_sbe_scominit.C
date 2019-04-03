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
//#include <p10_mmu_scom.H>
//#include <p10_xbus_fir_utils.H>

#include <p9_misc_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9n2_perv_scom_addresses_fld.H>
#include <p9_xbus_scom_addresses.H>
#include <p9_obus_scom_addresses.H>
//#include <p10_sbe_common.H>
#include <target_filters.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// XSCOM/LPC BAR constants
const uint64_t XSCOM_BAR_SMF_OFFSET = 0x0008000000000000ULL;
const uint64_t XSCOM_BAR_MASK = 0xFF000003FFFFFFFFULL;
const uint64_t LPC_BAR_MASK = 0xFF000000FFFFFFFFULL;

// FBC FIR constants
const uint64_t FBC_CENT_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t FBC_CENT_FIR_ACTION1 = 0x0040000000000000ULL;
const uint64_t FBC_CENT_FIR_MASK    = 0x151FC00000000000ULL;
const uint64_t FBC_WEST_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t FBC_WEST_FIR_ACTION1 = 0x0000000000000000ULL;
const uint64_t FBC_WEST_FIR_MASK    = 0x0000FFFFC0000000ULL;
const uint64_t FBC_EAST_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t FBC_EAST_FIR_ACTION1 = 0x0000000000000000ULL;
const uint64_t FBC_EAST_FIR_MASK    = 0x00FF1FFEC0000000ULL;

// LPC FIR constants
const uint64_t LPC_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t LPC_FIR_ACTION1 = 0xE700000000000000ULL;
const uint64_t LPC_FIR_MASK    = 0x18F0000000000000ULL;

// PBA FIR constants
const uint64_t PBA_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t PBA_FIR_ACTION1 = 0x0C0100600C000000ULL;
const uint64_t PBA_FIR_MASK = 0x3082448062FC0000ULL;

// PPE FIR constants
// FBC
const uint64_t FBC_PPE_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t FBC_PPE_FIR_ACTION1 = 0xF1C0000000000000ULL;
const uint64_t FBC_PPE_FIR_MASK    = 0x0E3C000000000000ULL;
// XBUS
const uint64_t XB_PPE_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t XB_PPE_FIR_ACTION1 = 0xF1C0000000000000ULL;
const uint64_t XB_PPE_FIR_MASK    = 0x0E38000000000000ULL;
// OBUS
const uint64_t OB_PPE_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t OB_PPE_FIR_ACTION1 = 0xF1C0000000000000ULL;
const uint64_t OB_PPE_FIR_MASK    = 0x0E38000000000000ULL;

// FBC XBUS PPE psave constants
const uint8_t PB_PSAVE_CFG_HUT = 0x3;
const uint8_t PB_PSAVE_CFG_LUT = 0x1;
const uint8_t PB_PSAVE_CFG_HUC = 0x1;
const uint8_t PB_PSAVE_CFG_LUC = 0xff;
const uint8_t PB_PSAVE_CFG_WSIZE = 0x1;

// nest trace setup constants
const uint8_t N1_PROBE1_SEL = 0x11;

// nest DTS enablement constants
const uint8_t NEST_THERM_MODE_SAMPLE_PULSE_COUNT = 0xF;
const uint8_t NEST_THERM_MODE_LOOP0_ENABLE = 0x2;
const uint8_t NEST_THERM_MODE_LOOP1_ENABLE = 0x1;

// XBUS chiplet partial good region constants
const uint32_t X_PG_IOX0_REGION_BIT = 5;
const uint32_t X_PG_IOX1_REGION_BIT = 6;
const uint32_t X_PG_IOX2_REGION_BIT = 7;

const uint32_t X_PG_PBIOX0_REGION_BIT = 9;
const uint32_t X_PG_PBIOX1_REGION_BIT = 10;
const uint32_t X_PG_PBIOX2_REGION_BIT = 11;

// FBC EXTFIR constants
// one register per chip (encompassing all links), in N3 chiplet
const uint64_t FBC_EXT_FIR_ACTION0       = 0x0000000000000000ULL;
const uint64_t FBC_EXT_FIR_ACTION1       = 0x0000000000000000ULL;
const uint64_t FBC_EXT_FIR_MASK          = 0x0100000000000000ULL;

const uint64_t FBC_EXT_FIR_MASK_X0_NF    = 0x8000000000000000ULL;
const uint64_t FBC_EXT_FIR_MASK_X1_NF    = 0x4000000000000000ULL;
const uint64_t FBC_EXT_FIR_MASK_X2_NF    = 0x2000000000000000ULL;

// FBC TL FIR constants
// one register per chip (encompassing all links), in N3 chiplet
const uint64_t FBC_IOE_TL_FIR_ACTION0    = 0x0000000000000000ULL;
const uint64_t FBC_IOE_TL_FIR_ACTION1    = 0x0049000000000000ULL;
const uint64_t FBC_IOE_TL_FIR_MASK       = 0xFF24F0303FFFF11FULL;

const uint64_t FBC_IOE_TL_FIR_MASK_X0_NF = 0x00C00C0C00000880ULL;
const uint64_t FBC_IOE_TL_FIR_MASK_X1_NF = 0x0018030300000440ULL;
const uint64_t FBC_IOE_TL_FIR_MASK_X2_NF = 0x000300C0C0000220ULL;

// FBC DL FIR constants
// one register per link, in XBUS chiplet
const uint64_t FBC_IOE_DL_FIR_ACTION0    = 0x0000000000000000ULL;
const uint64_t FBC_IOE_DL_FIR_ACTION1    = 0x0303C00000001FFCULL;
const uint64_t FBC_IOE_DL_FIR_MASK       = 0xFCFC3FFFFFFFE003ULL;

const uint64_t FBC_IOE_DL_FIR_MASK_NF    = 0xFFFFFFFFFFFFFFFFULL;

// XBUS PHY FIR constants
// one register per link, in XBUS chiplet
const uint64_t XBUS_PHY_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t XBUS_PHY_FIR_ACTION1 = 0x2068680000000000ULL;
const uint64_t XBUS_PHY_FIR_MASK    = 0xDF9797FFFFFFC000ULL;

const uint64_t XBUS_PHY_FIR_MASK_NF = 0xFFFFFFFFFFFFC000ULL;

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

fapi2::ReturnCode p10_sbe_scominit(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("Entering ...");
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    //uint64_t l_base_addr_nm0;
    //uint64_t l_base_addr_nm1;
    //uint64_t l_base_addr_m;
    uint64_t l_base_addr_mmio;

    // set fabric topology information in each pervasive chiplet (outside of EC/EP)
    {
        // read fabric topology attributes
        uint32_t l_fbc_system_id = 0;
        uint8_t l_fbc_group_id = 0;
        uint8_t l_fbc_chip_id = 0;
        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_SYSTEM_ID, i_target, l_fbc_system_id),
        //         "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_SYSTEM_ID)");
        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID, i_target, l_fbc_group_id),
        //         "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_GROUP_ID)");
        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_CHIP_ID, i_target, l_fbc_chip_id),
        //         "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_CHIP_ID)");

        for (auto& l_chplt_target : i_target.getChildren<fapi2::TARGET_TYPE_PERV>
             (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_TP |
                                               fapi2::TARGET_FILTER_ALL_NEST |
                                               fapi2::TARGET_FILTER_XBUS |
                                               fapi2::TARGET_FILTER_ALL_OBUS |
                                               fapi2::TARGET_FILTER_ALL_MC |
                                               fapi2::TARGET_FILTER_ALL_PCI),
              fapi2::TARGET_STATE_FUNCTIONAL))
        {
            fapi2::buffer<uint64_t> l_cplt_conf0;
            FAPI_TRY(fapi2::getScom(l_chplt_target, PERV_CPLT_CONF0, l_cplt_conf0),
                     "Error from getScom (PERV_CPLT_CONF0)");
            l_cplt_conf0.insertFromRight<PERV_1_CPLT_CONF0_TC_UNIT_SYS_ID_DC, PERV_1_CPLT_CONF0_TC_UNIT_SYS_ID_DC_LEN>
            (l_fbc_system_id)
            .insertFromRight<PERV_1_CPLT_CONF0_TC_UNIT_GROUP_ID_DC, PERV_1_CPLT_CONF0_TC_UNIT_GROUP_ID_DC_LEN>(l_fbc_group_id)
            .insertFromRight<PERV_1_CPLT_CONF0_TC_UNIT_CHIP_ID_DC, PERV_1_CPLT_CONF0_TC_UNIT_CHIP_ID_DC_LEN>(l_fbc_chip_id);
            FAPI_TRY(fapi2::putScom(l_chplt_target, PERV_CPLT_CONF0, l_cplt_conf0),
                     "Error from putScom (PERV_CPLT_CONF0)");
        }
    }

    //// determine base address of chip nm/m/mmmio regions in real address space
    //FAPI_TRY(p10_fbc_utils_get_chip_base_address_no_aliases(
    //             i_target,
    //             EFF_FBC_GRP_CHIP_IDS,
    //             l_base_addr_nm0,
    //             l_base_addr_nm1,
    //             l_base_addr_m,
    //             l_base_addr_mmio),
    //         "Error from p10_fbc_utils_get_chip_base_address");

    // set XSCOM BAR
    {
        fapi2::buffer<uint64_t> l_xscom_bar;
        uint64_t l_xscom_bar_offset;
        //fapi2::ATTR_CHIP_EC_FEATURE_SMF_SUPPORTED_Type l_smf_supported;
        fapi2::ATTR_SMF_CONFIG_Type l_smf_config;

        FAPI_DBG("Configuring XSCOM BAR");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET, FAPI_SYSTEM, l_xscom_bar_offset),
                 "Error from FAPI_ATTR_GET (ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET)");
        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_SMF_SUPPORTED, i_target, l_smf_supported),
        //         "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_SMF_SUPPORTED)");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SMF_CONFIG, fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(), l_smf_config),
                 "Error from FAPI_ATTR_GET (ATTR_SMF_CONFIG)");

        l_xscom_bar = l_base_addr_mmio;
        l_xscom_bar += l_xscom_bar_offset;

        //if (l_smf_config && l_smf_supported)
        if (l_smf_config)
        {
            l_xscom_bar += XSCOM_BAR_SMF_OFFSET;
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

        FAPI_TRY(fapi2::putScom(i_target, PU_XSCOM_BASE_REG, l_xscom_bar),
                 "Error from putScom (PU_XSCOM_BASE_REG)");
    }

    // set LPC BAR
    {
        fapi2::buffer<uint64_t> l_lpc_bar;
        uint64_t l_lpc_bar_offset;

        FAPI_DBG("Configuring LPC BAR");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_LPC_BAR_BASE_ADDR_OFFSET, FAPI_SYSTEM, l_lpc_bar_offset),
                 "Error from FAPI_ATTR_GET (ATRR_PROC_LPC_BAR_BASE_ADDR_OFFSET");

        l_lpc_bar = l_base_addr_mmio;
        l_lpc_bar += l_lpc_bar_offset;

        FAPI_ASSERT((l_lpc_bar & LPC_BAR_MASK) == 0,
                    fapi2::P10_SBE_SCOMINIT_LPC_BAR_ATTR_ERR()
                    .set_TARGET(i_target)
                    .set_BAR(l_lpc_bar)
                    .set_BAR_MASK(LPC_BAR_MASK)
                    .set_BAR_OVERLAP(l_lpc_bar & LPC_BAR_MASK)
                    .set_BAR_OFFSET(l_lpc_bar_offset)
                    .set_BASE_ADDR_MMIO(l_base_addr_mmio),
                    "Invalid LPC BAR configuration!");

        FAPI_TRY(fapi2::putScom(i_target, PU_LPC_BASE_REG, l_lpc_bar),
                 "Error from putScom (PU_LPC_BASE_REG)");
    }

    // configure FBC FIRs
    {
        fapi2::buffer<uint64_t> l_scom_data;

        // CENT
        FAPI_DBG("Configuring FBC CENT FIR");
        // clear FIR
        l_scom_data = 0;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM0_PB_CENT_FIR_REG, l_scom_data),
                 "Error from putScom (PU_PB_CENT_SM0_PB_CENT_FIR_REG)");

        // configure action/mask
        l_scom_data = FBC_CENT_FIR_ACTION0;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM0_PB_CENT_FIR_ACTION0_REG, l_scom_data),
                 "Error from putScom (PU_PB_CENT_SM0_PB_CENT_FIR_ACTION0_REG)");

        l_scom_data = FBC_CENT_FIR_ACTION1;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM0_PB_CENT_FIR_ACTION1_REG, l_scom_data),
                 "Error from putScom (PU_PB_CENT_SM0_PB_CENT_FIR_ACTION1_REG)");

        l_scom_data = FBC_CENT_FIR_MASK;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM0_PB_CENT_FIR_MASK_REG, l_scom_data),
                 "Error from putScom (PU_PB_CENT_SM0_PB_CENT_FIR_MASK_REG)");

        // set spare/masked FIRs bit as a signal to HB that the SBE will handle
        // setup of XBUS/OBUS related EXTIR bits
#ifdef __PPE__
        FAPI_TRY(fapi2::getScom(i_target, PU_PB_CENT_SM0_PB_CENT_FIR_REG, l_scom_data),
                 "Error from getScom (PU_PB_CENT_SM0_PB_CENT_FIR_REG)");
        l_scom_data.setBit<PU_PB_CENT_SM0_PB_CENT_FIR_MASK_REG_SPARE_13>();
        l_scom_data.setBit<PU_PB_CENT_SM0_PB_CENT_FIR_MASK_REG_SPARE_14>();
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM0_PB_CENT_FIR_REG, l_scom_data),
                 "Error from putScom (PU_PB_CENT_SM0_PB_CENT_FIR_REG)");
#endif

        // WEST
        FAPI_DBG("Configuring FBC WEST FIR");
        // clear FIR
        l_scom_data = 0;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_WEST_SM0_PB_WEST_FIR_REG, l_scom_data),
                 "Error from putScom (PU_PB_WEST_SM0_PB_WEST_FIR_REG)");

        // configure action/mask
        l_scom_data = FBC_WEST_FIR_ACTION0;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_WEST_SM0_PB_WEST_FIR_ACTION0_REG, l_scom_data),
                 "Error from putScom (PU_PB_WEST_SM0_PB_WEST_FIR_ACTION0_REG)");

        l_scom_data = FBC_WEST_FIR_ACTION1;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_WEST_SM0_PB_WEST_FIR_ACTION1_REG, l_scom_data),
                 "Error from putScom (PU_PB_WEST_SM0_PB_WEST_FIR_ACTION1_REG)");

        l_scom_data = FBC_WEST_FIR_MASK;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_WEST_SM0_PB_WEST_FIR_MASK_REG, l_scom_data),
                 "Error from putScom (PU_PB_WEST_SM0_PB_WEST_FIR_MASK_REG)");

        // EAST
        FAPI_DBG("Configuring FBC EAST FIR");
        // clear FIR
        l_scom_data = 0;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_EAST_FIR_REG, l_scom_data),
                 "Error from putScom (PU_PB_EAST_FIR_REG)");

        // configure action/mask
        l_scom_data = FBC_EAST_FIR_ACTION0;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_EAST_FIR_ACTION0_REG, l_scom_data),
                 "Error from putScom (PU_PB_EAST_FIR_ACTION0_REG)");

        l_scom_data = FBC_EAST_FIR_ACTION1;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_EAST_FIR_ACTION1_REG, l_scom_data),
                 "Error from putScom (PU_PB_EAST_FIR_ACTION1_REG)");

        l_scom_data = FBC_EAST_FIR_MASK;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_EAST_FIR_MASK_REG, l_scom_data),
                 "Error from putScom (PU_PB_EAST_FIR_MASK_REG)");

        // EXTFIR
        FAPI_DBG("Configuring FBC EXTFIR");
        // clear FIR
        l_scom_data = 0;
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM1_EXTFIR_REG, l_scom_data),
                 "Error from putScom (PU_PB_CENT_SM1_EXTFIR_REG)");
        // configure action/mask
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM1_EXTFIR_ACTION0_REG, FBC_EXT_FIR_ACTION0),
                 "Error from putScom (PU_PB_CENT_SM1_EXTFIR_ACTION0_REG)");
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM1_EXTFIR_ACTION1_REG, FBC_EXT_FIR_ACTION1),
                 "Error from putScom (PU_PB_CENT_SM1_EXTFIR_ACTION1_REG)");
#ifdef __PPE__
        FAPI_TRY(fapi2::putScom(i_target, PU_PB_CENT_SM1_EXTFIR_MASK_REG, FBC_EXT_FIR_MASK),
                 "Error from putScom (PU_PB_CENT_SM1_EXTFIR_MASK_REG)");
#endif
    }

    // configure PBA mode switches & FIRs
    {
        fapi2::buffer<uint64_t> l_scom_data;
        uint8_t l_hw423589_option1 = 0;

        //// set PBACFG
        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW423589_OPTION1,
        //                       i_target,
        //                       l_hw423589_option1));
        l_scom_data = 0;

        if (l_hw423589_option1)
        {
            l_scom_data.setBit<PU_PBACFG_CHSW_DIS_GROUP_SCOPE>();
        }

        FAPI_DBG("Resetting PBACFG with value = 0x%16llX", uint64_t(l_scom_data));
        FAPI_TRY(fapi2::putScom(i_target, PU_PBACFG, l_scom_data),
                 "Error from putScom (PU_PBACFG)");

        // clear FIR
        FAPI_DBG("Configuring PBA FIR");
        l_scom_data = 0;
        FAPI_TRY(fapi2::putScom(i_target, PU_PBAFIR, l_scom_data),
                 "Error from putScom (PU_PBAFIR)");

        // configure action/mask
        l_scom_data = PBA_FIR_ACTION0;
        FAPI_TRY(fapi2::putScom(i_target, PU_PBAFIRACT0, l_scom_data),
                 "Error from putScom (PU_PBAFIRACT0)");

        l_scom_data = PBA_FIR_ACTION1;
        FAPI_TRY(fapi2::putScom(i_target, PU_PBAFIRACT1, l_scom_data),
                 "Error from putScom (PU_PBAFIRACT1)");

        l_scom_data = PBA_FIR_MASK;
        FAPI_TRY(fapi2::putScom(i_target, PU_PBAFIRMASK, l_scom_data),
                 "Error from putScom (PU_PBAFIRMASK)");
    }

    // configure LPC FIRs
    {
        fapi2::buffer<uint64_t> l_scom_data;

        // clear FIR
        FAPI_DBG("Configuring LPC FIR");
        l_scom_data = 0;
        FAPI_TRY(fapi2::putScom(i_target, PU_SYNC_FIR_REG, l_scom_data),
                 "Error from putScom (PU_SYNC_FIR_REG)");

        // configure action/mask
        l_scom_data = LPC_FIR_ACTION0;
        FAPI_TRY(fapi2::putScom(i_target, PU_SYNC_FIR_ACTION0_REG, l_scom_data),
                 "Error from putScom (PU_SYNC_FIR_ACTION0_REG)");

        l_scom_data = LPC_FIR_ACTION1;
        FAPI_TRY(fapi2::putScom(i_target, PU_SYNC_FIR_ACTION1_REG, l_scom_data),
                 "Error from putScom (PU_SYNC_FIR_ACTION1_REG)");

        l_scom_data = LPC_FIR_MASK;
        FAPI_TRY(fapi2::putScom(i_target, PU_SYNC_FIR_MASK_REG, l_scom_data),
                 "Error from putScom (PU_SYNC_FIR_MASK_REG)");
    }

    // configure chiplet pervasive FIRs / LFIRs / XFIRs
    {
        uint8_t l_mc_sync_mode = 0;
        uint8_t l_use_dmi_buckets = 0;
        fapi2::TargetFilter l_target_filter = static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_TP |
                                              fapi2::TARGET_FILTER_ALL_NEST |
                                              fapi2::TARGET_FILTER_XBUS |
                                              fapi2::TARGET_FILTER_ALL_OBUS |
                                              fapi2::TARGET_FILTER_ALL_PCI);

        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target, l_mc_sync_mode),
        //         "Error from FAPI_ATTR_GET (ATTR_MC_SYNC_MODE)");

        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_DMI_MC_PLL_SCAN_BUCKETS, i_target, l_use_dmi_buckets),
        //         "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_DMI_MC_PLL_SCAN_BUCKETS)");

        if (l_mc_sync_mode || l_use_dmi_buckets)
        {
            l_target_filter = static_cast<fapi2::TargetFilter>(l_target_filter | fapi2::TARGET_FILTER_ALL_MC);
        }

        for (auto& l_chplt_target : i_target.getChildren<fapi2::TARGET_TYPE_PERV>(l_target_filter,
                fapi2::TARGET_STATE_FUNCTIONAL))
        {
            uint8_t l_unit_pos = 0;
            //fapi2::ATTR_CHIP_EC_FEATURE_SW432374_Type l_sw432374;
            fapi2::buffer<uint64_t> l_tp_lfir_mask_and;

            //FAPI_INF("Call p10_sbe_common_configure_chiplet_FIR");
            //FAPI_TRY(p10_sbe_common_configure_chiplet_FIR(l_chplt_target));

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_target, l_unit_pos),
                     "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

            if (l_unit_pos == PERV_CHIPLET_ID)
            {
                //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_SW432374, i_target, l_sw432374),
                //         "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_SW432374)");

                //if (l_sw432374)
                //{
                //    l_tp_lfir_mask_and.flush<1>().clearBit<PERV_1_LOCAL_FIR_IN37>();
                //    FAPI_TRY(fapi2::putScom(i_target, PERV_TP_LOCAL_FIR_MASK_AND, l_tp_lfir_mask_and),
                //             "Error from putScom (PERV_TP_LOCAL_FIR_MASK_AND)");
                //}
            }

            if (l_unit_pos == N3_CHIPLET_ID)
            {
                // configure FBC PPE FIRs
                FAPI_TRY(fapi2::putScom(i_target, PU_PB_PPE_LFIRACT0, FBC_PPE_FIR_ACTION0),
                         "Error from putScom (PU_PB_PPE_LFIRACT0)");

                FAPI_TRY(fapi2::putScom(i_target, PU_PB_PPE_LFIRACT1, FBC_PPE_FIR_ACTION1),
                         "Error from putScom (PU_PB_PPE_LFIRACT1)");

                FAPI_TRY(fapi2::putScom(i_target, PU_PB_PPE_LFIRMASK, FBC_PPE_FIR_MASK),
                         "Error from putScom (PU_PB_PPE_LFIRMASK)");
            }

            if (l_unit_pos == XB_CHIPLET_ID)
            {
                // configure XBUS PPE FIRs
                FAPI_TRY(fapi2::putScom(i_target, XBUS_IOPPE_PPE_FIR_ACTION0_REG, XB_PPE_FIR_ACTION0),
                         "Error from putScom (XBUS_IOPPE_PPE_FIR_ACTION0_REG)");

                FAPI_TRY(fapi2::putScom(i_target, XBUS_IOPPE_PPE_FIR_ACTION1_REG, XB_PPE_FIR_ACTION1),
                         "Error from putScom (XBUS_IOPPE_PPE_FIR_ACTION1_REG)");

                FAPI_TRY(fapi2::putScom(i_target, XBUS_IOPPE_PPE_FIR_MASK_REG, XB_PPE_FIR_MASK),
                         "Error from putScom (XBUS_IOPPE_PPE_FIR_MASK_REG)");
            }

            if (l_unit_pos == OB0_CHIPLET_ID)
            {
                // configure OBUS0 PPE FIRs
                FAPI_TRY(fapi2::putScom(i_target, OBUS_0_IOPPE_PPE_FIR_ACTION0_REG, OB_PPE_FIR_ACTION0),
                         "Error from putScom (OBUS_0_IOPPE_PPE_FIR_ACTION0_REG)");

                FAPI_TRY(fapi2::putScom(i_target, OBUS_0_IOPPE_PPE_FIR_ACTION1_REG, OB_PPE_FIR_ACTION1),
                         "Error from putScom (OBUS_0_IOPPE_PPE_FIR_ACTION1_REG)");

                FAPI_TRY(fapi2::putScom(i_target, OBUS_0_IOPPE_PPE_FIR_MASK_REG, OB_PPE_FIR_MASK),
                         "Error from putScom (OBUS_0_IOPPE_PPE_FIR_MASK_REG)");
            }

            if (l_unit_pos == OB1_CHIPLET_ID)
            {
                // configure OBUS1 PPE FIRs
                FAPI_TRY(fapi2::putScom(i_target, OBUS_1_IOPPE_PPE_FIR_ACTION0_REG, OB_PPE_FIR_ACTION0),
                         "Error from putScom (OBUS_1_IOPPE_PPE_FIR_ACTION0_REG)");

                FAPI_TRY(fapi2::putScom(i_target, OBUS_1_IOPPE_PPE_FIR_ACTION1_REG, OB_PPE_FIR_ACTION1),
                         "Error from putScom (OBUS_1_IOPPE_PPE_FIR_ACTION1_REG)");

                FAPI_TRY(fapi2::putScom(i_target, OBUS_1_IOPPE_PPE_FIR_MASK_REG, OB_PPE_FIR_MASK),
                         "Error from putScom (OBUS_1_IOPPE_PPE_FIR_MASK_REG)");
            }

            if (l_unit_pos == OB2_CHIPLET_ID)
            {
                // configure OBUS2 PPE FIRs
                FAPI_TRY(fapi2::putScom(i_target, OBUS_2_IOPPE_PPE_FIR_ACTION0_REG, OB_PPE_FIR_ACTION0),
                         "Error from putScom (OBUS_2_IOPPE_PPE_FIR_ACTION0_REG)");

                FAPI_TRY(fapi2::putScom(i_target, OBUS_2_IOPPE_PPE_FIR_ACTION1_REG, OB_PPE_FIR_ACTION1),
                         "Error from putScom (OBUS_2_IOPPE_PPE_FIR_ACTION1_REG)");

                FAPI_TRY(fapi2::putScom(i_target, OBUS_2_IOPPE_PPE_FIR_MASK_REG, OB_PPE_FIR_MASK),
                         "Error from putScom (OBUS_2_IOPPE_PPE_FIR_MASK_REG)");
            }

            if (l_unit_pos == OB3_CHIPLET_ID)
            {
                // configure OBUS3 PPE FIRs
                FAPI_TRY(fapi2::putScom(i_target, OBUS_3_IOPPE_PPE_FIR_ACTION0_REG, OB_PPE_FIR_ACTION0),
                         "Error from putScom (OBUS_3_IOPPE_PPE_FIR_ACTION0_REG)");

                FAPI_TRY(fapi2::putScom(i_target, OBUS_3_IOPPE_PPE_FIR_ACTION1_REG, OB_PPE_FIR_ACTION1),
                         "Error from putScom (OBUS_3_IOPPE_PPE_FIR_ACTION1_REG)");

                FAPI_TRY(fapi2::putScom(i_target, OBUS_3_IOPPE_PPE_FIR_MASK_REG, OB_PPE_FIR_MASK),
                         "Error from putScom (OBUS_3_IOPPE_PPE_FIR_MASK_REG)");
            }
        }
    }

    // configure NEST tracing logic
    {
        // mux N1 DBG broadcast(1) onto oob(5)
        fapi2::buffer<uint64_t> l_cplt_conf0;
        FAPI_TRY(fapi2::getScom(i_target, PERV_N1_CPLT_CONF0, l_cplt_conf0),
                 "Error from getScom (PERV_N1_CPLT_CONF0)");
        l_cplt_conf0.insertFromRight<PERV_1_CPLT_CONF0_CTRL_MISC_PROBE1_SEL_DC,
                                     PERV_1_CPLT_CONF0_CTRL_MISC_PROBE1_SEL_DC_LEN>(N1_PROBE1_SEL);
        FAPI_TRY(fapi2::putScom(i_target, PERV_N1_CPLT_CONF0, l_cplt_conf0),
                 "Error from putScom (PERV_N1_CPLT_CONF0)");

        // ungate global broadcast input on N3 DBG
        FAPI_TRY(fapi2::getScom(i_target, PERV_N3_CPLT_CONF0, l_cplt_conf0),
                 "Error from getScom (PERV_N3_CPLT_CONF0)");
        l_cplt_conf0.setBit<P9N2_PERV_1_CPLT_CONF0_TC_PCB_DBG_GLB_BRCST_EN>();
        FAPI_TRY(fapi2::putScom(i_target, PERV_N3_CPLT_CONF0, l_cplt_conf0),
                 "Error from putScom (PERV_N3_CPLT_CONF0)");
    }

    // configure PCI tracing logic
    {
        for (auto& l_chplt_target : i_target.getChildren<fapi2::TARGET_TYPE_PERV>
             (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_PCI),
              fapi2::TARGET_STATE_FUNCTIONAL))
        {
            fapi2::buffer<uint64_t> l_dbg_mode_reg;
            fapi2::buffer<uint64_t> l_dbg_trace_mode_reg_2;

            FAPI_TRY(fapi2::getScom(l_chplt_target, PERV_DBG_MODE_REG, l_dbg_mode_reg),
                     "Error from getScom (PERV_DBG_MODE_REG)");
            l_dbg_mode_reg.setBit<PERV_1_DBG_MODE_REG_STOP_ON_XSTOP_SELECTION>();
            l_dbg_mode_reg.clearBit<PERV_1_DBG_MODE_REG_STOP_ON_RECOV_ERR_SELECTION>();
            l_dbg_mode_reg.clearBit<PERV_1_DBG_MODE_REG_STOP_ON_SPATTN_SELECTION>();
            FAPI_TRY(fapi2::putScom(l_chplt_target, PERV_DBG_MODE_REG, l_dbg_mode_reg),
                     "Error from putScom (PERV_DBG_MODE_REG)");

            FAPI_TRY(fapi2::getScom(l_chplt_target, PERV_DBG_TRACE_MODE_REG_2, l_dbg_trace_mode_reg_2),
                     "Error from getScom (PERV_DBG_TRACE_MODE_REG_2)");
            l_dbg_trace_mode_reg_2.setBit<PERV_1_DBG_TRACE_MODE_REG_2_STOP_ON_ERR>();
            FAPI_TRY(fapi2::putScom(l_chplt_target, PERV_DBG_TRACE_MODE_REG_2, l_dbg_trace_mode_reg_2),
                     "Error from putScom (PERV_DBG_TRACE_MODE_REG_2)");
        }
    }

    // execute NMMU initfile
    {
        fapi2::ReturnCode l_rc;
        fapi2::buffer<uint64_t> l_scom_data;
        auto l_ex_targets = i_target.getChildren<fapi2::TARGET_TYPE_EX>();
        uint8_t l_lco_min;

        //FAPI_DBG("Executing NMMU initfile");
        //FAPI_EXEC_HWP(l_rc, p10_mmu_scom, i_target, FAPI_SYSTEM);

        //if (l_rc)
        //{
        //    FAPI_ERR("Error from p10_mmu_scom (p10.mmu.scom.initfile)");
        //    fapi2::current_err = l_rc;
        //    goto fapi_try_exit;
        //}

        // setup NMMU lco config (for all chips, but lco is only enabled for ndd2+)
        // > NMMU.MM_FBC.CQ_WRAP.NXCQ_SCOM.LCO_TARG_CONFIG
        //   enable only valid EXs
        // > NMMU.MM_FBC.CQ_WRAP.NXCQ_SCOM.LCO_TARG_MIN
        //   if there are more than 8 EXs, set to 8
        //   if 8 EXs or less, set to one less than number of EXs
        //   if 0 EXs or 1 EX, set to zero/one respectively
        FAPI_TRY(fapi2::getScom(i_target, PU_NMMU_MMCQ_PB_MODE_REG, l_scom_data),
                 "Error from getScom (PU_NMMU_MMCQ_PB_MODE_REG)");

        for (auto& l_ex : l_ex_targets)
        {
            uint8_t l_exid = l_ex.get();
            FAPI_TRY(l_scom_data.setBit(PU_NMMU_MMCQ_PB_MODE_REG_LCO_TARG_CONFIG + l_exid),
                     "Error from setBit (l_scom_data, PU_NMMU_MMCQ_PB_MODE_REG_LCO_TARG_CONFIG + l_exid)");
        }

        switch (l_ex_targets.size())
        {
            case 0:
                l_lco_min = 0;
                break;

            case 1:
                l_lco_min = 1;
                break;

            case 9:
            case 10:
            case 11:
            case 12:
                l_lco_min = 8;
                break;

            default:
                l_lco_min = l_ex_targets.size() - 1;
                break;
        }

        l_scom_data.insertFromRight<PU_NMMU_MMCQ_PB_MODE_REG_LCO_TARG_MIN, PU_NMMU_MMCQ_PB_MODE_REG_LCO_TARG_MIN_LEN>
        (l_lco_min);

        FAPI_TRY(fapi2::putScom(i_target, PU_NMMU_MMCQ_PB_MODE_REG, l_scom_data),
                 "Error from putScom (PU_NMMU_MMCQ_PB_MODE_REG)");
    }

    // enable nest DTS function
    {
        fapi2::buffer<uint64_t> l_data(0);
        FAPI_DBG("Enable DTS in N1 via THERM_MODE_REG[5,6-9,20]");
        FAPI_TRY(getScom(i_target, PERV_N1_THERM_MODE_REG, l_data));
        // DTS sampling enable | sample pulse count | DTS loop0 enable
        l_data.setBit<PERV_1_THERM_MODE_REG_DTS_SAMPLE_ENA>()
        .insertFromRight<PERV_1_THERM_MODE_REG_SAMPLE_PULSE_CNT, PERV_1_THERM_MODE_REG_SAMPLE_PULSE_CNT_LEN>
        (NEST_THERM_MODE_SAMPLE_PULSE_COUNT)
        .insertFromRight<PERV_1_THERM_MODE_REG_DTS_ENABLE_L1, PERV_1_THERM_MODE_REG_DTS_ENABLE_L1_LEN>
        (NEST_THERM_MODE_LOOP0_ENABLE);
        FAPI_TRY(putScom(i_target, PERV_N1_THERM_MODE_REG, l_data));

        FAPI_DBG("Enable DTSs (2 of them) in N3 via THERM_MODE_REG[5,6-9,20-21]");
        FAPI_TRY(getScom(i_target, PERV_N3_THERM_MODE_REG, l_data));
        // DTS sampling enable | sample pulse count | DTS loop0 and 1 enable
        l_data.setBit<PERV_1_THERM_MODE_REG_DTS_SAMPLE_ENA>()
        .insertFromRight<PERV_1_THERM_MODE_REG_SAMPLE_PULSE_CNT, PERV_1_THERM_MODE_REG_SAMPLE_PULSE_CNT_LEN>
        (NEST_THERM_MODE_SAMPLE_PULSE_COUNT)
        .insertFromRight<PERV_1_THERM_MODE_REG_DTS_ENABLE_L1, PERV_1_THERM_MODE_REG_DTS_ENABLE_L1_LEN>
        (NEST_THERM_MODE_LOOP0_ENABLE | NEST_THERM_MODE_LOOP1_ENABLE);
        FAPI_TRY(putScom(i_target, PERV_N3_THERM_MODE_REG, l_data));
    }

    // configure XBUS FIRs & PPE
    {
        bool l_xbus_chiplet_good = false;
        fapi2::buffer<uint64_t> l_psave_cfg;
        fapi2::buffer<uint64_t> l_zero = 0;
        fapi2::buffer<uint64_t> l_ones;
        l_ones.flush<1>();

        for (auto& l_chplt_target : i_target.getChildren<fapi2::TARGET_TYPE_PERV>
             (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_XBUS),
              fapi2::TARGET_STATE_FUNCTIONAL))
        {
            l_xbus_chiplet_good = true;
            //fapi2::buffer<uint16_t> l_attr_pg = 0;
            fapi2::buffer<uint32_t> l_attr_pg = 0;

            // obtain partial good information to determine viable X links
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, l_chplt_target, l_attr_pg),
                     "Error from FAPI_ATTR_GET (ATTR_PG)");

            // IOX0
            if (!l_attr_pg.getBit<X_PG_IOX0_REGION_BIT>() && !l_attr_pg.getBit<X_PG_PBIOX0_REGION_BIT>())
            {
                // XBUS0 FBC DL
                FAPI_TRY(putScom(i_target, XBUS_0_LL0_LL0_LL0_IOEL_FIR_REG, l_zero),
                         "Error from putScom (XBUS_0_LL0_IOEL_FIR_REG)");
                FAPI_TRY(putScom(i_target, XBUS_0_LL0_IOEL_FIR_ACTION0_REG, FBC_IOE_DL_FIR_ACTION0),
                         "Error from putScom (XBUS_0_LL0_IOEL_FIR_ACTION0_REG)");
                FAPI_TRY(putScom(i_target, XBUS_0_LL0_IOEL_FIR_ACTION1_REG, FBC_IOE_DL_FIR_ACTION1),
                         "Error from putScom (XBUS_0_LL0_IOEL_FIR_ACTION1_REG)");
                FAPI_TRY(putScom(i_target, XBUS_0_LL0_LL0_LL0_IOEL_FIR_MASK_REG, FBC_IOE_DL_FIR_MASK),
                         "Error from putScom (XBUS_0_LL0_IOEL_FIR_MASK_REG)");

                // XBUS0 PHY
                FAPI_TRY(fapi2::putScom(i_target, XBUS_FIR_REG, l_zero),
                         "Error from putScom (XBUS_FIR_REG)");
                FAPI_TRY(fapi2::putScom(i_target, XBUS_FIR_ACTION0_REG, XBUS_PHY_FIR_ACTION0),
                         "Error from putScom (XBUS_FIR_ACTION0_REG)");
                FAPI_TRY(fapi2::putScom(i_target, XBUS_FIR_ACTION1_REG, XBUS_PHY_FIR_ACTION1),
                         "Error from putScom (XBUS_FIR_ACTION1_REG)");
                FAPI_TRY(fapi2::putScom(i_target, XBUS_FIR_MASK_REG, XBUS_PHY_FIR_MASK),
                         "Error from putScom (XBUS_FIR_MASK_REG)");
            }

            // IOX1
            if (!l_attr_pg.getBit<X_PG_IOX1_REGION_BIT>() && !l_attr_pg.getBit<X_PG_PBIOX1_REGION_BIT>())
            {
                // XBUS1 FBC DL
                FAPI_TRY(putScom(i_target, XBUS_1_LL1_LL1_LL1_IOEL_FIR_REG, l_zero),
                         "Error from putScom (XBUS_1_LL1_IOEL_FIR_REG)");
                FAPI_TRY(putScom(i_target, XBUS_1_LL1_IOEL_FIR_ACTION0_REG, FBC_IOE_DL_FIR_ACTION0),
                         "Error from putScom (XBUS_1_LL1_IOEL_FIR_ACTION0_REG)");
                FAPI_TRY(putScom(i_target, XBUS_1_LL1_IOEL_FIR_ACTION1_REG, FBC_IOE_DL_FIR_ACTION1),
                         "Error from putScom (XBUS_1_LL1_IOEL_FIR_ACTION1_REG)");
                FAPI_TRY(putScom(i_target, XBUS_1_LL1_LL1_LL1_IOEL_FIR_MASK_REG, FBC_IOE_DL_FIR_MASK),
                         "Error from putScom (XBUS_1_LL1_IOEL_FIR_MASK_REG)");

                // XBUS1 PHY
                FAPI_TRY(fapi2::putScom(i_target, XBUS_1_FIR_REG, l_zero),
                         "Error from putScom (XBUS_1_FIR_REG)");
                FAPI_TRY(fapi2::putScom(i_target, XBUS_1_FIR_ACTION0_REG, XBUS_PHY_FIR_ACTION0),
                         "Error from putScom (XBUS_1_FIR_ACTION0_REG)");
                FAPI_TRY(fapi2::putScom(i_target, XBUS_1_FIR_ACTION1_REG, XBUS_PHY_FIR_ACTION1),
                         "Error from putScom (XBUS_1_FIR_ACTION1_REG)");
                FAPI_TRY(fapi2::putScom(i_target, XBUS_1_FIR_MASK_REG, XBUS_PHY_FIR_MASK),
                         "Error from putScom (XBUS_1_FIR_MASK_REG)");
            }

            // IOX2
            if (!l_attr_pg.getBit<X_PG_IOX2_REGION_BIT>() && !l_attr_pg.getBit<X_PG_PBIOX2_REGION_BIT>())
            {
                // XBUS2 FBC DL
                FAPI_TRY(putScom(i_target, XBUS_2_LL2_LL2_LL2_IOEL_FIR_REG, l_zero),
                         "Error from putScom (XBUS_2_LL2_IOEL_FIR_REG)");
                FAPI_TRY(putScom(i_target, XBUS_2_LL2_IOEL_FIR_ACTION0_REG, FBC_IOE_DL_FIR_ACTION0),
                         "Error from putScom (XBUS_2_LL2_IOEL_FIR_ACTION0_REG)");
                FAPI_TRY(putScom(i_target, XBUS_2_LL2_IOEL_FIR_ACTION1_REG, FBC_IOE_DL_FIR_ACTION1),
                         "Error from putScom (XBUS_2_LL2_IOEL_FIR_ACTION1_REG)");
                FAPI_TRY(putScom(i_target, XBUS_2_LL2_LL2_LL2_IOEL_FIR_MASK_REG, FBC_IOE_DL_FIR_MASK),
                         "Error from putScom (XBUS_2_LL2_IOEL_FIR_MASK_REG)");

                // XBUS2 PHY
                FAPI_TRY(fapi2::putScom(i_target, XBUS_2_FIR_REG, l_zero),
                         "Error from putScom (XBUS_2_FIR_REG)");
                FAPI_TRY(fapi2::putScom(i_target, XBUS_2_FIR_ACTION0_REG, XBUS_PHY_FIR_ACTION0),
                         "Error from putScom (XBUS_2_FIR_ACTION0_REG)");
                FAPI_TRY(fapi2::putScom(i_target, XBUS_2_FIR_ACTION1_REG, XBUS_PHY_FIR_ACTION1),
                         "Error from putScom (XBUS_2_FIR_ACTION1_REG)");
                FAPI_TRY(fapi2::putScom(i_target, XBUS_2_FIR_MASK_REG, XBUS_PHY_FIR_MASK),
                         "Error from putScom (XBUS_2_FIR_MASK_REG)");
            }
        }

        // XBUS FBC TL
        FAPI_TRY(putScom(i_target, PU_PB_IOE_FIR_REG, l_zero),
                 "Error from putScom (PU_PB_IOE_FIR_REG)");
        FAPI_TRY(putScom(i_target, PU_PB_IOE_FIR_ACTION0_REG, FBC_IOE_TL_FIR_ACTION0),
                 "Error from putScom (PU_PB_IOE_FIR_ACTION0_REG)");
        FAPI_TRY(putScom(i_target, PU_PB_IOE_FIR_ACTION1_REG, FBC_IOE_TL_FIR_ACTION1),
                 "Error from putScom (PU_PB_IOE_FIR_ACTION1_REG)");
        FAPI_TRY(putScom(i_target, PU_PB_IOE_FIR_MASK_REG,
                         (l_xbus_chiplet_good) ? (FBC_IOE_TL_FIR_MASK) : (l_ones())),
                 "Error from putScom (PU_PB_IOE_FIR_MASK_REG)");

        // PPE psave config
        FAPI_TRY(fapi2::getScom(i_target,
                                PU_PB_PSAVE_CFG,
                                l_psave_cfg),
                 "Error from getScom (PU_PB_PSAVE_CFG)");

        l_psave_cfg.setBit<PU_PB_PSAVE_CFG_X0_ACT>()
        .setBit<PU_PB_PSAVE_CFG_X1_ACT>()
        .setBit<PU_PB_PSAVE_CFG_X2_ACT>();
        l_psave_cfg.insertFromRight<PU_PB_PSAVE_CFG_HUT, PU_PB_PSAVE_CFG_HUT_LEN>(0x3);
        l_psave_cfg.insertFromRight<PU_PB_PSAVE_CFG_LUT, PU_PB_PSAVE_CFG_LUT_LEN>(0x1);
        l_psave_cfg.insertFromRight<PU_PB_PSAVE_CFG_HUC, PU_PB_PSAVE_CFG_HUC_LEN>(0x1);
        l_psave_cfg.insertFromRight<PU_PB_PSAVE_CFG_LUC, PU_PB_PSAVE_CFG_LUC_LEN>(0xff);
        l_psave_cfg.insertFromRight<PU_PB_PSAVE_CFG_WSIZE, PU_PB_PSAVE_CFG_WSIZE_LEN>(0x1);

        FAPI_TRY(fapi2::putScom(i_target,
                                PU_PB_PSAVE_CFG,
                                l_psave_cfg),
                 "Error from putScom (PU_PB_PSAVE_CFG)");
    }

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;
}
