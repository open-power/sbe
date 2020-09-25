/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbeCollectDump.C $                       */
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

#include "sbeCollectDump.H"
#include "fapi2.H"

using namespace fapi2;

//Constuctor for sbeCollectDump class.
sbeCollectDump::sbeCollectDump(uint32_t i_dumpID, sbeFifoType i_type)
{
    #define SBE_FUNC " sbeCollectDump "

    iv_dumpType = i_dumpID;
    iv_fifoType = i_type;

    //Get HDCT Xip section details
    iv_hdctXipSecDetails.init();

    //Parse the HDCT Xip section header
    iv_hdctSectionHdr = (hdctSectionHdr_t*)iv_hdctXipSecDetails.startAddr;
    SBE_DEBUG("EKB Commit ID: 0x%.8x%.8x ",
        ((iv_hdctSectionHdr->ekbCommitId & 0xFFFFFFFF00000000ull) >> 32),(iv_hdctSectionHdr->ekbCommitId & 0xFFFFFFFF));

    #undef SBE_FUNC
}

//collectAllHDCTEntries()
uint32_t sbeCollectDump::collectAllHDCTEntries()
{
    #define SBE_FUNC " collectAllHDCTEntries "
    SBE_ENTER(SBE_FUNC);

    SBE_EXIT(SBE_FUNC);
    return 0;
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::parserSingleHDCTEntry()
{
    #define SBE_FUNC " parserSingleHDCTEntry "
    SBE_ENTER(SBE_FUNC);


    SBE_EXIT(SBE_FUNC);
    return 0;
    #undef SBE_FUNC
}


//populateHDCTRowTOC()
uint32_t sbeCollectDump::populateHDCTRowTOC()
{
    #define SBE_FUNC " populateHDCTRowTOC "


    return 0;
    #undef SBE_FUNC
}

//collectSingleEntry()
uint32_t sbeCollectDump::collectSingleEntry(uint8_t * i_data, uint32_t i_len)
{
    return 0;
}

//writeDatatoFIFO()
uint32_t sbeCollectDump::writeDataToFIFO(uint8_t * i_data, uint32_t i_len)
{
    return 0;
}

//checkHWStateofChipAndChiplets()
bool sbeCollectDump::checkHWStateofChipAndChiplets()
{
    return 0;
}
