/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/z/ipl_table.C $                                 */
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
#include "core/ipl.H"

#include "sbecmdiplcontrol.H"
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
#include "assert.h"
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbecmdcntrldmt.H"
#include "sbeglobals.H"
// TODO Workaround
#include "plat_target_parms.H"

#include "fapi2.H"

#include "sbeXipUtils.H" // For getting hbbl offset
#include "sbeutil.H" // For getting SBE_TO_NEST_FREQ_FACTOR

#include "sbeSecureMemRegionManager.H"

// Forward declaration
using namespace fapi2;

// constants
static const uint32_t SBE_ROLE_MASK = 0x00000002;
static const uint32_t SBE_SYSTEM_QUIESCE_TIMEOUT_LOOP = 20000;

static const uint64_t SBE_LQA_DELAY_HW_US = 1000000ULL; // 1ms
static const uint64_t SBE_LQA_DELAY_SIM_CYCLES = 0x1ULL;

// Bit-33 used to checkstop the system, Since this is directly getting inserted
// will have to use bit (63-33) = 30th bit
static const uint64_t  N3_FIR_SYSTEM_CHECKSTOP_BIT = 30; // 63-33 = 30

// Externs

//Utility function to mask special attention
extern ReturnCode maskSpecialAttn( const Target<TARGET_TYPE_CORE>& i_target );




#ifndef __SBEFW_SEEPROM__
/*
 * --------------------------------------------- start PIBMEM CODE
 */

istepTableEntry_t istepTableEntries[] = {
   
};

REGISTER_ISTEP_TABLE(istepTableEntries)

//----------------------------------------------------------------------------

ReturnCode istepAttrSetup( voidfuncptr_t i_hwp)
{
    return performAttrSetup();
}
//----------------------------------------------------------------------------

ReturnCode istepWithProc( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    return rc;
}
//----------------------------------------------------------------------------

ReturnCode istepHwpTpSwitchGears( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepNestFreq( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepNestFreq "
    ReturnCode rc = FAPI2_RC_SUCCESS;
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------

ReturnCode istepSelectEx( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepWithEq( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepCacheInitf (voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepCacheInitf"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithCore( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithCore"
    ReturnCode rc = FAPI2_RC_SUCCESS;
    return rc;
    #undef SBE_FUNC
}
//----------------------------------------------------------------------------

ReturnCode istepWithEqConditional( voidfuncptr_t i_hwp)
{
    SBE_ENTER("istepWithEqCondtional");
    ReturnCode rc = FAPI2_RC_SUCCESS;
    SBE_EXIT("istepWithEqCondtional");
    return rc;
}
//----------------------------------------------------------------------------

ReturnCode istepWithCoreConditional( voidfuncptr_t i_hwp)
{
    SBE_ENTER("istepWithCoreCondtional");
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM > sysTgt;
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        uint8_t iplPhase = ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL;
        FAPI_ATTR_GET(ATTR_SYSTEM_IPL_PHASE, sysTgt, iplPhase);
        if( ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED == iplPhase )
        {
            break;
        }
        rc = istepWithCore(i_hwp);
     }while(0);
    SBE_EXIT("istepWithCoreCondtional");
    return rc;
}

//----------------------------------------------------------------------------
constexpr uint32_t HB_MEM_WINDOW_SIZE = 10*1024*1024; //10 MB
ReturnCode istepLoadBootLoader( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    return rc;
}

//----------------------------------------------------------------------------

ReturnCode istepStartInstruction( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    rc = istepWithCore(i_hwp);
    if(rc == FAPI2_RC_SUCCESS)
    {
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                SBE_RUNTIME_EVENT);
    }
    return rc;
}

//----------------------------------------------------------------------------
ReturnCode istepCheckSbeMaster( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepCheckSbeMaster "
    ReturnCode rc = FAPI2_RC_SUCCESS;
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepNoOp( voidfuncptr_t i_hwp)
{
    SBE_INFO("istepNoOp");
    return FAPI2_RC_SUCCESS ;
}

//----------------------------------------------------------------------------
ReturnCode istepLpcInit( voidfuncptr_t i_hwp)
{
    ReturnCode rc = FAPI2_RC_SUCCESS;
    return rc;
}
/*
 * end PIBMEMCODE -----------------------------------------------
 */
#endif // #ifndef __SBEFW_SEEPROM__

#ifdef __SBEFW_SEEPROM__
/*
 * --------------------------------------------- start SEEPROM CODE
 */
//----------------------------------------------------------------------------
ReturnCode istepWithCoreSetBlock( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithCoreSetBlock"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithCoreState( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithCoreState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepMpiplRstClrTpmBits( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepMpiplRstClrTpmBits"
    SBE_ENTER(SBE_FUNC);

    ReturnCode l_rc = performTpmReset();
    if( l_rc != FAPI2_RC_SUCCESS )
    {
        SBE_ERROR(SBE_FUNC" performTpmReset failed");
    }

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithExL2Flush( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithExL2Flush"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithExL3Flush( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithExL3Flush"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithProcSequenceDrtm( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepWithProcSequenceDrtm"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepStartMpipl( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepStartMpipl"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepMpiplQuadPoweroff( voidfuncptr_t i_hwp)
{
    #define SBE_FUNC "istepMpiplQuadPoweroff"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepWithProcQuiesceLQASet( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepWithProcQuiesceLQASet"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepMpiplSetFunctionalState( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepMpiplSetFunctionalState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode istepStopClockMpipl( voidfuncptr_t i_hwp )
{
    #define SBE_FUNC "istepStopClockMpipl"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;
    SBE_EXIT(SBE_FUNC);
    return l_fapiRc;
    #undef SBE_FUNC
}

/*
 * end SEEPROM CODE --------------------------------------------------
 */
#endif // ifdef __SBEFW_SEEPROM__
