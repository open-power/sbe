/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_attr_setup.C $ */
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


const uint32_t NUM_EQS_PER_CHIP  = 8; // Num of EQ pervasive chiplets per chip
const uint32_t NUM_CORES_PER_EQ  = 4; // Num of cores per EQ chiplet
const uint32_t NUM_PAUC_PER_CHIP = 4; // Num of PAU pervasive chiplets per chip
const uint32_t NUM_PAUS_PER_PAUC = 2; // Num of PAU logical units per PAU pervasive chiplet
const uint32_t NUM_IOHS_PER_CHIP = 8; // Num of IOHS pervasive chiplets per chip

const uint32_t PCI0_CHIPLET_ID  = 0x08;
const uint32_t MC0_CHIPLET_ID   = 0x0C;
const uint32_t PAU0_CHIPLET_ID  = 0x10;
const uint32_t AXON0_CHIPLET_ID = 0x18;
const uint32_t EQ0_CHIPLET_ID   = 0x20;



///
/// @brief Calculate functional unit target number from pervasive target type
///
/// @tparam T template parameter, functional target type
/// @param[in]    i_target_chip  Processor chip target
///
/// @return uint8_t
///
template<fapi2::TargetType T>
uint8_t
p10_sbe_scratch_regs_get_unit_num(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_perv)
{
    uint8_t l_perv_unit_num = i_target_perv.getChipletNumber();
    uint8_t l_functional_unit_num = 0;

    switch (T)
    {
        case fapi2::TARGET_TYPE_EQ:
            l_functional_unit_num = l_perv_unit_num - EQ0_CHIPLET_ID;
            break;

        case fapi2::TARGET_TYPE_PEC:
            l_functional_unit_num = l_perv_unit_num - PCI0_CHIPLET_ID;
            break;

        case fapi2::TARGET_TYPE_MC:
            l_functional_unit_num = l_perv_unit_num - MC0_CHIPLET_ID;
            break;

        case fapi2::TARGET_TYPE_PAUC:
            l_functional_unit_num = l_perv_unit_num - PAU0_CHIPLET_ID;
            break;

        case fapi2::TARGET_TYPE_IOHS:
            l_functional_unit_num = l_perv_unit_num - AXON0_CHIPLET_ID;
            break;
    }

    return l_functional_unit_num;
}


///
/// @brief Set platform ATTR_PG attribute for EQ chiplets, based on MPVD
///        driven partial good (ATTR_PG_MPVD) and mailbox gard records
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

    // EQ - apply core GARD information to partial good vector
    i_scratch1_reg.extract<CORE_GARD_STARTBIT, CORE_GARD_LENGTH>(l_core_gard);

    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_ALL_EQ),
            fapi2::TARGET_STATE_PRESENT))
    {
        // extract bits associated with this quad (4 cores)
        uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num<fapi2::TARGET_TYPE_EQ>(l_perv);
        uint32_t l_gard_mask = (l_core_gard >>
                                (NUM_CORES_PER_EQ * ((NUM_EQS_PER_CHIP - 1) - l_unit_num))) & CORE_GARD_EQ_MASK;
        FAPI_DBG("EQ%d, core gard mask: 0x%X", l_unit_num, l_gard_mask);

        // shift into position in partial good attribute
        fapi2::ATTR_PG_Type l_pg;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG before: 0x%08X", l_pg);
        l_pg |= ((l_gard_mask << EQ_PG_MMA_SHIFT)    |  // MMA0..3
                 (l_gard_mask << EQ_PG_L3_SHIFT)     |  // L30..3
                 (l_gard_mask << EQ_PG_ECL2_SHIFT));    // ECL20..3
        FAPI_DBG("  PG after: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
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

    // N1 - apply NMMU1 GARD information to partial good vector
    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_NEST_SOUTH),
            fapi2::TARGET_STATE_PRESENT))
    {
        fapi2::ATTR_PG_Type l_pg;
        FAPI_DBG("N1, NMMU1 region gard mask: 0x%X", i_scratch2_reg.getBit<NMMU1_GARD_BIT>());

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG before: 0x%08X", l_pg);
        l_pg |= (i_scratch2_reg.getBit<NMMU1_GARD_BIT>() << N1_PG_NMMU1_SHIFT);
        FAPI_DBG("  PG after: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
    }

    // apply GARD information to PCI0..1
    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_ALL_PCI),
            fapi2::TARGET_STATE_PRESENT))
    {
        uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num<fapi2::TARGET_TYPE_PEC>(l_perv);
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
    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_ALL_MC),
            fapi2::TARGET_STATE_PRESENT))
    {
        uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num<fapi2::TARGET_TYPE_MC>(l_perv);
        fapi2::ATTR_PG_Type l_pg;

        FAPI_DBG("MC%d, gard mask: 0x%X",
                 l_unit_num,
                 i_scratch2_reg.getBit(static_cast<uint32_t>(MC_GARD_STARTBIT) + l_unit_num));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG before: 0x%08X", l_pg);

        if (i_scratch2_reg.getBit(static_cast<uint32_t>(MC_GARD_STARTBIT) + l_unit_num))
        {
            l_pg = 0xFFFFFFFF;
        }

        FAPI_DBG("  PG after: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
    }

    // apply GARD information to PAU0..3
    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_ALL_PAU),
            fapi2::TARGET_STATE_PRESENT))
    {
        uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num<fapi2::TARGET_TYPE_PAUC>(l_perv);
        uint32_t l_gard_mask;
        fapi2::ATTR_PG_Type l_pg;

        // extract bits associated with the pau logic units
        i_scratch2_reg.extractToRight<PAU_GARD_STARTBIT, PAU_GARD_LENGTH>(l_gard_mask);
        l_gard_mask = (l_gard_mask >> (NUM_PAUS_PER_PAUC * ((NUM_PAUC_PER_CHIP - 1) - l_unit_num))) & PAU_GARD_PAUC_MASK;

        // PAUC1 has pau3 in first unit position, swap bits
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

        FAPI_DBG("  PG after: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
    }

    // apply GARD information to IOHS0..7
    for (const auto& l_perv : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(static_cast<fapi2::TargetFilter>
            (fapi2::TARGET_FILTER_ALL_IOHS),
            fapi2::TARGET_STATE_PRESENT))
    {
        uint8_t l_unit_num = p10_sbe_scratch_regs_get_unit_num<fapi2::TARGET_TYPE_IOHS>(l_perv);
        fapi2::ATTR_PG_Type l_pg;
        FAPI_DBG("IOHS%d, chiplet gard mask: 0x%X, NDL region gard mask: 0x%X",
                 l_unit_num,
                 i_scratch2_reg.getBit(static_cast<uint32_t>(IOHS_GARD_STARTBIT) + l_unit_num),
                 i_scratch7_reg.getBit(NDL_GARD_STARTBIT + l_unit_num));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG_MVPD, l_perv, l_pg));
        FAPI_DBG("  PG before: 0x%08X", l_pg);

        if (i_scratch2_reg.getBit(static_cast<uint32_t>(IOHS_GARD_STARTBIT) + l_unit_num))
        {
            l_pg = 0xFFFFFFFF;
        }
        else
        {
            l_pg |= (i_scratch7_reg.getBit(NDL_GARD_STARTBIT + l_unit_num) << IOHS_PG_NDL_SHIFT);
        }

        FAPI_DBG("  PG after: 0x%08X", l_pg);
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PG, l_perv, l_pg));
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

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_attr_system_ipl_phase),
                 "Error from FAPI_ATTR_GET (ATTR_SYSTEM_IPL_PHASE)");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, FAPI_SYSTEM, l_attr_contained_ipl_type),
                 "Error from FAPI_ATTR_GET (ATTR_CONTAINED_IPL_TYPE)");
    }

    // read_cbs_cs_reg -- Security_Access_Bit
    {
        fapi2::buffer<uint64_t> l_read_cbs_cs_reg;
        fapi2::ATTR_SECURITY_MODE_Type l_attr_security_mode = 0;
        fapi2::ATTR_SECURITY_ENABLE_Type l_attr_security_enable = fapi2::ENUM_ATTR_SECURITY_ENABLE_ENABLE;

        FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_CBS_CS, l_read_cbs_cs_reg),
                 "Error reading CBS Control/Status register");

        FAPI_DBG("Reading ATTR_SECURITY_MODE");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SECURITY_MODE, FAPI_SYSTEM, l_attr_security_mode));

        if (!l_attr_security_mode) //ATTR_SECURITY_MODE == 0b0, then clear the SAB bit
        {
            l_read_cbs_cs_reg.clearBit<FSXCOMP_FSXLOG_CBS_CS_SECURE_ACCESS_BIT>();
            FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_CBS_CS, l_read_cbs_cs_reg),
                     "Error writing CBS Control/Status register");
        }

        FAPI_DBG("Setting up ATTR_SECURITY_ENABLE with SAB state");

        if (!l_read_cbs_cs_reg.getBit<FSXCOMP_FSXLOG_CBS_CS_SECURE_ACCESS_BIT>())
        {
            l_attr_security_enable = fapi2::ENUM_ATTR_SECURITY_ENABLE_DISABLE;
        }

        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SECURITY_ENABLE, FAPI_SYSTEM, l_attr_security_enable),
                 "Error from FAPI_ATTR_SET (ATTR_SECURITY_ENABLE");
    }

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

        if (l_attr_contained_ipl_type != fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE)
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
        fapi2::ATTR_I2C_BUS_DIV_REF_Type l_attr_i2c_bus_div_ref = 0;
        fapi2::ATTR_FREQ_CORE_BOOT_MHZ_Type l_attr_freq_core_boot_mhz = 0;

        if (l_read_scratch8_reg.getBit<SCRATCH4_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 4 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_4_RW, l_read_scratch4_reg),
                     "Error reading Scrach 4 mailbox register");

            FAPI_DBG("Setting up ATTR_I2C_BUS_DIV_REF");
            l_read_scratch4_reg.extract<ATTR_I2C_BUS_DIV_REF_STARTBIT, ATTR_I2C_BUS_DIV_REF_LENGTH>(l_attr_i2c_bus_div_ref);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_I2C_BUS_DIV_REF, i_target_chip, l_attr_i2c_bus_div_ref),
                     "Error from FAPI_ATTR_SET (ATTR_I2C_BUS_DIV_REF)");

            FAPI_DBG("Setting up ATTR_FREQ_CORE_BOOT_MHZ");
            l_read_scratch4_reg.extract<ATTR_FREQ_CORE_BOOT_MHZ_STARTBIT, ATTR_FREQ_CORE_BOOT_MHZ_LENGTH>
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
        fapi2::ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT_Type l_attr_clock_mux_iohs_lcpll_input = { 0 };
        fapi2::ATTR_CLOCK_MUX_PCI_LCPLL_INPUT_Type l_attr_clock_mux_pci_lcpll_input = { 0 };

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

            FAPI_DBG("Setting up IOHS PLL mux attributes");

            for (const auto& l_perv_iohs_target :  i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                     static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_IOHS),
                     fapi2::TARGET_STATE_FUNCTIONAL))
            {
                fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = p10_sbe_scratch_regs_get_unit_num<fapi2::TARGET_TYPE_IOHS>
                        (l_perv_iohs_target);

                FAPI_TRY(l_read_scratch5_reg.extractToRight(l_attr_clock_mux_iohs_lcpll_input[l_attr_chip_unit_pos],
                         ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT_STARTBIT +
                         (ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT_LENGTH * l_attr_chip_unit_pos),
                         ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT_LENGTH));
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT, i_target_chip, l_attr_clock_mux_iohs_lcpll_input),
                     "Error from FAPI_ATTR_SET (ATTR_CLOCK_MUX_IOHS_LCPLL_INPUT)");

            for (const auto& l_perv_pci_target : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                     static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_PCI),
                     fapi2::TARGET_STATE_FUNCTIONAL))
            {
                fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = p10_sbe_scratch_regs_get_unit_num<fapi2::TARGET_TYPE_PEC>
                        (l_perv_pci_target);

                FAPI_TRY(l_read_scratch5_reg.extractToRight(l_attr_clock_mux_pci_lcpll_input[l_attr_chip_unit_pos],
                         ATTR_CLOCK_MUX_PCI_LCPLL_INPUT_STARTBIT +
                         (ATTR_CLOCK_MUX_PCI_LCPLL_INPUT_LENGTH * l_attr_chip_unit_pos),
                         ATTR_CLOCK_MUX_PCI_LCPLL_INPUT_LENGTH));
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CLOCK_MUX_PCI_LCPLL_INPUT, i_target_chip, l_attr_clock_mux_pci_lcpll_input),
                     "Error from FAPI_ATTR_SET (ATTR_CLOCK_MUX_PCI_LCPLL_INPUT)");
        }
    }

    // read_scratch6_reg -- Master/slave, node/chip selection, PLL bypass controls
    {
        fapi2::buffer<uint64_t> l_read_scratch6_reg = 0;
        fapi2::ATTR_CP_PLLTODFLT_BYPASS_Type l_attr_cp_plltodflt_bypass = fapi2::ENUM_ATTR_CP_PLLTODFLT_BYPASS_NO_BYPASS;
        fapi2::ATTR_CP_PLLNESTFLT_BYPASS_Type l_attr_cp_pllnestflt_bypass = fapi2::ENUM_ATTR_CP_PLLNESTFLT_BYPASS_NO_BYPASS;
        fapi2::ATTR_CP_PLLIOFLT_BYPASS_Type l_attr_cp_pllioflt_bypass = fapi2::ENUM_ATTR_CP_PLLIOFLT_BYPASS_NO_BYPASS;
        fapi2::ATTR_CP_PLLIOSSFLT_BYPASS_Type l_attr_cp_plliossflt_bypass = fapi2::ENUM_ATTR_CP_PLLIOSSFLT_BYPASS_NO_BYPASS;
        fapi2::ATTR_NEST_DPLL_BYPASS_Type l_attr_nest_dpll_bypass = fapi2::ENUM_ATTR_NEST_DPLL_BYPASS_NO_BYPASS;
        fapi2::ATTR_PAU_DPLL_BYPASS_Type l_attr_pau_dpll_bypass = fapi2::ENUM_ATTR_PAU_DPLL_BYPASS_NO_BYPASS;
        fapi2::ATTR_IO_TANK_PLL_BYPASS_Type l_attr_io_tank_pll_bypass = fapi2::ENUM_ATTR_IO_TANK_PLL_BYPASS_NO_BYPASS;
        fapi2::ATTR_PROC_FABRIC_EFF_TOPOLOGY_ID_Type l_attr_proc_fabric_eff_topology_id = 0;
        fapi2::ATTR_PROC_FABRIC_TOPOLOGY_MODE_Type l_attr_proc_fabric_topology_mode =
            fapi2::ENUM_ATTR_PROC_FABRIC_TOPOLOGY_MODE_MODE0;
        fapi2::ATTR_PROC_FABRIC_BROADCAST_MODE_Type l_attr_proc_fabric_broadcast_mode =
            fapi2::ENUM_ATTR_PROC_FABRIC_BROADCAST_MODE_1HOP_CHIP_IS_GROUP;
        fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type l_attr_proc_sbe_master_chip = fapi2::ENUM_ATTR_PROC_SBE_MASTER_CHIP_FALSE;
        fapi2::ATTR_PROC_FABRIC_TOPOLOGY_ID_Type l_attr_proc_fabric_topology_id = 0;

        if (l_read_scratch8_reg.getBit<SCRATCH6_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 6 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_6_RW, l_read_scratch6_reg),
                     "Error reading Scratch 6 mailbox register");

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

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NEST_DPLL_BYPASS, i_target_chip, l_attr_nest_dpll_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_NEST_DPLL_BYPASS)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PAU_DPLL_BYPASS, i_target_chip, l_attr_pau_dpll_bypass),
                     "Error from FAPI_ATTR_SET (ATTR_PAU_DPLL_BYPASS)");

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

            FAPI_DBG("Setting up ATTR_PROC_SBE_MASTER_CHIP");

            if (l_read_scratch6_reg.getBit<ATTR_PROC_SBE_MASTER_CHIP_BIT>())
            {
                l_attr_proc_sbe_master_chip = fapi2::ENUM_ATTR_PROC_SBE_MASTER_CHIP_TRUE;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_SBE_MASTER_CHIP, i_target_chip, l_attr_proc_sbe_master_chip),
                     "Error from FAPI_ATTR_SET (ATTR_PROC_SBE_MASTER_CHIP)");

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
        fapi2::ATTR_NDL_MESHCTRL_SETUP_Type l_attr_ndl_meshctrl_setup;

        if (l_read_scratch8_reg.getBit<SCRATCH9_REG_VALID_BIT>())
        {
            FAPI_DBG("Reading Scratch 9 mailbox register");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SCRATCH_REGISTER_9_RW, l_read_scratch9_reg),
                     "Error reading Scratch 9 mailbox register");

            FAPI_DBG("Setting up ATTR_FREQ_PAU_MHZ");
            l_read_scratch9_reg.extractToRight<ATTR_FREQ_PAU_MHZ_STARTBIT, ATTR_FREQ_PAU_MHZ_LENGTH>(l_attr_freq_pau_mhz);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_attr_freq_pau_mhz),
                     "Error from FAPI_ATTR_SET (ATTR_FREQ_PAU_MHZ)");

            FAPI_DBG("Setting up ATTR_MC_PLL_BUCKET");

            for (const auto& l_perv_mc_target : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                     static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC),
                     fapi2::TARGET_STATE_FUNCTIONAL))
            {
                fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = p10_sbe_scratch_regs_get_unit_num<fapi2::TARGET_TYPE_MC>
                        (l_perv_mc_target);

                FAPI_TRY(l_read_scratch9_reg.extractToRight(l_attr_mc_pll_bucket[l_attr_chip_unit_pos],
                         ATTR_MC_PLL_BUCKET_STARTBIT +
                         (ATTR_MC_PLL_BUCKET_LENGTH * l_attr_chip_unit_pos),
                         ATTR_MC_PLL_BUCKET_LENGTH));
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_MC_PLL_BUCKET, i_target_chip, l_attr_mc_pll_bucket),
                     "Error from FAPI_ATTR_SET (ATTR_MC_PLL_BUCKET)");

            FAPI_DBG("ATTR_NDL_MESHCTRL_SETUP");
            l_read_scratch9_reg.extractToRight<ATTR_NDL_MESHCTRL_SETUP_STARTBIT, ATTR_NDL_MESHCTRL_SETUP_LENGTH>
            (l_attr_ndl_meshctrl_setup);
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NDL_MESHCTRL_SETUP, i_target_chip, l_attr_ndl_meshctrl_setup),
                     "Error from FAPI_ATTR_SET (ATTR_NDL_MESHCTRL_SETUP)");
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
                    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = p10_sbe_scratch_regs_get_unit_num<fapi2::TARGET_TYPE_IOHS>
                            (l_perv_iohs_target);

                    FAPI_TRY(l_read_scratch10_reg.extractToRight(l_attr_iohs_pll_bucket[l_attr_chip_unit_pos],
                             ATTR_IOHS_PLL_BUCKET_STARTBIT +
                             (ATTR_IOHS_PLL_BUCKET_LENGTH * l_attr_chip_unit_pos),
                             ATTR_IOHS_PLL_BUCKET_LENGTH));
                }

                FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_IOHS_PLL_BUCKET, i_target_chip, l_attr_iohs_pll_bucket),
                         "Error from FAPI_ATTR_SET (ATTR_IOHS_PLL_BUCKET)");
            }
            else
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

    FAPI_INF("p10_sbe_attr_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
