/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/istep.C $                             */
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
#include <stdint.h>

#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
#include "assert.h"
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbecmdcntrldmt.H"
#include "sbeglobals.H"

#include "core/ipl.H"

extern sbeRole g_sbeRole;
static const uint8_t SLAVE_LAST_MINOR_ISTEP = 22;

namespace SBEAPP_POWER
{
using namespace fapi2;
//----------------------------------------------------------------------------
bool validateIstep (const uint8_t i_major, const uint8_t i_minor)
{
    #define SBE_FUNC "validateIstep "
    bool valid = false;
    do
    {
        if( 0 == i_minor )
        {
            break;
        }
        // istep 2.1 loads image to PIBMEM
        // So SBE control loop can not execute istep 2.1.
        if(( i_major == 2 ) && ( i_minor == 1) )
        {
            break;
        }

        if( SBE_ROLE_SLAVE == g_sbeRole )
        {
            if(( i_major == 3 && i_minor > SLAVE_LAST_MINOR_ISTEP ) ||
               ( i_major > 3 ))
            {
                break;
            }
        }

        uint32_t prevMajorNumber =
                    SbeRegAccess::theSbeRegAccess().getSbeMajorIstepNumber();
        uint32_t prevMinorNumber =
                    SbeRegAccess::theSbeRegAccess().getSbeMinorIstepNumber();
        SBE_INFO(SBE_FUNC"prevMajorNumber:%u prevMinorNumber:%u ",
                         prevMajorNumber, prevMinorNumber );
        if( 0 == prevMajorNumber )
        {
            prevMajorNumber = 2;
            prevMinorNumber = 1;
        }

        uint32_t nextMajorIstep = prevMajorNumber;
        uint32_t nextMinorIstep = prevMinorNumber + 1;

        for(size_t entry = 0; entry < istepTable.len; entry++)
        {
            auto istepTableEntry = &istepTable.istepMajorArr[entry];
            if( istepTableEntry->istepMajorNum == prevMajorNumber)
            {
                if( prevMinorNumber == istepTableEntry->len )
                {
                    nextMajorIstep = prevMajorNumber + 1;
                    nextMinorIstep =  1;
                }
            }
            if( i_major == istepTableEntry->istepMajorNum )
            {
                if( i_minor > istepTableEntry->len )
                {
                    break;
                }
                // If secuirty is not enabled, no further chacks asre required
                if( !SBE_GLOBAL->sbeFWSecurityEnabled)
                {
                    valid = true;
                    break;
                }

                if( ( i_major != nextMajorIstep) ||
                    ( i_minor != nextMinorIstep) )
                {
                    SBE_ERROR("Secuity validation failed for executing istep "
                           "Skipping istep or MPIPL via istep not allowed "
                           "in secure mode. nextMajorIstep:%u "
                           "nextMinorIstep:%u", nextMajorIstep, nextMinorIstep);
                    break;
                }
                valid = true;
                break;
            }
        }
    } while(0);

    return valid;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbeHandleIstep (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeHandleIstep "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint32_t len = 0;
    sbeIstepReqMsg_t req;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;

    // NOTE: In this function we will have two loops
    // First loop will deque data and prepare the response
    // Second response will enque the data on DS FIFO
    //loop 1
    do
    {
        len = sizeof( req )/sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult ( len, (uint32_t *)&req);
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL) //FIFO access issue
        {
            SBE_ERROR(SBE_FUNC"FIFO dequeue failed, rc[0x%X]", rc);
            break;
        }

        fapiRc = sbeExecuteIstep( req.major, req.minor );
        // TODO - F001A is not available till istep 2.3, which is driven by the
        // nest clock, so we can enable this only after 2.3, For time being
        // commenting this out.
        //bool checkstop = isSystemCheckstop();
        if( fapiRc != FAPI2_RC_SUCCESS )
        //if(( fapiRc != FAPI2_RC_SUCCESS ) || (checkstop))
        {
            SBE_ERROR(SBE_FUNC" sbeExecuteIstep() Failed. major:0x%08x"
                " minor:0x%08x", (uint32_t)req.major, (uint32_t)req.minor);
            //if(checkstop)
            //{
            //    respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
            //                       SBE_SEC_SYSTEM_CHECKSTOP );
            //}
            //else
            //{
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
                ffdc.setRc(fapiRc);
            //}

            break;
        }

    }while(0);

    //loop 2
    do
    {
        // FIFO error
        if ( rc )
        {
            break;
        }

        rc = sbeDsSendRespHdr(respHdr, &ffdc);
    }while(0);

    if( rc )
    {
        SBE_ERROR( SBE_FUNC"Failed. rc[0x%X]", rc);
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

} // namespace SBEAPP_POWER
