/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/nest/p9_sbe_mcs_setup.H $             */
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
/// @file  p9_sbe_mcs_setup.H
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


#ifndef _P9_SBE_MCS_SETUP_H_
#define _P9_SBE_MCS_SETUP_H_


#include <fapi2.H>


namespace p9SbeMcsSetup
{
enum P9_SBE_MCS_SETUP_Public_Constants
{
    MCFIRMASK_DEFAULT = 0x00280000,
    MC_CHANNELS_PER_GROUP = 0x00,
    GROUP_ID = 0x00,
    GROUP_SIZE = 0x0000,
    DISABLE_SPEC_OP_ASSO = 0x0007FFFF,
    GROUP_VALID = 1,
    DISABLE_ALL_SPEC_OPS = 1
};
}

typedef fapi2::ReturnCode (*p9_sbe_mcs_setup_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

/// @brief This function configures MCS BAR registers on master SBE chip
/// to support of dcbz operation execution by HBI code and response
/// on lpc_ack preps
///
/// @param[in]     i_target   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_mcs_setup(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);
}

#endif
