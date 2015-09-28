/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/lib/p9_common_poweronoff.H $          */
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
///
/// @file  p9_common_poweronoff.H
/// @brief common procedure for power on/off
///
/// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
/// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
/// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
/// *HWP Team               : PM
/// *HWP Consumed by        : SBE:SGPE:CME
/// *HWP Level              : 2
///

#ifndef __P9_COMMON_POWERONOFF_H__
#define __P9_COMMON_POWERONOFF_H__

namespace p9power
{
enum powerOperation_t
{
    POWER_ON      = 0x0,
    POWER_OFF     = 0xFF,
    POWER_ON_VDD  = 0x1,
    POWER_OFF_VDD = 0xFE
};
}

// Define only address offset to be compatible with both core and cache domain
#define    PPM_PFCS      0x000f0118
#define    PPM_PFCS_CLR  0x000f0119
#define    PPM_PFCS_OR   0x000f011a
#define    PPM_PFDLY     0x000f011b
#define    PPM_PFSNS     0x000f011c
#define    PPM_PFOFF     0x000f011d

/// @typedef p9_common_poweronoff_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_common_poweronoff_FP_t) (
    const fapi2::Target < fapi2::TARGET_TYPE_EQ |
    fapi2::TARGET_TYPE_CORE > &,
    const p9power::powerOperation_t i_operation);

extern "C"
{
/// @brief common procedure for power on/off
///
/// @param [in] i_target    TARGET_TYPE_EQ|TARGET_TYPE_CORE target
/// @param [in] i_operation ENUM(ON,OFF)
///
/// @attr
/// @attritem ATTR_PFET_TIMING - EX target, uint32
///
/// @retval FAPI_RC_SUCCESS
    fapi2::ReturnCode
    p9_common_poweronoff(
        const fapi2::Target < fapi2::TARGET_TYPE_EQ |
        fapi2::TARGET_TYPE_CORE > & i_target,
        const p9power::powerOperation_t i_operation);

} // extern C

#endif  // __P9_COMMON_POWERONOFF_H__
