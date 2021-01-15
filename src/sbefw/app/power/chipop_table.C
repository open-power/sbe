/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/chipop_table.C $                          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2021                        */
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
#include "sbecmdscomaccess.H"
#include "sbecmdiplcontrol.H"
#include "sbecmdflushnvdimm.H"
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
#include "sbecmdexitcachecontained.H"
#include "chipop_handler.H"
#include "sbecmdgeneric.H"
#include "istep.H"
#include "sbecmddump.H"
#include "sbehostfifomsg.H"

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
CMD_ARR(
    A1,
    {SBEAPP_POWER::sbeHandleIstep,
     SBE_CMD_EXECUTE_ISTEP,
     HARDWARE_FENCED_STATE|SBE_FENCE_AT_DUMPING,
    },
    {sbeHandleSuspendIO,
     SBE_CMD_SUSPEND_IO,
     SBE_FENCE_AT_DUMPING,
    },
    {sbeHandleFlushNVDIMM,
     SBE_CMD_FLUSH_NVDIMM,
     HARDWARE_FENCED_STATE|SBE_FENCE_AT_DUMPING,
    }
)

////////////////////////////////////////////////////////////////
// @brief g_sbeGenericCmdArray
//
////////////////////////////////////////////////////////////////
CMD_ARR(
    A8,
    {sbeGetCapabilities,
     SBE_CMD_GET_SBE_CAPABILITIES,
     SBE_NO_FENCE,
    },

    {sbeGetFfdc,
     SBE_CMD_GET_SBE_FFDC,
     SBE_NO_FENCE,
    },

    {sbeFifoQuiesce,
     SBE_CMD_QUIESCE,
     SBE_NO_FENCE,
    }
)

//////////////////////////////////////////////////////////////
// @brief g_sbeMemoryAccessCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    A4,
    {sbeGetMem,
     SBE_CMD_GETMEM,
     SBE_FENCE_AT_CONTINUOUS_IPL,
    },

    {sbePutMem,
     SBE_CMD_PUTMEM,
     PUT_HARDWARE_FENCED_STATE,
    },

    {sbeGetSram,
     SBE_CMD_GETSRAM,
     HARDWARE_FENCED_STATE,
    },

    {sbePutSram,
     SBE_CMD_PUTSRAM,
     PUT_HARDWARE_FENCED_STATE,
    }
)

//////////////////////////////////////////////////////////////
// @brief g_sbeInstructionCntlCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    A7,
    {sbeCntlInst,
     SBE_CMD_CONTROL_INSTRUCTIONS,
     HARDWARE_FENCED_STATE,
    }
)

//////////////////////////////////////////////////////////////
// @brief g_sbeRegAccessCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    A5,
    {sbeGetReg,
     SBE_CMD_GETREG,
     HARDWARE_FENCED_STATE,
    },
    {sbePutReg,
     SBE_CMD_PUTREG,
     PUT_HARDWARE_FENCED_STATE | SBE_FENCE_AT_SECURE_MODE,
    },
    {sbeGetHWReg,
     SBE_CMD_GETHWREG,
     HARDWARE_FENCED_STATE,
    },
    {sbePutHWReg,
     SBE_CMD_PUTHWREG,
     HARDWARE_FENCED_STATE,
    }
)

//////////////////////////////////////////////////////////////
// @brief g_sbeMpiplCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    A9,
    {sbeEnterMpipl,
     SBE_CMD_MPIPL_ENTER,
     PUT_HARDWARE_FENCED_STATE|SBE_FENCE_AT_ISTEP|
     SBE_FENCE_AT_DUMPING,
     // Allow Fspless system to enter MPIPL
     // Issue 157287
    },

    {sbeContinueMpipl,
     SBE_CMD_MPIPL_CONTINUE,
     HARDWARE_FENCED_STATE|SBE_FENCE_AT_ISTEP|
     SBE_FENCE_AT_RUNTIME|SBE_FENCE_AT_DUMPING,
     // Only allowed State is MPIPL
    },

    {sbeStopClocks,
     SBE_CMD_MPIPL_STOPCLOCKS,
     HARDWARE_FENCED_STATE|SBE_FENCE_AT_DUMPING,
    },

    {sbeGetTIInfo,
     SBE_CMD_GET_TI_INFO,
     PUT_HARDWARE_FENCED_STATE,
    }
)

//////////////////////////////////////////////////////////////
// @brief g_sbeRingAccessCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    A3,
    {sbeGetRing,
     SBE_CMD_GETRING,
     SBE_FENCE_AT_CONTINUOUS_IPL|SBE_FENCE_AT_QUIESCE,
    },

    {sbePutRing,
     SBE_CMD_PUTRING,
     HARDWARE_FENCED_STATE|SBE_FENCE_AT_QUIESCE,
    }
)

////////////////////////////////////////////////////////////////
// @brief g_sbeArrayAccessCmdArray[]
//
////////////////////////////////////////////////////////////////
CMD_ARR(
    A6,
    {sbeControlFastArray,
     SBE_CMD_CONTROL_FAST_ARRAY,
     SBE_FENCE_AT_QUIESCE,
    },
    {sbeControlTraceArray,
     SBE_CMD_CONTROL_TRACE_ARRAY,
     SBE_FENCE_AT_QUIESCE,
    }
)

//////////////////////////////////////////////////////////////
//// @brief g_sbeDumpCmdArray
////
////////////////////////////////////////////////////////////////
CMD_ARR(
    AA,
    {sbeGetDump,
     SBE_CMD_GET_DUMP,
     SBE_NO_FENCE,
    }
)

//////////////////////////////////////////////////////////////
//// @brief g_sbeHostFifoGenericCmdArray
////
////////////////////////////////////////////////////////////////
CMD_ARR(
    AB,
    {sbeHostHaltReq,
     SBE_CMD_HALT,
     SBE_NO_FENCE,
    }
)

//////////////////////////////////////////////////////////////
// @brief g_sbeCoreStateControlCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    D1,
   {sbeControlDeadmanTimer,
     SBE_PSU_CMD_CONTROL_DEADMAN,
     SBE_FENCE_AT_CONTINUOUS_IPL|SBE_FENCE_AT_QUIESCE|
     SBE_FENCE_AT_MPIPL|SBE_FENCE_AT_ISTEP|
     SBE_FENCE_AT_DUMPING,
    },
    {sbePsuExitCacheContainedMode,
     SBE_PSU_EXIT_CACHE_CONTAINED_MODE,
     SBE_FENCE_AT_CONTINUOUS_IPL|SBE_FENCE_AT_QUIESCE|
     SBE_FENCE_AT_MPIPL|SBE_FENCE_AT_ISTEP|
     SBE_FENCE_AT_DUMPING,
    }

)

//////////////////////////////////////////////////////////////
// @brief g_sbeControlTimerCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    D4,
   {sbeCntrlTimer,
     SBE_PSU_CMD_CONTROL_TIMER,
     SBE_FENCE_AT_CONTINUOUS_IPL|SBE_FENCE_AT_QUIESCE|
     SBE_FENCE_AT_MPIPL|SBE_FENCE_AT_DUMPING,
    }
)

//////////////////////////////////////////////////////////////
// @brief g_sbePutRingFromImageCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    D3,
   {sbePutRingFromImagePSU,
     SBE_PSU_MSG_PUT_RING_FROM_IMAGE,
     PUT_HARDWARE_FENCED_STATE|SBE_FENCE_AT_QUIESCE,
    }
)
//////////////////////////////////////////////////////////////
// @brief g_sbeRegisterAccessCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    D5,
   {sbePsuGetHWReg,
     SBE_PSU_CMD_GETHWREG,
     PUT_HARDWARE_FENCED_STATE,
    }
)

//
//////////////////////////////////////////////////////////////
// @brief g_sbePsuGenericCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    D6,
   {sbeUpdateMemAccessRegion,
     SBE_PSU_MSG_UPDATE_MEM_REGION,
     SBE_FENCE_AT_QUIESCE,
    }
)

//////////////////////////////////////////////////////////////
// @brief g_sbePsuGenericCmdArray
//
//////////////////////////////////////////////////////////////
CMD_ARR(
    D7,
    {sbePsuGetCapabilities,
     SBE_PSU_GENERIC_MSG_GET_CAPABILITIES,
     SBE_NO_FENCE,
    },

    {sbeReadMem,
     SBE_PSU_GENERIC_MSG_READ_SBE_MEM,
     SBE_NO_FENCE,
    },

    {sbeSetFFDCAddr,
     SBE_PSU_GENERIC_MSG_SET_FFDC_ADDR,
     SBE_NO_FENCE,
    },

    {sbePsuQuiesce,
     SBE_PSU_GENERIC_MSG_QUIESCE,
     SBE_NO_FENCE,
    },

    {sbeSetSystemFabricMap,
     SBE_PSU_GENERIC_MSG_SYSTEM_FABRIC_MAP,
     SBE_NO_FENCE,
    },

    {sbeStashKeyAddrPair,
     SBE_PSU_GENERIC_MSG_STASH_MPIPL_CONFIG,
     SBE_NO_FENCE,
    },

    {sbeSecurityListBinDump,
     SBE_PSU_GENERIC_MSG_SECURITY_LIST_BIN_DUMP,
     SBE_NO_FENCE,
    },

    {sbeUpdateOCMBTarget,
     SBE_PSU_GENERIC_MSG_UPDATE_OCMB_TARGET,
     SBE_NO_FENCE,
    }
)

// Mandatory macro inclusion
CMD_CLASS_DEFAULT_INTIALISATION
