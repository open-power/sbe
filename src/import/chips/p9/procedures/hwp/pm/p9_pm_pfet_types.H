/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/pm/p9_pm_pfet_types.H $    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/// @file p9_pm_pfet_types.H
///
/// *HWP HWP Owner: Greg Still  <stillgs@us.ibm.com>
/// *HWP FW  Owner: Sunil Kumar <skumar8j@in.ibm.com>
/// *HWP Team: PM
/// *HWP Level: 1
/// *HWP Consumed by: HS
///

#ifndef _P9_PM_PFETTYPE_H_
#define _P9_PM_PFETTYPE_H_

namespace PMPFETTYPE_C
{
// valid domain options
enum  pfet_dom_t
{
    BOTH,         // write to both domains
    ECO,          // eco only
    CORE,         // core only
};


// valid force options
enum  pfet_force_t
{
    NONE,          // no operation (00)
    VOFF,          // voff (01)
    VRET,          // Vret (10)... not supported
    VON,           // von (11)
    NO_FORCE_PARM, // use this when not writing to reg.
    VOFF_OVERRIDE
};

// valid read options
enum pfet_read_t
{
    PFET_READ_VOFF,
    PFET_READ_VREG,
    PFET_READ_VON,
    PFET_READ_VBETWEEN,
    PFET_READ_VOFFOVRD
};
}
#endif // _P8_PFETTYPE_H_
