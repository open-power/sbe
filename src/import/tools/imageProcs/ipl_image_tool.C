/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/tools/imageProcs/ipl_image_tool.C $                */
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

/// \file ipl_image_tool.C
/// \brief P10 IPL image (formerly XIP image) search and edit tool
///
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>

#include "p10_ipl_image.H"
#include "common_ringId.H"
#include "p10_ringId.H"
#if !defined(__PPE__) && !defined(OPENPOWER_BUILD) // Needed on PPE & OP-Build side to avoid having to include various APIs
    #include "p10_tor.H"
    #include "p10_scan_compression.H"
    #include <vector>
    #include <endian.h>
#endif
#include <p10_infrastruct_help.H>
#include <p10_ddco.H>
#include <p10_dynamic.H>

#if !defined(__PPE__) && !defined(OPENPOWER_BUILD) // Needed on PPE & OP-Build side to avoid having to include various APIs
    #include <prcdUtils.H>
    #include <croClientCapi.H>
    #include <ecmdClientCapi.H>
    #include <ecmdDataBuffer.H>
    #include <ecmdUtils.H>
    #include <ecmdSharedUtils.H>
    #include <ecmdStructs.H>
    #include <ecmdDefines.H>
    #include <ecmdReturnCodes.H>
    #include <fapi2.H>
    #include <fapi2ClientCapi.H>
    #include <mvpd_access_defs.H>
#endif

#define LINE_SIZE_MAX  1024     // Max size of a single snprintf dump.

// Listing mode IDs:
//
enum LISTING_MODE_ID
{
    LMID_TABLE,
    LMID_NORMAL,
    LMID_LONG,
    LMID_RAW
};

const char* g_usage =
    "Usage: ipl_image_tool <image> [-i<flag>...] normalize\n"
    "       ipl_image_tool <image> [-i<flag>...] get <item>\n"
    "       ipl_image_tool <image> [-i<flag>...] getv <item> <index>\n"
    "       ipl_image_tool <image> [-i<flag>...] set <item> <value> [<item1> <value1>...]\n"
    "       ipl_image_tool <image> [-i<flag>...] setv <item> <index> <value> [<item1> <index1> <value1>...]\n"
    "       ipl_image_tool <image> [-i<flag>...] report [<regex>]\n"
    "       ipl_image_tool <image> [-i<flag>...] attrdump <file>\n"
    "       ipl_image_tool <image> [-i<flag>...] append <section> <file> [<ddSupport>]\n"
    "       ipl_image_tool <image> [-i<flag>...] extract <section={<section>,(none)}> [<ddLevel>] <file>\n"
    "       ipl_image_tool <image> [-i<flag>...] delete <section> [<section1>...<sectionN>]\n"
    "       ipl_image_tool <image> [-i<flag>...] dissect <section={.rings,.overlays,.dynamic,.overrides,,(none)}> [table,normal(default),long,raw]\n"
    "       ipl_image_tool <image> [-i<flag>...] check-sbe-ring-section <ddLevel> <maximum size>\n"
    "\n"
    "This simple application uses the P9-XIP image APIs to normalize, search\n"
    "update and edit P9-XIP images. This program encapsulates several commands\n"
    "in a common command framework which requires a supported binary image to\n"
    "operate on, a command name, and command arguments that vary by command.\n"
    "Commands that modify the image always rewrite the image in-place in the\n"
    "filesystem. However the original image is only modified if the command has\n"
    "completed without error.\n"
    "\n"
    "The program operates predominantly on a P9-XIP formatted binary image, which\n"
    "must be normalized - unless the tool is being called to normalize the image\n"
    "in the first place with the 'normalize' command. The tool also validates the\n"
    "image prior to operating on the image.\n"
    "\n"
    "The program also operates in socalled standalone ring section images but\n"
    "which can only be used in the context of the following commands: 'report',\n"
    "'extract' and 'dissect'.\n"
    "\n"
    "The 'get' command retrieves a scalar value from the image and prints its\n"
    "representation on stdout (followed by a newline).  Scalar integer values\n"
    "and image addresses are printed as hex numbers (0x...).  Strings are printed\n"
    "verbatim.\n"
    "\n"
    "The 'getv' command retrieves a vector element from the image and prints its\n"
    "representation on stdout (followed by a newline).  Integer values\n"
    "and image addresses are printed as hex numbers (0x...).  Vectors of strings\n"
    "are not supported.\n"
    "\n"
    "The 'set' command allows setting integer and string values in the image.\n"
    "New integer values can be specified in decimal or hex (0x...).  Strings are\n"
    "taken verbatim from the command line.  Note that new string values will be\n"
    "silently truncated to the length of the current string if the new value is\n"
    "longer than the current string. Updating address values is currently not\n"
    "supported.  Any number of item/value pairs can be specified with a single\n"
    "'set' command.\n"
    "\n"
    "The 'setv' command is provided to set individual vector elements of\n"
    "integral arrays.\n"
    "\n"
    "The 'report' command prints a report including a dump of the header and\n"
    "section table, a listing of the types and values of all items that appear\n"
    "in the TOC. The TOC listing includes the\n"
    "sequence number of the entry in the TOC, the item name, the item type and\n"
    "the item value.\n"
    "\n"
    "The 'attrdump' command prints a listing of the names, types and values\n"
    "of all attribute items that appear in the TOC and their value from \n"
    "the attribute dump file\n"
    "\n"
    "The 'append' command either creates or extends the section named by the\n"
    "section argument, by appending the contents of the named file verbatim.\n"
    "Currently the section must either be the final (highest address) section of\n"
    "the image, or must be empty, in which case the append command creates the\n"
    "section as the final section of the image. The 'append' command writes the\n"
    "relocatable image address where the input file was loaded to stdout.  The\n"
    "last argument, ddSupport, indicates if the section being added has ddLevel\n"
    "metadata support (=1) or not (=0). If this arg is omitted, it's assumed\n"
    "to be false (=0).\n"
    "\n"
    "The 'extract' command extracts a section from the binary image.  The last\n"
    "argument, ddLevel, indicates (in hex) the DD level to be extracted.  If\n"
    "the section doesn't have DD level support, a message is returned stating\n"
    "that and to reissue the command w/o the ddLevel arg.  If the section does\n"
    "have DD support but the specified ddLevel cannot be found, a message is\n"
    "returned stating that and no section is returned.  If the arg is omitted,\n"
    "the entire section is returned."
    "\n"
    "The 'delete' command deletes 0 or more sections, starting with <section0>.\n"
    "Each section to be deleted must either be the final (highest address)\n"
    "section of the image at the time it is deleted, or must be empty. The\n"
    "'delete' command writes the size of the final modified image to stdout.\n"
    "\n"
    "The 'dissect' command summarizes the content of a ring section. It\n"
    "accepts two different types of images:  1) The regular XIP image and 2)\n"
    "a standalone ring section image (that has the TOR header). Wrt an XIP\n"
    "image, the ring section named by the ring section argument is summarized.\n"
    "Wrt a standalone ring section image, the ring section based on the TOR\n"
    "magic word in the image, is summarized. (Hint. To find out what type of\n"
    "image file you're supplying, run \"ipl_image_tool.exe <image file> report\"\n"
    "on it first.) Note that the first sub-argument must be PASSED for an XIP\n"
    "image and must be OMITTED for a standalone image. For either image, the\n"
    "second sub-argument is optional and offers the following listing choices:\n"
    "   table:           Tabular overview. No dump of ring block.\n"
    "   normal(default): All header info. No dump of ring block.\n"
    "   long:            All header info and binary dump of ring block.\n"
    "   raw:             All header info and binary+raw dump of ring block.\n"
    "Note that if the second sub-argument is omitted, a 'normal' listing of\n"
    "the ring section is chosen.\n"
    "\n"
    "The 'check-sbe-ring-section' command checks that the size of the SBE ring\n"
    "section, within the .rings section of the HW image, of the specified\n"
    "ddLevel (in hex), does not exceed the specified <maximum size>\n"
    "\n"
    "-i<flag>:\n"
    "\t-ifs  Causes the validation step to ignore image size check against the\n"
    "\tfile size.\n"
    "\t-iv   Causes all validation checking to be ignored.\n"
    ;

P9_XIP_ERROR_STRINGS(g_errorStrings);
P9_XIP_TYPE_STRINGS(g_typeStrings);
P9_XIP_TYPE_ABBREVS(g_typeAbbrevs);

P9_XIP_SECTION_NAMES_HW(g_sectionNamesHw);
P9_XIP_SECTION_NAMES_SBE(g_sectionNamesSbe);
P9_XIP_SECTION_NAMES_QME(g_sectionNamesQme);
P9_XIP_SECTION_NAMES_XGPE(g_sectionNamesXgpe);
P9_XIP_SECTION_NAMES_PGPE(g_sectionNamesPgpe);
P9_XIP_SECTION_NAMES_RESTORE(g_sectionNamesRestore);
P9_XIP_SECTION_NAMES_IOPPE(g_sectionNamesIoppe);
P9_XIP_SECTION_NAMES_IOPXRAM(g_sectionNamesIopxram);

#define ERRBUF_SIZE 60

typedef struct
{
    int index;
    int regex;
    regex_t preg;
} ReportControl;

off_t g_imageSize;


// Determine name of section given by its index in section table

static inline const char* get_sectionName(uint64_t magic, int index)
{
    switch (magic)
    {
        case P9_XIP_MAGIC_HW:
            return P9_XIP_SECTION_NAME(g_sectionNamesHw, index);

        case P9_XIP_MAGIC_SEEPROM:
            return P9_XIP_SECTION_NAME(g_sectionNamesSbe, index);

        case P9_XIP_MAGIC_QME:
            return P9_XIP_SECTION_NAME(g_sectionNamesQme, index);

        case P9_XIP_MAGIC_XGPE:
            return P9_XIP_SECTION_NAME(g_sectionNamesXgpe, index);

        case P9_XIP_MAGIC_PGPE:
            return P9_XIP_SECTION_NAME(g_sectionNamesPgpe, index);

        case P9_XIP_MAGIC_RESTORE:
            return P9_XIP_SECTION_NAME(g_sectionNamesRestore, index);

        case P9_XIP_MAGIC_IOPPE:
            return P9_XIP_SECTION_NAME(g_sectionNamesIoppe, index);

        case P9_XIP_MAGIC_IOPXRAM:
            return P9_XIP_SECTION_NAME(g_sectionNamesIopxram, index);
    }

    return "";
}

// Determine index of section given by its name in section table

static inline int get_sectionId(uint64_t i_magic, const char* i_sectionName)
{
    int i;

    for (i = 0; i < IPL_IMAGE_SECTIONS; i++)
        if (strcmp(i_sectionName, get_sectionName(i_magic, i)) == 0)
        {
            return i;
        }

    return -1;
}

// Normalize a P9-XIP image.  We normalize a copy of the image first so that
// the original image will be available for debugging in case the
// normalization fails, then validate and copy the normalized image back to
// the mmap()-ed file.

static int
normalize(void* io_image, const int i_argc, const char** i_argv, uint32_t i_maskIgnores)
{
    int rc;
    void* copy;

    do
    {

        // The 'normalize' command takes no arguments

        if (i_argc != 0)
        {
            fprintf(stderr, "%s", g_usage);
            exit(1);
        }

        copy = malloc(g_imageSize);

        if (copy == 0)
        {
            perror("malloc() failed : ");
            exit(1);
        }

        memcpy(copy, io_image, g_imageSize);

        rc = p9_xip_normalize(copy);

        if (rc)
        {
            break;
        }

        if ( !(i_maskIgnores & P9_XIP_IGNORE_ALL) )
        {
            rc = p9_xip_validate2(copy, g_imageSize, i_maskIgnores);
        }

        if (rc)
        {
            break;
        }

        memcpy(io_image, copy, g_imageSize);

    }
    while (0);

    return rc;
}


// Print a line of attribute report, listing the symbol, type and current
// value.
static int
attrListing(const P9XipItem* i_item, const char* prefix)
{
    int rc = 0;
    uint64_t data = 0;
    uint32_t i;
    char name[43];

    if (i_item->iv_address == 0)
    {
        //TOC item not present in fixed section
        return rc;
    }

    for (i = 0; i < i_item->iv_elements; i++)
    {
        rc = p9_xip_get_item(i_item, &data, i);

        if (rc)
        {
            return rc;
        }

        if (i_item->iv_elements > 1)
        {
            snprintf(name, sizeof(name), "%s[%d]", i_item->iv_id, i);
        }
        else
        {
            strncpy(name, i_item->iv_id, (sizeof(name) - 1));
        }

        printf("%s%-42s | %s | ", prefix, name,
               P9_XIP_TYPE_STRING(g_typeAbbrevs, i_item->iv_type));

        switch (i_item->iv_type)
        {
            case P9_XIP_UINT8:
                printf("0x%02x", (uint8_t)data);
                break;

            case P9_XIP_UINT16:
                printf("0x%04x", (uint16_t)data);
                break;

            case P9_XIP_UINT32:
                printf("0x%08x", (uint32_t)data);
                break;

            case P9_XIP_UINT64:
                printf("0x%016lx", data);
                break;

            case P9_XIP_INT8:
                printf("0x%02x", (uint8_t)data);
                break;

            case P9_XIP_INT16:
                printf("0x%04x", (uint16_t)data);
                break;

            case P9_XIP_INT32:
                printf("0x%08x", (uint32_t)data);
                break;

            case P9_XIP_INT64:
                printf("0x%016lx", data);
                break;

            case P9_XIP_STRING:
                printf("%s", (char*)(i_item->iv_imageData));
                break;

            case P9_XIP_ADDRESS:
                printf("0x%04x:0x%08x",
                       (uint16_t)((data >> 32) & 0xffff),
                       (uint32_t)(data & 0xffffffff));
                break;

            default:
                printf("unknown type\n");
                rc = P9_XIP_BUG;
                break;
        }

        printf("\n");
    }

    return rc;
}


// Print a line of a report, listing the index, symbol, type and current
// value.

static int
tocListing(void* i_image,
           const P9XipItem* i_item,
           void* arg)
{
    P9XipItem item;
    int rc = 0;
    ReportControl* control = (ReportControl*)arg;
    char prefix[10];

    do
    {
        if (control->regex)
        {
            if (regexec(&(control->preg), i_item->iv_id, 0, 0, 0))
            {
                break;
            }
        }

        rc = p9_xip_find(i_image, i_item->iv_id, &item);

        if (!rc)
        {
            snprintf(prefix, sizeof(prefix), "0x%04x | ", control->index);
            attrListing(&item, prefix);
        }
    }
    while (0);

    control->index += 1;
    return rc;
}


/// Function: resolve_image_section_type
///
/// Brief: Resolves the type of the input image section, e.g. XIP image.
///
/// \param[in]  i_image             A pointer to the image section.
///
/// \param[out] o_imageSectionType  The resolved image section type.
///
static void
resolve_image_section_type( const void* i_image,
                            image_section_type_t& o_imageSectionType )
{
    if (be64toh(((P9XipHeader*)i_image)->iv_magic) >> 32 == P9_XIP_MAGIC)
    {
        o_imageSectionType = IST_XIP;
    }

#if !defined(__PPE__) && !defined(OPENPOWER_BUILD)
    else if (be32toh(((TorHeader_t*)i_image)->magic) >> 8 == TOR_MAGIC)
    {
        o_imageSectionType = IST_TOR;
    }
    else if (be32toh(((DdContHeader_t*)i_image)->iv_magic) == DDCO_MAGIC)
    {
        o_imageSectionType = IST_DDCO;
    }

    else if (be64toh(((MvpdHeader_t*)i_image)->magic) >> 8 == MVPD_MAGIC)
    {
        if ((uint32_t)g_imageSize == SIZE_OF_MVPD_FILE)
        {
            o_imageSectionType = IST_MVPD;
        }
        else
        {
            printf("ERROR: The size of the supplied file(=0x%08x) does not match the size"
                   " of a valid MVPD file(SIZE_OF_MVPD_FILE=0x%08x)\n",
                   (uint32_t)g_imageSize, SIZE_OF_MVPD_FILE);
            exit(1);
        }
    }

#endif

    else if ( (be64toh(((DynamicHdr_t*)i_image)->magic) == DYN_MAGIC_FEATURE) ||
              (be64toh(((DynamicHdr_t*)i_image)->magic) == DYN_MAGIC_SERVICE) )
    {
        o_imageSectionType = IST_DYN;
    }
    else
    {
        o_imageSectionType = IST_UNDEFINED;
    }

    return;
}


// Dump the image header, including the section table

static void
dumpHeader(void* i_image, image_section_type_t i_imageSectionType)
{
    int i;
    uint8_t numOf64BitsSet;
    uint16_t numOfRecords;
    P9XipHeader header;
    P9XipSection* section;
#if !defined(__PPE__) && !defined(OPENPOWER_BUILD)
    TorHeader_t* torHeader = (TorHeader_t*)i_image;
    DdContHeader_t* ddContHeader = (DdContHeader_t*)i_image;
#endif
    char magicString[9];

    switch (i_imageSectionType)
    {

        case IST_XIP:

            // Dump header information. Since the TOC may not exist we need to get
            // the information from the header explicitly.

            p9_xip_translate_header(&header, (P9XipHeader*)i_image);

            memcpy(magicString, (char*)(&(((P9XipHeader*)i_image)->iv_magic)), 8);
            magicString[8] = 0;

            printf("Image section type : 0x%02x \"XIP image\"\n", i_imageSectionType);
            printf("Magic number       : 0x%016lx \"%s\"\n",
                   header.iv_magic, magicString);
            printf("Header version     : 0x%02x\n", header.iv_headerVersion);
            printf("Link Address       : 0x%016lx\n", header.iv_linkAddress);
            printf("L1 Loader Address  : 0x%08x\n", (uint32_t)header.iv_L1LoaderAddr);
            printf("L2 Loader Address  : 0x%08x\n", (uint32_t)header.iv_L2LoaderAddr);
            printf("Kernel Address     : 0x%08x\n", (uint32_t)header.iv_kernelAddr);
            printf("Data Addr          : 0x%08x\n", (uint32_t)header.iv_dataAddr);
            printf("Image size         : 0x%08x (%d)\n",
                   header.iv_imageSize, header.iv_imageSize);
            printf("Normalized         : %s\n", header.iv_normalized ? "Yes" : "No");
            printf("TOC Sorted         : %s\n", header.iv_tocSorted ? "Yes" : "No");
            printf("Build Date         : %02d/%02d/%04d\n",
                   (header.iv_buildDate / 100) % 100,
                   header.iv_buildDate % 100,
                   header.iv_buildDate / 10000);
            printf("Build Time         : %02d:%02d\n",
                   header.iv_buildTime / 100,
                   header.iv_buildTime % 100);
            printf("Build User         : %s\n", header.iv_buildUser);
            printf("Build Host         : %s\n", header.iv_buildHost);
            printf("Build Tag          : %s\n", header.iv_buildTag);
            printf("Build Head Commit  : %s\n", header.iv_buildHeadCommit);
            printf("\n");

            printf("Section Table      :\n\n");
            printf("    Name            Align   DD   Start        End          Size\n");
            printf("\n");

            for (i = 0; i < IPL_IMAGE_SECTIONS; i++)
            {
                section = &(header.iv_section[i]);
                printf("   %-16s %d       %d    0x%08x   ",
                       get_sectionName(header.iv_magic, i),
                       section->iv_alignment,
                       section->iv_ddSupport,
                       section->iv_offset);

                if (section->iv_size == 0)
                {
                    printf("          ");
                }
                else
                {
                    printf("0x%08x", section->iv_offset + section->iv_size - 1);
                }

                printf("   0x%08x (%d)\n", section->iv_size, section->iv_size);
            }

            break;

#if !defined(__PPE__) && !defined(OPENPOWER_BUILD)

        case IST_TOR:

            memcpy(magicString, (char*) & (torHeader->magic), 4);
            magicString[4] = 0;

            printf("Image section type : 0x%02x \"TOR ring section\"\n", i_imageSectionType);
            printf("Magic number       : 0x%08x \"%s\"\n",
                   be32toh(torHeader->magic), magicString);
            printf("Header version     : 0x%02x\n", torHeader->version);
            printf("Chip type          : 0x%02x \"%s\"\n",
                   torHeader->chipId  , (chipIdToTypeMap[torHeader->chipId]).c_str());
            printf("DD level           : 0x%02x\n", torHeader->ddLevel);
            printf("Image size         : 0x%08x (%d)\n",
                   be32toh(torHeader->size), be32toh(torHeader->size));

            break;

        case IST_DDCO:

            memcpy(magicString, (char*) & (ddContHeader->iv_magic), 4);
            magicString[4] = 0;

            printf("Image section type : 0x%02x \"DDCO image section\"\n", i_imageSectionType);
            printf("Magic number       : 0x%08x \"%s\"\n",
                   be32toh(ddContHeader->iv_magic), magicString);
            printf("Number DD levels   : %d\n", ddContHeader->iv_num);

            break;

        case IST_MVPD:

            do
            {
                uint32_t rc = ECMD_SUCCESS;
                ecmdDataBuffer keyword_data;
                ecmdDllInfo DLLINFO;
                ecmdLooperData drawer_looper;
                ecmdChipTarget drawer_target;

                rc = ecmdLoadDll("");

                if (rc)
                {
                    printf("ecmdLoadDll failed\n");
                    break;
                }

                rc = fapi2InitExtension();

                if (rc)
                {
                    ecmdOutputError("Error initializing FAPI2 extension!\n");
                    break;
                }

                drawer_target.cageState   = ECMD_TARGET_FIELD_WILDCARD;
                drawer_target.nodeState   = ECMD_TARGET_FIELD_WILDCARD;
                drawer_target.slotState   = ECMD_TARGET_FIELD_UNUSED;
                drawer_target.posState    = ECMD_TARGET_FIELD_UNUSED;
                drawer_target.chipUnitNumState   = ECMD_TARGET_FIELD_UNUSED;
                drawer_target.threadState = ECMD_TARGET_FIELD_UNUSED;
                rc = ecmdConfigLooperInit(drawer_target, ECMD_SELECTED_TARGETS_LOOP_DEFALL, drawer_looper);

                if (rc)
                {
                    ecmdOutputError("Error initializing drawer looper!\n");
                    break;
                }

                while(ecmdConfigLooperNext(drawer_target, drawer_looper))
                {
                    ecmdLooperData pu_looper;
                    ecmdChipTarget pu_target;

                    pu_target.chipType = "pu";
                    pu_target.chipTypeState = ECMD_TARGET_FIELD_VALID;

                    pu_target.cage = drawer_target.cage;
                    pu_target.node = drawer_target.node;

                    pu_target.cageState   = ECMD_TARGET_FIELD_VALID;
                    pu_target.nodeState   = ECMD_TARGET_FIELD_VALID;
                    pu_target.slotState   = ECMD_TARGET_FIELD_WILDCARD;
                    pu_target.posState    = ECMD_TARGET_FIELD_WILDCARD;
                    pu_target.chipUnitNumState   = ECMD_TARGET_FIELD_UNUSED;
                    pu_target.threadState = ECMD_TARGET_FIELD_UNUSED;

                    rc = ecmdConfigLooperInit(pu_target, ECMD_SELECTED_TARGETS_LOOP_DEFALL, pu_looper);

                    if(rc)
                    {
                        ecmdOutputError("Error initializing chip looper!\n");
                        break;
                    }

                    // Fetching MVPD details (DD level, chip type, RTV)
                    //
                    // Notes:
                    // - getModuleVpdKeyword is used to access the HW's MVPD or the MODVPDFILE
                    //   in the Cronus config file (if DEBUG==8.v). But we want to access the
                    //   MVPD file supplied as an argument to ipl_image_tool.exe. So instead we
                    //   use getModuleVpdKeywordFromImage.

                    uint32_t input_data_length = SIZE_OF_MVPD_FILE * 8; //Mvpd size in bits
                    ecmdDataBuffer image_buffer;
                    image_buffer.setBitLength(input_data_length);
                    image_buffer.insert((uint8_t*) i_image, 0, input_data_length, 0);

                    rc = getModuleVpdKeywordFromImage(pu_target, "CRP0", "DD", 5, image_buffer, keyword_data);

                    if(rc)
                    {
                        ecmdOutputError("getModuleVpdKeywordFromImage failed to fetch the DD level from MVPD!\n");
                        break;
                    }

                    uint8_t* pmvpdDD = new uint8_t[4];

                    for(uint32_t i = 1; i < 5; i++)
                    {
                        *(pmvpdDD + (i - 1)) = keyword_data.getByte(i) - '0';
                    }

                    uint8_t mvpdDD = ((((*pmvpdDD * 10) + * (pmvpdDD + 1)) << 4) |
                                      (*(pmvpdDD + 2) * 10 + * (pmvpdDD + 3)));

                    printf("DD level in MVPD : 0x%02x\n", mvpdDD);

                    //Fetching chipType from MVPD
                    rc = getModuleVpdKeywordFromImage(pu_target, "CRP0", "CI", 1, image_buffer, keyword_data);

                    if(rc)
                    {
                        ecmdOutputError("getModuleVpdKeywordFromImage failed to fetch the chipType from MVPD!\n");
                        break;
                    }

                    printf("chipType from MVPD : 0x%02x", keyword_data.getByte(0));

                    if (keyword_data.getByte(0) == MVPD_CHIP_NAME_P10)
                    {
                        printf(" (This is a P10 MVPD)\n");
                    }
                    else
                    {
                        printf(" (This is *not* a P10 MVPD)\n");
                    }

                    //Fetching the ring table version from MVPD(#P rings)
                    rc = getModuleVpdKeywordFromImage(pu_target, "CP00", "#P", 1, image_buffer, keyword_data);

                    if(rc)
                    {
                        ecmdOutputError("getModuleVpdKeywordFromImage failed to fetch the ring"
                                        " table version of #P rings from MVPD!\n");
                        break;
                    }

                    printf("Ring Table version of #P rings in MVPD : 0x%02x\n", keyword_data.getByte(0));

                    //Fetching the ring table version from MVPD(#R rings)
                    rc = getModuleVpdKeywordFromImage(pu_target, "CP00", "#R", 1, image_buffer, keyword_data);

                    if(rc)
                    {
                        ecmdOutputError("getModuleVpdKeywordFromImage failed to fetch the ring"
                                        " table version of #R rings from MVPD!\n");
                        break;
                    }

                    printf("Ring Table version of #R rings in MVPD : 0x%02x\n", keyword_data.getByte(0));

                    //Fetching the ring table version from MVPD(#G rings)
                    rc = getModuleVpdKeywordFromImage(pu_target, "CP00", "#G", 1, image_buffer, keyword_data);

                    if(rc)
                    {
                        ecmdOutputError("getModuleVpdKeywordFromImage failed to fetch the ring"
                                        " table version of #G rings from MVPD!\n");
                        break;
                    }

                    printf("Ring Table version of #G rings in MVPD : 0x%02x\n", keyword_data.getByte(0));
                }

                ecmdUnloadDll();
            }
            while(0);

            break;

#endif

        case IST_DYN:

            //Display Dynamic binary images - Features/Services
            //=================================================
            if (be64toh(((DynamicHdr_t*)i_image)->magic) == DYN_MAGIC_FEATURE)
            {
                printf("***** Dynamic Feature binary file *****\n");
            }
            else
            {
                printf("***** Dynamic Service binary file *****\n");
            }

            //Get dynamic header fields
            //=========================
            //uint64_t    magic;
            //uint8_t     version;
            //uint8_t     numOf64BitsSet;
            //uint16_t    numOfRecords;
            //uint16_t    sizeOfBinary;
            printf("Magic\t\t:%016lx\n", be64toh(((DynamicHdr_t*)i_image)->magic));
            printf("Version\t\t:%d.%d\n", ((DynamicHdr_t*)i_image)->version >> 4, ((DynamicHdr_t*)i_image)->version & 0x0F);
            printf("64-bits set\t:%d\n", ((DynamicHdr_t*)i_image)->numOf64BitsSet);
            printf("Num of records\t:%d\n", htobe16(((DynamicHdr_t*)i_image)->numOfRecords));
            printf("Size of bin file:%d bytes\n\n", htobe16(((DynamicHdr_t*)i_image)->sizeOfBinary));

            //Get number of records and 64-bit vector size
            dynamic_get_bitVectorSize_n_numRecords(i_image, &numOf64BitsSet, &numOfRecords);

            //Get properties of dynamic feature
            //=================================
            //uint8_t     featureNameSize;
            //char*       featureName;
            //uint16_t    featureValue;
            //uint8_t     featureDescSize;
            //char*       featureDesc;
            if (be64toh(((DynamicHdr_t*)i_image)->magic) == DYN_MAGIC_FEATURE)
            {
                uint8_t len, j, space_size;
                uint16_t i, val;
                char* ch;

                printf("Feature Name            | Feature Value | Feature Description\n");
                printf("------------------------|---------------|--------------------\n");

                for ( i = 0; i < numOfRecords; i++ )
                {
                    //Get feature name size
                    dynamic_get_name_length_perValue(i_image, i, &len);

                    //Get feature name
                    ch = (char*)(malloc(sizeof(len)));
                    dynamic_get_name(i_image, i, ch);
                    printf("%s", (char*)ch);

                    space_size = ( len > 24 ) ? 1 : ( 24 - len );

                    for ( j = 0; j < space_size; j++ )
                    {
                        printf(" ");
                    }

                    //Get feature value
                    dynamic_get_value(i_image, ch, &val);
                    printf("  %04d            ", val);

                    //Get feature desc size
                    dynamic_get_desc_length_perValue(i_image, i, &len);

                    //Get feature desc
                    ch = (char*)realloc(ch, len);
                    dynamic_get_desc_perValue(i_image, i, ch);
                    printf("%s\n", (char*)ch);

                    //Free up the used memory
                    free(ch);
                }
            }
            else
            {
                //Get properties of dynamic service
                //=================================
                //uint8_t     serviceNameSize;
                //char*       serviceName;
                //uint16_t    serviceValue;
                //uint64_t*   serviceBitVector;
                //uint8_t     serviceDescSize;
                //char*       serviceDesc;

                uint8_t len, j, space_size;
                uint16_t i, val;
                char* ch;
                uint64_t* val64;

                //Get title displayed in order
                printf("Service Name            | Service Value | Service Vector");

                for ( i = 0; i < ( (numOf64BitsSet * 17) + 1 - 14 ); i++ )
                {
                    printf(" ");
                }

                printf("| Service Description\n");
                printf("------------------------|---------------|---------------");

                for ( i = 0; i < ( (numOf64BitsSet * 17) + 1 - 14 ); i++ )
                {
                    printf("-");
                }

                printf("|--------------------\n");

                for ( i = 0; i < numOfRecords; i++ )
                {
                    //Get service name size
                    dynamic_get_name_length_perValue(i_image, i, &len);

                    //Get service name
                    ch = (char*)(malloc(sizeof(len)));
                    dynamic_get_name(i_image, i, ch);
                    printf("%s", (char*)ch);

                    space_size = ( len > 24 ) ? 1 : ( 24 - len );

                    for ( j = 0; j < space_size; j++ )
                    {
                        printf(" ");
                    }

                    //Get service value
                    dynamic_get_value(i_image, ch, &val);
                    printf("  %04d            ", val);

                    //Get service bit vector
                    val64 = (uint64_t*)(malloc(sizeof(8 * numOf64BitsSet)));
                    dynamic_get_bitVector_perValue(i_image, i, val64);

                    for ( j = 0; j < numOf64BitsSet; j++ )
                    {
                        printf("%016lx ", *(uint64_t*)(val64 + j));
                    }

                    printf("   ");

                    //Get service desc size
                    dynamic_get_desc_length_perValue(i_image, i, &len);

                    //Get service desc
                    ch = (char*)realloc(ch, len);
                    dynamic_get_desc_perValue(i_image, i, ch);
                    printf("%s\n", (char*)ch);

                    //Free up the used memory
                    free(ch);
                    free(val64);
                }
            }

            break;

        default:

            fprintf(stderr,
                    "ERROR: In dumpHeader(): Invalid image section type (=%d)\n",
                    i_imageSectionType);
            uint64_t l_magic = *((uint64_t*)i_image);
            fprintf(stderr, "Here's the first 8 bytes of the image section: 0x%016lx ",
                    be64toh(l_magic));
            fprintf(stderr, "\"%c%c%c%c%c%c%c%c\"\n",
                    *(((uint8_t*)&l_magic) + 0),
                    *(((uint8_t*)&l_magic) + 1),
                    *(((uint8_t*)&l_magic) + 2),
                    *(((uint8_t*)&l_magic) + 3),
                    *(((uint8_t*)&l_magic) + 4),
                    *(((uint8_t*)&l_magic) + 5),
                    *(((uint8_t*)&l_magic) + 6),
                    *(((uint8_t*)&l_magic) + 7));
            fprintf(stderr,
                    "If you're seeing \"TOR\" or \"DDCO\" in the first 8 bytes, you're probably using the PPE compiled version of ipl_image_tool. Use the EKB version instead!\n\n");
            exit(EXIT_FAILURE);

    }

    return;
}


// Print a report

static int
report(void*                      io_image,
       const int                  i_argc,
       const char**               i_argv,
       const image_section_type_t i_imageSectionType)
{
    int rc = INFRASTRUCT_RC_SUCCESS;
    ReportControl control;
    char errbuf[ERRBUF_SIZE];

    do
    {

        if (i_imageSectionType == IST_XIP)
        {

            // Basic syntax check : [<regexp>]

            if (i_argc > 1)
            {
                fprintf(stderr, "%s", g_usage);
                exit(1);
            }

            // Compile a regular expression if supplied

            if (i_argc == 1)
            {
                rc = regcomp(&(control.preg), i_argv[0], REG_NOSUB);

                if (rc)
                {
                    regerror(rc, &(control.preg), errbuf, ERRBUF_SIZE);
                    fprintf(stderr, "Error from regcomp() : %s\n", errbuf);
                    exit(1);
                }

                control.regex = 1;
            }
            else
            {
                control.regex = 0;

                printf("\n");
                dumpHeader(io_image, IST_XIP);
                printf("\nTOC Report\n\n");
            }

            // Map the TOC with the mapReport() function

            control.index = 0;
            rc = p9_xip_map_toc(io_image, tocListing, (void*)(&control));

            if (rc)
            {
                break;
            }
        }
        else
        {
            printf("\n");
            dumpHeader(io_image, i_imageSectionType);
            printf("\n");
        }

    }
    while (0);

    return rc;
}

//Print attributes from dump image
static int
reportAttr(void* io_image, size_t i_imageSize, void* io_dump)
{
    int rc = 0;
    P9XipToc* imageToc = NULL;
    P9XipItem item = {0};
    size_t entries = 0;

    //check for seeprom image validity
    rc = p9_xip_validate(io_image, i_imageSize);

    if (rc)
    {
        return rc;
    }

    //get toc listing from seeprom image
    rc = p9_xip_get_toc(io_image, &imageToc, &entries, 0, 0);

    if (rc)
    {
        return rc;
    }

    //loop through each toc listing and print its value from pibmem dump
    for (; entries--; imageToc++)
    {
        rc = p9_xip_decode_toc_dump(io_image, io_dump, imageToc, &item);

        if (!rc)
        {
            //helper function to print the attributes
            attrListing(&item, "");
        }
    }

    return rc;
}

// Set a scalar or vector element values in the image.  The 'i_setv' argument
// indicates set/setv (0/1).

static int
set(void* io_image, const int i_argc, const char** i_argv, int i_setv)
{
    int rc = P9_XIP_BUG, arg, base, clause_args, index_val;
    P9XipItem item;
    unsigned long long newValue;
    const char* key, *index, *value;
    char* endptr;

    do
    {

        // Basic syntax check: <item> <value> [ <item1> <value1> ... ]
        // Basic syntax check: <item> <index> <value> [ <item1> <index1> <value1> ... ]

        clause_args = (i_setv ? 3 : 2);

        if ((i_argc % clause_args) != 0)
        {
            fprintf(stderr, "%s", g_usage);
            exit(1);
        }

        for (arg = 0; arg < i_argc; arg += clause_args)
        {

            key = i_argv[arg];

            if (i_setv)
            {
                index = i_argv[arg + 1];
                index_val = strtol(index, 0, 0);
                value = i_argv[arg + 2];
            }
            else
            {
                index = "";
                index_val = 0;
                value = i_argv[arg + 1];
            }

            // Search for the item to see what type of data it expects, then
            // case split on the type.

            rc = p9_xip_find(io_image, key, &item);

            if (rc)
            {
                break;
            }

            if (index_val < 0)
            {
                fprintf(stderr,
                        "Illegal negative vector index %s for %s\n",
                        index, key);
                exit(1);
            }
            else if ((item.iv_elements != 0) &&
                     (index_val >= item.iv_elements))
            {
                fprintf(stderr,
                        "Index %s out-of-bounds for %s (%d elements)\n",
                        index, key, item.iv_elements);
                exit(1);
            }

            switch (item.iv_type)
            {
                case P9_XIP_UINT8:
                case P9_XIP_UINT16:
                case P9_XIP_UINT32:
                case P9_XIP_UINT64:

                    // We need to do a bit of preprocessing on the string to
                    // determine its format and set the base for strtoull(),
                    // otherwise strtoull() will be confused by leading zeros
                    // e.g. in time strings generated in hhmm format and try to
                    // process the string as octal.

                    if ((strlen(value) >= 2) && (value[0] == '0') &&
                        ((value[1] == 'x') || (value[1] == 'X')))
                    {
                        base = 16;
                    }
                    else
                    {
                        base = 10;
                    }

                    errno = 0;
                    newValue = strtoull(value, &endptr, base);

                    if ((errno != 0) || (endptr != (value + strlen(value))))
                    {
                        fprintf(stderr,
                                "Error parsing putative integer value : %s\n",
                                value);
                        exit(1);
                    }

                    switch (item.iv_type)
                    {

                        case P9_XIP_UINT8:
                            if ((uint8_t)newValue != newValue)
                            {
                                fprintf(stderr,
                                        "Value 0x%016llx too large for 8-bit type\n",
                                        newValue);
                                exit(1);
                            }

                            break;

                        case P9_XIP_UINT16:
                            if ((uint16_t)newValue != newValue)
                            {
                                fprintf(stderr,
                                        "Value 0x%016llx too large for 16-bit type\n",
                                        newValue);
                                exit(1);
                            }

                            break;

                        case P9_XIP_UINT32:
                            if ((uint32_t)newValue != newValue)
                            {
                                fprintf(stderr,
                                        "Value 0x%016llx too large for 32-bit type\n",
                                        newValue);
                                exit(1);
                            }

                            break;

                        case P9_XIP_UINT64:
                            break;

                        default:
                            break;
                    }

                    rc = p9_xip_set_element(io_image, key, index_val, newValue);

                    if (rc)
                    {
                        rc = P9_XIP_BUG;
                    }

                    break;

                case P9_XIP_STRING:

                    if (i_setv)
                    {
                        fprintf(stderr, "Can't use 'setv' for string data %s\n",
                                key);
                        exit(1);
                    }

                    rc = p9_xip_set_string(io_image, key, (char*)value);

                    if (rc)
                    {
                        rc = P9_XIP_BUG;
                    }

                    break;

                case P9_XIP_INT8:
                case P9_XIP_INT16:
                case P9_XIP_INT32:
                case P9_XIP_INT64:
                    fprintf(stderr,
                            "Item %s has int type %s, "
                            "which is not supported for '%s'.\n",
                            i_argv[arg],
                            P9_XIP_TYPE_STRING(g_typeStrings, item.iv_type),
                            (i_setv ? "setv" : "set"));
                    exit(1);
                    break;

                default:
                    fprintf(stderr,
                            "Item %s has type %s, "
                            "which is not supported for '%s'.\n",
                            i_argv[arg],
                            P9_XIP_TYPE_STRING(g_typeStrings, item.iv_type),
                            (i_setv ? "setv" : "set"));
                    exit(1);
                    break;
            }

            if (rc)
            {
                break;
            }

        }
    }
    while (0);

    //if good rc, we need to msync the mmaped file to push contents to
    //the actual file. Per man page this is required although some
    //file systems (notably AFS) don't seem to require (GSA does)
    if(!rc)
    {
        uint8_t i = 0;

        do
        {
            rc = msync(io_image, g_imageSize , MS_SYNC);

            if(rc)
            {
                i++;
                fprintf(stderr,
                        "msync failed with errno %d\n", errno);
            }
        }
        while(rc && i < 5);

        if(rc)
        {
            exit(3);
        }
    }

    return rc;
}


// Get a value from the image, and return on stdout.  The 'i_getv' argument
// indicates get/getv (0/1)

static int
get(void* i_image, const int i_argc, const char** i_argv, int i_getv)
{
    int rc, nargs, index_val;
    P9XipItem item;
    const char* key, *index;
    uint64_t data;
    char* s;

    do
    {

        // Basic syntax check: <item>
        // Basic syntax check: <item> <index>

        nargs = (i_getv ? 2 : 1);

        if (i_argc != nargs)
        {
            fprintf(stderr, "%s", g_usage);
            exit(1);
        }

        key = i_argv[0];

        if (i_getv)
        {
            index = i_argv[1];
            index_val = strtol(index, 0, 0);
        }
        else
        {
            index = "";
            index_val = 0;
        }

        // Search for the item to determine its type, then case split on the
        // type.

        rc = p9_xip_find(i_image, key, &item);

        if (rc)
        {
            break;
        }

        if (index_val < 0)
        {
            fprintf(stderr,
                    "Illegal negative vector index %s for %s\n",
                    index, key);
            exit(1);
        }
        else if ((item.iv_elements != 0) &&
                 (index_val >= item.iv_elements))
        {
            fprintf(stderr, "Index %s out-of-bounds for %s (%d elements)\n",
                    index, key, item.iv_elements);
            exit(1);
        }

        switch (item.iv_type)
        {

            case P9_XIP_UINT8:
            case P9_XIP_UINT16:
            case P9_XIP_UINT32:
            case P9_XIP_UINT64:
                rc = p9_xip_get_element(i_image, key, index_val, &data);

                if (rc)
                {
                    rc = P9_XIP_BUG;
                    break;
                }

                switch (item.iv_type)
                {
                    case P9_XIP_UINT8:
                        printf("0x%02x\n", (uint8_t)data);
                        break;

                    case P9_XIP_UINT16:
                        printf("0x%04x\n", (uint16_t)data);
                        break;

                    case P9_XIP_UINT32:
                        printf("0x%08x\n", (uint32_t)data);
                        break;

                    case P9_XIP_UINT64:
                        printf("0x%016lx\n", data);
                        break;

                    default:
                        break;
                }

                break;

            case P9_XIP_ADDRESS:
                if (i_getv)
                {
                    fprintf(stderr, "Can't use 'getv' for address data : %s\n",
                            key);
                    exit(1);
                }

                rc = p9_xip_get_scalar(i_image, key, &data);

                if (rc)
                {
                    rc = P9_XIP_BUG;
                    break;
                }

                printf("0x%012lx\n", data);
                break;

            case P9_XIP_STRING:
                if (i_getv)
                {
                    fprintf(stderr, "Can't use 'getv' for string data : %s\n",
                            key);
                    exit(1);
                }

                rc = p9_xip_get_string(i_image, key, &s);

                if (rc)
                {
                    rc = P9_XIP_BUG;
                    break;
                }

                printf("%s\n", s);
                break;

            default:
                fprintf(stderr, "%s%d : Bug, unexpected type %d\n",
                        __FILE__, __LINE__, item.iv_type);
                exit(1);
                break;
        }
    }
    while (0);

    return rc;
}


// Append a file to section
static int
append(const char* i_imageFile, const int i_imageFd, void* io_image,
       int i_argc, const char** i_argv)
{
    uint8_t i_ddSupport = 0;
    int fileFd, newImageFd, sectionId, rc;
    struct stat buf;
    const char* section;
    const char* file;
    void* appendImage;
    void* newImage;
    uint32_t size, newSize, sectionOffset;
    uint64_t homerAddress;
    P9XipHeader header;

    do
    {

        // Basic syntax check: <section> <file>

        if (i_argc != 2 && i_argc != 3)
        {
            fprintf(stderr, "%s", g_usage);
            exit(1);
        }

        section = i_argv[0];
        file = i_argv[1];

        if (i_argc == 3)
        {
            if (strcmp(i_argv[2], "1") == 0)
            {
                i_ddSupport = 1;
            }
            else if (strcmp(i_argv[2], "0") == 0)
            {
                i_ddSupport = 0;
            }
            else
            {
                fprintf(stderr, "%s", g_usage);
                exit(1);
            }
        }

        p9_xip_translate_header(&header, (P9XipHeader*)io_image);

        // Translate the section name to a section Id
        sectionId = get_sectionId(header.iv_magic, section);

        if (sectionId < 0)
        {
            fprintf(stderr, "Unrecognized section name : '%s\n", section);
            exit(1);
        }

        // Open and mmap the file to be appended

        fileFd = open(file, O_RDONLY);

        if (fileFd < 0)
        {
            fprintf(stderr, "open() of the file (=%s) to be appended failed\n", file);
            exit(1);
        }

        rc = fstat(fileFd, &buf);

        if (rc)
        {
            perror("fstat() of the file to be appended failed : ");
            exit(1);
        }

        appendImage = mmap(0, buf.st_size, PROT_READ, MAP_SHARED, fileFd, 0);

        if (appendImage == MAP_FAILED)
        {
            perror("mmap() of the file to be appended failed : ");
            exit(1);
        }


        // malloc() a buffer for the new image, adding space for alignment

        rc = p9_xip_image_size(io_image, &size);

        if (rc)
        {
            break;
        }

        newSize = size + buf.st_size + P9_XIP_MAX_SECTION_ALIGNMENT;

        newImage = malloc(newSize);

        if (newImage == 0)
        {
            fprintf(stderr, "Can't malloc() a buffer for the new image\n");
            exit(1);
        }


        // Copy the image.  At this point the original image file must be
        // closed.

        memcpy(newImage, io_image, size);

        rc = close(i_imageFd);

        if (rc)
        {
            perror("close() of the original image file failed : ");
            exit(1);
        }


        // Do the append and print the image address where the data was loaded.
        // We will not fail for unaligned addresses, as we have no knowledge
        // of whether or why the user wants the final image address.

        rc = p9_xip_append(newImage,
                           sectionId,
                           appendImage,
                           buf.st_size,
                           newSize,
                           &sectionOffset,
                           i_ddSupport);

        if (rc)
        {
            break;
        }

        rc = p9_xip_section2image(newImage, sectionId, sectionOffset,
                                  &homerAddress);

        if (rc && (rc != P9_XIP_ALIGNMENT_ERROR))
        {
            break;
        }

        printf("0x%016lx\n", homerAddress);


        // Now write the new image back to the filesystem

        newImageFd = open(i_imageFile, O_WRONLY | O_TRUNC);

        if (newImageFd < 0)
        {
            perror("re-open() of image file failed : ");
            exit(1);
        }

        rc = p9_xip_image_size(newImage, &size);

        if (rc)
        {
            break;
        }

        rc = write(newImageFd, newImage, size);

        if ((rc < 0) || ((uint32_t)rc != size))
        {
            perror("write() of modified image failed : ");
            exit(1);
        }

        rc = close(newImageFd);

        if (rc)
        {
            perror("close() of modified image failed : ");
            exit(1);
        }
    }
    while (0);

    return rc;
}

// Extract section from an image incl a DD-specific sub-section.
static int
extract(void* i_image,
        int i_argc,
        const char** i_argv,
        image_section_type_t i_imageSectionType)
{
    int rc = INFRASTRUCT_RC_SUCCESS;
    const char* i_sectionName;  //Direct copy of input arg, thus i_
    const char* i_fileName;     //Same
    std::string i_ddLevelStr;   //Same
    uint8_t ddLevel = UNDEFINED_DD_LEVEL;
    MyBool_t  bDdSupportExpected = UNDEFINED_BOOLEAN;
    MyBool_t  bDdSupport = UNDEFINED_BOOLEAN;
    int          fileFd;
    int          sectionId;
    P9XipHeader  xipHeader;
    P9XipSection xipSection;
    uint8_t*     ddcoSection;
    uint8_t*     finalSection; // Final (whole or DD specific) section to copy to file
    uint32_t     finalSectionSize;
    void*        newImage;

    do
    {

        if (i_imageSectionType == IST_XIP)
        {
            if (i_argc == 2)
            {
                i_sectionName = i_argv[0];
                i_fileName = i_argv[1];

                bDdSupportExpected = false;
            }
            else if (i_argc == 3)
            {
                i_sectionName = i_argv[0];
                i_ddLevelStr = i_argv[1];
                i_fileName = i_argv[2];
                bDdSupportExpected = true;
                ddLevel = strtol(i_ddLevelStr.c_str(), NULL, 16);
            }
            else
            {
                fprintf(stderr, "\nToo few or too many arguments passed to the 'extract' for an XIP image.\n");
                fprintf(stderr,
                        "The 'extract' command must be followed by a valid XIP section name (e.g., .rings), then optionally a ddLevel (in hex format), and then a filename.\n");
                exit(EXIT_FAILURE);
            }

            p9_xip_translate_header(&xipHeader, (P9XipHeader*)i_image);
            sectionId = get_sectionId(xipHeader.iv_magic, i_sectionName);

            if (sectionId < 0)
            {
                fprintf(stderr, "\nUnrecognized section name : '%s;\n", i_sectionName);
                exit(EXIT_FAILURE);
            }

            printf("\nInput parms to the 'extract' command:\n"
                   "  Section:      %s\n", i_sectionName);

            if (bDdSupportExpected)
            {
                printf("  DD level (numerical hex value): %s (0x%x)\n",
                       i_ddLevelStr.c_str(), ddLevel);
            }

            printf("  Output file:  %s\n", i_fileName);

            rc = p9_xip_dd_section_support(i_image, sectionId, UNDEFINED_IPL_IMAGE_SID, &bDdSupport);

            if (rc)
            {
                fprintf(stderr, "\np9_xip_dd_section_support() failed w/rc=0x%x\n", rc);
                exit(EXIT_FAILURE);
            }

            if (bDdSupportExpected && !bDdSupport)
            {
                fprintf(stderr, "\nSection \"%s\" has no DD level support.\n", i_sectionName);
                fprintf(stderr, "To extract the entire section, omit the <ddLevel> arg.\n");
                exit(EXIT_FAILURE);
            }

            // Get the XIP section
            rc = p9_xip_get_section(i_image, sectionId, &xipSection, UNDEFINED_DD_LEVEL);

            if (rc)
            {
                fprintf(stderr, "\np9_xip_get_section() failed w/rc=0x%x\n", rc);
                exit(EXIT_FAILURE);
            }

            if (bDdSupportExpected)
            {
                ddcoSection = (uint8_t*)i_image + xipSection.iv_offset;
            }
            else
            {
                finalSection = (uint8_t*)i_image + xipSection.iv_offset;
                finalSectionSize = xipSection.iv_size;
            }
        }
        else if (i_imageSectionType == IST_DDCO)
        {
            if (i_argc == 2)
            {
                i_ddLevelStr = i_argv[0];
                i_fileName = i_argv[1];
            }
            else
            {
                fprintf(stderr, "\nToo few or too many arguments passed to the 'extract' command for a DDCO image.\n");
                fprintf(stderr, "The 'extract' command must be followed by a ddLevel (in hex format) and then a filename.\n");
                exit(EXIT_FAILURE);
            }

            ddLevel = strtol(i_ddLevelStr.c_str(), NULL, 16);
            printf("\nInput parms to the 'extract' command:\n"
                   "  DD level (converted value): %s (0x%x)\n"
                   "  Output file:  %s\n",
                   i_ddLevelStr.c_str(), ddLevel, i_fileName);

            bDdSupportExpected = true;

            ddcoSection = (uint8_t*)i_image;
        }
        else if (i_imageSectionType == IST_TOR)
        {
            fprintf(stderr, "\nNothing to do for a TOR image. Image is already fully extracted.\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            fprintf(stderr, "\nUnsupported image section type (=%d)\n",
                    i_imageSectionType);
            exit(EXIT_FAILURE);
        }

        // Extract the DD specific section
        if (bDdSupportExpected)
        {
            rc = p9_dd_get(ddcoSection, ddLevel, &finalSection, &finalSectionSize);
        }

        // Copy the final section into the file
        newImage = malloc(finalSectionSize);

        if (newImage == 0)
        {
            fprintf(stderr, "\nCan't malloc() a buffer for the new image\n");
            exit(1);
        }

        memcpy(newImage, finalSection, finalSectionSize);

        fileFd = open(i_fileName, O_CREAT | O_WRONLY | O_TRUNC, 0755);

        if (fileFd < 0)
        {
            perror("open() of the fixed section : ");
            exit(1);
        }

        rc = write(fileFd, newImage, finalSectionSize);

        if (rc < 0 || (uint32_t)rc != finalSectionSize)
        {
            perror("write() of fixed section : ");
            exit(EXIT_FAILURE);
        }

        rc = close(fileFd);

        if (rc)
        {
            perror("close() of fixed section : ");
            exit(EXIT_FAILURE);
        }

    }
    while (0);

    return rc;

}


// Delete 0 or more sections in order.

static int
deleteSection(const char* i_imageFile, const int i_imageFd, void* io_image,
              int i_argc, const char** i_argv)
{
    int newImageFd, sectionId, rc, argc;
    const char* section;
    const char** argv;
    void* newImage;
    void* tempImage;
    uint32_t size;
    P9XipHeader header;

    do
    {

        // malloc() a buffer for the new image

        rc = p9_xip_image_size(io_image, &size);

        if (rc)
        {
            break;
        }

        newImage = malloc(size);

        if (newImage == 0)
        {
            fprintf(stderr, "Can't malloc() a buffer for the new image\n");
            exit(1);
        }


        // Copy the image. At this point the original image file must be
        // closed.

        memcpy(newImage, io_image, size);

        rc = close(i_imageFd);

        if (rc)
        {
            perror("close() of the original image file failed : ");
            exit(1);
        }

        // Create a temporary place holder for image

        tempImage = malloc(size);

        if (tempImage == 0)
        {
            fprintf(stderr, "Can't malloc() a buffer for the temporary image\n");
            exit(1);
        }


        p9_xip_translate_header(&header, (P9XipHeader*)io_image);

        // Delete the sections in argument order

        for (argc = i_argc, argv = i_argv; argc != 0; argc--, argv++)
        {

            // Translate the section name to a section Id

            section = *argv;

            sectionId = get_sectionId(header.iv_magic, section);

            if (sectionId < 0)
            {
                fprintf(stderr, "Unrecognized section name : '%s;\n", section);
                exit(1);
            }

            // Delete the section

            rc = p9_xip_delete_section(newImage, tempImage, size, sectionId);

            if (rc)
            {
                break;
            }
        }

        if (rc)
        {
            break;
        }

        // Print the final size of the new image

        rc = p9_xip_image_size(newImage, &size);

        if (rc)
        {
            break;
        }

        printf("%u\n", size);

        // Now write the new image back to the filesystem

        newImageFd = open(i_imageFile, O_WRONLY | O_TRUNC);

        if (newImageFd < 0)
        {
            perror("re-open() of image file failed : ");
            exit(1);
        }

        rc = write(newImageFd, newImage, size);

        if ((rc < 0) || ((uint32_t)rc != size))
        {
            perror("write() of modified image failed : ");
            exit(1);
        }

        rc = close(newImageFd);

        if (rc)
        {
            perror("close() of modified image failed : ");
            exit(1);
        }
    }
    while (0);

    return rc;
}


// 'TEST' is an undocumented command provided to test the APIs.  It searches
// and modifies a copy of the image but puts the image back together as it
// was, then verifies that the the original image and the copy are identical.

#define BOMB_IF(test)                                   \
    if (test) {                                         \
        fprintf(stderr, "%s:%d : Error in TEST\n",      \
                __FILE__, __LINE__);                    \
        exit(1);                                        \
    }

#define BOMB_IF_RC                                              \
    if (rc) {                                                   \
        fprintf(stderr, "%s:%d : Error in TEST, rc = %s\n",     \
                __FILE__, __LINE__,                             \
                P9_XIP_ERROR_STRING(g_errorStrings, rc));      \
        exit(1);                                                \
    }


static int
TEST(void* io_image, const int i_argc, const char** i_argv)
{
    int rc;
    uint64_t linkAddress, entryPoint, data, data1, magicKey, L1_LoaderAddr[2];
    char* key, *revision, *revdup, *longString, *shortString;
    void* originalImage;
    uint32_t imageSize;
    P9XipItem item;
    P9XipHeader header;
    P9XipSection section;
    //ProcSbeFixed* fixed;
    uint32_t tocSize;

    do
    {
        rc = p9_xip_image_size(io_image, &imageSize);
        BOMB_IF_RC;
        originalImage = malloc(imageSize);
        BOMB_IF(originalImage == 0);
        memcpy(originalImage, io_image, imageSize);

        rc = p9_xip_get_scalar(io_image, "toc_sorted", &data);
        BOMB_IF_RC;
        BOMB_IF(data != 1);

        rc = p9_xip_get_scalar(io_image, "image_size", &data);
        BOMB_IF_RC;
        BOMB_IF(data != (uint64_t)g_imageSize);

        rc = p9_xip_get_scalar(io_image, "magic", &magicKey);
        BOMB_IF_RC;

        switch (magicKey)
        {
            case P9_XIP_MAGIC_SEEPROM:
                key = (char*)"";
                // Can't do this test here as the TOC has been stripped
                break;

            default:
                BOMB_IF(1);
                break;
        }

        rc = p9_xip_get_scalar(io_image, "link_address", &linkAddress);
        BOMB_IF_RC;

        if (magicKey != P9_XIP_MAGIC_SEEPROM)
        {
            rc = p9_xip_get_scalar(io_image, "entry_point", &entryPoint);
            BOMB_IF_RC;
        }

        rc = p9_xip_get_scalar(io_image, "L1_LoaderAddr", &data);
        BOMB_IF_RC;
        BOMB_IF((magicKey != P9_XIP_MAGIC_SEEPROM) && (entryPoint != (linkAddress + data)));

        rc =
            p9_xip_set_scalar(io_image, "toc_sorted", 0) ||
            p9_xip_set_scalar(io_image, "image_size", 0);
        BOMB_IF_RC;

        data = 0;
        data += (rc = p9_xip_get_scalar(io_image, "toc_sorted", &data), data);
        BOMB_IF_RC;
        data += (rc = p9_xip_get_scalar(io_image, "image_size", &data), data);
        BOMB_IF_RC;
        BOMB_IF(data != 0);

        // Write back keys found during read check.

        rc =
            p9_xip_set_scalar(io_image, "toc_sorted", 1) ||
            p9_xip_set_scalar(io_image, "image_size", g_imageSize);
        BOMB_IF_RC;

        // We'll rewrite the revision keyword with a long string and a short
        // string, and verify that rewriting is being done correctly.  In the
        // end we copy the original revision string back in, which is safe
        // because the memory allocation for strings does not change when they
        // are modified.

        revdup = strdup(revision);
        longString = (char*)"A very long string";
        shortString = (char*)"?";

        if (magicKey != P9_XIP_MAGIC_SEEPROM)
        {
            rc =
                p9_xip_set_string(io_image, key, longString) ||
                p9_xip_get_string(io_image, key, &revision);
            BOMB_IF_RC;
            BOMB_IF((strlen(revision) != strlen(revdup)) ||
                    (strncmp(revision, longString, strlen(revdup)) != 0));

            rc =
                p9_xip_set_string(io_image, key, shortString) ||
                p9_xip_get_string(io_image, key, &revision);
            BOMB_IF_RC;
            BOMB_IF(strcmp(revision, shortString) != 0);

            memcpy(revision, revdup, strlen(revdup) + 1);
        }

        // Use p9_xip_[read,write]_uint64 to modify the image and restore it
        // to its original form.

        rc = p9_xip_find(io_image, "L1_LoaderAddr", &item);
        BOMB_IF_RC;
        rc = p9_xip_get_scalar(io_image, "L1_LoaderAddr", &(L1_LoaderAddr[0]));
        BOMB_IF_RC;

        rc = p9_xip_read_uint64(io_image, item.iv_address, &(L1_LoaderAddr[1]));
        BOMB_IF_RC;
        BOMB_IF(L1_LoaderAddr[0] != L1_LoaderAddr[1]);

        rc = p9_xip_write_uint64(io_image, item.iv_address,
                                 0xdeadbeefdeadc0deull);
        BOMB_IF_RC;
        rc = p9_xip_read_uint64(io_image, item.iv_address, &(L1_LoaderAddr[1]));
        BOMB_IF_RC;
        BOMB_IF(L1_LoaderAddr[1] != 0xdeadbeefdeadc0deull);

        rc = p9_xip_write_uint64(io_image, item.iv_address, L1_LoaderAddr[0]);
        BOMB_IF_RC;

        // Try p9_xip_get_section against the translated header

        p9_xip_translate_header(&header, (P9XipHeader*)io_image);
        rc = p9_xip_get_section(io_image, P9_XIP_SECTION_TOC, &section, UNDEFINED_DD_LEVEL);
        BOMB_IF_RC;
        BOMB_IF((section.iv_size !=
                 header.iv_section[P9_XIP_SECTION_TOC].iv_size));


        // Make sure the .fixed section access compiles and seems to
        // work. Modify an entry via the .fixed and verify it with normal TOC
        // access.

        if (magicKey == P9_XIP_MAGIC_SEEPROM)
        {

            BOMB_IF(0 != 0);

            exit(1);

            rc = p9_xip_get_scalar(io_image, "proc_p9_ex_dpll_initf_control",
                                   &data);
            BOMB_IF_RC;
            //fixed =
            //(ProcSbeFixed*)((unsigned long)io_image + P9_XIP_FIXED_OFFSET);
            //fixed->proc_p9_ex_dpll_initf_control = 0xdeadbeefdeadc0deull;
            rc = p9_xip_get_scalar(io_image, "proc_p9_ex_dpll_initf_control",
                                   &data1);
            BOMB_IF_RC;
#ifdef _BIG_ENDIAN
            BOMB_IF(data1 != 0xdeadbeefdeadc0deull);
#else
            BOMB_IF(data1 != 0xdec0addeefbeaddeull);
#endif
            rc = p9_xip_set_scalar(io_image, "proc_p9_ex_dpll_initf_control",
                                   data);
            BOMB_IF_RC;
        }

        // Temporarily "delete" the .toc section and try to get/set via the
        // mini-TOC for .fixed, and make sure that we can't get things that
        // are not in the mini-toc.

        tocSize =
            ((P9XipHeader*)io_image)->iv_section[P9_XIP_SECTION_TOC].iv_size;

        ((P9XipHeader*)io_image)->iv_section[P9_XIP_SECTION_TOC].iv_size =
            0;

        rc = p9_xip_get_scalar(io_image, "proc_p9_ex_dpll_initf_control",
                               &data);
        rc = p9_xip_set_scalar(io_image, "proc_p9_ex_dpll_initf_control",
                               0xdeadbeef);
        rc = p9_xip_get_scalar(io_image, "proc_p9_ex_dpll_initf_control",
                               &data1);
        BOMB_IF(data1 != 0xdeadbeef);
        rc = p9_xip_set_scalar(io_image, "proc_p9_ex_dpll_initf_control",
                               data);
        BOMB_IF_RC;

        BOMB_IF(p9_xip_find(io_image, "proc_p9_ex_dpll_initf", 0) !=
                P9_XIP_ITEM_NOT_FOUND);

        ((P9XipHeader*)io_image)->iv_section[P9_XIP_SECTION_TOC].iv_size =
            tocSize;

        if (magicKey != P9_XIP_MAGIC_SEEPROM)
        {
            BOMB_IF(p9_xip_find(io_image, "proc_p9_ex_dpll_initf", 0) != 0);
        }

        printf("\nYou will see an expected warning below about P9_XIP_WOULD_OVERFLOW\n"
               "It means the TEST is working (not failing)\n\n");

        // Finally compare against the original
        BOMB_IF(memcmp(io_image, originalImage, imageSize));

    }
    while (0);

    return rc;
}

// Prints out the raw decompressed RS4 ring content
void print_raw_ring( uint8_t*  data,
                     uint32_t  bits )
{
    uint32_t i;
    uint8_t  bytePerWordCount = 0; // Nibble count in each word
    uint32_t bytePerLineCount = 0; // Column count
    uint8_t  rem = bits % 8;      // Rem raw bits beyond 1-byte boundary
    uint8_t  nibblesToPrint;      // The last 1 or 2 nibbles to dump

    for (i = 0; i < bits / 8; i++)
    {
        printf("%02x", *(data + i));

        if (++bytePerWordCount == 4)
        {
            printf(" ");
            bytePerWordCount = 0;
        }

        if (++bytePerLineCount == 32)
        {
            printf("\n");
            bytePerLineCount = 0;
        }
    }

    // Dump remaining bits (in whole nibbles and with any
    //   unused bits being zeroed)
    if (rem)
    {
        // Ensure the rightmost (8-rem) unused bits are zeroed out
        nibblesToPrint = (*(data + i) >> (8 - rem)) << (8 - rem);

        if (rem <= 4)
        {
            // Content only in first nibble. Dump only first nibble
            printf("%01x", nibblesToPrint >> 4);
        }
        else
        {
            // Content in both nibbles. Dump both nibbles
            printf("%02x", nibblesToPrint);
        }
    }

    printf("\n");
}


#if !defined(__PPE__) && !defined(OPENPOWER_BUILD)  // Needed on the ppe & OP-Build side to avoid TOR API

/// Function:  dissectRingSection()
///
/// Brief:  Dissects and summarizes content of a ring section.
///
/// \param[in] i_ringSection    A pointer to a TOR compliant ring section.
///
/// \param[in] i_listingModeId  The listing mode: {table, normal(default), long, raw}.
///
static
int dissectRingSection( uint8_t*    i_ringSection,
                        uint8_t     i_listingModeId )
{
    int         rc = INFRASTRUCT_RC_SUCCESS;
    uint32_t    i;
    RingId_t    numRingIds = 0;
    uint32_t    torMagic;
    ChipId_t    chipId = UNDEFINED_CHIP_ID;
    uint8_t     ddLevel = UNDEFINED_DD_LEVEL;
    RingId_t    ringId;
    RingId_t    rpIndex;
    uint8_t     chipletId;
    RingProperties_t* ringProps = NULL;
    ChipletData_t*    chipletData = NULL;
    void*       rs4Buf;
    uint32_t    ringBlockSize;
    char*       ringName = NULL;
    uint32_t    ringSeqNo  = 0; // Ring sequence number
    CompressedScanData* rs4 = NULL;
    CompressedScanData* rs4ForDisplay = NULL;
    uint32_t    maxRingBufSize;
    void*       dataBuf;
    void*       careBuf;
    uint8_t*    data;
    uint8_t*    care;
    uint32_t    bits;
    uint16_t    rs4Size;
    uint8_t     typeField;  // The RS4 header iv_type field
    double      comprRate;
    MyBool_t bRingsFound = UNDEFINED_BOOLEAN;
    MyBool_t bPrintHeader = UNDEFINED_BOOLEAN;


    //
    // Get TOR header fields
    //
    TorHeader_t* torHeader = reinterpret_cast<TorHeader_t*>(i_ringSection);
    torMagic    = be32toh(torHeader->magic);
    chipId      = torHeader->chipId;
    ddLevel     = torHeader->ddLevel;

    //
    // Get some chip ID specific metadata
    //
    rc = ringid_get_num_ring_ids( chipId,
                                  &numRingIds );

    if (rc)
    {
        fprintf(stderr, "ringid_get_num_ring_ids() failed w/rc=0x%08x for chipId=%d\n",
                (uint32_t)rc, chipId);
        exit(EXIT_FAILURE);
    }

    rc = ringid_get_ringProps( chipId,
                               &ringProps );

    if (rc)
    {
        fprintf(stderr, "ringid_get_ringProps() failed w/rc=0x%08x for chipId=%d\n",
                (uint32_t)rc, chipId);
        exit(EXIT_FAILURE);
    }

    //
    // Allocate buffer to hold max length RS4 ring
    //
    rs4Buf = operator new(MAX_RING_BUF_SIZE_TOOL);

    if (!rs4Buf)
    {
        fprintf(stderr, "malloc of rs4Buf failed!\n");
        exit(EXIT_FAILURE);
    }

    //
    // Allocate buffers to hold decompressed raw rings for data and care
    //
    maxRingBufSize = MAX_RING_BUF_SIZE_TOOL;

    dataBuf = operator new(maxRingBufSize);
    careBuf = operator new(maxRingBufSize);

    if (!dataBuf || !careBuf)
    {
        fprintf(stderr, "malloc of dataBuf or careBuf failed!\n");
        operator delete(rs4Buf);
        operator delete(dataBuf);
        operator delete(careBuf);
        exit(EXIT_FAILURE);
    }

    data = (uint8_t*)dataBuf;
    care = (uint8_t*)careBuf;

    // Assume no rings will be found so on the next loop so we can print
    // the info
    bRingsFound = false;
    bPrintHeader = true;

    //--------------------------------
    // Master Ring ID / RP Index loop
    //--------------------------------
    // This loop supports traversing and retrieving rings from both TOR
    // and DYN ring sections.
    //
    // In each case, there's an additional identifier, beyond ringId, that
    // is needed to fully qualify the identification of the ring as follows:
    //
    // TOR ring section:
    // - chipletId used to identify the TOR instance slot
    // - Note that iv_selector = 0xffff
    // DYN ring section:
    // - iv_selector in the RS4 header
    // - Note that chipletId is non-care for DYN rings (since always Common rings)
    //
    // So since in both cases we need to traverse a "sub-qualifier" ID
    // value, we introduce the variable, subQualId, for this purpose:
    // - subQualId represents chipletId for TOR ring sections
    // - subQualId represents selector for DYN ring section
    //
    uint16_t subQualId;
    uint16_t MIN_QUAL_ID = 0;
    uint16_t MAX_QUAL_ID = 0xffff;
    Rs4Selector_t selector = UNDEFINED_RS4_SELECTOR;
    uint8_t  numInst; // Number of chiplet instances

    rs4 = (CompressedScanData*)rs4Buf;

    for (rpIndex = 0; rpIndex < numRingIds; rpIndex++)
    {
        //Get the ringId
        ringId = ringProps[rpIndex].ringId;

        // Check the ringId validity
        rc = ringid_check_ringId( chipId, ringId);

        switch (rc)
        {
            case TOR_SUCCESS:
                // Good ringId, represents an ENGD ring. Continue...
                break;

            case TOR_HOLE_RING_ID:
                // Valid rc but nothing to do for hole ring ID.
                continue;

            case TOR_INVALID_RING_ID:
                // We should never get called with an invalid ringId
                MY_ERR("ERROR: dissectRingSection: ringid_check_ringId() returned"
                       " rc=TOR_INVALID_RING_ID (rc=0x%08x) for ringId=0x%08x\n",
                       rc, ringId);
                exit(EXIT_FAILURE);

            default:
                // Should never come here
                MY_ERR("CODE BUG: dissectRingSection: A non-supported rc(=0x%08x)"
                       " was returned by ringid_check_ringId(). Fix code.\n",
                       rc);
                exit(EXIT_FAILURE);
        }

        //
        // Get remaining metadata for the chipletType associated with the ringId
        //
        rc = ringid_get_chipletProps( chipId,
                                      torMagic,
                                      torHeader->version,
                                      ringProps[rpIndex].chipletType,
                                      &chipletData );

        if (rc)
        {
            if (rc == TOR_INVALID_CHIPLET_TYPE)
            {
                continue;
            }
            else
            {
                fprintf(stderr, "ERROR: ringid_get_chipletProps() failed w/rc=0x%08x\n", rc);
                return rc;
            }
        }

        //
        // Determine whether Common or Instance section
        //
        if ( ringProps[rpIndex].idxRing & INSTANCE_RING_MARK )
        {
            numInst = chipletData->numChipletInstances;
        }
        else
        {
            numInst = 1;
        }

        MIN_QUAL_ID = (torMagic != TOR_MAGIC_DYN) ?
                      chipletData->chipletBaseId :
                      0;
        MAX_QUAL_ID = (torMagic != TOR_MAGIC_DYN) ?
                      (chipletData->chipletBaseId + numInst - 1) :
                      MAX_RS4_SELECTOR;

        for (subQualId = MIN_QUAL_ID; subQualId <= MAX_QUAL_ID; subQualId++)
        {
            ringBlockSize = MAX_RING_BUF_SIZE_TOOL;

            if (torMagic != TOR_MAGIC_DYN)
            {
                chipletId = subQualId;

                rc = tor_get_single_ring( i_ringSection,
                                          ddLevel,
                                          ringId,
                                          chipletId,
                                          rs4Buf, //IO buffer (caller mgd)
                                          ringBlockSize,
                                          true, //No-care. =true to minimize chance of err code
                                          0 );

                // Here we will be forgiving with the RS4 MAGIC and VERSION rc since they will
                // be rechecked in rs4_decompress, in case we display the raw rings. And the
                // RS4 TYPE rc, we certainly want to also dissect in this case so we can see
                // which other rings might have same error conditions.
                if ( rc == TOR_SUCCESS || rc == TOR_INVALID_RS4_MAGIC ||
                     rc == TOR_INVALID_RS4_VERSION ||
                     rc == TOR_INVALID_RS4_TYPE )
                {
                    selector = be16toh(((CompressedScanData*)rs4Buf)->iv_selector);
                }
            }
            else
            {
                selector = subQualId;

                rc = dyn_get_ring( i_ringSection,
                                   ringId,
                                   selector,
                                   ddLevel,
                                   rs4Buf,          // IO buffer (caller managed)
                                   ringBlockSize,
                                   0 );

                // Here we will be forgiving with the RS4 MAGIC and VERSION rc since they will
                // be rechecked in rs4_decompress, in case we display the raw rings. And the
                // RS4 TYPE rc, we certainly want to also dissect in this case so we can see
                // which other rings might have same error conditions.
                if ( rc == TOR_SUCCESS || rc == TOR_INVALID_RS4_MAGIC ||
                     rc == TOR_INVALID_RS4_VERSION ||
                     rc == TOR_INVALID_RS4_TYPE )
                    if (!rc)
                    {
                        chipletId = be32toh(rs4->iv_scanAddr) >> 24;
                    }
            }

            //
            // Gather ring details and print it.
            //
            if (rc == TOR_SUCCESS)
            {
                if(bPrintHeader == true )
                {
                    // print the table header info
                    if (i_listingModeId == LMID_TABLE)
                    {
                        fprintf(stdout, "-----------------------------------------------------------------\n");
                        fprintf(stdout, "*                         Ring table                            *\n");
                        fprintf(stdout, "-----------------------------------------------------------------\n");

                        if (torMagic != TOR_MAGIC_DYN)
                        {
                            fprintf(stdout, "   #      DD    Inst      Bits   Compr   Name\n");
                        }
                        else
                        {
                            fprintf(stdout, "   #      DD    Sel       Bits   Compr   Name\n");
                        }

                        fprintf(stdout, "-----------------------------------------------------------------\n");
                    }
                    else
                    {
                        fprintf( stdout, "-----------------------------\n"
                                 "*       Ring summary        *\n");
                    }

                    bPrintHeader = false;
                }

                bRingsFound = true;

                // Sanity check RS4 container's ringId matches the requested.
                RingId_t ringIdTmp = be16toh(rs4->iv_ringId);

                if ( ringIdTmp != ringId )
                {
                    RingId_t rpIndexTmp = ringid_convert_ringId_to_rpIndex(ringIdTmp);

                    if ( (ringProps[rpIndexTmp].ringClass & RMRK_ROOT) !=
                         (ringProps[rpIndex].ringClass & RMRK_ROOT) )
                    {
                        fprintf(stderr, "COND in ipl_image_tool: Found a ring. But the"
                                " requested ringId"
                                " and the RS4 header ringId differ. However, since"
                                " one, and only one, of the rings is a ROOT ring,"
                                " it's not a valid find and we'll skip it.\n"
                                "Requested ringId: 0x%x\n"
                                "RS4 headr ringId: 0x%x\n",
                                ringId, ringIdTmp);
                    }
                    else
                    {
                        fprintf(stderr, "ERROR in ipl_image_tool:  Found a ring."
                                " But RS4 header ringId(=0x%x) differs from requested"
                                " ringId(=0x%x) and chipletId=0x%02x\n",
                                ringIdTmp, ringId, chipletId);
                        operator delete(rs4Buf);
                        operator delete(dataBuf);
                        operator delete(careBuf);
                        exit(EXIT_FAILURE);
                    }
                }

                // Check ring block size is !=zero.
                if ( be16toh(rs4->iv_size) == 0 )
                {
                    fprintf(stderr, "ERROR in ipl_image_tool: Found"
                            " a ring. But RS4 header's iv_size is zero.\n");
                    operator delete(rs4Buf);
                    operator delete(dataBuf);
                    operator delete(careBuf);
                    exit(EXIT_FAILURE);
                }

                ringSeqNo++;

                ringName = ringProps[rpIndex].ringName;

                data = (uint8_t*)dataBuf;
                care = (uint8_t*)careBuf;

                // - Decompress ring to obtain ring length and to verify compressed string

                rs4ForDisplay = rs4;
                rc = _rs4_decompress(data, care, maxRingBufSize, &bits, rs4);

                if (rc)
                {
                    fprintf(stderr, "rs4 decompress error %d\n", rc);
                    exit(EXIT_FAILURE);
                }

                rs4Size = be16toh(rs4ForDisplay->iv_size);

                comprRate = (double)rs4Size * 8.0 / (double)bits * 100.0;

                typeField = rs4ForDisplay->iv_type;

                // tabular ring list if "table".
                if (i_listingModeId == LMID_TABLE)
                {
                    fprintf(stdout,
                            "%4i    "
                            "0x%02x   "
                            "%3u    " // chipletId for TOR, selector for DYN
                            "%7d  "
                            "%6.2f   "
                            "%s\n",
                            ringSeqNo, ddLevel, subQualId, bits, comprRate, ringName);
                }

                // Summarize all characteristics of the ring block if "normal", "long" or "raw"
                if ( i_listingModeId == LMID_NORMAL ||
                     i_listingModeId == LMID_LONG ||
                     i_listingModeId == LMID_RAW )
                {
                    fprintf( stdout,
                             "-----------------------------\n"
                             "%i\n"
                             "ddLevel = 0x%02x\n"
                             "ringId = 0x%03x (%u)\n"
                             "ringName = %s\n"
                             "chipletId = 0x%02x\n"
                             "iv_type = 0x%02x\n"
                             "selector = %u\n"
                             "rs4Size [bytes] = 0x%x\n"
                             "rawLength [bits] = 0x%x\n"
                             "compression [%%] = %6.2f\n",
                             ringSeqNo, ddLevel, ringId, ringId, ringName,
                             chipletId, typeField, selector, rs4Size, bits, comprRate);
                }

                // Dump ring block if "long" or "raw"
                if ( i_listingModeId == LMID_LONG ||
                     i_listingModeId == LMID_RAW )
                {
                    fprintf(stdout, "Binary ring block dump:\n");

                    // Output 8 bytes per line (in 2 byte chunks)
                    for (i = 0; i < rs4Size / 8; i++)
                    {
                        fprintf( stdout,
                                 "%04x: %04x %04x %04x %04x\n",
                                 i * 8,
                                 (uint16_t)( htobe64(*((uint64_t*)rs4ForDisplay + i)) >> 48),
                                 (uint16_t)( htobe64(*((uint64_t*)rs4ForDisplay + i)) >> 32),
                                 (uint16_t)( htobe64(*((uint64_t*)rs4ForDisplay + i)) >> 16),
                                 (uint16_t)( htobe64(*((uint64_t*)rs4ForDisplay + i))) );
                    }

                    // Output rem above 8 bytes (in 2 byte chunks, 1 byte resolution)
                    uint8_t l_rem = rs4Size - rs4Size / 8 * 8;

                    if (l_rem)
                    {
                        fprintf( stdout, "%04x:", i * 8);

                        for (uint8_t ii = 0; ii < l_rem; ii++)
                        {
                            if ( (ii - ii / 2 * 2) == 0 )
                            {
                                fprintf( stdout, " ");
                            }

                            fprintf( stdout, "%02x",
                                     (uint8_t)( htobe64(*((uint64_t*)rs4ForDisplay + i)) >> (56 - ii * 8)) );
                        }
                    }

                    fprintf( stdout, "\n");
                }

                // Below we dump the raw decompressed ring content in the exact same
                //   format that it appears as in EKB's ifCompiler generated raw ring
                //   files, i.e. *.bin.srd (DATA) and *.bin.srd.bitsModified (CARE).
                if (i_listingModeId == LMID_RAW)
                {
                    fprintf( stdout, "\nRaw decompressed DATA nibbles:\n");
                    print_raw_ring( data, bits);

                    fprintf( stdout, "\nRaw decompressed CARE nibbles:\n");
                    print_raw_ring( care, bits);

                    fprintf( stdout, "\n");
                }

            }
            else if ( rc == TOR_RING_IS_EMPTY        ||
                      rc == TOR_RING_HAS_NO_TOR_SLOT ||
                      rc == TOR_INVALID_CHIPLET_ID   ||
                      rc == TOR_INVALID_CHIPLET_TYPE ||
                      rc == TOR_AMBIGUOUS_API_PARMS  ||
                      rc == TOR_INVALID_RING_ID      ||
                      rc == TOR_HOLE_RING_ID         ||
                      rc == TOR_DYN_RING_NOT_FOUND )
            {
                // All these errors are acceptable in the context of ipl_image_tool dissect.
                rc = INFRASTRUCT_RC_SUCCESS;
            }
            else
            {
                fprintf(stderr, "CODE BUG: tor_get_single_ring() or dyn_get_ring() returned"
                        " unacceptable error code rc=%d\n", rc);
                operator delete(rs4Buf);
                operator delete(dataBuf);
                operator delete(careBuf);
                exit(EXIT_FAILURE);
            }

        }  // End of for(subQualId={chipletId or selector})

    }  // End of for(rpIndex)

    if( bRingsFound == false )
    {
        fprintf(stdout, "No rings for DD level: 0x%x\n", ddLevel);
    }

    if (i_listingModeId == LMID_TABLE)
    {
        fprintf(stdout, "------------------------------------------------------------------------------\n");
    }
    else
    {
        fprintf(stdout, "------------------------------------------\n");
    }

    operator delete(rs4Buf);
    operator delete(dataBuf);
    operator delete(careBuf);

    return rc;
}


/// Function:  unpack_ddco_image()
///
/// Brief:  Unpacks DDCO image and puts DD sections into vector iterator.
///
/// \param[in]  i_ddcoImage    A pointer to a DDCO type image.
///
/// \param[in]  i_sectionName  The name of the XIP section (if any).
///
/// \param[out] o_ringSectionPtrs  Vector containing ptrs to the individual DD sub-sections in DDCO.
///
/// Assumptions:
///
static int
unpack_ddco_image( const void* i_ddcoImage,
                   const char* i_sectionName,
                   std::vector<void*>& o_ringSectionPtrs )
{
    int     rc = INFRASTRUCT_RC_SUCCESS;
    struct  p9_dd_block* block;
    struct  p9_dd_block  block_he;
    struct  p9_dd_iter iter = {NULL, 0};
    uint8_t*  ringSection = NULL;
    uint32_t  ringSectionSize;
    image_section_type_t  imageSectionType = IST_UNDEFINED;

    // Initialize our iterator to the beginning of the DDCO container
    iter.iv_cont = (struct p9_dd_cont*)i_ddcoImage;

    fprintf(stdout, "----------------------------------------------\n");

    fprintf(stdout, "*            DD container summary            *\n");

    fprintf(stdout, "----------------------------------------------\n");

    if (i_sectionName == NULL)
    {
        fprintf(stdout, "XIP section   : Standalone (not an XIP section)\n");
    }
    else
    {
        fprintf(stdout, "XIP section   : %s\n", i_sectionName);
    }

    fprintf(stdout, "Num DD levels : %d\n", iter.iv_cont->iv_num);

    fprintf(stdout, "----------------------------------------------\n");

    fprintf(stdout, "\n");

    // For each DD specific block, grab the TOR ringSection
    while ((block = p9_dd_next(&iter)))
    {
        p9_dd_betoh(block, &block_he);

        rc = p9_dd_get((uint8_t*)i_ddcoImage, block_he.iv_dd, &ringSection, &ringSectionSize);

        if (rc)
        {
            fprintf(stderr, "ERROR: Failed getting ddLevel=0x%#x sub-section from DDCO "
                    "image w/rc=%d\n",
                    block_he.iv_dd, rc);
            exit(EXIT_FAILURE);
        }

        if (block_he.iv_dd != ((TorHeader_t*)ringSection)->ddLevel)
        {
            //Error trace and exit.
            fprintf(stderr, "ERROR: Incorrect DD level returned from container.\n"
                    "\tExpected:0x%#x  Got:0x%#x\n",
                    block_he.iv_dd, ((TorHeader_t*)ringSection)->ddLevel);
            exit(EXIT_FAILURE);
        }

        resolve_image_section_type( (void*)ringSection, imageSectionType);

        if (imageSectionType != IST_TOR)
        {
            fprintf( stderr, "\nERROR: The DDCO sub-section's type (=%d) is not the "
                     "TOR type (=%d).\n",
                     imageSectionType, IST_TOR);
            exit(EXIT_FAILURE);
        }

        // Finally, we're good. Let's put the ring section on the stack.
        o_ringSectionPtrs.push_back((void*)ringSection);

    }

    return rc;
}


/// Function:  dissectRingSectionPrep()
///
/// Brief:  Processes XIP tool input parms and prepares parameters to be passed
///         to dissectRingSection() which does the actual dissection and
///         summarizing of the ring section.
///
/// \param[in] i_image A pointer to a P9-XIP image or TOR ringSection in host memory.
///
/// \param[in] i_argc  Additional number of arguments beyond "dissect" keyword.
///
/// \param[in] i_argv  Additional arguments beyond "dissect" keyword.
///
/// \param[in] i_imageSectionType  Image section type, e.g. IST_XIP or IST_TOR.
///
/// Assumptions:
///
static int
dissectRingSectionPrep(void*                      i_image,
                       int                        i_argc,
                       const char**               i_argv,
                       const image_section_type_t i_imageSectionType)
{
    int             rc = 0;
    const char*     sectionName = NULL;
    const char*     listingModeName = NULL;
    uint8_t         sectionId, listingModeId;
    P9XipHeader     hostHeader;
    P9XipSection    hostSection;
    image_section_type_t l_imageSectionType = IST_UNDEFINED;
    MyBool_t        bEcLvlSupported = UNDEFINED_BOOLEAN;
    void*           ringSection = NULL;
    std::vector<void*>ringSectionPtrs;

    //
    // Treat input image section according to its type.
    //

    switch(i_imageSectionType)
    {

        case IST_XIP:

            if (i_argc == 1)
            {
                sectionName = i_argv[0];
                // ..and default value for listingMode will be used.
            }
            else if (i_argc == 2)
            {
                sectionName = i_argv[0];
                listingModeName = i_argv[1];
            }
            else
            {
                fprintf(stderr,
                        "\nERROR:  The number of sub-arguments (=%d) is too few or too many for the 'dissect' command for an XIP image\n\n",
                        i_argc);
                exit(EXIT_FAILURE);
            }

            p9_xip_translate_header(&hostHeader, (P9XipHeader*)i_image);

            // Determine XIP ring section ID from the section name and XIP magic, e.g.
            //         .rings && MAGIC_SEEPROM   =>  P9_XIP_SECTION_SBE_RINGS
            //
            if (strcmp(sectionName, ".rings") == 0)
            {
                switch (hostHeader.iv_magic)
                {
                    case P9_XIP_MAGIC_SEEPROM:
                        sectionId = P9_XIP_SECTION_SBE_RINGS;
                        break;

                    case P9_XIP_MAGIC_QME:
                        sectionId = P9_XIP_SECTION_QME_RINGS;
                        break;

                    default:
                        fprintf(stderr,
                                "\nERROR: .rings is not a valid section for image w/magic=0x%016lx\n",
                                hostHeader.iv_magic);
                        exit(EXIT_FAILURE);
                }
            }
            else if (strcmp(sectionName, ".overlays") == 0)
            {
                if (hostHeader.iv_magic == P9_XIP_MAGIC_HW)
                {
                    sectionId = P9_XIP_SECTION_HW_OVERLAYS;
                }
                else
                {
                    fprintf(stderr,
                            "\nERROR: .overlays is not a valid section for image w/magic=0x%016lx\n",
                            hostHeader.iv_magic);
                    exit(EXIT_FAILURE);
                }
            }
            else if (strcmp(sectionName, ".dynamic") == 0)
            {
                if (hostHeader.iv_magic == P9_XIP_MAGIC_HW)
                {
                    sectionId = P9_XIP_SECTION_HW_DYNAMIC;
                }
                else
                {
                    fprintf(stderr,
                            "\nERROR: .dynamic is not a valid section for image w/magic=0x%016lx\n",
                            hostHeader.iv_magic);
                    exit(EXIT_FAILURE);
                }
            }
            else if (strcmp(sectionName, ".overrides") == 0)
            {
                if (hostHeader.iv_magic == P9_XIP_MAGIC_SEEPROM)
                {
                    sectionId = P9_XIP_SECTION_SBE_OVERRIDES;
                }
                else
                {
                    fprintf(stderr,
                            "\nERROR: .overrides is not a valid section for image w/magic=0x%016lx\n",
                            hostHeader.iv_magic);
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                fprintf(stderr,
                        "\nERROR : %s is an invalid XIP [ring] section name.\n", sectionName);
                fprintf(stderr, "Valid ring <section> names for the 'dissect' function are:\n");
                fprintf(stderr, "\t.rings\n");
                fprintf(stderr, "\t.overlays\n");
                fprintf(stderr, "\t.dynamic\n");
                fprintf(stderr, "\t.overrides\n");
                fprintf(stderr, "\n");
                exit(EXIT_FAILURE);
            }

            // Determine if XIP section has DD support
            //
            rc = p9_xip_dd_section_support( i_image, sectionId, UNDEFINED_IPL_IMAGE_SID, &bEcLvlSupported );

            if (rc)
            {
                fprintf(stderr, "p9_xip_dd_section_support() failed w/rc=0x%08x (1)\n", (uint32_t)rc );
                exit(EXIT_FAILURE);
            }

            if( bEcLvlSupported )
            {
                rc = p9_xip_get_section( i_image, sectionId, &hostSection);

                if (rc)
                {
                    fprintf(stderr, "ERROR: Failed getting %s [DDCO] section w/rc=%d\n",
                            sectionName, rc);
                    exit(EXIT_FAILURE);
                }

                void*  l_ddcoImage = NULL;
                l_ddcoImage = (void*)((uint8_t*)i_image + hostSection.iv_offset);

                rc = unpack_ddco_image( l_ddcoImage, sectionName, ringSectionPtrs);

                if (rc)
                {
                    fprintf(stderr, "ERROR: Failed unpacking DDCO image w/rc=%d\n",
                            rc);
                    exit(EXIT_FAILURE);
                }

            }
            else
            {
                rc = p9_xip_get_section(i_image, sectionId, &hostSection);

                if (rc)
                {
                    fprintf(stderr, "ERROR: Failed getting %s section w/rc=%d\n",
                            sectionName, rc);
                    exit(EXIT_FAILURE);
                }

                if (hostSection.iv_offset == 0)
                {
                    fprintf(stdout, "ERROR: Ring section (ID=%d) is empty. Nothing to do. Quitting.\n",
                            sectionId);
                    exit(EXIT_FAILURE);
                }

                ringSection = (void*)((uintptr_t)i_image + hostSection.iv_offset);
                ringSectionPtrs.push_back(ringSection);

                resolve_image_section_type( ringSection, l_imageSectionType);

                if (l_imageSectionType != IST_TOR)
                {
                    fprintf( stderr,
                             "\nERROR:  The XIP section's section type (=%d) is not the TOR ring section type (=%d).\n",
                             l_imageSectionType, IST_TOR);
                    exit(EXIT_FAILURE);
                }

            }

            break;

        case IST_TOR:

            if (i_argc == 0)
            {
                // Nothing TBD. It's legal to pass no sub-arguments to 'dissect' for standalone
                // TOR ring section file. Default value for listingMode will be used.
            }
            else if (i_argc == 1)
            {
                listingModeName = i_argv[0];
            }
            else
            {
                fprintf(stderr,
                        "\nERROR:  The number of sub-arguments (=%d) is too few or too many for the 'dissect' command for a standalone TOR section\n\n",
                        i_argc);
                exit(EXIT_FAILURE);
            }

            ringSection = i_image;
            ringSectionPtrs.push_back(ringSection);

            break;

        case IST_DDCO:

            if (i_argc == 0)
            {
                // Nothing TBD. It's legal to pass no sub-arguments to 'dissect' for standalone
                // DDCO ring section file. Default value for listingMode will be used.
            }
            else if (i_argc == 1)
            {
                listingModeName = i_argv[0];
            }
            else
            {
                fprintf(stderr,
                        "\nERROR:  The number of sub-arguments (=%d) is too few or too many for the 'dissect' command for a standalone DDCO section\n\n",
                        i_argc);
                exit(EXIT_FAILURE);
            }

            rc = unpack_ddco_image( i_image, sectionName, ringSectionPtrs);

            if (rc)
            {
                fprintf(stderr, "ERROR: Failed unpacking DDCO image w/rc=%d\n",
                        rc);
                exit(EXIT_FAILURE);
            }

            break;

        default:

            fprintf(stderr,
                    "\nERROR : dissectRingSectionPrep: Invalid image section type (=0x%02x)\n",
                    i_imageSectionType);

            uint64_t l_magic = *((uint64_t*)i_image);
            fprintf(stderr, "Here's the first 8 bytes of the image section: 0x%016lx ",
                    be64toh(l_magic));
            fprintf(stderr, "\"%c%c%c%c%c%c%c%c\"\n",
                    *(((uint8_t*)&l_magic) + 0),
                    *(((uint8_t*)&l_magic) + 1),
                    *(((uint8_t*)&l_magic) + 2),
                    *(((uint8_t*)&l_magic) + 3),
                    *(((uint8_t*)&l_magic) + 4),
                    *(((uint8_t*)&l_magic) + 5),
                    *(((uint8_t*)&l_magic) + 6),
                    *(((uint8_t*)&l_magic) + 7));
            fprintf(stderr,
                    "If you're seeing \"TOR\" or \"DDCO\" in the first 8 bytes, you're probably using the PPE version of ipl_image_tool. Use the EKB version instead!\n");
            exit(EXIT_FAILURE);
    }

    // Determine mode of listing.
    //
    if ( listingModeName == NULL )
    {
        listingModeId = LMID_NORMAL;
    }
    else if (strcmp(listingModeName, "table") == 0)
    {
        listingModeId = LMID_TABLE;
    }
    else if (strcmp(listingModeName, "normal") == 0)
    {
        listingModeId = LMID_NORMAL;
    }
    else if (strcmp(listingModeName, "long") == 0)
    {
        listingModeId = LMID_LONG;
    }
    else if (strcmp(listingModeName, "raw") == 0)
    {
        listingModeId = LMID_RAW;
    }
    else
    {
        fprintf(stderr,
                "\nERROR : %s is an invalid listing mode name.\n", listingModeName);
        fprintf(stderr, "Valid listing mode names the 'dissect' function are:\n");
        fprintf(stderr, "\ttable\n");
        fprintf(stderr, "\tnormal (default if omitted)\n");
        fprintf(stderr, "\tlong\n");
        fprintf(stderr, "\traw\n");
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }

    for (size_t iPos = 0; iPos < ringSectionPtrs.size(); iPos++)
    {
        //
        // Trace out TOR header info
        //
        fprintf(stdout, "\n");
        fprintf(stdout, "----------------------------------------------\n");
        fprintf(stdout, "*             TOR header summary             *\n");
        fprintf(stdout, "----------------------------------------------\n");
        dumpHeader(ringSectionPtrs.at(iPos), IST_TOR);
        fprintf(stdout, "----------------------------------------------\n");
        fprintf(stdout, "\n");

        rc = dissectRingSection(static_cast<uint8_t*>(ringSectionPtrs.at(iPos)), listingModeId);

        if (rc)
        {
            fprintf(stderr, "ERROR: dissectRingSectionPrep: dissectRingSection() failed"
                    " w/rc=0x%08x\n", rc);
            break;
        }

        fprintf(stdout, "\n");
    }

    return rc;
}

#endif



/// Function:  openAndMap()
///
/// Brief:  Opens and mmaps the file.
///
static void
openAndMap(const char* i_fileName, int i_writable, int* o_fd, void** o_image, const uint32_t i_maskIgnores)
{
    int rc, openMode, mmapProt, mmapShared;
    struct stat buf;

    if (i_writable)
    {
        openMode = O_RDWR;
        mmapProt = PROT_READ | PROT_WRITE;
        mmapShared = MAP_SHARED;
    }
    else
    {
        openMode = O_RDONLY;
        mmapProt = PROT_READ;
        mmapShared = MAP_PRIVATE;
    }

    *o_fd = open(i_fileName, openMode);

    if (*o_fd < 0)
    {
        perror("open() of the image failed : ");
        fprintf(stderr, "ERROR: open() failed for filename=%s failed\n", i_fileName);
        exit(EXIT_FAILURE);
    }

    rc = fstat(*o_fd, &buf);

    if (rc)
    {
        perror("fstat() of the image failed : ");
        exit(1);
    }

    g_imageSize = buf.st_size;

    *o_image = mmap(0, g_imageSize, mmapProt, mmapShared, *o_fd, 0);

    if (*o_image == MAP_FAILED)
    {
        perror("mmap() of the image failed : ");
        exit(1);
    }

    if ( !(i_maskIgnores & P9_XIP_IGNORE_ALL) &&
         (be64toh(((P9XipHeader*)o_image)->iv_magic) >> 32 == P9_XIP_MAGIC) )
    {
        rc = p9_xip_validate2(*o_image, g_imageSize, i_maskIgnores);

        if (rc)
        {
            fprintf(stderr, "p9_xip_validate2() failed : %s\n",
                    P9_XIP_ERROR_STRING(g_errorStrings, rc));
            exit(1);
        }
    }

}


static inline void
openAndMapWritable(const char* i_imageFile, int* o_fd, void** o_image, const uint32_t i_maskIgnores)
{
    openAndMap(i_imageFile, 1, o_fd, o_image, i_maskIgnores);
}


static inline void
openAndMapReadOnly(const char* i_imageFile, int* o_fd, void** o_image, const uint32_t i_maskIgnores)
{
    openAndMap(i_imageFile, 0, o_fd, o_image, i_maskIgnores);
}

///
/// @brief Checks .rings section created for SBE image to
///        see if it is less than the passed in size.
///
/// @param[in]  i_hwImage - pointer to an unsigned hw image.
/// @param[in]  i_ddLevel - DD level .rings section to check
/// @param[in]  i_maxSize - Maximum allowable section size
///
/// @return IMGBUILD_SUCCESS if section is less than i_maxSize
///
static
int check_sbe_ring_section_size( void* i_hwImage,
                                 uint32_t i_ddLevel,
                                 uint32_t i_maxSize )
{
    int rc = 0;
#if !defined(__PPE__) && !defined(OPENPOWER_BUILD) // Needed on ppe & OP-Build side to avoid TOR API

    P9XipSection l_ringsSection;

    rc = p9_xip_get_sub_section( i_hwImage,
                                 P9_XIP_SECTION_HW_SBE,
                                 P9_XIP_SECTION_SBE_RINGS,
                                 &l_ringsSection,
                                 i_ddLevel );

    if (rc)
    {
        fprintf(stderr, "ipl_image_tool: p9_xip_get_sub_section failed w/rc=0x%x while getting "
                "DD (=0x%x) ring block withing the SBE .rings section.\n",
                (uint32_t)rc, i_ddLevel);
        exit (EXIT_FAILURE);
    }

    // verify the .rings section is populated
    if (l_ringsSection.iv_size == 0)
    {
        fprintf(stderr, "ipl_image_tool: Ring section size in HW image is zero.\n");
        rc = P9_XIP_DATA_NOT_PRESENT;
        return rc;
    }

    if( l_ringsSection.iv_size == 0 )
    {
        fprintf(stderr, "ipl_image_tool: No rings for dd_level 0x%x found\n", i_ddLevel);
    }

    fprintf(stderr, "ipl_image_tool: SBE .rings section size for DD level 0x%x ", i_ddLevel);

    // return failure if the block size would exceed the maximum allowed size
    if( l_ringsSection.iv_size > i_maxSize )
    {
        fprintf(stderr, "is %d bytes, which exceeds maximum size limit of %i\n",
                l_ringsSection.iv_size, i_maxSize);
        rc = P9_XIP_SBE_DD_SIZE_ERR;
    }
    else
    {
        fprintf(stderr, "is %d bytes - OK\n", l_ringsSection.iv_size);
    }

#endif
    return rc;
}

// Parse and execute a pre-tokenized command

static void
command(const char* i_imageFile, const int i_argc, const char** i_argv, const uint32_t i_maskIgnores)
{
    int rc = INFRASTRUCT_RC_SUCCESS;
    void* image;
    void* attrDump;
    int fd;
    image_section_type_t l_imageSectionType = IST_UNDEFINED;

    // First, determine image section type, IST, of the supplied input image.
    //
    openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
    resolve_image_section_type(image, l_imageSectionType);
    close(fd);

    // Second, determine the command action and act accordingly.
    //
    if ( strcmp(i_argv[0], "normalize") == 0 &&
         l_imageSectionType == IST_XIP )
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = normalize(image, i_argc - 1, &(i_argv[1]), i_maskIgnores);

    }
    else if ( strcmp(i_argv[0], "set") == 0 &&
              l_imageSectionType == IST_XIP )
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = set(image, i_argc - 1, &(i_argv[1]), 0);

    }
    else if ( strcmp(i_argv[0], "setv") == 0 &&
              l_imageSectionType == IST_XIP )
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = set(image, i_argc - 1, &(i_argv[1]), 1);

    }
    else if ( strcmp(i_argv[0], "get") == 0 &&
              l_imageSectionType == IST_XIP )
    {

        openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
        rc = get(image, i_argc - 1, &(i_argv[1]), 0);

    }
    else if ( strcmp(i_argv[0], "getv") == 0 &&
              l_imageSectionType == IST_XIP )
    {

        openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
        rc = get(image, i_argc - 1, &(i_argv[1]), 1);

    }
    else if (strcmp(i_argv[0], "report") == 0)
    {

        openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
        rc = report(image, i_argc - 1, &(i_argv[1]), l_imageSectionType);

    }
    else if ( strcmp(i_argv[0], "attrdump") == 0 &&
              l_imageSectionType == IST_XIP )
    {

        openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
        //capture the g_imageSize size for validation, since the next
        //openAndMapReadOnly will overwrite it
        size_t imageSize = g_imageSize;
        //first argument after command is dump file
        openAndMapReadOnly(i_argv[1], &fd, &attrDump, P9_XIP_IGNORE_ALL);
        rc = reportAttr(image, imageSize, attrDump);

    }
    else if ( strcmp(i_argv[0], "append") == 0 &&
              l_imageSectionType == IST_XIP )
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = append(i_imageFile, fd, image, i_argc - 1, &(i_argv[1]));

    }
    else if ( strcmp(i_argv[0], "extract") == 0)
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = extract(image, i_argc - 1, &(i_argv[1]), l_imageSectionType);

    }
    else if ( strcmp(i_argv[0], "delete") == 0 &&
              l_imageSectionType == IST_XIP )
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = deleteSection(i_imageFile, fd, image, i_argc - 1,
                           &(i_argv[1]));

    }
    else if (strcmp(i_argv[0], "dissect") == 0)
    {

        openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
#if !defined(__PPE__) && !defined(OPENPOWER_BUILD) // Needed on ppe & OP-Build side to avoid TOR API
        rc = dissectRingSectionPrep(image, i_argc - 1, &(i_argv[1]), l_imageSectionType);
#else
        fprintf(stderr, "\n");
        fprintf(stderr, "---------------------------------------------\n");
        fprintf(stderr, "  dissect feature not supported in PPE repo  \n");
        fprintf(stderr, "  => Use EKB version of ipl_image_tool       \n");
        fprintf(stderr, "---------------------------------------------\n\n");
        exit(EXIT_FAILURE);
#endif

        if (rc)
        {
            fprintf(stderr, "ERROR: command: dissectRingSectionPrep() failed w/rc=0x%08x\n", rc);
            exit(EXIT_FAILURE);
        }


    }
    else if ( strcmp(i_argv[0], "check-sbe-ring-section") == 0 &&
              l_imageSectionType == IST_XIP )
    {

        if(i_argc != 3)
        {
            fprintf(stderr, "%s", g_usage);
            exit(EXIT_FAILURE);
        }

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);

        // grab the size and dd level from the cmdline
        const char* i_ddLevel = i_argv[1];

        uint8_t l_ddLevel = strtol(i_ddLevel, 0, 0);

        uint32_t l_maxSize = strtol(i_argv[2], 0, 10);

        // validate that the dd level specific .rings section generated for the
        // sbe image will not exceed the given size.

        rc = check_sbe_ring_section_size(image, l_ddLevel, l_maxSize) ;

    }
    else if ( strcmp(i_argv[0], "TEST") == 0 &&
              l_imageSectionType == IST_XIP )
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = TEST(image, i_argc - 1, &(i_argv[1]));

    }
    else
    {

        fprintf(stderr, "%s", g_usage);
        exit(EXIT_FAILURE);

    }

    if (rc)
    {
        fprintf(stderr,
                "\nERROR: In command(): Command failed : %s\n",
                P9_XIP_ERROR_STRING(g_errorStrings, rc));
        exit(EXIT_FAILURE);
    }
}


// Open, map and validate the image, then parse and execute the command.  The
// image is memory-mapped read/write, i.e, it may be modified in-place.
// Commands that modify the size of the image will close and recreate the
// file.

int
main(int argc, const char** argv)
{
    uint8_t   argcMin, idxArgvFlagsStart;
    uint8_t   numFlags = 0, idxArgv, bMoreFlags;
    uint32_t  maskIgnores = 0;

    argcMin = 3;
    idxArgvFlagsStart = argcMin - 1; // -i flags must start after image file name.

    numFlags = 0;
    bMoreFlags = 1;

    do
    {
        idxArgv = idxArgvFlagsStart + numFlags;

        if (idxArgv <= (argc - 1))
        {
            if (strncmp(argv[idxArgv], "-i", 1) == 0)
            {
                numFlags++;
                bMoreFlags = 1;

                if (strncmp(argv[idxArgv], "-ifs", 4) == 0)
                {
                    maskIgnores = maskIgnores | P9_XIP_IGNORE_FILE_SIZE;
                }
                else if (strncmp(argv[idxArgv], "-iv", 3) == 0)
                {
                    maskIgnores = maskIgnores | P9_XIP_IGNORE_ALL;
                }
                else
                {
                    fprintf(stderr, "%s", g_usage);
                    fprintf(stderr, "\n");
                    fprintf(stderr, "argv[%i]=%s is an unsupported flag.", idxArgv, argv[idxArgv]);
                    fprintf(stderr, "See top of above help menu for supported flags.\n");
                    exit(1);
                }
            }
            else
            {
                bMoreFlags = 0;
            }
        }
        else
        {
            bMoreFlags = 0;
            break;
        }
    }
    while (bMoreFlags);

    if ((argc < (argcMin + numFlags)) ||
        (strncmp(argv[1], "-h", 2) == 0) ||
        (strncmp(argv[1], "--h", 3) == 0) )
    {
        fprintf(stderr, "%s", g_usage);
        exit(1);
    }

    command(argv[1], argc - idxArgv, &(argv[idxArgv]), maskIgnores);

    return 0;
}
