//------------------------------------------------------------------------------
/// @file  p9_sbe_startclock_chiplets.H
///
/// @brief Start clock procedure for XBUS, OBUS, PCIe
//------------------------------------------------------------------------------
// *HWP HWP Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HWP Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner         : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team             : Perv
// *HWP Level            : 1
// *HWP Consumed by      : SBE
//------------------------------------------------------------------------------


#ifndef _P9_SBE_STARTCLOCK_CHIPLETS_H_
#define _P9_SBE_STARTCLOCK_CHIPLETS_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_startclock_chiplets_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV> &);

/// @brief Start Xbus, Obus, PCIe clocks
/// Start clocks on configured chiplets for all chips (master and slaves)
///
/// @param[in]     i_target_chiplets   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_startclock_chiplets(const
            fapi2::Target<fapi2::TARGET_TYPE_PERV> & i_target_chiplets);
}

#endif
