/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbearchregdump.H $                    */
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

#ifndef __SBEFW_ARCHREGDUMP_H
#define __SBEFW_ARCHREGDUMP_H

#include <stdint.h>
#include "fapi2.H"

/**
 * @brief This function dumps all the SPRs/GPRs associated with proc's core
 * in a location provided by Hostboot's stash addr. Internally this method
 * takes care if ramming is possible for a core-thread for spr/gpr read and
 * dump the same in the format agreed via PBA
 *
 * @param[in] void
 *
 * @return FAPI2_RC_SUCCESS if success, else error code
 */
fapi2::ReturnCode sbeDumpArchRegs(void);

#endif /* __SBEFW_ARCHREGDUMP_H */
