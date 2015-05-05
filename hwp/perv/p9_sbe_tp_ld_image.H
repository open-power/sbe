//------------------------------------------------------------------------------
/// @file  p9_sbe_tp_ld_image.H
///
/// @brief Proc SBE load Image
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


#ifndef _P9_SBE_TP_LD_IMAGE_H_
#define _P9_SBE_TP_LD_IMAGE_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_tp_ld_image_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief This procedure copies the .pibmem0 section of image from SEEPROM to the PIBMEM.
/// The pibmem0 section contains the PORE branch table (error handlers) used for the majority of the SEEPROM IPL as well as
/// performance sensitive routines such as the decompression-scan routine and the LCO loader.
/// Once the image is loaded then the error handlers are switched to
///
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_tp_ld_image(const
                                         fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
