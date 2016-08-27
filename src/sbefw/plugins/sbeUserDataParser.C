/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/plugins/sbeUserDataParser.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
#include <sys/stat.h>
#include <errno.h>

#include <cstdlib>
#include <cstring>
#include <sstream>

#include <utilfile.H>
#include <utilmem.H>

#include "sbeFFDCType.H"

#define SBE_TEMP_DUMP_FILE              "/tmp/trace_dump.bin"
#define SBE_TRACE_BIN                   "/tmp/sbe_trace.bin"
#define PPE2FSP_TOOL                    "ppe2fsp"
#define P9_XIP_TOOL                     "p9_xip_tool"
#define SBE_TRACE_CMD                   "fsp-trace -s"
#define SBE_TRACE_HEADLINE              "SBE Traces"
#define SBE_ATTR_DUMP_HEADLINE          "SBE Attributes Dump"
#define P9_XIP_ATTR_CMD                 "-ifs attrdump"
#define SBE_STRING_FILE                 "sbeStringFile"
#define SBE_SEEPROM_BIN                 "sbe_seeprom.bin"

#define SBE_PARSER_MAX_LOCAL_BUFFER     8192

#define SBE_PARSER_PRINT_DELIMITER {std::cout << \
"*****************************************************************************"\
<< std::endl;}
#define SBE_PARSER_PRINT_HEADING(x) { SBE_PARSER_PRINT_DELIMITER \
std::cout << \
"                             "x<< std::endl;\
SBE_PARSER_PRINT_DELIMITER }

std::string findSbeFile(const char *name)
{
#ifdef CONTEXT_x86_nfp
    //TODO via RTC:157433
    std::string tmp = getenv("bb");
    tmp += "obj/x86.nfp/sbei/sbfw/img/";
    std::string file = tmp + name;

    struct  stat    l_stat;
    if(stat(file.c_str(), &l_stat) < 0)
    {
        //Can't find the file
    }

    return file;
#endif
#ifdef CONTEXT_ppc
    std::string tmp = getenv("PATH");
    tmp += ":/nfs:/maint";

    char *path = strdup(tmp.c_str());
    char *dir = NULL;

    std::string file;

    struct  stat    l_stat;

    for(dir = strtok( path, ":" ); dir; dir = strtok(NULL, ":"))
    {
        file = file + dir + "/" +  name;

        if(stat(file.c_str(), &l_stat) < 0)
        {
            // String file not found, go to next one
            file.clear();
        }
        else
        {
            // update where trexStringFile is
            break;
        }
    }

    free(path);
    path = NULL;        //sm05c

    return file;
#endif
}

void sbeParserSysCall(const char *cmd)
{
    FILE *stream;
    char buffer[256];

    stream = popen(cmd, "r" );
    if(stream)
    {
        while(!feof(stream))
        {
            if(fgets(buffer, 256, stream) != NULL)
            {
                std::cout << buffer;
            }
        }
    }
    else
    {
        std::cout << "command failed :[" << cmd << "]" << std::endl;
        std::cout << "errno [" << errno << "]" << std::endl;
    }
}

int parseSbeFFDC(ErrlUsrParser & i_parser, const void * i_pBuffer,
        const uint32_t i_buflen)
{
    int l_rc = 0;
    uint32_t fapiRc = 0;
    sbeFFDCDataHeader_t l_pData = {0};
    char l_buffer[SBE_PARSER_MAX_LOCAL_BUFFER] = {0};
    char *l_pBuffer = (char*)i_pBuffer;
    uint32_t l_buflen = i_buflen;

    do
    {
        if(i_pBuffer == NULL)
        {
            l_rc = -1;
            break;
        }
        //seek l_mem to the binary blob of FFDC package
        UtilMem l_mem(const_cast<void*>(i_pBuffer),i_buflen);

        // The data is a buffer of SBE FFDC data
        i_parser.PrintHeading("SBE FFDC Parser");

        if(l_buflen < sizeof(fapiRc))
        {
            i_parser.PrintHexDump(l_pBuffer, l_buflen);
            break;
        }
        l_buflen -= sizeof(fapiRc);
        l_pBuffer += sizeof(fapiRc);
        l_mem >> fapiRc;
        i_parser.PrintNumber("FAPI RC ", "0x%08X", fapiRc);

        if(l_buflen < sizeof(l_pData))
        {
            i_parser.PrintHexDump(l_pBuffer, l_buflen);
            break;
        }
        l_buflen -= sizeof(l_pData);
        l_pBuffer += sizeof(l_pData);
        l_mem >> l_pData;

        i_parser.PrintNumber("Primary Status ", "0x%04X",
                (uint16_t)l_pData.primaryStatus);
        i_parser.PrintNumber("Secondary Status ", "0x%04X",
                (uint16_t)l_pData.secondaryStatus);

        //loop through the number of fields configured
        uint32_t l_dumpFields = l_pData.dumpFields.get();
        while(l_dumpFields && !l_rc)
        {
            if(l_dumpFields & 0x0001)
            {
                if(l_buflen < sizeof(uint32_t))
                {
                    //Complete this loop and let the tools print as much data
                    //as possible but break from next loop
                    l_rc = -1;
                }
                sbeFFDCUserDataIdentifier_t l_ffdcUserDataId = {0};
                l_buflen -= sizeof(uint32_t); //l_ffdcUserDataId
                l_pBuffer += sizeof(uint32_t); //l_ffdcUserDataId

                l_mem >> l_ffdcUserDataId;

                // TODO via RTC:158462 continue even for attribute dump
                // Need to extend p9_xip_tool for partial attr dump handling
                if((l_buflen < l_ffdcUserDataId.fieldLen) &&
                    (l_ffdcUserDataId.fieldId != SBE_FFDC_TRACE_DUMP))
                {
                    i_parser.PrintHexDump(l_pBuffer, l_buflen);
                    break;
                }

                l_buflen -= l_ffdcUserDataId.fieldLen;
                l_pBuffer += l_ffdcUserDataId.fieldLen;

                l_mem.read(l_buffer, l_ffdcUserDataId.fieldLen);
                std::ostringstream  l_strFile;
                //Generate temp dump file name
                l_strFile << SBE_TEMP_DUMP_FILE;

                //Write dump into the temporary file
                UtilFile l_fileObj(l_strFile.str().c_str());
                errlHndl_t l_errlHndl = l_fileObj.open("w");
                if ( l_errlHndl )
                {
                    std::cerr << "Error opening "
                              << l_strFile.str() << std::endl;
                    l_errlHndl->commit(HWSV_COMP_ID, ERRL_ACTION_REPORT);
                    delete l_errlHndl;
                    l_errlHndl = NULL;
                    i_parser.PrintHexDump(l_buffer, l_ffdcUserDataId.fieldLen);
                    return -1;
                }
                else
                {
                    l_fileObj.write( l_buffer, l_ffdcUserDataId.fieldLen);
                    l_fileObj.Close();
                }

                //Specific handling
                if(l_ffdcUserDataId.fieldId == SBE_FFDC_ATTR_DUMP)
                {
                    SBE_PARSER_PRINT_HEADING(SBE_ATTR_DUMP_HEADLINE)
                    //command
                    std::ostringstream  l_strCmd1;
                    // p9_xip_tool <sbe seeprom bin file>
                    // -ifs attrdump <attr dump file> 2>&1
                    l_strCmd1 << findSbeFile(P9_XIP_TOOL)
                              << " "
                              << findSbeFile(SBE_SEEPROM_BIN)
                              << " "
                              << P9_XIP_ATTR_CMD
                              << " "
                              << l_strFile.str().c_str()
                              << " "
                              << "2>&1";

                    //Call out the command
                    sbeParserSysCall( l_strCmd1.str().c_str() );
                }
                else if(l_ffdcUserDataId.fieldId == SBE_FFDC_TRACE_DUMP)
                {
                    SBE_PARSER_PRINT_HEADING(SBE_TRACE_HEADLINE)
                    //command
                    std::ostringstream  l_strCmd1, l_strCmd2;
                    // ppe2fsp <trace dump file> <trace bin file> 2>&1
                    l_strCmd1 << findSbeFile(PPE2FSP_TOOL)
                              << " "
                              << l_strFile.str().c_str()
                              << " "
                              << SBE_TRACE_BIN
                              << " "
                              << "2>&1";

                    // fsp-trace -s <sbe string file> <trace bin file> 2>&1
                    l_strCmd2 << SBE_TRACE_CMD
                              << " "
                              << findSbeFile(SBE_STRING_FILE)
                              << " "
                              << SBE_TRACE_BIN
                              << " "
                              << "2>&1";

                    //Call out the commands
                    sbeParserSysCall( l_strCmd1.str().c_str() );
                    sbeParserSysCall( l_strCmd2.str().c_str() );
                }

                //Delete the temp file
                l_fileObj.Remove();
            }
            l_dumpFields >>= 1;
            if(l_rc != 0)
            {
                break;
            }
        }
    } while(false);

    return l_rc;
}
