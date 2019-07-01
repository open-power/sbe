/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/utils/p10_plat_ring_traverse.H $         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
#ifndef _PLAT_RING_TRAVERSE_H_
#define _PLAT_RING_TRAVERSE_H_

#include <p10_putRingUtils.H>

/// @brief This structure represents the layout of the Section-TOR
///        Section-TOR has the offsets to the different chiplet's
///        Common Ring section and Instance Ring section
struct SectionTOR
{
    uint32_t TOC_PERV_COMMON_RING;      // Offset of Perv Common Ring section
    uint32_t TOC_PERV_INSTANCE_RING;    // Offset of Perv Instance Ring section

    uint32_t TOC_N0_COMMON_RING;        // Offset of N0 Common Ring section
    uint32_t TOC_N0_INSTANCE_RING;      // Offset of N0 Instance Ring section

    uint32_t TOC_N1_COMMON_RING;        // Offset of N1 Common Ring section
    uint32_t TOC_N1_INSTANCE_RING;      // Offset of N1 Instance Ring section

    uint32_t TOC_PCI_COMMON_RING;       // Offset of PCI Common Ring section
    uint32_t TOC_PCI_INSTANCE_RING;     // Offset of PCI Instance Ring section

    uint32_t TOC_MC_COMMON_RING;        // Offset of MC Common Ring section
    uint32_t TOC_MC_INSTANCE_RING;      // Offset of MC Instance Ring section

    uint32_t TOC_PAU0_COMMON_RING;      // Offset of PAU0 Common Ring section
    uint32_t TOC_PAU0_INSTANCE_RING;    // Offset of PAU0 Instance Ring section

    uint32_t TOC_PAU1_COMMON_RING;      // Offset of PAU1 Common Ring section
    uint32_t TOC_PAU1_INSTANCE_RING;    // Offset of PAU1 Instance Ring section

    uint32_t TOC_PAU2_COMMON_RING;      // Offset of PAU2 Common Ring section
    uint32_t TOC_PAU2_INSTANCE_RING;    // Offset of PAU2 Instance Ring section

    uint32_t TOC_PAU3_COMMON_RING;      // Offset of PAU3 Common Ring section
    uint32_t TOC_PAU3_INSTANCE_RING;    // Offset of PAU3 Instance Ring section

    uint32_t TOC_AXON_COMMON_RING;      // Offset of AXON Common Ring section
    uint32_t TOC_AXON_INSTANCE_RING;    // Offset of AXON Instance Ring section

    uint32_t TOC_EQ_COMMON_RING;        // Offset of Quad Common Ring section
    uint32_t TOC_EQ_INSTANCE_RING;      // Offset of Quad Instance Ring section
};


///
/// @brief This is a plat specific (SBE Plat) function that locates the
///        Ring Container in the image and calls the functin to decompress the
///        RS4 string and apply it to the hardware.
/// @param[in] i_pImgPtr        points to SBE image
/// @param[in] i_target         The target of Ring apply.
/// @param[in] i_ringID         The Ring ID that identifies the ring to be applied.
/// @param[in] i_ringMode       different ring access operation mode
/// @param[in] i_opMode         operational mode of putring
/// @return FAPI2_RC_SUCCESS on success, else error code.
///
fapi2::ReturnCode lookUpRingSection(
    uint8_t* i_pImgPtr,
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    const RingId_t i_ringID,
    const fapi2::RingMode i_ringMode,
    RS4::OpMode_t    i_opMode       =   RS4::SCANNING_MODE );
///
/// @brief This is a plat pecific (SBE Plat) function that locates the
///        Ring Container in the image and calls the functin to decompress the
///        RS4 string and apply it to the hardware.
/// @param[in] i_target             The target of Ring apply.
/// @param[in] i_pChipletSectn      points to chiplet region of ring section.
/// @param[in] i_ringID             The Ring ID that identifies the ring to be applied.
/// @param[in] i_applyOverride      override is enabled or not
/// @param[in] i_ringMode           different ring access operation mode
/// @param[in] i_opMode             operational mode of putring
/// @return FAPI2_RC_SUCCESS on success, else error code.
///
fapi2::ReturnCode getRS4ImageFromTor(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    uint8_t* i_pChipletSectn ,
    const RingId_t i_ringID,
    bool i_applyOverride,
    const fapi2::RingMode i_ringMode,
    RS4::OpMode_t    i_opMode       =   RS4::SCANNING_MODE );

extern "C"
{

    typedef fapi2::ReturnCode( *p10_plat_ring_traverse_FP_t )
    ( uint8_t* i_pImgPtr,
      const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
      const RingId_t i_ringID,
      const fapi2::RingMode i_ringMode,
      RS4::OpMode_t i_opMode );
///
/// @brief     entry point for SBE ring traversal HWP.
/// @param[in] i_pImgPtr        points to SBE image
/// @param[in] i_target         The target of Ring apply.
/// @param[in] i_ringID         The Ring ID that identifies the ring to be applied.
/// @param[in] i_ringMode       different ring access operation mode
/// @param[in] i_opMode         operational mode of putring
/// @return FAPI2_RC_SUCCESS on success, else error code.
///
    fapi2::ReturnCode p10_plat_ring_traverse(
        uint8_t* i_pImgPtr,
        const fapi2::Target<fapi2::TARGET_TYPE_ALL>& i_target,
        const RingId_t i_ringID,
        const fapi2::RingMode i_ringMode,
        RS4::OpMode_t i_opMode );

}
#endif
