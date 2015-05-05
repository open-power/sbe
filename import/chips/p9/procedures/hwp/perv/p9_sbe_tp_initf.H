/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_tp_initf.H $              */
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
/// @file  p9_sbe_tp_initf.H
///
/// @brief TP chiplet scaninits for the TP rings
// *!
// *! OWNER NAME  : Abhishek Agarwal  Email: abagarw8@in.ibm.com
// *! BACKUP NAME :                   Email:
//------------------------------------------------------------------------------
// *HWP HWP Owner   : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP FW Owner    : Brian Silver <bsilver@us.ibm.com>
// *HWP Team        : Perv
// *HWP Level       : 1
// *HWP Consumed by : SBE
//------------------------------------------------------------------------------


#ifndef _P9_SBE_TP_INITF_H_
#define _P9_SBE_TP_INITF_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_tp_initf_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

/// @brief -- This doesn't include the gptr/time/repair rings,
/// -- since they are scanned in tp_chiplet_init2.
/// -- This doesn't include the net/pib/fuse rings,
/// -- since they are used by the SBE hardware itself.
///
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_tp_initf(const
                                      fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);
}

#endif
