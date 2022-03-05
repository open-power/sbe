/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_attr_setup.C $ */
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
//------------------------------------------------------------------------------
/// @file  p10_sbe_attr_setup.C
///
/// @brief Read scratch Regs, update ATTR
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include <p10_sbe_attr_setup.H>
#include <p10_scom_perv.H>
#include <p10_sbe_scratch_regs.H>
#include <target_filters.H>
#include <p10_sbe_hb_structures.H>
#include <p10_determine_eco_mode.H>
#include <p10_perv_sbe_cmn.H>

const uint32_t NUM_EQS_PER_CHIP  = 8;  // Num of EQ pervasive chiplets per chip
const uint32_t NUM_CORES_PER_EQ  = 4;  // Num of cores per EQ chiplet
const uint32_t NUM_PAUC_PER_CHIP = 4;  // Num of PAU pervasive chiplets per chip
const uint32_t NUM_PAUS_PER_PAUC = 2;  // Num of PAU logical units per PAU pervasive chiplet
const uint32_t NUM_IOHS_PER_CHIP = 8;  // Num of IOHS pervasive chiplets per chip
const uint32_t CORE0_ECL2_PG_BIT = 13; // ECL2 bit in EQ ATTR_PG
const uint32_t CORE0_L3_PG_BIT   = 17; // L3 bit in EQ ATTR_PG
const uint32_t CORE0_MMA_PG_BIT  = 23; // MMA bit in EQ ATTR_PG

const uint32_t SBE_LFR_REG       = 0x000C0002040;

/// @brief detect presence of at least one runnable core within an EQ, given
///        the chiplet level partial good information
///
/// @param[in] i_eq_pg EQ partial good attribute value
///
/// @return Boolean indicating presence of core supporting instruction execution
///
bool
p10_sbe_scratch_regs_eq_has_runnable_cores(
    const uint32_t i_eq_pg)
{
    uint32_t core_slice_mask_exp_zero = 0x00044100; // ecl20,l30,mma0
    uint32_t core_slice_mask_exp_one  = 0x00000008; // co0

    for (uint8_t c = 0; c < 4; c++)
    {
        if (!(i_eq_pg & core_slice_mask_exp_zero) &&
            (i_eq_pg & core_slice_mask_exp_one))
        {
            return true;
        }

        core_slice_mask_exp_zero = core_slice_mask_exp_zero >> 1;
        core_slice_mask_exp_one  = core_slice_mask_exp_one >> 1;
    }

    return false;
}

///
/// @brief Set platform ATTR_PG attribute for EQ chiplets, based on MPVD
///        driven partial good (ATTR_PG_MPVD).
///
/// @param[in] i_target_chip  Processor chip target
///
/// @return fapi2::ReturnCode
///
fapi2::ReturnCode
p10_sbe_attr_update_eq_pg_from_mvpd(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_DBG("Start");
    bool l_good_core_chip_detected = false;

    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_ALL_EQ),
            fapi2::TARGET_STATE_PRESENT))
    {
        // read MVPD attribute for that eq and write it to PG attribute.
        fapi2::ATTR_PG_Type l_pg;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG via ATTR_PG_MVPD: 0x%08X  Inverse:  0x%08X", l_pg, ~l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));

        // For IOSCM, no elements in the EQ MVPD record will be marked good
        // However, we only care about cores, L3s, and MMAs

        if (p10_sbe_scratch_regs_eq_has_runnable_cores(l_pg))
        {
            l_good_core_chip_detected = true;
            FAPI_DBG("  SBE Good core chip detected");
        }
    }

    if (!l_good_core_chip_detected)
    {
        FAPI_INF("  Chip with no good cores detected");
        fapi2::ATTR_ZERO_CORE_CHIP_Type l_all_zero_core_chip = fapi2::ENUM_ATTR_ZERO_CORE_CHIP_TRUE;
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_ZERO_CORE_CHIP, i_target_chip, l_all_zero_core_chip));
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

///
/// @brief Set platform ATTR_PG attribute for EQ chiplets, based on MVPD
///        driven partial good (ATTR_PG_MVPD) and mailbox gard records
///
/// @param[in] i_target_chip  Processor chip target
/// @param[in] i_scratch1_reg Buffer reflecting mailbox1 gard record content
///
/// @return fapi2::ReturnCode
///
fapi2::ReturnCode
p10_sbe_scratch_regs_write_eq_pg_from_scratch(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
    const fapi2::buffer<uint64_t> i_scratch1_reg)
{
    FAPI_DBG("Start");

    uint32_t l_core_gard = 0;
    bool l_good_core_chip_detected = false;

    // EQ - apply core GARD information to partial good vector
    i_scratch1_reg.extract<CORE_GARD_STARTBIT, CORE_GARD_LENGTH>(l_core_gard);

    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_ALL_EQ),
            fapi2::TARGET_STATE_PRESENT))
    {
        // extract bits associated with this quad (4 cores)
        uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num(l_perv, fapi2::TARGET_TYPE_EQ);
        uint32_t l_gard_mask = (l_core_gard >>
                                (NUM_CORES_PER_EQ * ((NUM_EQS_PER_CHIP - 1) - l_unit_num))) & CORE_GARD_EQ_MASK;
        FAPI_DBG("EQ%d, core gard mask: 0x%08X", l_unit_num, l_gard_mask);

        // shift into position in partial good attribute
        fapi2::ATTR_PG_Type l_pg;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG via ATTR_PG_MVPD: 0x%08X  Inverse:  0x%08X", l_pg, ~l_pg);

#ifdef __PPE__

        // For IOSCM, no elements in the EQ MVPD record will be marked good
        // However, we only care about cores, L3s, and MMAs

        if (p10_sbe_scratch_regs_eq_has_runnable_cores(l_pg))
        {
            l_good_core_chip_detected = true;
            FAPI_DBG("  SBE Good core chip detected");
        }

#endif
        l_pg |= ((l_gard_mask << (EQ_PG_MMA_SHIFT))    |  // MMA0..3
                 (l_gard_mask << (EQ_PG_L3_SHIFT))     |  // L30..3
                 (l_gard_mask << (EQ_PG_ECL2_SHIFT)));    // ECL20..3
        FAPI_DBG("  PG after: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));

#ifndef __PPE__

        // For Cronus, detect if there is at least 1 core good in this quad.
        // This is done post-gard as this is how Cronus communicates desired
        // configuration.

        if (p10_sbe_scratch_regs_eq_has_runnable_cores(l_pg))
        {
            l_good_core_chip_detected = true;
            FAPI_DBG("  Cronus Good core chip detected");
        }

#endif
    }

    if (!l_good_core_chip_detected)
    {
        FAPI_INF("  Chip with no good cores detected");
        fapi2::ATTR_ZERO_CORE_CHIP_Type l_all_zero_core_chip = fapi2::ENUM_ATTR_ZERO_CORE_CHIP_TRUE;
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_ZERO_CORE_CHIP, i_target_chip, l_all_zero_core_chip));
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

///
/// @brief Set platform ATTR_PG attribute for non-EQ chiplets, based on MPVD
///        driven partial good (ATTR_PG_MPVD) and mailbox gard records
///
/// @param[in] i_target_chip  Processor chip target
/// @param[in] i_scratch2_reg Buffer reflecting mailbox2 gard record content
/// @param[in] i_scratch7_reg Buffer reflecting mailbox7 gard record content
///
/// @return fapi2::ReturnCode
///
fapi2::ReturnCode
p10_sbe_scratch_regs_write_noneq_pg_from_scratch(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
    const fapi2::buffer<uint64_t> i_scratch2_reg,
    const fapi2::buffer<uint64_t> i_scratch7_reg)
{
    FAPI_DBG("Start");

    // TP - set partial good vectors
    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_TP),
            fapi2::TARGET_STATE_PRESENT))
    {
        fapi2::ATTR_PG_Type l_pg;
        FAPI_DBG("TP");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
    }

    // N0 - set partial good vectors
    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_NEST_NORTH),
            fapi2::TARGET_STATE_PRESENT))
    {
        fapi2::ATTR_PG_Type l_pg;
        FAPI_DBG("N0");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
    }

    // apply GARD information to PCI0..1
    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_ALL_PCI),
            fapi2::TARGET_STATE_PRESENT))
    {
        uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num(l_perv, fapi2::TARGET_TYPE_PEC);
        fapi2::ATTR_PG_Type l_pg;
        FAPI_DBG("PCI%d, gard mask: 0x%X",
                 l_unit_num,
                 i_scratch2_reg.getBit(static_cast<uint32_t>(PCI_GARD_STARTBIT) + l_unit_num));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG before: 0x%08X", l_pg);

        if (i_scratch2_reg.getBit(static_cast<uint32_t>(PCI_GARD_STARTBIT) + l_unit_num))
        {
            l_pg = 0xFFFFFFFF;
        }

        FAPI_DBG("  PG after: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
    }

    // apply GARD information to MC0..3
    {
        fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type l_attr_proc_sbe_master_chip = fapi2::ENUM_ATTR_PROC_SBE_MASTER_CHIP_FALSE;
        auto l_mc_chiplets = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                 static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC), fapi2::TARGET_STATE_PRESENT);
        fapi2::buffer<uint32_t> l_good_mcs = 0, l_garded_mcs = 0;
        bool l_preserve_first_good = false;

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_SBE_MASTER_CHIP, i_target_chip, l_attr_proc_sbe_master_chip));

        // Collect gard and PG information for all MC
        for (const auto& l_perv : l_mc_chiplets)
        {
            uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num(l_perv, fapi2::TARGET_TYPE_MC);
            fapi2::ATTR_PG_Type l_pg;
            bool l_garded = i_scratch2_reg.getBit(static_cast<uint32_t>(MC_GARD_STARTBIT) + l_unit_num);

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
            FAPI_DBG("MC%d, garded: %d, PG before: 0x%08X", l_unit_num, l_garded, l_pg);

            l_good_mcs.writeBit(l_pg != 0xFFFFFFFF, l_unit_num);
            l_garded_mcs.writeBit(l_garded, l_unit_num);
        }

        // Make sure that one good MC stays ungarded since we need one for HB
        FAPI_DBG("Good MCs: 0x%08X, garded MCs: 0x%08X", l_good_mcs, l_garded_mcs);

        if ((l_attr_proc_sbe_master_chip == fapi2::ENUM_ATTR_PROC_SBE_MASTER_CHIP_TRUE) &&
            (l_good_mcs & ~l_garded_mcs) == 0)
        {
            FAPI_DBG("Garding would leave no good MCs enabled, forcing first good MC on");
            l_preserve_first_good = true;
        }

        // Reprocess, deconfigure any that are unneeded
        for (const auto& l_perv : l_mc_chiplets)
        {
            uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num(l_perv, fapi2::TARGET_TYPE_MC);
            fapi2::ATTR_PG_Type l_pg;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));

            if (l_preserve_first_good && l_good_mcs.getBit(l_unit_num))
            {
                FAPI_DBG("Preserving MC%d", l_unit_num);
                l_preserve_first_good = false;
            }
            else if (l_garded_mcs.getBit(l_unit_num))
            {
                FAPI_DBG("Garding MC%d", l_unit_num);
                l_pg = 0xFFFFFFFF;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
        }
    }

    {
        // apply GARD information to PAU0..3, N1
        // track logical requirement to enable NMMU1 unit as processing PAU unit resources
        bool l_nmmu1_needed = false;

        // PAUC
        for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
                (fapi2::TARGET_FILTER_ALL_PAU),
                fapi2::TARGET_STATE_PRESENT))
        {
            uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num(l_perv, fapi2::TARGET_TYPE_PAUC);
            uint32_t l_gard_mask = 0x0;
            fapi2::ATTR_PG_Type l_pg;

            // extract bits associated with the pau logic units
            i_scratch2_reg.extractToRight<PAU_GARD_STARTBIT, PAU_GARD_LENGTH>(l_gard_mask);
            l_gard_mask = (l_gard_mask >> (NUM_PAUS_PER_PAUC * ((NUM_PAUC_PER_CHIP - 1) - l_unit_num))) & PAU_GARD_PAUC_MASK;

            // PAUC1 has pau3 in first unit bit position, swap bits
            if (l_unit_num == 1)
            {
                l_gard_mask = ((l_gard_mask & 0x1) << 1) |
                              ((l_gard_mask >> 1) & 0x1);
            }

            FAPI_DBG("PAU%d, chiplet gard mask: 0x%X, region gard mask: 0x%X",
                     l_unit_num,
                     i_scratch2_reg.getBit(static_cast<uint32_t>(PAUC_GARD_STARTBIT) + l_unit_num),
                     l_gard_mask);

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
            FAPI_DBG("  PG before: 0x%08X", l_pg);

            if (i_scratch2_reg.getBit(static_cast<uint32_t>(PAUC_GARD_STARTBIT) + l_unit_num))
            {
                l_pg = 0xFFFFFFFF;
            }
            else
            {
                l_pg |= (l_gard_mask << PAUC_PG_PAU_SHIFT);
            }

            // NMMU1 required if any of pau0,4,5 are enabled, make decision from final PAUC chiplet PG data
            // PAUC0 -> pau0
            // PAUC2 -> pau4/5
            if ((l_unit_num % 2) == 0)
            {
                if (((l_pg >> PAUC_PG_PAU_SHIFT) & PAU_GARD_PAUC_MASK) != PAU_GARD_PAUC_MASK)
                {
                    l_nmmu1_needed = true;
                    FAPI_DBG("  NMMU1 required based on PAUC%d chiplet", l_unit_num);
                }
            }

            FAPI_DBG("  PG after: 0x%08X", l_pg);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
        }

        // N1 - qualify NMM1 partial good state based on PAU logic unit state calculated above
        for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
                (fapi2::TARGET_FILTER_NEST_SOUTH),
                fapi2::TARGET_STATE_PRESENT))
        {
            fapi2::ATTR_PG_Type l_pg;

#ifndef __PPE__
            FAPI_DBG("N1, NMMU1 region %s required", (l_nmmu1_needed) ? ("") : ("NOT"));
#endif
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
            FAPI_DBG("  PG before: 0x%08X", l_pg);

            if (l_nmmu1_needed)
            {
                // ensure that NMM1 VPD partial good state supports requested PAU logic unit configuration
                FAPI_ASSERT(((l_pg >> N1_PG_NMMU1_SHIFT) & 0x1) == 0x0,
                            fapi2::P10_SBE_ATTR_SETUP_NMMU1_ERR()
                            .set_TARGET_CHIP(i_target_chip)
                            .set_N1_PG_MVPD(l_pg),
                            "NMMU1 unit is required by PAU unit config, but it is disabled in module VPD");
            }
            else
            {
                // NMMU1 not needed, ensure it's deconfigured
                l_pg |= (0x1 << N1_PG_NMMU1_SHIFT);
            }

            FAPI_DBG("  PG after: 0x%08X", l_pg);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
        }
    }

    // apply GARD information to IOHS0..7
    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_ALL_IOHS),
            fapi2::TARGET_STATE_PRESENT))
    {
        uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num(l_perv, fapi2::TARGET_TYPE_IOHS);
        fapi2::ATTR_PG_Type l_pg;
        FAPI_DBG("IOHS%d, chiplet gard mask: 0x%X",
                 l_unit_num,
                 i_scratch2_reg.getBit(static_cast<uint32_t>(IOHS_GARD_STARTBIT) + l_unit_num));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG before: 0x%08X", l_pg);

        if (i_scratch2_reg.getBit(static_cast<uint32_t>(IOHS_GARD_STARTBIT) + l_unit_num))
        {
            l_pg = 0xFFFFFFFF;
        }

        FAPI_DBG("  PG after: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

///
/// @brief Update platform ATTR_PG attribute for EQ chiplets, in a fused core mode.
///        If only one core is functional in fused mode, SBE will gard that core.
///        eg. If first core is functional and second core is non-functional
///            gard both the cores.
///
/// @param[in] i_target_chip  Processor chip target
///
/// @return fapi2::ReturnCode
///

fapi2::ReturnCode
p10_sbe_update_eq_pg_for_fusedcore(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_DBG("Entering ...");
    fapi2::ATTR_FUSED_CORE_MODE_Type fused_core_mode;
    fapi2::buffer<uint32_t> temp_pg = 0;
    fapi2::buffer<uint64_t> scratch1_reg = 0;

    FAPI_TRY(fapi2::getScom(i_target_chip,
                            scomt::perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_1_RW, scratch1_reg),
             "Error reading Scratch 1 mailbox register");
    FAPI_DBG("Scratch Reg1 [0x%08X]", ((scratch1_reg >> 32) & 0xFFFFFFFF));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE,
                           fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(), fused_core_mode),
             "Error from FAPI_ATTR_GET (ATTR_FUSED_CORE_MODE)");
    FAPI_DBG("Fused core mode: 0x%08X", fused_core_mode);

    if(fused_core_mode)
    {
        for (const auto& perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
             (static_cast<fapi2::TargetFilter>
              (fapi2::TARGET_FILTER_ALL_EQ),
              fapi2::TARGET_STATE_PRESENT))
        {
            fapi2::ATTR_PG_Type pg;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, perv, pg));
            uint8_t unit_num = p10_sbe_scratch_regs_get_unit_num(perv, fapi2::TARGET_TYPE_EQ);
            FAPI_DBG("Eq[%d] PG before: 0x%08X", unit_num, pg);
            temp_pg = pg;

            for(uint8_t core = 0; core < NUM_CORES_PER_EQ - 1; core = core + 2)
            {
                // Check if any of the even(0)/odd(1) pair is bad then both
                // core0 (even) & core1 (odd) goes bad. Same goes for core2 & core3 pair
                if(temp_pg.getBit(core + CORE0_ECL2_PG_BIT) || temp_pg.getBit(core + CORE0_ECL2_PG_BIT + 1))
                {
                    temp_pg.setBit(core + CORE0_ECL2_PG_BIT); //core0/2 - ecl2 bit
                    temp_pg.setBit(core + CORE0_ECL2_PG_BIT + 1); // core1/3 - ecl2 bit
                    temp_pg.setBit(core + CORE0_L3_PG_BIT); //core0/2 - l3 bit
                    temp_pg.setBit(core + CORE0_L3_PG_BIT + 1); //core1/3 - l3 bit
                    temp_pg.setBit(core + CORE0_MMA_PG_BIT); //core0/2 - mma bit
                    temp_pg.setBit(core + CORE0_MMA_PG_BIT + 1); //core1/3 - mma bit
                    // Set the Scratch Bit - (EQ * 4 + Core0/1/2/3)
                    scratch1_reg.setBit(unit_num * NUM_CORES_PER_EQ + core);
                    scratch1_reg.setBit(unit_num * NUM_CORES_PER_EQ + core + 1);
                }
            }

            pg = temp_pg;
            FAPI_DBG("Eq[%d] PG after: [0x%08X] ", unit_num, pg);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, perv, pg));
        }

        FAPI_DBG("Updated Scratch Reg1 [0x%08X]", ((scratch1_reg >> 32) & 0xFFFFFFFF));
        FAPI_TRY(fapi2::putScom(i_target_chip,
                                scomt::perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_1_RW, scratch1_reg),
                 "Error writing Scratch 1 mailbox register");
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

fapi2::ReturnCode
p10_sbe_scratch_regs_resource_recovery_flow(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_DBG("Start");

    fapi2::ATTR_ACTIVE_CORES_NUM_Type l_attr_num_active = 0;
    fapi2::ATTR_ACTIVE_CORES_NUM_Type l_attr_num_backing = 0;
    fapi2::Target<fapi2::TARGET_TYPE_EQ> l_eq_target;
    uint32_t l_num_active = 0;
    uint32_t l_num_backing = 0;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_CONTAINED_IPL_TYPE_Type l_attr_contained_ipl_type;
    uint32_t l_fused_core_num_base = 0;
    bool b_fused = false;
    bool b_fused_first_half = false;

    auto l_core_functional_vector = i_target_chip.getChildren<fapi2::TARGET_TYPE_CORE>
                                    (fapi2::TARGET_STATE_FUNCTIONAL);

    fapi2::ATTR_FUSED_CORE_MODE_Type l_attr_fused_mode;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE,
                           FAPI_SYSTEM,
                           l_attr_fused_mode));

    if (l_attr_fused_mode == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_FUSED)
    {
        b_fused = true;
        FAPI_DBG("Fused core mode detected");
    }

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, FAPI_SYSTEM, l_attr_contained_ipl_type),
             "Error from FAPI_ATTR_GET (ATTR_CONTAINED_IPL_TYPE)");

    // Compute the number of active cores/backing caches need to be configured
    FAPI_TRY(p10_perv_sbe_cmn_min_active_backing_nums(i_target_chip, l_core_functional_vector,
             l_attr_num_active, l_attr_num_backing),
             "Error from p10_perv_sbe_cmn_min_active_backing_nums");

    for (auto const& core : l_core_functional_vector)
    {
        fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = 0;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               core,
                               l_attr_chip_unit_pos));

        uint32_t l_core_num  = (uint32_t)l_attr_chip_unit_pos;

        l_eq_target = core.getParent<fapi2::TARGET_TYPE_EQ>();

        fapi2::ATTR_ECO_MODE_Type l_eco_mode;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ECO_MODE, core, l_eco_mode));

        bool b_skip_active = l_eco_mode;
        bool b_skip_backing = l_eco_mode && (l_attr_contained_ipl_type !=
                                             fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CHIP);

        if (b_fused)
        {
            if (b_fused_first_half)
            {
                // Check that the second normal core is functional.
                FAPI_DBG("Odd core check: base core == %d, core num = %d",
                         l_fused_core_num_base, l_core_num);
                FAPI_ASSERT(l_core_num == l_fused_core_num_base + 1,
                            fapi2::SBE_SELECT_EX_FUSED_ODD_ERROR()
                            .set_CHIP(i_target_chip)
                            .set_CORE_NUM(l_core_num)
                            .set_FUSED_CORE_NUM_BASE(l_fused_core_num_base),
                            "Odd core within fused set is not functional");
                b_fused_first_half = false;
            }
            else
            {
                // Check that the first normal core is even.
                FAPI_ASSERT(l_core_num % 2 == 0,
                            fapi2::SBE_SELECT_EX_FUSED_NOT_EVEN_ERROR()
                            .set_CHIP(i_target_chip)
                            .set_CORE_NUM(l_core_num),
                            "The first core found in fused mode was not an even core");
                b_fused_first_half = true;
                l_fused_core_num_base = l_core_num;
            }
        }

        // Skip cache only cores in trying to fullfil the active cores
        if ((l_num_active < l_attr_num_active) && !b_skip_active)
        {
            ++l_num_active;
            continue;  // next core
        }

        if ((l_num_backing < l_attr_num_backing) && !b_skip_backing)
        {
            ++l_num_backing;
            continue;  // next core
        }
    }

    if((l_num_active != l_attr_num_active) || (l_num_backing != l_attr_num_backing))
    {
        // Minimum config of cores to boot the system is not met.
        // In this case SBE will rely on fallBackCoreAttr.

        // Read the attribute.
        fapi2::ATTR_HB_FALLBACK_CORES_Type fallBackCoreAttr = 0;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_HB_FALLBACK_CORES, i_target_chip, fallBackCoreAttr));

        if(fallBackCoreAttr == 0xFFFFFFFF)
        {
            // fallBackCoreAttr is not set. This can be first IPL.
            // So, SBE will update the PG attribute for EQ based on PG_MVPD.

            FAPI_TRY(p10_sbe_attr_update_eq_pg_from_mvpd(i_target_chip),
                     "Error from p10_sbe_attr_update_eq_pg_from_mvpd");
        }
        else
        {
            // Not first IPL.
            // fallBackCoreAttr is set by HB.
            // SBE will not use the scratch reg 2838. Instead it will use fallBackCoreAttr
            // to update ATTR_PG for EQ chiplets.

            FAPI_TRY(p10_sbe_scratch_regs_write_eq_pg_from_scratch(i_target_chip,
                     fallBackCoreAttr),
                     "Error from p10_sbe_scratch_regs_write_eq_pg_from_scratch");
        }

        FAPI_TRY(p10_sbe_update_eq_pg_for_fusedcore(i_target_chip),
                 "Error from p10_sbe_update_eq_pg_for_fusedcore");
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

// description in header
fapi2::ReturnCode p10_sbe_attr_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt::perv;

    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::buffer<uint64_t> l_read_scratch8_reg = 0;
    fapi2::ATTR_SYSTEM_IPL_PHASE_Type l_attr_system_ipl_phase = fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL;
    fapi2::ATTR_CONTAINED_IPL_TYPE_Type l_attr_contained_ipl_type = fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE;

    FAPI_INF("p10_sbe_attr_setup: Entering ...");

    // set core LPAR mode, confirm fused core mode
    {
        fapi2::ATTR_CORE_LPAR_MODE_Type l_attr_core_lpar_mode = fapi2::ENUM_ATTR_CORE_LPAR_MODE_LPAR_PER_THREAD;
        fapi2::buffer<uint64_t> l_perv_ctrl0;

        FAPI_DBG("Read PERV_CTRL0 to set core LPAR mode");
        FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_PERV_CTRL0_RW, l_perv_ctrl0));

        l_attr_core_lpar_mode = l_perv_ctrl0.getBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_EX_SINGLE_LPAR_EN_DC>();

        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CORE_LPAR_MODE, FAPI_SYSTEM, l_attr_core_lpar_mode));
    }

    FAPI_DBG("Read Scratch8 for validity of Scratch register");
    FAPI_TRY(fapi2::getScom(i_target_chip,
                            FSXCOMP_FSXLOG_SCRATCH_REGISTER_8_RW,
                            l_read_scratch8_reg),
             "Error reading Scratch 8 mailbox register");

    // parse and post selected attributes which drive behavior of other mailbox parsing
    {
        if (l_read_scratch8_reg.getBit<SCRATCH5_REG_VALID_BIT>())
        {
            fapi2::buffer<uint64_t> l_read_scratch5_reg = 0;
            uint8_t l_ipl_type = 0x0;

            FAPI_DBG("Reading Scratch 5 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_5_RW, l_read_scratch5_reg),
                     "Error reading Scratch 5 mailbox register");

            FAPI_DBG("Setting up ATTR_SYSTEM_IPL_PHASE, ATTR_CONTAINED_IPL_TYPE");
            l_read_scratch5_reg.extractToRight<IPL_TYPE_STARTBIT, IPL_TYPE_LENGTH>(l_ipl_type);

            if (l_ipl_type == IPL_TYPE_CACHE_CONTAINED)
            {
                l_attr_contained_ipl_type = fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CACHE;
            }
            else if (l_ipl_type == IPL_TYPE_CHIP_CONTAINED)
            {
                l_attr_contained_ipl_type = fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CHIP;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_attr_system_ipl_phase),
                     "Error from FAPI_ATTR_SET (ATTR_SYSTEM_IPL_PHASE)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CONTAINED_IPL_TYPE, FAPI_SYSTEM, l_attr_contained_ipl_type),
                     "Error from FAPI_ATTR_SET (ATTR_CONTAINED_IPL_TYPE");
        }

        if (l_read_scratch8_reg.getBit<SCRATCH6_REG_VALID_BIT>())
        {
            fapi2::buffer<uint64_t> l_read_scratch6_reg = 0;
            fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type l_attr_proc_sbe_master_chip = fapi2::ENUM_ATTR_PROC_SBE_MASTER_CHIP_FALSE;

            FAPI_DBG("Reading Scratch 6 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_6_RW, l_read_scratch6_reg),
                     "Error reading Scratch 6 mailbox register");

            FAPI_DBG("Setting up ATTR_PROC_SBE_MASTER_CHIP");

            if (l_read_scratch6_reg.getBit<ATTR_PROC_SBE_MASTER_CHIP_BIT>())
            {
                l_attr_proc_sbe_master_chip = fapi2::ENUM_ATTR_PROC_SBE_MASTER_CHIP_TRUE;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_SBE_MASTER_CHIP, i_target_chip, l_attr_proc_sbe_master_chip),
                     "Error from FAPI_ATTR_SET (ATTR_PROC_SBE_MASTER_CHIP)");
        }

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_attr_system_ipl_phase),
                 "Error from FAPI_ATTR_GET (ATTR_SYSTEM_IPL_PHASE)");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, FAPI_SYSTEM, l_attr_contained_ipl_type),
                 "Error from FAPI_ATTR_GET (ATTR_CONTAINED_IPL_TYPE)");
    }

    // read_cbs_cs_reg -- Security_Access_Bit
    {
        fapi2::buffer<uint64_t> l_read_cbs_cs_reg;
        fapi2::buffer<uint64_t> l_read_scratch3_reg;
        BootloaderSecureSettings l_secure_settings;
        fapi2::buffer<uint64_t> l_data64;
        uint8_t l_security_mode = 0; // Read from Ppe LFR Register
        fapi2::ATTR_SECURITY_ENABLE_Type l_attr_security_enable = fapi2::ENUM_ATTR_SECURITY_ENABLE_ENABLE;

        if (fapi2::is_platform<fapi2::PLAT_SBE>())
        {
            // LFR Register Bit 19 is updated with Security Mode from Measurment
            // Seeprom execution
            FAPI_DBG("Reading Bit 19 from SBE LFR Register");
            FAPI_TRY(fapi2::getScom(i_target_chip, SBE_LFR_REG, l_data64),
                     "Error reading SBE LFR Register");
            l_security_mode = l_data64.getBit<19>();
        }

        // Transfer to SBE Security Backdoor Bit, Need to invert the bit since
        //////// 1 == Secure mode == Backdoor disabled
        //////// 0 == Unsecure mode == Backdoor enabled
        l_secure_settings.secBackdoorBit = !l_security_mode;

        FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_CBS_CS, l_read_cbs_cs_reg),
                 "Error reading CBS Control/Status register");

        if(l_read_cbs_cs_reg.getBit<5>() == 0)
        {
            if( !(l_security_mode) && (l_read_scratch8_reg.getBit<SCRATCH3_REG_VALID_BIT>()) )
            {
                FAPI_DBG("Reading mailbox scratch register3 bit6 to check for"
                         " external security override request");
                FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_3_RW, l_read_scratch3_reg),
                         "Error reading Scratch 3 mailbox register");
                FAPI_DBG("Copying mailbox scratch register 3 bits 6,7 to ATTR_SECURE_SETTINGS");
                l_secure_settings.securityOverride = l_read_scratch3_reg.getBit<6>();
                l_secure_settings.allowAttrOverrides = l_read_scratch3_reg.getBit<7>();
            }
        }

        FAPI_DBG("Setting up ATTR_SECURITY_ENABLE with SAB state");

        if (!l_read_cbs_cs_reg.getBit<FSXCOMP_FSXLOG_CBS_CS_SECURE_ACCESS_BIT>())
        {
            l_attr_security_enable = fapi2::ENUM_ATTR_SECURITY_ENABLE_DISABLE;
        }

        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SECURITY_ENABLE, FAPI_SYSTEM, l_attr_security_enable),
                 "Error from FAPI_ATTR_SET (ATTR_SECURITY_ENABLE");
        // Include the Secure Access Bit now
        l_secure_settings.secureAccessBit = l_read_cbs_cs_reg.getBit<FSXCOMP_FSXLOG_CBS_CS_SECURE_ACCESS_BIT>();
        FAPI_DBG("Setting up ATTR_SECURITY_SETTINGS");
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SECURE_SETTINGS, FAPI_SYSTEM, l_secure_settings.data8));
    }

    // populate scratch registers from attribute state/targeting model, if not set by setup_sbe_config
    FAPI_TRY(p10_sbe_scratch_regs_update(i_target_chip, false, true));

    // read_scratch1_reg -- set EQ chiplet PG
    {
        fapi2::buffer<uint64_t> l_read_scratch1_reg = 0;

        if (l_read_scratch8_reg.getBit<SCRATCH1_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 1 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_1_RW, l_read_scratch1_reg),
                     "Error reading Scratch 1 mailbox register");
        }

        FAPI_TRY(p10_sbe_scratch_regs_write_eq_pg_from_scratch(i_target_chip,
                 l_read_scratch1_reg),
                 "Error from p10_sbe_scratch_regs_write_eq_pg_from_scratch");

        FAPI_TRY(p10_sbe_update_eq_pg_for_fusedcore(i_target_chip),
                 "Error from p10_sbe_update_eq_pg_for_fusedcore");

        for (const auto& l_core_target : i_target_chip.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL))
        {
#ifdef __PPE__
            // At this point the SBE core targets are still blue print and are all
            // functional, Cores targets and other targets are updated once we exit
            // from the procedure. Earlier there were no use-case of working with
            // functional targets in this procedure, it was only using presence
            // target, with this requirement change, we have to update SBE Core
            // Targeting to reflect the right functional core target. That's not
            // possible when are you still executing in the procedure, so we have
            // to depend of ATTR_PG_bit
            fapi2::buffer<fapi2::ATTR_PG_Type> eqPg;
            fapi2::ATTR_CHIP_UNIT_POS_Type core_num = 0;
            const auto eq = l_core_target.getParent<fapi2::TARGET_TYPE_EQ>();
            const auto perv = eq.getParent<fapi2::TARGET_TYPE_PERV>();
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, perv, eqPg));
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_core_target, core_num),
                     "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

            //Find out if the ECL2 is zero for functional core target
            if(eqPg.getBit(CORE0_ECL2_PG_BIT + (core_num % NUM_CORES_PER_EQ)))
            {
                // If functional then only pass it onto p10_determine_eco_mode
                continue;
            }

#endif
            FAPI_TRY(p10_determine_eco_mode(l_core_target),
                     "Error from p10_determine_eco_mode");
        }
    }

    // read_scratch2_reg -- set TP/N0/N1/PCI/MC/PAU/IOHS chiplet PG
    // read_scratch7_reg -- set IOHS region PG / chip contained active cores vector
    {
        fapi2::buffer<uint64_t> l_read_scratch2_reg = 0;
        fapi2::buffer<uint64_t> l_read_scratch7_reg = 0;

        if (l_read_scratch8_reg.getBit<SCRATCH2_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 2 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_2_RW, l_read_scratch2_reg),
                     "Error reading Scratch 2 mailbox register");
        }

        if (l_read_scratch8_reg.getBit<SCRATCH7_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 7 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_7_RW, l_read_scratch7_reg),
                     "Error reading Scratch 7 mailbox register");
        }

        FAPI_TRY(p10_sbe_scratch_regs_write_noneq_pg_from_scratch(i_target_chip,
                 l_read_scratch2_reg,
                 l_read_scratch7_reg),
                 "Error from p10_sbe_scratch_regs_write_noneq_pg_from_scratch");

        if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CHIP)
        {
            fapi2::ATTR_CHIP_CONTAINED_ACTIVE_CORES_VEC_Type l_attr_chip_contained_active_cores_vec;

            FAPI_DBG("Setting up chip contained active cores vector");
            l_read_scratch7_reg.extractToRight<ATTR_CHIP_CONTAINED_ACTIVE_CORES_VEC_STARTBIT, ATTR_CHIP_CONTAINED_ACTIVE_CORES_VEC_LENGTH>
            (l_attr_chip_contained_active_cores_vec);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CHIP_CONTAINED_ACTIVE_CORES_VEC, i_target_chip,
                                   l_attr_chip_contained_active_cores_vec),
                     "Error from FAPI_ATTR_SET (ATTR_CHIP_CONTAINED_ACTIVE_CORES_VEC)");
        }
    }

    // read_scratch3_reg -- FW Mode/Control flags
    {
        fapi2::buffer<uint64_t> l_read_scratch3_reg = 0;
        fapi2::ATTR_BOOT_FLAGS_Type l_attr_boot_flags = 0;

        if (l_read_scratch8_reg.getBit<SCRATCH3_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 3 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_3_RW, l_read_scratch3_reg),
                     "Error reading Scratch 3 mailbox register");

            FAPI_DBG("Setting up ATTR_BOOT_FLAGS");
            l_read_scratch3_reg.extract<ATTR_BOOT_FLAGS_STARTBIT, ATTR_BOOT_FLAGS_LENGTH>(l_attr_boot_flags);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_BOOT_FLAGS, FAPI_SYSTEM, l_attr_boot_flags),
                     "Error from FAPI_ATTR_SET (ATTR_BOOT_FLAGS)");
        }
    }

    // read_scratch4_reg -- Nest/Boot frequency
    {
        fapi2::buffer<uint64_t> l_read_scratch4_reg = 0;
        fapi2::ATTR_SPI_BUS_DIV_REF_Type l_attr_spi_bus_div_ref = 0;
        fapi2::ATTR_FREQ_CORE_BOOT_MHZ_Type l_attr_freq_core_boot_mhz = 0;

        if (l_read_scratch8_reg.getBit<SCRATCH4_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 4 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_4_RW, l_read_scratch4_reg),
                     "Error reading Scrach 4 mailbox register");

            FAPI_DBG("Setting up ATTR_SPI_BUS_DIV_REF");
            l_read_scratch4_reg.extract<ATTR_SPI_BUS_DIV_REF_STARTBIT, ATTR_SPI_BUS_DIV_REF_LENGTH>(l_attr_spi_bus_div_ref);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SPI_BUS_DIV_REF, i_target_chip, l_attr_spi_bus_div_ref),
                     "Error from FAPI_ATTR_SET (ATTR_SPI_BUS_DIV_REF)");

            FAPI_DBG("Setting up ATTR_FREQ_CORE_BOOT_MHZ");
            l_read_scratch4_reg.extractToRight<ATTR_FREQ_CORE_BOOT_MHZ_STARTBIT, ATTR_FREQ_CORE_BOOT_MHZ_LENGTH>
            (l_attr_freq_core_boot_mhz);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_FREQ_CORE_BOOT_MHZ, i_target_chip, l_attr_freq_core_boot_mhz),
                     "Error from FAPI_ATTR_SET (ATTR_FREQ_CORE_BOOT_MHZ");
        }
    }

    // read_scratch5_reg -- HWP control flags/PLL muxes
    {
        fapi2::buffer<uint64_t> l_read_scratch5_reg = 0;
        fapi2::ATTR_RUNN_MODE_Type l_attr_runn_mode = fapi2::ENUM_ATTR_RUNN_MODE_OFF;
        fapi2::ATTR_DISABLE_HBBL_VECTORS_Type l_attr_disable_hbbl_vectors = fapi2::ENUM_ATTR_DISABLE_HBBL_VECTORS_FALSE;
        fapi2::ATTR_SBE_SELECT_EX_POLICY_Type l_attr_sbe_select_ex_policy = fapi2::ENUM_ATTR_SBE_SELECT_EX_POLICY_HB_DEFAULT;
        fapi2::ATTR_CLOCKSTOP_ON_XSTOP_Type l_attr_clockstop_on_xstop = fapi2::ENUM_ATTR_CLOCKSTOP_ON_XSTOP_DISABLED;
        fapi2::ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT_Type l_attr_clock_mux_iohs_lcpll_input = { 0 };
        fapi2::ATTR_CLOCK_MUX_PCI_LCPLL_INPUT_Type l_attr_clock_mux_pci_lcpll_input = { 0 };
        fapi2::ATTR_CONTAINED_LOAD_PATH_Type l_attr_contained_load_path = fapi2::ENUM_ATTR_CONTAINED_LOAD_PATH_PBA;
        uint8_t l_clockstop_on_xstop = ATTR_CLOCKSTOP_ON_XSTOP_DISABLED;

        if (l_read_scratch8_reg.getBit<SCRATCH5_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 5 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_5_RW, l_read_scratch5_reg),
                     "Error reading Scratch 5 mailbox register");

            FAPI_DBG("Setting up ATTR_RUNN_MDOE");

            if (l_read_scratch5_reg.getBit<ATTR_RUNN_MODE_BIT>())
            {
                l_attr_runn_mode = fapi2::ENUM_ATTR_RUNN_MODE_ON;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_RUNN_MODE, FAPI_SYSTEM, l_attr_runn_mode),
                     "Error from FAPI_ATTR_SET (ATTR_RUNN_MODE)");

            FAPI_DBG("Setting up ATTR_DISABLE_HBBL_VECTORS");

            if (l_read_scratch5_reg.getBit<ATTR_DISABLE_HBBL_VECTORS_BIT>())
            {
                l_attr_disable_hbbl_vectors = fapi2::ENUM_ATTR_DISABLE_HBBL_VECTORS_TRUE;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_DISABLE_HBBL_VECTORS, FAPI_SYSTEM, l_attr_disable_hbbl_vectors),
                     "Error from FAPI_ATTR_SET (ATTR_DISABLE_HBBL_VECTORS)");

            FAPI_DBG("Setting up ATTR_SBE_SELECT_EX_POLICY");
            l_read_scratch5_reg.extractToRight<ATTR_SBE_SELECT_EX_POLICY_STARTBIT, ATTR_SBE_SELECT_EX_POLICY_LENGTH>
            (l_attr_sbe_select_ex_policy);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SBE_SELECT_EX_POLICY, FAPI_SYSTEM, l_attr_sbe_select_ex_policy),
                     "Error from FAPI_ATTR_SET (ATTR_SBE_SELECT_EX_POLICY)");

            FAPI_DBG("Setting up ATTR_CLOCKSTOP_ON_XSTOP");
            l_read_scratch5_reg.extractToRight<ATTR_CLOCKSTOP_ON_XSTOP_STARTBIT, ATTR_CLOCKSTOP_ON_XSTOP_LENGTH>
            (l_clockstop_on_xstop);

            if (l_clockstop_on_xstop == ATTR_CLOCKSTOP_ON_XSTOP_XSTOP)
            {
                l_attr_clockstop_on_xstop = fapi2::ENUM_ATTR_CLOCKSTOP_ON_XSTOP_STOP_ON_XSTOP;
            }
            else if (l_clockstop_on_xstop == ATTR_CLOCKSTOP_ON_XSTOP_XSTOP_SPATTN)
            {
                l_attr_clockstop_on_xstop = fapi2::ENUM_ATTR_CLOCKSTOP_ON_XSTOP_STOP_ON_XSTOP_AND_SPATTN;
            }
            else if (l_clockstop_on_xstop == ATTR_CLOCKSTOP_ON_XSTOP_STAGED_XSTOP)
            {
                l_attr_clockstop_on_xstop = fapi2::ENUM_ATTR_CLOCKSTOP_ON_XSTOP_STOP_ON_STAGED_XSTOP;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CLOCKSTOP_ON_XSTOP, i_target_chip, l_attr_clockstop_on_xstop),
                     "Error from FAPI_ATTR_SET (ATTR_CLOCKSTOP_ON_XSTOP)");


            if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE)
            {
                FAPI_DBG("Setting up IOHS PLL mux attributes");

                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT, i_target_chip, l_attr_clock_mux_iohs_lcpll_input),
                         "Error from FAPI_ATTR_GET (ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT)");

                for (const auto& l_perv_iohs_target :  i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                         static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_IOHS),
                         fapi2::TARGET_STATE_FUNCTIONAL))
                {
                    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = p10_sbe_scratch_regs_get_unit_num(l_perv_iohs_target,
                            fapi2::TARGET_TYPE_IOHS);

                    FAPI_TRY(l_read_scratch5_reg.extractToRight(l_attr_clock_mux_iohs_lcpll_input[l_attr_chip_unit_pos],
                             ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT_STARTBIT +
                             (ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT_LENGTH * l_attr_chip_unit_pos),
                             ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT_LENGTH));
                }

                FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT, i_target_chip, l_attr_clock_mux_iohs_lcpll_input),
                         "Error from FAPI_ATTR_SET (ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT)");

                FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CLOCK_MUX_PCI_LCPLL_INPUT, i_target_chip, l_attr_clock_mux_pci_lcpll_input),
                         "Error from FAPI_ATTR_GET (ATTR_CLOCK_MUX_PCI_LCPLL_INPUT)");

                for (const auto& l_perv_pci_target : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                         static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_PCI),
                         fapi2::TARGET_STATE_FUNCTIONAL))
                {
                    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = p10_sbe_scratch_regs_get_unit_num(l_perv_pci_target,
                            fapi2::TARGET_TYPE_PEC);

                    FAPI_TRY(l_read_scratch5_reg.extractToRight(l_attr_clock_mux_pci_lcpll_input[l_attr_chip_unit_pos],
                             ATTR_CLOCK_MUX_PCI_LCPLL_INPUT_STARTBIT +
                             (ATTR_CLOCK_MUX_PCI_LCPLL_INPUT_LENGTH * l_attr_chip_unit_pos),
                             ATTR_CLOCK_MUX_PCI_LCPLL_INPUT_LENGTH));
                }

                FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CLOCK_MUX_PCI_LCPLL_INPUT, i_target_chip, l_attr_clock_mux_pci_lcpll_input),
                         "Error from FAPI_ATTR_SET (ATTR_CLOCK_MUX_PCI_LCPLL_INPUT)");
            }

            FAPI_DBG("Setting up ATTR_CONTAINED_LOAD_PATH");

            if (l_read_scratch5_reg.getBit<ATTR_CONTAINED_LOAD_PATH_BIT>())
            {
                l_attr_contained_load_path = fapi2::ENUM_ATTR_CONTAINED_LOAD_PATH_L2SQ;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CONTAINED_LOAD_PATH, FAPI_SYSTEM, l_attr_contained_load_path),
                     "Error from FAPI_ATTR_SET (ATTR_CONTAINED_LOAD_PATH)");
        }
    }

    // read_scratch6_reg -- Master/slave, node/chip selection, PLL bypass controls
    {
        fapi2::buffer<uint64_t> l_read_scratch6_reg = 0;
        fapi2::ATTR_FILTER_PLL_BUCKET_Type l_attr_filter_pll_bucket = 0;
        fapi2::ATTR_PCI_PLL_BUCKET_Type l_attr_pci_pll_bucket = 0;
        fapi2::ATTR_CP_REFCLOCK_SELECT_Type l_attr_cp_refclock_select = fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0;
        fapi2::ATTR_SKEWADJ_BYPASS_Type l_attr_skewadj_bypass = fapi2::ENUM_ATTR_SKEWADJ_BYPASS_NO_BYPASS;
        fapi2::ATTR_DCADJ_BYPASS_Type l_attr_dcadj_bypass = fapi2::ENUM_ATTR_DCADJ_BYPASS_NO_BYPASS;
        fapi2::ATTR_CP_PLLTODFLT_BYPASS_Type l_attr_cp_plltodflt_bypass = fapi2::ENUM_ATTR_CP_PLLTODFLT_BYPASS_NO_BYPASS;
        fapi2::ATTR_CP_PLLNESTFLT_BYPASS_Type l_attr_cp_pllnestflt_bypass = fapi2::ENUM_ATTR_CP_PLLNESTFLT_BYPASS_NO_BYPASS;
        fapi2::ATTR_CP_PLLIOFLT_BYPASS_Type l_attr_cp_pllioflt_bypass = fapi2::ENUM_ATTR_CP_PLLIOFLT_BYPASS_NO_BYPASS;
        fapi2::ATTR_CP_PLLIOSSFLT_BYPASS_Type l_attr_cp_plliossflt_bypass = fapi2::ENUM_ATTR_CP_PLLIOSSFLT_BYPASS_NO_BYPASS;
        fapi2::ATTR_NEST_DPLL_BYPASS_Type l_attr_nest_dpll_bypass = fapi2::ENUM_ATTR_NEST_DPLL_BYPASS_NO_BYPASS;
        fapi2::ATTR_PAU_DPLL_BYPASS_Type l_attr_pau_dpll_bypass = fapi2::ENUM_ATTR_PAU_DPLL_BYPASS_NO_BYPASS;
        fapi2::ATTR_BOOT_PAU_DPLL_BYPASS_Type l_attr_boot_pau_dpll_bypass = fapi2::ENUM_ATTR_BOOT_PAU_DPLL_BYPASS_NO_BYPASS;
        fapi2::ATTR_IO_TANK_PLL_BYPASS_Type l_attr_io_tank_pll_bypass = fapi2::ENUM_ATTR_IO_TANK_PLL_BYPASS_NO_BYPASS;
        fapi2::ATTR_PROC_FABRIC_EFF_TOPOLOGY_ID_Type l_attr_proc_fabric_eff_topology_id = 0;
        fapi2::ATTR_PROC_FABRIC_TOPOLOGY_MODE_Type l_attr_proc_fabric_topology_mode =
            fapi2::ENUM_ATTR_PROC_FABRIC_TOPOLOGY_MODE_MODE0;
        fapi2::ATTR_PROC_FABRIC_BROADCAST_MODE_Type l_attr_proc_fabric_broadcast_mode =
            fapi2::ENUM_ATTR_PROC_FABRIC_BROADCAST_MODE_1HOP_CHIP_IS_GROUP;
        fapi2::ATTR_PROC_FABRIC_TOPOLOGY_ID_Type l_attr_proc_fabric_topology_id = 0;

        if (l_read_scratch8_reg.getBit<SCRATCH6_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 6 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_6_RW, l_read_scratch6_reg),
                     "Error reading Scratch 6 mailbox register");

            FAPI_DBG("Setting up filter PLL bucket attribute");
            FAPI_TRY(l_read_scratch6_reg.extractToRight(l_attr_filter_pll_bucket,
                     ATTR_FILTER_PLL_BUCKET_STARTBIT,
                     ATTR_FILTER_PLL_BUCKET_LENGTH));

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_FILTER_PLL_BUCKET, i_target_chip, l_attr_filter_pll_bucket),
                     "Error from FAPI_ATTR_SET (ATTR_FILTER_PLL_BUCKET)");

            FAPI_DBG("Setting up PCI PLL bucket attribute");
            FAPI_TRY(l_read_scratch6_reg.extractToRight(l_attr_pci_pll_bucket,
                     ATTR_PCI_PLL_BUCKET_STARTBIT,
                     ATTR_PCI_PLL_BUCKET_LENGTH));

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PCI_PLL_BUCKET, i_target_chip, l_attr_pci_pll_bucket),
                     "Error from FAPI_ATTR_SET (ATTR_PCI_PLL_BUCKET)");

            FAPI_DBG("Setting up refclock select value");
            l_read_scratch6_reg.extractToRight<ATTR_CP_REFCLOCK_SELECT_STARTBIT, ATTR_CP_REFCLOCK_SELECT_LENGTH>
            (l_attr_cp_refclock_select);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CP_REFCLOCK_SELECT, i_target_chip, l_attr_cp_refclock_select),
                     "Error from FAPI_ATTR_SET (ATTR_CP_REFCLOCK_SELECT)");

            FAPI_DBG("Setting up skew adjust/duty cycle adjust bypass attributes");

            if (l_read_scratch6_reg.getBit<ATTR_SKEWADJ_BYPASS_BIT>())
            {
                l_attr_skewadj_bypass = fapi2::ENUM_ATTR_SKEWADJ_BYPASS_BYPASS;
            }

            if (l_read_scratch6_reg.getBit<ATTR_DCADJ_BYPASS_BIT>())
            {
                l_attr_dcadj_bypass = fapi2::ENUM_ATTR_DCADJ_BYPASS_BYPASS;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SKEWADJ_BYPASS, i_target_chip, l_attr_skewadj_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_SKEWADJ_BYPASS)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_DCADJ_BYPASS, i_target_chip, l_attr_dcadj_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_DCADJ_BYPASS)");

            FAPI_DBG("Setting up filter PLL bypass attributes");

            if (l_read_scratch6_reg.getBit<ATTR_CP_PLLTODFLT_BYPASS_BIT>())
            {
                l_attr_cp_plltodflt_bypass = fapi2::ENUM_ATTR_CP_PLLTODFLT_BYPASS_BYPASS;
            }

            if (l_read_scratch6_reg.getBit<ATTR_CP_PLLNESTFLT_BYPASS_BIT>())
            {
                l_attr_cp_pllnestflt_bypass = fapi2::ENUM_ATTR_CP_PLLNESTFLT_BYPASS_BYPASS;
            }

            if (l_read_scratch6_reg.getBit<ATTR_CP_PLLIOFLT_BYPASS_BIT>())
            {
                l_attr_cp_pllioflt_bypass = fapi2::ENUM_ATTR_CP_PLLIOFLT_BYPASS_BYPASS;
            }

            if (l_read_scratch6_reg.getBit<ATTR_CP_PLLIOSSFLT_BYPASS_BIT>())
            {
                l_attr_cp_plliossflt_bypass = fapi2::ENUM_ATTR_CP_PLLIOSSFLT_BYPASS_BYPASS;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CP_PLLTODFLT_BYPASS, i_target_chip, l_attr_cp_plltodflt_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_CP_PLLTODFLT_BYPASS)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CP_PLLNESTFLT_BYPASS, i_target_chip, l_attr_cp_pllnestflt_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_CP_PLLNESTFLT_BYPASS)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CP_PLLIOFLT_BYPASS, i_target_chip, l_attr_cp_pllioflt_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_CP_PLLIOFLT_BYPASS)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CP_PLLIOSSFLT_BYPASS, i_target_chip, l_attr_cp_plliossflt_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_CP_PLLIOSSFLT_BYPASS)");

            FAPI_DBG("Setting up DPLL bypass attributes");

            if (l_read_scratch6_reg.getBit<ATTR_NEST_DPLL_BYPASS_BIT>())
            {
                l_attr_nest_dpll_bypass = fapi2::ENUM_ATTR_NEST_DPLL_BYPASS_BYPASS;
            }

            if (l_read_scratch6_reg.getBit<ATTR_PAU_DPLL_BYPASS_BIT>())
            {
                l_attr_pau_dpll_bypass = fapi2::ENUM_ATTR_PAU_DPLL_BYPASS_BYPASS;
            }

            if (l_read_scratch6_reg.getBit<ATTR_BOOT_PAU_DPLL_BYPASS_BIT>())
            {
                l_attr_boot_pau_dpll_bypass = fapi2::ENUM_ATTR_BOOT_PAU_DPLL_BYPASS_BYPASS;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NEST_DPLL_BYPASS, i_target_chip, l_attr_nest_dpll_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_NEST_DPLL_BYPASS)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PAU_DPLL_BYPASS, i_target_chip, l_attr_pau_dpll_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_PAU_DPLL_BYPASS)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_BOOT_PAU_DPLL_BYPASS, i_target_chip, l_attr_boot_pau_dpll_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_BOOT_PAU_DPLL_BYPASS)");

            FAPI_DBG("Setting up tank PLL bypass attributes");

            if (l_read_scratch6_reg.getBit<ATTR_IO_TANK_PLL_BYPASS_BIT>())
            {
                l_attr_io_tank_pll_bypass = fapi2::ENUM_ATTR_IO_TANK_PLL_BYPASS_BYPASS;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_IO_TANK_PLL_BYPASS, i_target_chip, l_attr_io_tank_pll_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_IO_TANK_PLL_BYPASS)")

            FAPI_DBG("Setting up ATTR_PROC_FABRIC_EFF_TOPOLOGY_ID");

            if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE)
            {
                l_read_scratch6_reg.extractToRight<ATTR_PROC_FABRIC_EFF_TOPOLOGY_ID_STARTBIT, ATTR_PROC_FABRIC_EFF_TOPOLOGY_ID_LENGTH>
                (l_attr_proc_fabric_eff_topology_id);
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_FABRIC_EFF_TOPOLOGY_ID, i_target_chip, l_attr_proc_fabric_eff_topology_id),
                     "Error from FAPI_ATTR_SET (ATTR_PROC_FABRIC_EFF_TOPOLOGY_ID)");

            FAPI_DBG("Setting up ATTR_PROC_FABRIC_TOPOLOGY_MODE, ATTR_PROC_FABRIC_BROADCAST_MODE");

            if (l_read_scratch6_reg.getBit<ATTR_PROC_FABRIC_TOPOLOGY_MODE_BIT>())
            {
                l_attr_proc_fabric_topology_mode = fapi2::ENUM_ATTR_PROC_FABRIC_TOPOLOGY_MODE_MODE1;
            }

            l_read_scratch6_reg.extractToRight<ATTR_PROC_FABRIC_BROADCAST_MODE_STARTBIT, ATTR_PROC_FABRIC_BROADCAST_MODE_LENGTH>
            (l_attr_proc_fabric_broadcast_mode);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_FABRIC_TOPOLOGY_MODE, FAPI_SYSTEM, l_attr_proc_fabric_topology_mode),
                     "Error from FAPI_ATTR_SET (ATTR_PROC_FABRIC_TOPOLOGY_MODE)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_FABRIC_BROADCAST_MODE, FAPI_SYSTEM, l_attr_proc_fabric_broadcast_mode),
                     "Error from FAPI_ATTR_SET (ATTR_PROC_FABRIC_BROADCAST_MODE)");

            FAPI_DBG("Setting up ATTR_PROC_FABRIC_TOPOLOGY_ID");

            if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE)
            {
                l_read_scratch6_reg.extractToRight<ATTR_PROC_FABRIC_TOPOLOGY_ID_STARTBIT, ATTR_PROC_FABRIC_TOPOLOGY_ID_LENGTH>
                (l_attr_proc_fabric_topology_id);
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_FABRIC_TOPOLOGY_ID, i_target_chip, l_attr_proc_fabric_topology_id),
                     "Error from FAPI_ATTR_SET (ATTR_PROC_FABRIC_TOPOLOGY_ID)");
        }
    }

    // read_scratch9_reg - PAU/MC frequency
    {
        fapi2::buffer<uint64_t> l_read_scratch9_reg = 0;
        fapi2::ATTR_FREQ_PAU_MHZ_Type l_attr_freq_pau_mhz = 0;
        fapi2::ATTR_MC_PLL_BUCKET_Type l_attr_mc_pll_bucket = { 0 };

        if (l_read_scratch8_reg.getBit<SCRATCH9_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 9 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_9_RW, l_read_scratch9_reg),
                     "Error reading Scratch 9 mailbox register");

            FAPI_DBG("Setting up ATTR_FREQ_PAU_MHZ");
            l_read_scratch9_reg.extractToRight<ATTR_FREQ_PAU_MHZ_STARTBIT, ATTR_FREQ_PAU_MHZ_LENGTH>(l_attr_freq_pau_mhz);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_attr_freq_pau_mhz),
                     "Error from FAPI_ATTR_SET (ATTR_FREQ_PAU_MHZ)");

            if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE)
            {
                FAPI_DBG("Setting up ATTR_MC_PLL_BUCKET");

                for (const auto& l_perv_mc_target : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                         static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC),
                         fapi2::TARGET_STATE_FUNCTIONAL))
                {
                    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = p10_sbe_scratch_regs_get_unit_num(l_perv_mc_target,
                            fapi2::TARGET_TYPE_MC);

                    FAPI_TRY(l_read_scratch9_reg.extractToRight(l_attr_mc_pll_bucket[l_attr_chip_unit_pos],
                             ATTR_MC_PLL_BUCKET_STARTBIT +
                             (ATTR_MC_PLL_BUCKET_LENGTH * l_attr_chip_unit_pos),
                             ATTR_MC_PLL_BUCKET_LENGTH));
                }

                FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_MC_PLL_BUCKET, i_target_chip, l_attr_mc_pll_bucket),
                         "Error from FAPI_ATTR_SET (ATTR_MC_PLL_BUCKET)");
            }
        }
    }

    // read_scratch10_reg - IOHS frequency / chip contained backing caches vector
    {

        if (l_read_scratch8_reg.getBit<SCRATCH10_REG_VALID_BIT>())
        {
            fapi2::buffer<uint64_t> l_read_scratch10_reg = 0;

            FAPI_DBG("Reading Scratch 10 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_10_RW, l_read_scratch10_reg),
                     "Error reading Scratch 10 mailbox register");

            if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE)
            {
                fapi2::ATTR_IOHS_PLL_BUCKET_Type l_attr_iohs_pll_bucket = { 0 };

                FAPI_DBG("Setting up ATTR_IOHS_PLL_BUCKET");

                for (const auto& l_perv_iohs_target : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                         static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_IOHS),
                         fapi2::TARGET_STATE_FUNCTIONAL))
                {
                    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = p10_sbe_scratch_regs_get_unit_num(l_perv_iohs_target,
                            fapi2::TARGET_TYPE_IOHS);

                    FAPI_TRY(l_read_scratch10_reg.extractToRight(l_attr_iohs_pll_bucket[l_attr_chip_unit_pos],
                             ATTR_IOHS_PLL_BUCKET_STARTBIT +
                             (ATTR_IOHS_PLL_BUCKET_LENGTH * l_attr_chip_unit_pos),
                             ATTR_IOHS_PLL_BUCKET_LENGTH));
                }

                FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_IOHS_PLL_BUCKET, i_target_chip, l_attr_iohs_pll_bucket),
                         "Error from FAPI_ATTR_SET (ATTR_IOHS_PLL_BUCKET)");
            }
            else if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CHIP)
            {
                fapi2::ATTR_CHIP_CONTAINED_BACKING_CACHES_VEC_Type l_attr_chip_contained_backing_caches_vec;

                FAPI_DBG("Setting up chip contained backing caches vector");
                l_read_scratch10_reg.extractToRight<ATTR_CHIP_CONTAINED_BACKING_CACHES_VEC_STARTBIT, ATTR_CHIP_CONTAINED_BACKING_CACHES_VEC_LENGTH>
                (l_attr_chip_contained_backing_caches_vec);
                FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CHIP_CONTAINED_BACKING_CACHES_VEC, i_target_chip,
                                       l_attr_chip_contained_backing_caches_vec),
                         "Error from FAPI_ATTR_SET (ATTR_CHIP_CONTAINED_BACKING_CACHES_VEC)");
            }
        }
    }

    // read_scratch13_reg -- TPM SPI settings
    {
        fapi2::buffer<uint64_t> l_read_scratch13_reg = 0;
        fapi2::ATTR_TPM_SPI_BUS_DIV_Type l_attr_tpm_spi_bus_div = 0;

        if (l_read_scratch8_reg.getBit<SCRATCH13_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 13 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_13_RW, l_read_scratch13_reg),
                     "Error reading Scratch 13 mailbox register");

            FAPI_DBG("Setting up ATTR_TPM_SPI_BUS_DIV");
            l_read_scratch13_reg.extract<ATTR_TPM_SPI_BUS_DIV_STARTBIT, ATTR_TPM_SPI_BUS_DIV_LENGTH>(l_attr_tpm_spi_bus_div);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_TPM_SPI_BUS_DIV, i_target_chip, l_attr_tpm_spi_bus_div),
                     "Error from FAPI_ATTR_SET (ATTR_TPM_SPI_BUS_DIV)");
        }
    }

    FAPI_INF("p10_sbe_attr_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
