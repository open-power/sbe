/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_sbe_scominit.C $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
//------------------------------------------------------------------------------
///
/// @file p9_sbe_scominit.C
/// @brief Peform SCOM initialization required for fabric & HBI operation (FAPI2)
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
#include <p9_sbe_scominit.H>
#include <p9_fbc_utils.H>

#include <p9_misc_scom_addresses.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>


//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// XSCOM/LPC BAR constants
const uint64_t XSCOM_BAR_MASK = 0xFF000003FFFFFFFFULL;
const uint64_t LPC_BAR_MASK = 0xFF000000FFFFFFFFULL;

// FBC FIR constants
const uint64_t FBC_CENT_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t FBC_CENT_FIR_ACTION1 = 0x0440000000000000ULL;
const uint64_t FBC_CENT_FIR_MASK    = 0x111FC00000000000ULL;
const uint64_t FBC_WEST_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t FBC_WEST_FIR_ACTION1 = 0x0000000000000000ULL;
const uint64_t FBC_WEST_FIR_MASK    = 0x0000FFFFC0000000ULL;
const uint64_t FBC_EAST_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t FBC_EAST_FIR_ACTION1 = 0x0000000000000000ULL;
const uint64_t FBC_EAST_FIR_MASK    = 0x00FF1FFFC0000000ULL;

// LPC FIR constants
const uint64_t LPC_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t LPC_FIR_ACTION1 = 0xFF00000000000000ULL;
const uint64_t LPC_FIR_MASK    = 0x00F0000000000000ULL;

// PBA FIR constants
const uint64_t PBA_FIR_ACTION0 = 0x0000000000000000ULL;
const uint64_t PBA_FIR_ACTION1 = 0x0C0100600C000000ULL;
const uint64_t PBA_FIR_MASK = 0x3082448062FC0000ULL;

// chiplet pervasive FIR constants
const uint64_t PERV_LFIR_ACTION0[15] =
{
    0x0000000000000000ULL, // TP
    0x0000000000000000ULL, // N0
    0x0000000000000000ULL, // N1
    0x0000000000000000ULL, // N2
    0x0000000000000000ULL, // N3
    0x0000000000000000ULL, // X
    0x0000000000000000ULL, // MC0
    0x0000000000000000ULL, // MC1
    0x0000000000000000ULL, // OB0
    0x0000000000000000ULL, // OB1
    0x0000000000000000ULL, // OB2
    0x0000000000000000ULL, // OB3
    0x0000000000000000ULL, // PCI0
    0x0000000000000000ULL, // PCI1
    0x0000000000000000ULL  // PCI2
};

const uint64_t PERV_LFIR_ACTION1[15] =
{
    0x8000000000000000ULL, // TP
    0x8000000000000000ULL, // N0
    0x8000000000000000ULL, // N1
    0x8000000000000000ULL, // N2
    0x8000000000000000ULL, // N3
    0x8000000000000000ULL, // X
    0x8000000000000000ULL, // MC0
    0x8000000000000000ULL, // MC1
    0x8000000000000000ULL, // OB0
    0x8000000000000000ULL, // OB1
    0x8000000000000000ULL, // OB2
    0x8000000000000000ULL, // OB3
    0x8000000000000000ULL, // PCI0
    0x8000000000000000ULL, // PCI1
    0x8000000000000000ULL  // PCI2
};

const uint64_t PERV_LFIR_MASK[15] =
{
    0xFFFFFFFFFFC00000ULL, // TP
    0xFFFFFFFFFFC00000ULL, // N0
    0xFFFFFFFFFFC00000ULL, // N1
    0xFFFFFFFFFFC00000ULL, // N2
    0xFFFFFFFFFFC00000ULL, // N3
    0xFFFFFFFFFFC00000ULL, // X
    0xFFFFFFFFFFC00000ULL, // MC0
    0xFFFFFFFFFFC00000ULL, // MC1
    0xFFFFFFFFFFC00000ULL, // OB0
    0xFFFFFFFFFFC00000ULL, // OB1
    0xFFFFFFFFFFC00000ULL, // OB2
    0xFFFFFFFFFFC00000ULL, // OB3
    0xFFFFFFFFFFC00000ULL, // PCI0
    0xFFFFFFFFFFC00000ULL, // PCI1
    0xFFFFFFFFFFC00000ULL  // PCI2
};

// chiplet XIR constants
const uint64_t PERV_XFIR_MASK[15] =
{
    0x9FFFFFE000000000ULL, // TP
    0x2007FFE000000000ULL, // N0
    0x201FFFE000000000ULL, // N1
    0x200FFFE000000000ULL, // N2
    0x000007E000000000ULL, // N3
    0x210FFFE000000000ULL, // X
    0x20007FE000000000ULL, // MC0
    0x20007FE000000000ULL, // MC1
    0x29FFFFE000000000ULL, // OB0
    0x29FFFFE000000000ULL, // OB1
    0x29FFFFE000000000ULL, // OB2
    0x29FFFFE000000000ULL, // OB3
    0x21FFFFE000000000ULL, // PCI0
    0x207FFFE000000000ULL, // PCI1
    0x201FFFE000000000ULL  // PCI2
};


//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_sbe_scominit(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)

{
    FAPI_DBG("Entering ...");
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    uint64_t l_base_addr_nm0;
    uint64_t l_base_addr_nm1;
    uint64_t l_base_addr_m;
    uint64_t l_base_addr_mmio;

    // set fabric topology information in each pervasive chiplet (outside of EC/EP)
    {
        // read fabric topology attributes
        uint32_t l_fbc_system_id;
        uint8_t l_fbc_group_id;
        uint8_t l_fbc_chip_id;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_SYSTEM_ID, i_target, l_fbc_system_id),
                 "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_SYSTEM_ID)");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID, i_target, l_fbc_group_id),
                 "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_GROUP_ID)");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_CHIP_ID, i_target, l_fbc_chip_id),
                 "Error from FAPI_ATTR_GET (ATTR_PROC_FABRIC_CHIP_ID)");

        for (auto l_chplt_target : i_target.getChildren<fapi2::TARGET_TYPE_PERV>
             (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_TP |
                                               fapi2::TARGET_FILTER_ALL_NEST |
                                               fapi2::TARGET_FILTER_XBUS |
                                               fapi2::TARGET_FILTER_ALL_OBUS |
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

    // determine base address of chip nm/m/mmmio regions in real address space
    FAPI_TRY(p9_fbc_utils_get_chip_base_address(i_target,
             l_base_addr_nm0,
             l_base_addr_nm1,
             l_base_addr_m,
             l_base_addr_mmio),
             "Error from p9_fbc_utils_get_chip_base_address");

    // set XSCOM BAR
    {
        fapi2::buffer<uint64_t> l_xscom_bar;
        uint64_t l_xscom_bar_offset;

        FAPI_DBG("Configuring XSCOM BAR");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET, FAPI_SYSTEM, l_xscom_bar_offset),
                 "Error from FAPI_ATTR_GET (ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET)");

        l_xscom_bar = l_base_addr_mmio;
        l_xscom_bar += l_xscom_bar_offset;

        FAPI_ASSERT((l_xscom_bar & XSCOM_BAR_MASK) == 0,
                    fapi2::P9_SBE_SCOMINIT_XSCOM_BAR_ATTR_ERR().
                    set_TARGET(i_target).
                    set_XSCOM_BAR(l_xscom_bar).
                    set_XSCOM_BAR_OFFSET(l_xscom_bar_offset).
                    set_BASE_ADDR_MMIO(l_base_addr_mmio),
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
                    fapi2::P9_SBE_SCOMINIT_LPC_BAR_ATTR_ERR().
                    set_TARGET(i_target).
                    set_LPC_BAR(l_lpc_bar).
                    set_LPC_BAR_OFFSET(l_lpc_bar_offset).
                    set_BASE_ADDR_MMIO(l_base_addr_mmio),
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
    }

    // configure PBA FIRs
    {
        fapi2::buffer<uint64_t> l_scom_data;

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

    // configure chiplet pervasive FIRs / XFIRs
    {
        for (auto l_chplt_target : i_target.getChildren<fapi2::TARGET_TYPE_PERV>
             (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_TP |
                                               fapi2::TARGET_FILTER_ALL_NEST |
                                               fapi2::TARGET_FILTER_XBUS |
                                               fapi2::TARGET_FILTER_ALL_MC |
                                               fapi2::TARGET_FILTER_ALL_OBUS |
                                               fapi2::TARGET_FILTER_ALL_PCI),
              fapi2::TARGET_STATE_FUNCTIONAL))
        {
            uint8_t l_unit_idx;
            fapi2::buffer<uint64_t> l_scom_data;

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_target, l_unit_idx),
                     "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");
            l_unit_idx--;


            // PERV LFIR
            FAPI_DBG("Configuring PERV LFIR (chiplet ID: %02X)", l_unit_idx + 1);

            // reset pervasive FIR
            l_scom_data = 0;
            FAPI_TRY(fapi2::putScom(l_chplt_target, PERV_LOCAL_FIR, l_scom_data),
                     "Error from putScom (PERV_LOCAL_FIR)");

            // configure pervasive FIR action/mask
            l_scom_data = PERV_LFIR_ACTION0[l_unit_idx];
            FAPI_TRY(fapi2::putScom(l_chplt_target, PERV_LOCAL_FIR_ACTION0, l_scom_data),
                     "Error from putScom (PERV_LOCAL_FIR_ACTION0)");

            l_scom_data = PERV_LFIR_ACTION1[l_unit_idx];
            FAPI_TRY(fapi2::putScom(l_chplt_target, PERV_LOCAL_FIR_ACTION1, l_scom_data),
                     "Error from putScom (PERV_LOCAL_FIR_ACTION1)");

            l_scom_data = PERV_LFIR_MASK[l_unit_idx];
            FAPI_TRY(fapi2::putScom(l_chplt_target, PERV_LOCAL_FIR_MASK, l_scom_data),
                     "Error from putScom (PERV_LOCAL_FIR_MASK)");

            // XFIR
            FAPI_DBG("Configuring chiplet XFIR (chiplet ID: %02X)", l_unit_idx + 1);
            // reset XFIR
            l_scom_data = 0;
            FAPI_TRY(fapi2::putScom(l_chplt_target, PERV_XFIR, l_scom_data),
                     "Error from putScom (PERV_XFIR)");

            // configure XFIR mask
            l_scom_data = PERV_XFIR_MASK[l_unit_idx];
            FAPI_TRY(fapi2::putScom(l_chplt_target, PERV_FIR_MASK, l_scom_data),
                     "Error from putScom (PERV_FIR_MASK");
        }
    }

fapi_try_exit:
    FAPI_DBG("Exiting ...");
    return fapi2::current_err;

}
