/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/ppe/boltonlib/gpe/gpe.h $         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018,2019                        */
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
#ifndef __GPE_H__
#define __GPE_H__

/// \file pgp.h
/// \brief The GPE environment for PK.

// This is a 'circular' reference in PK, but included here to simplify PGAS
// programming.

#ifndef HWMACRO_GPE
    #define HWMACRO_GPE
    #include "ppe42.h"
#endif

#ifndef PBASLVCTLN
    #define PBASLVCTLN 0
#endif

#include "ocb_register_addresses.h"
#include "gpe_common.h"

#include "pba_register_addresses.h"


#endif  /* __GPE_H__ */
