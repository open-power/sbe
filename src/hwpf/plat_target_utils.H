/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_target_utils.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2020                        */
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
/**
 * @file plat_target_util.H
 * @brief platform utility definitions for fapi2 targets
 */

#ifndef __FAPI2_PLAT_TARGET_UTIL__
#define __FAPI2_PLAT_TARGET_UTIL__

#include <proc_sbe_fixed.H>

typedef struct {
  fapi2attr::SystemAttributes_t    G_system_attrs __attribute__ ((aligned (8)));
  fapi2attr::ProcChipAttributes_t  G_proc_chip_attrs __attribute__ ((aligned (8)));
  fapi2attr::PervAttributes_t      G_perv_attrs __attribute__ ((aligned (8)));
  fapi2attr::CoreAttributes_t      G_core_attrs __attribute__ ((aligned (8)));
  fapi2attr::EQAttributes_t        G_eq_attrs __attribute__ ((aligned (8)));
  fapi2attr::OCMBChipAttributes_t  G_ocmb_chip_attrs __attribute__ ((aligned (8)));
} G_sbe_attrs_t;
//
// Platform Utility functions..
//
namespace fapi2
{
    /// @brief Function to initialize the G_targets vector based on partial good
    ///      attributes
    ReturnCode plat_TargetsInit();

    /// @brief Function to initialize the G_targets vector for OCMB targets
    /// based on UpdateOCMBTarget chip-op. 
    void plat_OCMBTargetsInit(uint8_t * ocmbParams);

    /// @brief Initializes all platInit attributes (the values for which come
    // from the scratch registers)
    ReturnCode plat_AttrInit();

    /// @brief Function to apply any gard records set (via
    //  ATTR_EQ_GARD/ATTR_EC_GARD) to mark corresponding targets non functional
    ReturnCode plat_ApplyGards();

    /// @brief Function to initialize the G_targets vector with functional state
    ///        basis the PG Attribute
    ReturnCode plat_UpdateFunctionalState();

    /// @brief Function to initialize the G_targets vector based on partial good
    ///      attributes
    Target<TARGET_TYPE_PROC_CHIP> plat_getChipTarget();

    /// @brief Function to return a platform target handle, given the chiplet
    //         number and the fapi2 Target type
    //  @tparam K The fapi2 TargetType
    //  @param i_chipletNumber The chiplet number of the target
    //  @return Platform handle
    //  @note The caller can use the platform handle to construct a Target of
    //  it's choice. Ex:
    //  fapi2::Target<fapi2::TARGET_TYPE_CORE>
    //  l_core(plat_getTargetHandleByChipletNumber<fapi2::TARGET_TYPE_CORE>(0x20);
    template <TargetType K>
    plat_target_handle_t plat_getTargetHandleByChipletNumber(
            const uint8_t i_chipletNumber);

    /// @brief Function to return a platform target handle, given the target
    ///        instance number and the fapi2 Target type
    //  @tparam K The fapi2 TargetType
    //  @param i_targetNum The instance number for the target
    //  @return Platform handle
    //  @note The caller can use the platform handle to construct a Target of
    //  it's choice. Example:
    //  fapi2::Target<fapi2::TARGET_TYPE_MI>
    //  l_mi(plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_MI>(0);
    template <TargetType K>
    plat_target_handle_t plat_getTargetHandleByInstance(
            const uint8_t i_targetNum);


    /// @brief Function to return a OCMB platform target handle, given the target
    ///        instance number.
    //  @param i_targetNum The instance number for the target
    //  @return Platform handle
    template <TargetType K>
    plat_target_handle_t plat_getOCMBTargetHandleByInstance(
            const uint8_t i_targetNum);
}
#endif
