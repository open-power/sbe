/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_sbe_mcs_setup.C $  */
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
/// @file p9_sbe_mcs_setup.C
/// @brief Configure MC unit to support HB execution (FAPI2)
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
#include <p9_sbe_mcs_setup.H>
#include <p9_fbc_utils.H>
#include <p9_mc_scom_addresses.H>
#include <p9_mc_scom_addresses_fld.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------
const uint8_t MCS_MCFGP_BASE_ADDRESS_START_BIT = 8;

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

///
/// @brief Set hostboot dcbz configuration for one unit target
///
/// @tparam T template parameter, passed in target.
/// @param[in] i_target Reference to an MC target (MCS/MI)
/// @param[in] i_chip_base_address Chip non-mirrored base address
/// @return FAPI2_RC_SUCCESS if success, else error code.
///
template<fapi2::TargetType T>
fapi2::ReturnCode set_hb_dcbz_config(
    const fapi2::Target<T>& i_target,
    const uint64_t i_chip_base_address)
{
    FAPI_DBG("Start");
    fapi2::buffer<uint64_t> l_mcfgp;
    fapi2::buffer<uint64_t> l_mcmode1;
    fapi2::buffer<uint64_t> l_mcperf1;
    fapi2::buffer<uint64_t> l_mcfirmask_and;
    fapi2::buffer<uint64_t> l_mcaction;

    // MCFGP -- set BAR valid, configure single MC group with minimum size at
    // chip base address
    FAPI_TRY(fapi2::getScom(i_target, MCS_MCFGP, l_mcfgp),
             "Error from getScom (MCS_MCFGP)");
    l_mcfgp.setBit<MCS_MCFGP_VALID>();
    l_mcfgp.clearBit<MCS_MCFGP_MC_CHANNELS_PER_GROUP,
                     MCS_MCFGP_MC_CHANNELS_PER_GROUP_LEN>();
    l_mcfgp.clearBit<MCS_MCFGP_CHANNEL_0_GROUP_MEMBER_IDENTIFICATION,
                     MCS_MCFGP_CHANNEL_0_GROUP_MEMBER_IDENTIFICATION_LEN>();
    l_mcfgp.clearBit<MCS_MCFGP_GROUP_SIZE, MCS_MCFGP_GROUP_SIZE_LEN>();
    // group base address field covers RA 8:31
    l_mcfgp.insert(i_chip_base_address,
                   MCS_MCFGP_GROUP_BASE_ADDRESS,
                   MCS_MCFGP_GROUP_BASE_ADDRESS_LEN,
                   MCS_MCFGP_BASE_ADDRESS_START_BIT);
    FAPI_TRY(fapi2::putScom(i_target, MCS_MCFGP, l_mcfgp),
             "Error from putScom (MCS_MCFGP)");

    // MCMODE1 -- disable speculation, cmd bypass, fp command bypass
    FAPI_TRY(fapi2::getScom(i_target, MCS_MCMODE1, l_mcmode1),
             "Error from getScom (MCS_MCMODE1)");
    l_mcmode1.setBit<MCS_MCMODE1_DISABLE_ALL_SPEC_OPS>();
    l_mcmode1.setBit<MCS_MCMODE1_DISABLE_SPEC_OP,
                     MCS_MCMODE1_DISABLE_SPEC_OP_LEN>();
    l_mcmode1.setBit<MCS_MCMODE1_DISABLE_COMMAND_BYPASS,
                     MCS_MCMODE1_DISABLE_COMMAND_BYPASS_LEN>();
    l_mcmode1.setBit<MCS_MCMODE1_DISABLE_FP_COMMAND_BYPASS>();
    FAPI_TRY(fapi2::putScom(i_target, MCS_MCMODE1, l_mcmode1),
             "Error from putScom (MCS_MCMODE1)");

    // MCS_MCPERF1 -- disable fast path
    FAPI_TRY(fapi2::getScom(i_target, MCS_MCPERF1, l_mcperf1),
             "Error from getScom (MCS_MCPERF1)");
    l_mcperf1.setBit<MCS_MCPERF1_DISABLE_FASTPATH>();
    FAPI_TRY(fapi2::putScom(i_target, MCS_MCPERF1, l_mcperf1),
             "Error from putScom (MCS_MCPERF1)");

    // Unmask MC FIR
    // Set MC Fault Isolation Action1 Register
    l_mcaction.setBit<MCS_MCFIR_MC_INTERNAL_RECOVERABLE_ERROR>();
    FAPI_TRY(fapi2::putScom(i_target, MCS_MCFIRACT1, l_mcaction),
             "Error from putScom (MCS_MCFIRACT1)");

    // Clear FIR bits in MC Fault Isolation Mask Register
    l_mcfirmask_and.flush<1>();
    l_mcfirmask_and.clearBit<MCS_MCFIR_COMMAND_LIST_TIMEOUT>();
    l_mcfirmask_and.clearBit<MCS_MCFIR_MC_INTERNAL_RECOVERABLE_ERROR>();
    l_mcfirmask_and.clearBit<MCS_MCFIR_MC_INTERNAL_NONRECOVERABLE_ERROR>();
    l_mcfirmask_and.clearBit<MCS_MCFIR_POWERBUS_PROTOCOL_ERROR>();
    l_mcfirmask_and.clearBit<MCS_MCFIR_MULTIPLE_BAR>();

    if (T != fapi2::TARGET_TYPE_MI)
    {
        // unimplemented for Cumulus/MI type
        l_mcfirmask_and.clearBit<MCS_MCFIR_INVALID_ADDRESS>();
    }

    FAPI_TRY(fapi2::putScom(i_target, MCS_MCFIRMASK_AND, l_mcfirmask_and),
             "Error from putScom (MCS_MCFIRMASK_AND)");

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}


// HWP entry point
fapi2::ReturnCode
p9_sbe_mcs_setup(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_INF("Start");

    fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type l_is_master_sbe;
    fapi2::ATTR_IS_MPIPL_Type l_is_mpipl;
    fapi2::ATTR_SYSTEM_IPL_PHASE_Type l_ipl_type;

    auto l_mcs_chiplets = i_target.getChildren<fapi2::TARGET_TYPE_MCS>();
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

#ifdef __PPE__

        // assert that a viable MCS/MI chiplet is found to service dcbz on the
        // master processor
        if (!l_mcs_chiplets.size() &&
            !l_mi_chiplets.size())
        {
            // collect PG FFDC
            uint16_t l_n1_pg = 0xFFFF;
            uint16_t l_n3_pg = 0xFFFF;

            for (auto l_tgt : i_target.getChildren<fapi2::TARGET_TYPE_PERV>(
                     static_cast<fapi2::TargetFilter>(
                         fapi2::TARGET_FILTER_NEST_WEST |
                         fapi2::TARGET_FILTER_NEST_EAST),
                     fapi2::TARGET_STATE_FUNCTIONAL))
            {
                uint8_t l_attr_chip_unit_pos = 0;
                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                                       l_tgt,
                                       l_attr_chip_unit_pos),
                         "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");
                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG,
                                       l_tgt,
                                       ((l_attr_chip_unit_pos == N3_CHIPLET_ID) ?
                                        (l_n3_pg) :
                                        (l_n1_pg))),
                         "Error from FAPI_ATTR_GET(ATTR_PG)");
            }

            FAPI_ASSERT(false,
                        fapi2::P9_SBE_MCS_SETUP_NO_MC_FOUND_ERR()
                        .set_CHIP(i_target)
                        .set_IS_MASTER_SBE(l_is_master_sbe)
                        .set_IS_MPIPL(l_is_mpipl)
                        .set_IPL_TYPE(l_ipl_type)
                        .set_N1_PG(l_n1_pg)
                        .set_N3_PG(l_n3_pg),
                        "No functional MC unit target found on master chip");
        }

#endif

        // determine base address
        //   = (drawer non-mirrored base address) + (hostboot HRMOR offset)
        //   min MCS base size is 4GB, local HB will always be below
        FAPI_TRY(p9_fbc_utils_get_chip_base_address_no_aliases(i_target,
                 ABS_FBC_GRP_ID_ONLY,
                 l_chip_base_address_nm0,
                 l_chip_base_address_nm1,
                 l_chip_base_address_m,
                 l_chip_base_address_mmio),
                 "Error from p9_fbc_utils_get_chip_base_addrs");

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET,
                               FAPI_SYSTEM,
                               l_hostboot_hrmor_offset),
                 "Error from FAPI_ATTR_GET (ATTR_HOSTBOOT_HRMOR_OFFSET)");

        l_chip_base_address_nm0 += l_hostboot_hrmor_offset;
        l_chip_base_address_nm0 &= 0xFFFFFFFF00000000; // only keep 4GB and up

        if (l_mcs_chiplets.size())
        {
            FAPI_TRY(set_hb_dcbz_config<fapi2::TARGET_TYPE_MCS>(
                         l_mcs_chiplets.front(),
                         l_chip_base_address_nm0),
                     "Error from set_hb_dcbz_config (MCS)");
        }
        else if (l_mi_chiplets.size())
        {
            FAPI_TRY(set_hb_dcbz_config<fapi2::TARGET_TYPE_MI>(
                         l_mi_chiplets.front(),
                         l_chip_base_address_nm0),
                     "Error from set_hb_dcbz_config (MI)");
        }
        else
        {
            FAPI_INF("No MCS/MI targets found! Nothing to do!");
        }
    }

fapi_try_exit:
    FAPI_INF("End");
    return fapi2::current_err;

}

