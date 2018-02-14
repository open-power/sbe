/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/z/chipop_table.C $                              */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2018                        */
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
#include "sbecmdscomaccess.H"
#include "sbecmdiplcontrol.H"
#include "sbecmdgeneric.H"
#include "sbecmdmemaccess.H"
#include "sbecmdregaccess.H"
#include "sbecmdcntrldmt.H"
#include "sbecmdringaccess.H"
#include "sbecmdsram.H"
#include "sbecmdcntlinst.H"
#include "sbecmdringaccess.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeHostMsg.H"
#include "sbe_host_intf.H"
#include "sbestates.H"
#include "sberegaccess.H"
#include "sbecmdmpipl.H"
#include "sbecmdtracearray.H"
#include "sbecmdCntrlTimer.H"
#include "sbecmdfastarray.H"

#include "core/chipop_handler.H"

#include "power/istep.H"

static const uint16_t HARDWARE_FENCED_STATE =
     SBE_FENCE_AT_CONTINUOUS_IPL|SBE_FENCE_AT_DMT;

static const uint16_t PUT_HARDWARE_FENCED_STATE =
     HARDWARE_FENCED_STATE|SBE_FENCE_AT_MPIPL;

////////////////////////////////////////////////////////////////
// @brief g_sbeScomCmdArray
////////////////////////////////////////////////////////////////
CMD_ARR(
    A2,
    {SBE_COMMON::sbeGetScom,
     SBE_CMD_GETSCOM,
     HARDWARE_FENCED_STATE,
    },
    {SBE_COMMON::sbePutScom,
     SBE_CMD_PUTSCOM,
     HARDWARE_FENCED_STATE,
    },
    {SBE_COMMON::sbeModifyScom,
     SBE_CMD_MODIFYSCOM,
     HARDWARE_FENCED_STATE,
    },
    {SBE_COMMON::sbePutScomUnderMask,
     SBE_CMD_PUTSCOM_MASK,
     HARDWARE_FENCED_STATE,
    },
    {SBE_COMMON::sbeMultiScom,
     SBE_CMD_MULTISCOM,
     HARDWARE_FENCED_STATE,
    }
)

////////////////////////////////////////////////////////////////
// @brief g_sbeIplControlCmdArray
//
////////////////////////////////////////////////////////////////
/*CMD_ARR(
    A1,
    {SBEAPP_POWER::sbeHandleIstep,
     SBE_CMD_EXECUTE_ISTEP,
     HARDWARE_FENCED_STATE|SBE_FENCE_AT_DUMPING,
    },
    {sbeHandleSuspendIO,
     SBE_CMD_SUSPEND_IO,
     SBE_FENCE_AT_DUMPING,
    }
)*/



// Mandatory macro inclusion
CMD_CLASS_DEFAULT_INTIALISATION
