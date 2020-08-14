/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbecmddump.C $                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
 * @file: ppe/sbe/sbefw/sbecmdgeneric.C
 *
 * @brief This file contains the SBE dump commands.
 *
 */

#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbe_build_info.H"
#include "sbeFifoMsgUtils.H"
#include "sbeFFDC.H"
#include "chipop_handler.H"
#include "sbecmddump.H"

uint32_t sbeGetDump( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeGetDump"
    SBE_DEBUG(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeGetDumpReq_t msg = {};
    sbeResponseFfdc_t ffdc;
    sbeFifoType type;

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Will attempt to dequeue an entry for the dump Type plus
        // the expected EOT entry at the end.
                uint32_t len2dequeue  = sizeof(msg)/sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult (len2dequeue, (uint32_t *)&msg, true, false, type);

        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

       //TODO: Enable the code once the sbeCollectDump class is ready.
#if 0
        //Create the sbeCollectDump object.
                sbeCollectDump dumpObj(msg.dumpType);

        //Call collectAllEntries.
        rc = collectAllHDCTEntries();
#endif

        if(rc)
        {
            SBE_ERROR(SBE_FUNC" Dump collection failed for dumpType 0x%08X",
                    msg.dumpType);
            respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                          SBE_SEC_GET_DUMP_FAILED);
            ffdc.setRc(rc);
        }
    } while(false);
    do
    {
        // Build the response header packet.
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);
        rc = sbeDsSendRespHdr(respHdr, &ffdc);
        // will let command processor routine handle the failure.
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
