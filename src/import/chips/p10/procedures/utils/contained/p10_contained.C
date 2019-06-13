/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained.C $ */
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

#include <fapi2.H>
#include <p10_contained.H>

fapi2::ReturnCode get_contained_run_mode(bool& io_is_runn)
{
    fapi2::ATTR_RUNN_MODE_Type tmp;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_MODE, SYS, tmp));

    io_is_runn = tmp == fapi2::ENUM_ATTR_RUNN_MODE_ON;

fapi_try_exit:
    return fapi2::current_err;
}

fapi2::ReturnCode get_contained_ipl_type(bool& io_is_cache_contained,
        bool& io_is_chip_contained)
{
    fapi2::ATTR_CONTAINED_IPL_TYPE_Type tmp;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, SYS, tmp));

    io_is_cache_contained = tmp == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CACHE;
    io_is_chip_contained = tmp == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CHIP;

fapi_try_exit:
    return fapi2::current_err;
}
