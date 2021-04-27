/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_target_utils.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2021                        */
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
  fapi2attr::PMICAttributes_t      G_pmic_attrs __attribute__ ((aligned (8)));
  fapi2attr::GI2CAttributes_t      G_gi2c_attrs __attribute__ ((aligned (8)));


} G_sbe_attrs_t;
//
// Platform Utility functions..
//
namespace fapi2
{
    enum psuConfigVersion
    {
        SBE_PSU_I2C_CONFIG_VERSION_1 = 0x01,
        SBE_PSU_I2C_CONFIG_VERSION_2 = 0x02,
        SBE_PSU_I2C_CONFIG_VERSION_LATEST = SBE_PSU_I2C_CONFIG_VERSION_1,
        SBE_PSU_I2C_CONFIG_TYPES_SUPPORTED = 0x03, // see SbeMemConfigData_t
    };

    enum SbeDimmTargetTypes
    {
        DIMM_OCMB_TARGET = 0x00,
        DIMM_PMIC_TARGET = 0x01,
        DIMM_GI2C_TARGET = 0x02
    };

    typedef struct __attribute__((packed))
    {
        uint8_t i2c_port;
        uint8_t i2c_engine;
        uint8_t i2c_devAddr;
        uint8_t i2c_functional;
        uint8_t i2c_present;
        uint8_t i2c_reserved[3];
    }SbeI2cDevice_t;

    typedef struct __attribute__((packed))
    {
        uint16_t i2c_config_version;

        // How many unique types we support today ?
        // TYPE_PMIC+TYPE_GENERIC_I2C_DEVICE+TYPE_OCMB_CHIP = 3
        // SBE_PSU_I2C_CONFIG_TYPES_SUPPORTED
        uint16_t i2c_config_types_supported;  // pmic gi2c ocmb

        // FAPI_POS will be used as an index into the arrays created below

        // TYPE_PMIC
        uint16_t pmic_chips_max_number;
        SbeI2cDevice_t pmic_chips[PMIC_TARGET_COUNT];

        // TYPE_GENERIC_I2C_DEVICE
        uint16_t gi2c_chips_max_number;
        SbeI2cDevice_t gi2c_chips[GI2C_TARGET_COUNT];

        // TYPE_OCMB_CHIP
        uint16_t ocmb_chips_max_number;
        SbeI2cDevice_t ocmb_chips[OCMB_TARGET_COUNT];
    }SbeMemConfigData_t;

    /// @brief Function to initialize the G_targets vector based on partial good
    ///      attributes
    ReturnCode plat_TargetsInit();

    /// @brief Function to initialize the G_targets vector for OCMB targets
    /// based on UpdateOCMBTarget chip-op.
    void plat_OCMBTargetsInit(uint8_t * ocmbParams);

    /// @brief Function to initialize the G_targets vector for DIMM related
    /// targets based on sbePsuMemConfig chip-op.
    void plat_DIMMTargetInit(uint8_t instance_id,
                            SbeDimmTargetTypes targType,
                            SbeI2cDevice_t& devConfig);

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
