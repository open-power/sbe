/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_setup_boot_freq.H $       */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */

///
/// @file  p9_sbe_setup_boot_freq.H
/// @brief Setup Boot Frequency
///
/// *HW Owner    : Sudheendra K Srivathsa <sudheendraks@in.ibm.com>
/// *FW Owner    : Sangeetha T S <sangeet2@in.ibm.com>
/// *Team        : PM
/// *Consumed by : SBE
/// *Level       : 2
///

#ifndef __P9_SBE_SETUP_BOOT_FREQ_H__
#define __P9_SBE_SETUP_BOOT_FREQ_H__

/// @typedef p9_sbe_setup_boot_freq_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_sbe_setup_boot_freq_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

extern "C"
{

/// @brief Read an attribute containing the boot frequency and set that
///        into each configured EQ chiplet.
/// @param [in] i_target TARGET_TYPE_PROC_CHIP
/// @attr
/// @attritem ATTR_BOOT_FREQ_MULT - 11 bit frequency multiplier of refclk
/// @return FAPI2_RC_SUCCESS
    fapi2::ReturnCode
    p9_sbe_setup_boot_freq(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

} // extern C

#endif  // __P9_SBE_SETUP_BOOT_FREQ_H__
