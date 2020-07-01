/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_i2c_access.H $                                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018,2020                        */
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
#pragma once

#include <plat_includes.H>
#include "fapi2.H"

namespace fapi2
{

  /**
   * @brief Platform-level implementation of getI2c()
   * @param[in]  target     FAPI HW target to operate on.
   * @param[in]  get_size   Size that getI2c will read from the HW target.
   * @param[in]  cfgData    Buffer that holds data to write to the HW target.
   *                        May be empty if no address/command is required
   *                        before the read.
   * @param[out] o_data     Buffer that holds data read from HW target.
   * @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
   */
  ReturnCode platGetI2c( const Target<TARGET_TYPE_ALL>& target,
                         const size_t get_size,
                         const std::vector<uint8_t>& cfgData,
                         std::vector<uint8_t>& o_data );

  /**
   * @brief Platform-level implementation of putI2c()
   * @param[in] target     FAPI HW target to operate on.
   * @param[in] data       Buffer that holds data to write to the HW target.
   * @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
   */
  ReturnCode platPutI2c( const Target<TARGET_TYPE_ALL>& target,
                         const std::vector<uint8_t>& data );

  /**
   * @brief Get scom over i2c interface
   * @param[in]  target      Target pointer
   * @param[in]  addr        address
   * @param[out] data        data pointer
   * @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
   */
  ReturnCode i2cGetScom( const void *target, const uint32_t addr, uint64_t *o_data);

  /**
   * @brief Put scom over i2c interface
   * @param[in]  target      Target pointer
   * @param[in]  addr        address
   * @param[in]  data        data to be written
   * @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
   */
  ReturnCode i2cPutScom( const void *target, const uint32_t addr, uint64_t data);

};
