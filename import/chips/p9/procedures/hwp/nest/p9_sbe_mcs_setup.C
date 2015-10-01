/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/nest/p9_sbe_mcs_setup.C $             */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//------------------------------------------------------------------------------
/// @file  p9_sbe_mcs_setup.C
///
/// @brief Configure one MCS unit on the master chip to low point of
/// coherency acknowledge preparations(lpc_ack preps). in support
/// of dcbz(Data Cache Block Zero) operations executed by HBI code
/// (while still running cache contained prior to memory configuration).
//------------------------------------------------------------------------------
// *HWP HW Owner        : Girisankar Paulraj <gpaulraj@in.ibm.com>
// *HWP HW Backup Owner : Joe McGill <jcmgill@us.ibm.com>
// *HWP FW Owner         : Thi N. Tran <thi@us.ibm.com>
// *HWP Team             : Nest
// *HWP Level            : 2
// *HWP Consumed by      : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_mcs_setup.H"

#include "p9_mc_scom_addresses.H"


static fapi2::ReturnCode p9_sbe_mcs_setup_bar_cnfg(const
        fapi2::Target<fapi2::TARGET_TYPE_MCS>& i_target_mcs);

fapi2::ReturnCode p9_sbe_mcs_setup(const
                                   fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    // Local Variable definition
    // for collect the following attributes
    //  1) Chip is master sbe chip.
    //  2) IPL is in MPIPL mode
    //  3) IPL is in normal IPL type
    uint8_t l_master_sbe = 0, l_mpipl_mode = 0, l_ipl_type = 0;
    bool l_mcs_found = 0;
    auto l_mcs_functional_vector = i_target.getChildren<fapi2::TARGET_TYPE_MCS>
                                   (fapi2::TARGET_STATE_FUNCTIONAL);
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    FAPI_DBG("Entering ...");

    // Collecting System IPL attributes...
    FAPI_INF(" Collecting attributes for master SBE  chip, MPIPL mode and IPL type");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_SBE_MASTER_CHIP, i_target,
                           l_master_sbe));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_MPIPL, FAPI_SYSTEM, l_mpipl_mode));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_ipl_type));
    FAPI_INF(" Checking whether It is Master SBE chip or not");

    if ( l_ipl_type == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL && l_master_sbe
         && !l_mpipl_mode )
    {
        for (auto l_target_mcs : l_mcs_functional_vector)
        {
            FAPI_TRY(p9_sbe_mcs_setup_bar_cnfg(l_target_mcs));

            l_mcs_found = 1;
            break;
        }

        FAPI_ASSERT(l_mcs_found,
                    fapi2::P9_SBE_MCS_SETUP_NO_MCS_FOUND_ERR()
                    .set_CHIP(i_target),
                    "No MCS CHIPLET found ");
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring one MCS BAR on Master SBE processor and set as required MCS found
///
/// @param[in]     i_target_mcs   Reference to TARGET_TYPE_MCS target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_mcs_setup_bar_cnfg(const
        fapi2::Target<fapi2::TARGET_TYPE_MCS>& i_target_mcs)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    // configures the following on MCFGP registers:-
    // 1) MCS valid bit  - 1 (Bit 0)
    // 2) No of MCS in the group as 1 (Bit 1:4)
    // 3) MCS group ID - 000 (Bit 5:7)
    // 4) MCS size as 4GB     (Bit 13:23)
    // 5) Disable spec all ops  (Bti 32)
    // 6) Disable all spec ops associates ( Bit 33:51)
    // and Masking all MC FIR calls
    FAPI_INF("configures first MCS unit MCFGP BAR to acknowledge lpc_ack preps");
    //Setting MCFGP register value
    FAPI_TRY(fapi2::getScom(i_target_mcs, MCS_MCFGP, l_data64));
    l_data64.writeBit<0>
    (p9SbeMcsSetup::GROUP_VALID);  //MCFGP.MCFGP_VALID = p9SbeMcsSetup::GROUP_VALID
    //MCFGP.MCFGP_CHANNEL_0_GROUP_MEMBER_IDENTIFICATION = p9SbeMcsSetup::GROUP_ID
    l_data64.insertFromRight<5, 3>(p9SbeMcsSetup::GROUP_ID);
    //MCFGP.MCFGP_GROUP_SIZE = p9SbeMcsSetup::GROUP_SIZE
    l_data64.insertFromRight<13, 11>(p9SbeMcsSetup::GROUP_SIZE);
    //MCFGP.MCFGP_MC_CHANNELS_PER_GROUP = p9SbeMcsSetup::MC_CHANNELS_PER_GROUP
    l_data64.insertFromRight<1, 4>(p9SbeMcsSetup::MC_CHANNELS_PER_GROUP);
    FAPI_TRY(fapi2::putScom(i_target_mcs, MCS_MCFGP, l_data64));
    //Setting MCMODE1 register value
    FAPI_TRY(fapi2::getScom(i_target_mcs, MCS_MCMODE1, l_data64));
    //MCMODE1.MCMODE1_DISABLE_ALL_SPEC_OPS = p9SbeMcsSetup::DISABLE_ALL_SPEC_OPS
    l_data64.writeBit<32>(p9SbeMcsSetup::DISABLE_ALL_SPEC_OPS);
    //MCMODE1.MCMODE1_DISABLE_SPEC_OP = p9SbeMcsSetup::DISABLE_SPEC_OP_ASSO
    l_data64.insertFromRight<33, 19>(p9SbeMcsSetup::DISABLE_SPEC_OP_ASSO);
    FAPI_TRY(fapi2::putScom(i_target_mcs, MCS_MCMODE1, l_data64));
    //Setting MCFIRMASK register value
    l_data64.flush<0>();
    //MCFIRMASK.MCFIRMASK_FIR_MASK = p9SbeMcsSetup::MCFIRMASK_DEFAULT
    l_data64.insertFromRight<0, 26>(p9SbeMcsSetup::MCFIRMASK_DEFAULT);
    FAPI_TRY(fapi2::putScom(i_target_mcs, MCS_MCFIRMASK, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
