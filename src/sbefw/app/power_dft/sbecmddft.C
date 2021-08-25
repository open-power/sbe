/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmddft.C $                         */
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
#include "sbeFifoMsgUtils.H"
#include "assert.h"
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbecmdcntrldmt.H"
#include "sbeglobals.H"

#include "sbecmddft.H"

#include "ipl.H"
#include "sbeConsole.H"
#include "sbeFFDC.H"

#include "fapi2.H"
#include <plat_hw_access.H>
#include <fapi2_attribute_service.H> // for FAPI_ATTR_GET
#include "sbe_spi_cmd.h"
#include "sbe_spi_cmd_secure_mode.h"

#include <p10_contained_run.H>


extern sbeRole g_sbeRole;

namespace SBEAPP_DFT
{
using namespace fapi2;

uint32_t sbeSeepromLoad (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeSeepromLoad "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeSeepromEngine_t req;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;

    do
    {
        len = sizeof( req )/sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult ( len, (uint32_t *)&req);
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL) //FIFO access issue
        {
            SBE_DEBUG(SBE_FUNC"FIFO dequeue failed, rc[0x%X]", rc);
            break;
        }

        //Chipop code goes here

        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();

        uint32_t brd        = req.brd;
        uint16_t rcv_dly    = 4;
        uint8_t spi_engine  = req.engine;
        uint16_t port       = 1;
        
        uint64_t l_seeprom_starting_addr = req.seeprom_start_addr;
		uint64_t l_seeprom_img_bytes = req.seeprom_img_size;

        uint32_t l_arry_size = l_seeprom_img_bytes ;
        // if the image is not 8 byte aligned, add byte buffer so it is
        if (l_seeprom_img_bytes % 8 != 0){
            l_arry_size = l_seeprom_img_bytes + ( 8 - (l_seeprom_img_bytes % 8));
        }

        SpiControlHandle handle = SpiControlHandle(proc, PIB, brd, rcv_dly, spi_engine, port, transparent, no_ecc_address_correction, mmSPIsm_disable);
        SpiInit(handle);

        // Set up OCC SRAM read address
        uint64_t l_addr_reg = 0x0006D070;
        uint64_t l_data_reg = 0x0006D075;
        uint64_t l_occ_start_addr = 0xFFF0100000000000;
        fapi2::buffer<uint64_t> o_data;
        fapi2::putScom(proc, l_addr_reg, l_occ_start_addr);

        fapi2::buffer<uint64_t> l_cplt_cnfg_data;
        uint64_t l_cplt_cnfg_addr = 0x010009;
        bool l_secure_mode = false;
		fapi2::getScom(proc, l_cplt_cnfg_addr, l_cplt_cnfg_data);
		l_secure_mode = l_cplt_cnfg_data.getBit<12>() == 1;
		
		uint8_t w_buffer[256];
        uint8_t r_buffer[256];
        bool l_false_compare = false;
        uint32_t l_page_size = 256;

        uint32_t i = 0, page_idx = 0, seeprom_addr = l_seeprom_starting_addr;
		// page_idx - offset of bytes to be written in a 256B page
		// seeprom_addr - offset of bytes from beginning addr of seeprom
		// i - offset of bytes to be written in given array size
        for ( ; i < l_arry_size && !l_false_compare; i++) {

            //Read OCC sram
            if (i % 8 == 0) { fapi2::getScom(proc, l_data_reg, o_data);FAPI_INF("o_data is now: %d", o_data);}

            w_buffer[page_idx] = (o_data >>((7-(i%8))*8)) & 0xff;

            //Write, read, compare
            if ( (page_idx % l_page_size == (l_page_size - 1)) || (i == (l_arry_size - 1))) {
                if (l_secure_mode){
					// Chunk functions breaks up 256B pages into 40B pages
					SpiWriteChunkSECURE(handle, seeprom_addr - page_idx, page_idx + 1, w_buffer, 0);
					SpiReadChunkSECURE(handle, seeprom_addr - page_idx, page_idx + 1, r_buffer, 0);
					FAPI_INF("FINISHED WRITING/READING SECURE, addr: %d, len: %d", seeprom_addr - page_idx, page_idx + 1);
				}
				else{
					SpiPageWrite(handle, seeprom_addr - page_idx, page_idx + 1, w_buffer);
					SpiRead(handle, seeprom_addr - page_idx, page_idx + 1, r_buffer, 0);
					FAPI_INF("FINISHED WRITING/READING, addr: %d, len: %d", seeprom_addr - page_idx, page_idx + 1);
				}

				FAPI_INF("COMPARING PAGE NUM %d, page size:%d", i/l_page_size, l_page_size);
                for(uint32_t x=0; x < (page_idx + 1) && !l_false_compare; x++){
                    if(r_buffer[x]!=w_buffer[x]){
                            FAPI_INF("ERROR: False compare on page-byte %d, read %d, expected %d", x, r_buffer[x], w_buffer[x]);
                            l_false_compare = true;
                    }
                }
                FAPI_INF("Completed a write, read, compare of %dB page to seeprom", l_page_size)
                page_idx = 0;
            }
            else {
                page_idx++;
            }
			seeprom_addr++;
        }

        for (uint8_t z=0; z < 100; z++){
            FAPI_INF("index:%d r_buffer:%x w_buffer:%x", z, r_buffer[z], w_buffer[z]);
        }
        FAPI_INF("ATTR_SEEPROM_IMG_SIZE: 0x%08x", l_seeprom_img_bytes);
        FAPI_INF("l_false_compare: %d", l_false_compare);
        FAPI_INF("l_arry_size: 0x%08x", l_arry_size);
        FAPI_INF("i: %x", i);
        FAPI_INF("seeprom_addr: %x", seeprom_addr);
        FAPI_INF("l_secure_mode: %d", l_secure_mode);
        FAPI_INF("l_page_size: %d", l_page_size);
        FAPI_INF("last scom out: 0x%08x", o_data);

        //End of chipop code
        if( l_false_compare )
        {
            SBE_DEBUG(SBE_FUNC" sbeSeepromLoad() failed with a false compare after reading and writing.");
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
        }

        FAPI_INF("sbeSeepromLoad Complete. On engine %d, brd %d", handle.engine, handle.brd);
        rc = sbeDsSendRespHdr(respHdr);
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeHandleRangeIpl (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeHandleRangeIpl "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint32_t len = 0;
    sbeIplRangeReqMsg_t req;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;

    // Dequeue data, then call sbeDoRangeIpl
    do
    {
        len = sizeof( req )/sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult ( len, (uint32_t *)&req);
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL) //FIFO access issue
        {
            SBE_DEBUG(SBE_FUNC"FIFO dequeue failed, rc[0x%X]", rc);
            break;
        }

        fapiRc = sbeDoRangeIpl( req.startMajor, req.startMinor, req.endMajor, req.endMinor );

        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_DEBUG(SBE_FUNC" sbeDoRangeIpl() failed.");
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
            ffdc.setRc(fapiRc);
        }

        rc = sbeDsSendRespHdr(respHdr);

    }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

ReturnCode sbeDoRangeIpl(const size_t i_startMajor,
                   const size_t i_startMinor,
                   const size_t i_endMajor,
                   const size_t i_endMinor)
{
    #define SBE_FUNC "sbeDoRangeIpl"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;

    size_t entry = 0;
    bool match = false;
    size_t minorStep = i_startMinor;

    do
    {
        // Set SBE state as IPLing
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                                SBE_PLCK_EVENT);
        SBE_DEBUG("IPL starting at %d.%d", i_startMajor, i_startMinor);
        SBE_DEBUG("IPL through %d.%d", i_endMajor, i_endMinor);
        // Run isteps
        for(size_t majorStep = i_startMajor; majorStep <= i_endMajor; majorStep++)
        {
            match = false;
            entry = 0;
            // Because the istep table does not start at step 0 (and may start at any step)
            // we must establish the index of each desired major step into the table
            // Loop through istep table to find the index of this step
            for(entry = 0; entry <= istepTable.len; entry++)
            {
                SBE_DEBUG("Searching for major step %04x", majorStep);
                SBE_DEBUG("Searching in entry %04x", entry);
                if (istepTable.istepMajorArr[entry].istepMajorNum == majorStep)
                {
                    SBE_DEBUG("Major step %d corresponds to entry %d", majorStep, entry);
                    match = true;
                    break;
                }
            }

            if (!match)
            {
                #if 0
                // Fail if we did not find the requested major step in the table
                SBE_DEBUG( SBE_FUNC"Major step %04x not found in istep table!", majorStep);
                (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                                            SBE_DUMP_FAILURE_EVENT);
                majorStep = i_endMajor;
                minorStep = i_endMinor;
                #endif
                SBE_DEBUG("No match for major step %04x", majorStep);
                continue;
            }

            SBE_DEBUG("Current entry: %d", entry);

            auto istepTableEntry = &istepTable.istepMajorArr[entry];
            SBE_DEBUG("Entering major step %04x", istepTableEntry->istepMajorNum);

            SBE_DEBUG("Length of this major step: %04x", istepTableEntry->len);
            // Complete the major step...
            //for(size_t step = i_startMinor; step <= istepTableEntry->len; step++)
            //for(size_t minorStep = i_startMinor; minorStep <= istepTableEntry->len; minorStep++)
            while (minorStep <= istepTableEntry->len)
            {
                SBE_DEBUG(f"minorStep: %04x", minorStep);
                SBE_DEBUG(f"target minor: %04x", i_endMinor);
                auto istepMap = &istepTableEntry->istepMinorArr[minorStep-1];
                if(istepMap->istepWrapper != NULL)
                {
                    SBE_DEBUG("performing istep %d.%d", istepTableEntry->istepMajorNum, minorStep);
                    rc = istepMap->istepWrapper(istepMap->istepHwp);
                }

                // ...unless requested to stop midway through the final step
                if ((majorStep == i_endMajor) && (minorStep == i_endMinor))
                // Perform the requested step, inclusive of the end step
                {
                    SBE_DEBUG("Reached end of IPL at step %d.%d", majorStep, minorStep);
                    majorStep = i_endMajor;
                    minorStep = i_endMinor;
                    break;
                }

                if(rc != FAPI2_RC_SUCCESS)
                {
                    SBE_DEBUG(SBE_FUNC"Failed istep execution in plck mode: "
                            "Major: %d, Minor: %d",
                            istepTableEntry->istepMajorNum, minorStep);

                    uint32_t secRc = SBE_PRI_GENERIC_EXECUTION_FAILURE;
                    //uint32_t secRc = checkstop ? SBE_SEC_SYSTEM_CHECKSTOP:
                    //                    SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;

                    captureAsyncFFDC(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                     secRc);
                    // exit outer loop as well
                    //entry = istepTable.len;
                    majorStep = i_endMajor;
                    minorStep = i_endMinor;
                    break;
                }

                minorStep++;

                (void)SbeRegAccess::theSbeRegAccess().updateSbeStep(
                                    istepTableEntry->istepMajorNum,
                                    minorStep);
#if 0
                // Check if we reached runtime
                if(SBE_STATE_RUNTIME ==
                                SbeRegAccess::theSbeRegAccess().getSbeState())
                {
                    // Exit outer loop as well
                    entry = istepTable.len;
                    break;
                }
#endif
            }
            // Start at the beginning of the next step
            // JDG NOTE: is this always valid?
            minorStep = 1;
        }
        FAPI_DBG("Exhausted major steps");
    } while(false);
    // Store l_rc in a global variable that will be a part of the SBE FFDC
//    g_iplFailRc = rc;
    SBE_EXIT(SBE_FUNC);
    #undef SBE_FUNC
    return rc;
}

uint32_t gpeStartDDS (uint8_t *i_pArg)
{
    //Polls SPRG0 until the GPE reports it is finished
    #define SBE_FUNC "sbeSeepromLoad "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    gpeStartDDS_t req;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;

    do
    {
        len = sizeof( req )/sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult ( len, (uint32_t *)&req);
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL) //FIFO access issue
        {
            SBE_DEBUG(SBE_FUNC"FIFO dequeue failed, rc[0x%X]", rc);
            break;
        }

        //Chipop code goes here

        FAPI_INF(">> Start gpeStartDDS chipop");
        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();

        fapi2::buffer<uint64_t> i_status;
        int64_t i_timeout_cnt = 0;
        FAPI_DBG("Polling SPRG0 to check status of DDS, polling for 100 second then timing out");
        fapi2::getScom(proc, 0x00060013, i_status);

        while ( (i_status & 0xFF) < 0x80)
        {
            fapi2::delay(100000, 50000); //100,000ns or 50k sim cycles
            fapi2::getScom(proc, 0x00060013, i_status);
            i_timeout_cnt ++;
            if ( i_timeout_cnt > 1000000 ) //1000000 * 100,000ns is 100 sec before timeout
            {
                SBE_DEBUG(SBE_FUNC"TIMEOUT: gpeStartDDS() failed. DDS did not halt in allotted time");
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
                break;
            }
        }

        FAPI_DBG("final status: %X", i_status);
        FAPI_INF("<< gpeStartDDS complete");

        //End of chipop code, fail parameters

        rc = sbeDsSendRespHdr(respHdr);
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}


//----------------------------------------------------------------------------
}
