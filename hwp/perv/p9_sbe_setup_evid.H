///
/// @file  p9_sbe_setup_evid.H
/// @brief Setup External Voltage IDs and Boot Frequency
///
/// *HW Owner    : Greg Still <stillgs@us.ibm.com>
/// *FW Owner    : TBD
/// *Team        : Power Management
/// *Consumed by : SBE
/// *Level       : 1
///

#ifndef __P9_SBE_SETUP_EVID_H__
#define __P9_SBE_SETUP_EVID_H__

using fapi2::ReturnCode;
using fapi2::TARGET_TYPE_PROC_CHIP;
using fapi2::Target;

extern "C"
{

/// @typedef p9_sbe_setup_evid_FP_t
/// function pointer typedef definition for HWP call support
typedef ReturnCode (*p9_sbe_setup_evid_FP_t) (
     const fapi2::Target<TARGET_TYPE_PROC_CHIP> &);

/// @brief Read attributes containing this part's boot voltages (VDD, VCS and VDN)
///        and set these voltage using the AVSBUS interface (VDD, VCS) an I2C (VDN).
///        Also reads a differnt attribute containing the boot frequency and set that
///        into each configured EQ chiplet.
/// @param [in] i_target TARGET_TYPE_PROC_CHIP
/// @attr
/// @attritem   ATTR_BOOT_FREQ          uint16_t - 9 bit frequency multiplier of the refclk right justified
/// @attritem   ATTR_VCS_BOOT_VOLTAGE   uint16_t - 1mV grandularity setting for the VCS rail
/// @attritem   ATTR_VDD_BOOT_VOLTAGE   uint16_t - 1mV grandularity setting for the VDD rail
/// @attritem   ATTR_VDN_BOOT_VOLTAGE   uint16_t - 1mV grandularity setting for the VDN rail
///
/// @retval FAPI_RC_SUCCESS
ReturnCode
p9_sbe_setup_evid(const Target<TARGET_TYPE_PROC_CHIP>& i_target);


} // extern C

#endif  // __P9_SBE_SETUP_EVID_H__
