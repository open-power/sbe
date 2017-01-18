/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbefapiutil.H $                                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
/*
 * @file: ppe/src/sbefw/sbefapiutil.H
 *
 * @brief This file contains the SBE FAPI Utility
 *
 */

#ifndef __SBEFW_SBEFAPIUTIL_H
#define __SBEFW_SBEFAPIUTIL_H

#include <stdint.h>
#include "sbe_sp_intf.H"
#include "sbeutil.H"
#include "fapi2.H"
#include "plat_hw_access.H"

/* @brief - Map SBE target Types to Fapi Target Types
 *
 * @param[in] - i_sbeTargetType     SBE target type
 * @param[in] - i_chipletId         Chiplet Id
 *
 * @return    - fapi target type
 */
fapi2::TargetType sbeGetFapiTargetType(const uint16_t i_sbeTargetType,
                const uint16_t i_chipletId);

/*@brief - create fapi target handle for the target type
 *
 * @param[in] - i_taretType - sbe target type
 * @param[in] - i_chipletId - chiplet id
 * @param[out] - o_tgtHndl - fapi target handle
 * @param[in] - i_fapiTargetMask - expected fapi targets, by default
 *                            no masking
 *
 * @return - true - on success
 *           false - on failure
 */
bool sbeGetFapiTargetHandle(const uint16_t i_targetType,
                            const uint16_t i_chipletId,
                            fapi2::plat_target_handle_t &o_tgtHndl,
                            const fapi2::TargetType i_fapiTargetMask =
                                                fapi2::TARGET_TYPE_ALL);

/*@brief - Map sbe ring access modes to fapi ring access modes
 *
 * @param[in] - i_ringMode - sbe ring access mode
 *
 * @return - l_fapiRingMode - fapi ring mode
 */
uint16_t sbeToFapiRingMode(uint16_t i_ringMode);


#endif /* __SBEFW_SBEFAPIUTIL_H */
