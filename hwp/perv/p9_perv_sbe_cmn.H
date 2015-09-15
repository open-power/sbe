//------------------------------------------------------------------------------
/// @file  p9_perv_sbe_cmn.H
///
/// @brief Modules for scan 0 and array init
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


#ifndef _P9_PERV_SBE_CMN_H_
#define _P9_PERV_SBE_CMN_H_


#include <fapi2.H>


fapi2::ReturnCode p9_perv_sbe_cmn_array_init_module(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV> & i_target_chiplets,
        const fapi2::buffer<uint16_t> i_regions,
        const fapi2::buffer<uint64_t> i_loop_counter,
        const bool i_select_sram,
        const bool i_select_edram,
        const fapi2::buffer<uint64_t> i_start_abist_match_value);

fapi2::ReturnCode p9_perv_sbe_cmn_scan0_module(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV> & i_target_chiplets,
        const fapi2::buffer<uint16_t> i_regions,
        const fapi2::buffer<uint16_t> i_scan_types);

#endif
