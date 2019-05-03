/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_mcs_setup.C $ */
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
/// @file p10_sbe_mcs_setup.C
/// @brief Configure MC unit to support HB execution (FAPI2)
///
/// @author Joe McGill <jmcgill@us.ibm.com>
///

//
// *HWP HW Maintainer: Nicholas Landi <nlandi@ibm.com>
// *HWP FW Maintainer: Raja Das <rajadas2@in.ibm.com>
// *HWP Consumed by  : SBE
//


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p10_sbe_mcs_setup.H>
#include <p10_fbc_utils.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------
const uint8_t MC0_PBI01_SCOMFIR_MCFGP0_BASE_ADDRESS_START_BIT = 8;
const uint8_t NUMBER_MCS = 4;

const uint8_t MCFIR_COMMAND_LIST_TIMEOUT = 8;
const uint8_t MCFIR_MC_INTERNAL_RECOVERABLE_ERROR = 0;
const uint8_t MCFIR_MC_INTERNAL_NONRECOVERABLE_ERROR = 1;
const uint8_t MCFIR_POWERBUS_PROTOCOL_ERROR = 2;
const uint8_t MCFIR_MULTIPLE_BAR = 4;
const uint8_t MCFIR_INVALID_SMF_ACCESS = 22;

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

///
/// @brief Set hostboot dcbz configuration for one unit target and store
///        previous register states in attributes
///
/// @tparam T template parameter, passed in target.
/// @param[in] i_target_mc Reference to an MC target (MI)
/// @param[in] i_target_chip Reference to chip target
/// @param[in] i_chip_base_address Chip non-mirrored base address
/// @return FAPI2_RC_SUCCESS if success, else error code.
///
fapi2::ReturnCode set_hb_dcbz_config(
    const fapi2::Target<fapi2::TARGET_TYPE_MI>& i_target_mc,
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
    const uint64_t i_chip_base_address)
{
    using namespace scomt;
    using namespace scomt::mc;

    fapi2::buffer<uint64_t> l_mcfgp;
    fapi2::buffer<uint64_t> l_mcmode0;
    fapi2::buffer<uint64_t> l_mcmode1;
    fapi2::buffer<uint64_t> l_mcmode2;
    fapi2::buffer<uint64_t> l_mcperf1;
    fapi2::buffer<uint64_t> l_mcfirmask;
    fapi2::buffer<uint64_t> l_mcaction0;
    fapi2::buffer<uint64_t> l_mcaction1;
    fapi2::buffer<uint64_t> l_mcto;
    uint64_t l_register_states[9];

    // MCFGP -- set BAR valid, configure single MC group with minimum size at
    // chip base address///////////////////////////////////////////////////////

    FAPI_TRY(GET_SCOMFIR_MCFGP0(i_target_mc, l_mcfgp));
    l_register_states[fapi2::ENUM_ATTR_PROC_SBE_MCS_SETUP_REG_STATES_MCFGP0] = l_mcfgp;

    SET_SCOMFIR_MCFGP0_0_VALID(l_mcfgp);
    SET_SCOMFIR_MCFGP0_0_MC_CHANNELS_PER_GROUP(0x1, l_mcfgp);
    SET_SCOMFIR_MCFGP0_0_GROUP_MEMBER_IDENTIFICATION(0x0, l_mcfgp);
    SET_SCOMFIR_MCFGP0_0_GROUP_SIZE(0x0, l_mcfgp);
    l_mcfgp.insert(i_chip_base_address,
                   SCOMFIR_MCFGP0_0_GROUP_BASE_ADDRESS,
                   SCOMFIR_MCFGP0_0_GROUP_BASE_ADDRESS_LEN,
                   MC0_PBI01_SCOMFIR_MCFGP0_BASE_ADDRESS_START_BIT);

    FAPI_TRY(PUT_SCOMFIR_MCFGP0(i_target_mc, l_mcfgp));


    // MCMODE0 -- Group Address Interleave/////////////////////////////////////

    FAPI_TRY(GET_SCOMFIR_MCMODE0(i_target_mc, l_mcmode0));
    l_register_states[fapi2::ENUM_ATTR_PROC_SBE_MCS_SETUP_REG_STATES_MCMODE0] = l_mcmode0;

    SET_SCOMFIR_MCMODE0_GROUP_ADDRESS_INTERLEAVE_GRANULARITY(0x0, l_mcmode0);

    FAPI_TRY(PUT_SCOMFIR_MCMODE0(i_target_mc, l_mcmode0));

    // MCMODE1 -- disable speculation, cmd bypass, fp command bypass///////////

    FAPI_TRY(GET_SCOMFIR_MCMODE1(i_target_mc, l_mcmode1));
    l_register_states[fapi2::ENUM_ATTR_PROC_SBE_MCS_SETUP_REG_STATES_MCMODE1] = l_mcmode1;

    SET_SCOMFIR_MCMODE1_DISABLE_ALL_SPEC_OPS(l_mcmode1);
    SET_SCOMFIR_MCMODE1_DISABLE_FP_COMMAND_BYPASS(l_mcmode1);
    SET_SCOMFIR_MCMODE1_DISABLE_COMMAND_BYPASS(0x0, l_mcmode1);

    FAPI_TRY(PUT_SCOMFIR_MCMODE1(i_target_mc, l_mcmode1));


    // MCMODE2 -- enable one subchannel (must be same channel set up in MCFGP)/
    FAPI_TRY(GET_SCOMFIR_MCMODE2(i_target_mc, l_mcmode2));
    l_register_states[fapi2::ENUM_ATTR_PROC_SBE_MCS_SETUP_REG_STATES_MCMODE2] = l_mcmode2;

    SET_SCOMFIR_MCMODE2_CHANNEL0_SUBCHANNEL0_ENABLE(l_mcmode2);

    FAPI_TRY(PUT_SCOMFIR_MCMODE2(i_target_mc, l_mcmode2));

    // MCPERF1 -- disable fast path////////////////////////////////////////////
    FAPI_TRY(GET_SCOMFIR_MCPERF1(i_target_mc, l_mcperf1));
    l_register_states[fapi2::ENUM_ATTR_PROC_SBE_MCS_SETUP_REG_STATES_MCPERF1] = l_mcperf1;

    SET_SCOMFIR_MCPERF1_DISABLE_FASTPATH(l_mcperf1);

    FAPI_TRY(PUT_SCOMFIR_MCPERF1(i_target_mc, l_mcperf1));

    // Get state of FIRMASK before mcs_setup///////////////////////////////////
    FAPI_TRY(GET_SCOMFIR_MCFIRMASK_RW(i_target_mc, l_mcfirmask));
    l_register_states[fapi2::ENUM_ATTR_PROC_SBE_MCS_SETUP_REG_STATES_MCFIRMASK] = l_mcfirmask;

    //// TODO: RTC: 209749
    // Still need to use clearBit until these are defined in scom headers//////
    l_mcfirmask.flush<1>();
    l_mcfirmask.clearBit<MCFIR_COMMAND_LIST_TIMEOUT>();
    l_mcfirmask.clearBit<MCFIR_MC_INTERNAL_RECOVERABLE_ERROR>();
    l_mcfirmask.clearBit<MCFIR_MC_INTERNAL_NONRECOVERABLE_ERROR>();
    l_mcfirmask.clearBit<MCFIR_POWERBUS_PROTOCOL_ERROR>();
    l_mcfirmask.clearBit<MCFIR_MULTIPLE_BAR>();
    l_mcfirmask.clearBit<MCFIR_INVALID_SMF_ACCESS>();

    FAPI_TRY(PUT_SCOMFIR_MCFIRMASK_RW(i_target_mc, l_mcfirmask));


    // TODO: RTC: 209497
    // HW487308
    // Unmask MC FIR
    // Set MC Fault Isolation Action0 Register/////////////////////////////////
//    FAPI_TRY(fapi2::getScom(i_target_mc, scomt::mi::SCOMFIR_MCFIRACT0,
//                            l_mcaction0),
//             "Error from getScom (MCFIRACT0)");
//    l_register_states[fapi2::ENUM_ATTR_PROC_SBE_MCS_SETUP_REG_STATES_MCFIRACT0] = l_mcaction0;
//
//    l_mcaction0.flush<0>();
//    FAPI_TRY(fapi2::putScom(i_target_mc, scomt::mi::SCOMFIR_MCFIRACT0,
//                            l_mcaction0),
//             "Error from putScom (MCFIRACT0)");
    // Set MC Fault Isolation Action1 Register
//    FAPI_TRY(fapi2::getScom(i_target_mc, scomt::mi::SCOMFIR_MCFIRACT1,
//                            l_mcaction1),
//             "Error from getScom (MCFIRACT1)");
//    l_register_states[fapi2::ENUM_ATTR_PROC_SBE_MCS_SETUP_REG_STATES_MCFIRACT1] = l_mcaction1;
//    l_mcaction1.flush<0>();
//    l_mcaction1.insertFromRight(scomt::omi::MC0_PBI01_MCFIRACT1
//    FAPI_TRY(fapi2::putScom(i_target_mc, MCFIRACT1, l_mcaction0),
//             "Error from putScom (MCFIRACT1)");

    // MCTO ////////////////////////////////////////////////////////////////////
    FAPI_TRY(GET_SCOMFIR_MCTO(i_target_mc, l_mcto));
    l_register_states[fapi2::ENUM_ATTR_PROC_SBE_MCS_SETUP_REG_STATES_MCTO] = l_mcto;

    SET_SCOMFIR_MCTO_SELECT_PB_HANG_PULSE(l_mcto);
    CLEAR_SCOMFIR_MCTO_SELECT_LOCAL_HANG_PULSE(l_mcto);
    SET_SCOMFIR_MCTO_CL_TIMEOUT_VALUE(0x7, l_mcto);
    SET_SCOMFIR_MCTO_ENABLE_APO_HANG(l_mcto);
    SET_SCOMFIR_MCTO_CHANNEL_TIMEOUT_VALUE(0x1, l_mcto);

    FAPI_TRY(PUT_SCOMFIR_MCTO(i_target_mc, l_mcto));

    FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_SBE_MCS_SETUP_REG_STATES,
                           i_target_chip,
                           l_register_states),
             "Error from FAPI_ATTR_SET (ATTR_PROC_SBE_MCS_SETUP_REG_STATES)");

fapi_try_exit:
    return fapi2::current_err;
}


// HWP entry point
fapi2::ReturnCode
p10_sbe_mcs_setup(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_INF("Start");

    fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type l_is_master_sbe;
    fapi2::ATTR_IS_MPIPL_Type l_is_mpipl;
    fapi2::ATTR_SYSTEM_IPL_PHASE_Type l_ipl_type;

    auto l_mi_chiplets = i_target.getChildren<fapi2::TARGET_TYPE_MI>();
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    // configure one MC on master chip (only if IPL is loading hostboot, and is
    // not memory preserving)
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_SBE_MASTER_CHIP,
                           i_target,
                           l_is_master_sbe),
             "Error from FAPI_ATTR_GET (ATTR_PROC_SBE_MASTER_CHIP)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_MPIPL,
                           FAPI_SYSTEM,
                           l_is_mpipl),
             "Error from FAPI_ATTR_GET (ATTR_IS_MPIPL)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE,
                           FAPI_SYSTEM,
                           l_ipl_type),
             "Error from FAPI_ATTR_GET (ATTR_SYSTEM_IPL_PHASE)");

    if ((l_ipl_type == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL) &&
        (l_is_master_sbe == fapi2::ENUM_ATTR_PROC_SBE_MASTER_CHIP_TRUE) &&
        (l_is_mpipl == fapi2::ENUM_ATTR_IS_MPIPL_FALSE))
    {
        uint64_t l_chip_base_address_nm0;
        uint64_t l_chip_base_address_nm1;
        uint64_t l_chip_base_address_m;
        uint64_t l_chip_base_address_mmio;
        uint64_t l_hostboot_hrmor_offset;
        fapi2::ATTR_PROC_SBE_MCS_SETUP_SELECTED_MC_Type  l_mc_unit_pos;

#ifdef __PPE__

        // assert that a viable MI chiplet is found to service dcbz on the
        // master processor
        if (!l_mi_chiplets.size())
        {
            // collect PG FFDC
            uint16_t l_mc_pg[NUMBER_MCS] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };

            for (auto l_tgt : i_target.getChildren<fapi2::TARGET_TYPE_PERV>(
                     static_cast<fapi2::TargetFilter>(
                         fapi2::TARGET_FILTER_ALL_MC),
                     fapi2::TARGET_STATE_FUNCTIONAL))
            {
                uint8_t l_attr_chip_unit_pos = 0;
                uint16_t l_pg = 0xFFFF;
                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                                       l_tgt,
                                       l_attr_chip_unit_pos),
                         "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");


                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG,
                                       l_tgt,
                                       l_pg),
                         "Error from FAPI_ATTR_GET(ATTR_PG)");

                //Expected MC cplt_ids of 0xC-0xF
                l_mc_pg[l_attr_chip_unit_pos & 0x03] = l_pg;
            }

            FAPI_ASSERT(false,
                        fapi2::P10_SBE_MI_SETUP_NO_MC_FOUND_ERR()
                        .set_CHIP(i_target)
                        .set_IS_MASTER_SBE(l_is_master_sbe)
                        .set_IS_MPIPL(l_is_mpipl)
                        .set_IPL_TYPE(l_ipl_type)
                        .set_MC0_PG(l_mc_pg[0])
                        .set_MC1_PG(l_mc_pg[1])
                        .set_MC2_PG(l_mc_pg[2])
                        .set_MC3_PG(l_mc_pg[3]),
                        "No functional MC unit target found on master chip");
        }

#endif
        // determine base address
        //   = (drawer non-mirrored base address) + (hostboot HRMOR offset)
        //   min MI base size is 4GB, local HB will always be below
        FAPI_TRY(p10_fbc_utils_get_chip_base_address(i_target,
                 p10_fbc_utils_addr_mode::HB_BOOT_ID,
                 l_chip_base_address_nm0,
                 l_chip_base_address_nm1,
                 l_chip_base_address_m,
                 l_chip_base_address_mmio),
                 "Error from p10_fbc_utils_get_chip_base_addrs");

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET,
                               FAPI_SYSTEM,
                               l_hostboot_hrmor_offset),
                 "Error from FAPI_ATTR_GET (ATTR_HOSTBOOT_HRMOR_OFFSET)");

        l_chip_base_address_nm0 += l_hostboot_hrmor_offset;
        l_chip_base_address_nm0 &= 0xFFFFFFFF00000000; // only keep 4GB and up

        FAPI_DBG("Base address found: %016llx", l_chip_base_address_nm0);

        if (l_mi_chiplets.size())
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                                   l_mi_chiplets.front(),
                                   l_mc_unit_pos),
                     "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_SBE_MCS_SETUP_SELECTED_MC,
                                   i_target,
                                   l_mc_unit_pos),
                     "Error from FAPI_ATTR_SET (ATTR_PROC_MC_SETUP)");

            FAPI_TRY(set_hb_dcbz_config(
                         l_mi_chiplets.front(),
                         i_target,
                         l_chip_base_address_nm0),
                     "Error from set_hb_dcbz_config (MI)");
        }
        else
        {
            FAPI_INF("No MI targets found! Nothing to do!");
        }
    }

fapi_try_exit:
    FAPI_INF("End");
    return fapi2::current_err;

}
