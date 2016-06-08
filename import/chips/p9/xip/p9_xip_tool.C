/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/xip/p9_xip_tool.C $                                  */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */

/// \file p9_xip_tool.c
/// \brief P9-XIP image search/edit tool
///
/// Note: This file was originally stored under .../procedures/ipl/sbe.  It
/// was moved here at version 1.19.

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

#define __PPE__

#include "p9_xip_image.h"
#ifdef XIP_TOOL_ENABLE_DISSECT
    #include "p9_tor.H"
    #include "p9_scan_compression.H"
    using namespace P9_TOR;
#endif

#define LINE_SIZE_MAX  1024     // Max size of a single snprintf dump.
#define RING_BUF_SIZE_MAX  1000000

// Listing mode IDs:
//
enum LISTING_MODE_ID
{
    LMID_SHORT  = 0,
    LMID_NORMAL = 1, // default
    LMID_LONG   = 2
};

// Usage: p9_xip_tool <image> [-<flag> ...] normalize
//        p9_xip_tool <image> [-<flag> ...] get <item>
//        p9_xip_tool <image> [-<flag> ...] getv <item> <index>
//        p9_xip_tool <image> [-<flag> ...] set <item> <value> [ <item1> <value1> ... ]
//        p9_xip_tool <image> [-<flag> ...] setv <item> <index> <value> [ <item1> <index1> <value1> ... ]
//        p9_xip_tool <image> [-<flag> ...] report [<regex>]
//        p9_xip_tool <image> [-<flag> ...] append <section> <file>
//        p9_xip_tool <image> [-<flag> ...] extract <section> <file>
//        p9_xip_tool <image> [-<flag> ...] delete <section> [ <section1> ... <sectionN> ]
//        p9_xip_tool <image> [-<flag> ...] dissect <ring section> [short,normal(default),long]
//        p9_xip_tool <image> [-<flag> ...] disasm <text section>
//
// This simple application uses the P9-XIP image APIs to normalize, search
// update and edit P9-XIP images. This program encapsulates several commands
// in a common command framework which requires an image to operate on, a
// command name, and command arguments that vary by command.  Commands that
// modify the image always rewrite the image in-place in the filesystem;
// however the original image is only modified if the command has completed
// without error.
//
// The program operates on a P9-XIP format binary image, which must be
// normalized - unless the tool is being called to normalize the image in the
// first place using the 'normalize' command. The tool also validates the
// image prior to operating on the image.
//
// The 'get' command retrieves a scalar value from the image and prints its
// representation on stdout (followed by a newline).  Scalar integer values
// and image addresses are printed as hex numbers (0x...).  Strings are printed
// verbatim.
//
// The 'getv' command retrieves a vector element from the image and prints its
// representation on stdout (followed by a newline).  Integer values
// and image addresses are printed as hex numbers (0x...).  Vectors of strings
// are not supported.
//
// The 'set' command allows setting integer and string values in the image.
// New integer values can be specified in decimal or hex (0x...).  Strings are
// taken verbatim from the command line.  Note that new string values will be
// silently truncated to the length of the current string if the new value is
// longer than the current string. Updating address values is currently not
// supported.  Any number of item/value pairs can be specified with a single
// 'set' command.
//
// The 'setv' command is provided to set individual vector elements of
// integral arrays.
//
// The 'report' command prints a report including a dump of the header and
// section table, a listing of the types and values of all items that appear
// in the TOC. The TOC listing includes the
// sequence number of the entry in the TOC, the item name, the item type and
// the item value.
//
// The 'append' command either creates or extends the section named by the
// section argument, by appending the contents of the named file verbatim.
// Currently the section must either be the final (highest address) section of
// the image, or must be empty, in which case the append command creates the
// section as the final section of the image. The 'append' command writes the
// relocatable image address where the input file was loaded to stdout.
//
// The 'extract' command extracts a sections from the binary image.
//
// The 'delete' command deletes 0 or more sections, starting with <section0>.
// Each section to be deleted must either be the final (highest address)
// section of the image at the time it is deleted, or must be empty. The
// 'delete' command writes the size of the final modified image to stdout.
//
// The 'dissect' command dissects the ring section named by the section argument
// and summarizes the content of the ring section.  The second argument to
// 'dissect', i.e. [short,normal(default),long], specifies how much information
// is included in the listing:
//   short:  The bare necessities.
//   normal: Everything but a raw binary dump of the actual ring block.
//   long:   Everything inclusing a raw binary dump of the actual ring block.
// Note that iff the second argument is omitted, a 'normal' listing of the ring
// section will occur.
//
// The 'disasm' command disassembles the section named by the section argument.
//
// The following -i<flag> are supported:
// -ifs
//    causes the validation step to ignore image size check against the file
//    size.
// -iv
//    causes all validation checking to be ignored. (Skips validation step.)

const char* g_usage =
    "Usage: p9_xip_tool <image> [-i<flag> ...] normalize\n"
    "       p9_xip_tool <image> [-i<flag> ...] get <item>\n"
    "       p9_xip_tool <image> [-i<flag> ...] getv <item> <index>\n"
    "       p9_xip_tool <image> [-i<flag> ...] set <item> <value> [ <item1> <value1> ... ]\n"
    "       p9_xip_tool <image> [-i<flag> ...] setv <item> <index> <value> [ <item1> <index1> <value1> ... ]\n"
    "       p9_xip_tool <image> [-i<flag> ...] report [<regex>]\n"
    "       p9_xip_tool <image> [-i<flag> ...] append <section> <file>\n"
    "       p9_xip_tool <image> [-i<flag> ...] extract <section> <file>\n"
    "       p9_xip_tool <image> [-i<flag> ...] delete <section> [ <section1> ... <sectionN> ]\n"
    "       p9_xip_tool <image> [-i<flag> ...] dis <section>\n"
    "       p9_xip_tool <image> [-i<flag> ...] dissect <ring section> [short,normal(default),long]\n"
    "       p9_xip_tool <image> [-i<flag> ...] disasm <text section>\n"
    "\n"
    "This simple application uses the P9-XIP image APIs to normalize, search\n"
    "update and edit P9-XIP images. This program encapsulates several commands\n"
    "in a common command framework which requires an image to operate on, a\n"
    "command name, and command arguments that vary by command.  Commands that\n"
    "modify the image always rewrite the image in-place in the filesystem;\n"
    "however the original image is only modified if the command has completed\n"
    "without error.\n"
    "\n"
    "The program operates on a P9-XIP format binary image, which must be\n"
    "normalized - unless the tool is being called to normalize the image in the\n"
    "first place using the 'normalize' command. The tool also validates the\n"
    "image prior to operating on the image.\n"
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
    "The 'append' command either creates or extends the section named by the\n"
    "section argument, by appending the contents of the named file verbatim.\n"
    "Currently the section must either be the final (highest address) section of\n"
    "the image, or must be empty, in which case the append command creates the\n"
    "section as the final section of the image. The 'append' command writes the\n"
    "relocatable image address where the input file was loaded to stdout.\n"
    "\n"
    "The 'extract' command extracs a sections from a binary image.\n"
    "\n"
    "The 'delete' command deletes 0 or more sections, starting with <section0>.\n"
    "Each section to be deleted must either be the final (highest address)\n"
    "section of the image at the time it is deleted, or must be empty. The\n"
    "'delete' command writes the size of the final modified image to stdout.\n"
    "\n"
    "The 'dissect' command dissects the ring section named by the section argument\n"
    "and summarizes the content of the ring section.  The second argument to\n"
    "'dissect', i.e. [short,normal(default),long], specifies how much information\n"
    "is included in the listing:\n"
    "   short:  The bare necessities.\n"
    "   normal: Everything but a raw binary dump of the actual ring block.\n"
    "   long:   Everything inclusing a raw binary dump of the actual ring block.\n"
    "Note that iff the second argument is omitted, a 'normal' listing of the ring\n"
    "section will occur.\n"
    "\n"
    "The 'disasm' command disassembles the text section named by the section\n"
    "argument.\n"
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
P9_XIP_SECTION_NAMES_SGPE(g_sectionNamesSgpe);
P9_XIP_SECTION_NAMES_RESTORE(g_sectionNamesRestore);
P9_XIP_SECTION_NAMES_CME(g_sectionNamesCme);
P9_XIP_SECTION_NAMES_PGPE(g_sectionNamesPgpe);
P9_XIP_SECTION_NAMES_IOPPE(g_sectionNamesIoppe);
P9_XIP_SECTION_NAMES_FPPE(g_sectionNamesFppe);
P9_XIP_SECTION_NAMES_SBE(g_sectionNamesSbe);

// Disassembler error support.
DIS_ERROR_STRINGS(g_errorStringsDis);

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
//      case P9_XIP_MAGIC_BASE:
//          FIXME
//          break;
//      case P9_XIP_MAGIC_CENTAUR:
//          FIXME
//          break;
        case P9_XIP_MAGIC_SEEPROM:
            return P9_XIP_SECTION_NAME(g_sectionNamesSbe, index);

        case P9_XIP_MAGIC_HW:
            return P9_XIP_SECTION_NAME(g_sectionNamesHw, index);

        case P9_XIP_MAGIC_SGPE:
            return P9_XIP_SECTION_NAME(g_sectionNamesSgpe, index);

        case P9_XIP_MAGIC_RESTORE:
            return P9_XIP_SECTION_NAME(g_sectionNamesRestore, index);

        case P9_XIP_MAGIC_CME:
            return P9_XIP_SECTION_NAME(g_sectionNamesCme, index);

        case P9_XIP_MAGIC_PGPE:
            return P9_XIP_SECTION_NAME(g_sectionNamesPgpe, index);

        case P9_XIP_MAGIC_IOPPE:
            return P9_XIP_SECTION_NAME(g_sectionNamesIoppe, index);

        case P9_XIP_MAGIC_FPPE:
            return P9_XIP_SECTION_NAME(g_sectionNamesFppe, index);
    }

    return "";
}

// Determine index of section given by its name in section table

static inline int get_sectionId(uint64_t i_magic, const char* i_section)
{
    int i;

    for (i = 0; i < P9_XIP_SECTIONS; i++)
        if (strcmp(i_section, get_sectionName(i_magic, i)) == 0)
        {
            return i;
        }

    return -1;
}

// Normalize a P9-XIP image.  We normalize a copy of the image first so that
// the original image will be available for debugging in case the
// normalization fails, then validate and copy the normalized image back to
// the mmap()-ed file.

int
normalize(void* io_image, const int i_argc, const char** i_argv, uint32_t i_maskIgnores)
{
    int rc;
    void* copy;

    do
    {

        // The 'normalize' command takes no arguments

        if (i_argc != 0)
        {
            fprintf(stderr, g_usage);
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


// Print a line of a report, listing the index, symbol, type and current
// value.

int
tocListing(void* io_image,
           const P9XipItem* i_item,
           void* arg)
{
    int rc;
    ReportControl* control;
    uint64_t data;
    char* s;

    control = (ReportControl*)arg;

    do
    {
        rc = 0;

        if (control->regex)
        {
            if (regexec(&(control->preg), i_item->iv_id, 0, 0, 0))
            {
                break;
            }
        }

        printf("0x%04x | %-42s | %s | ",
               control->index, i_item->iv_id,
               P9_XIP_TYPE_STRING(g_typeAbbrevs, i_item->iv_type));

        switch (i_item->iv_type)
        {
            case P9_XIP_UINT8:
                rc = p9_xip_get_scalar(io_image, i_item->iv_id, &data);

                if (rc)
                {
                    break;
                }

                printf("0x%02x", (uint8_t)data);
                break;

            case P9_XIP_UINT16:
                rc = p9_xip_get_scalar(io_image, i_item->iv_id, &data);

                if (rc)
                {
                    break;
                }

                printf("0x%04x", (uint16_t)data);
                break;

            case P9_XIP_UINT32:
                rc = p9_xip_get_scalar(io_image, i_item->iv_id, &data);

                if (rc)
                {
                    break;
                }

                printf("0x%08x", (uint32_t)data);
                break;

            case P9_XIP_UINT64:
                rc = p9_xip_get_scalar(io_image, i_item->iv_id, &data);

                if (rc)
                {
                    break;
                }

                printf("0x%016lx", data);
                break;

            case P9_XIP_INT8:
                rc = p9_xip_get_scalar(io_image, i_item->iv_id, &data);

                if (rc)
                {
                    break;
                }

                printf("0x%02x", (uint8_t)data);
                break;

            case P9_XIP_INT16:
                rc = p9_xip_get_scalar(io_image, i_item->iv_id, &data);

                if (rc)
                {
                    break;
                }

                printf("0x%04x", (uint16_t)data);
                break;

            case P9_XIP_INT32:
                rc = p9_xip_get_scalar(io_image, i_item->iv_id, &data);

                if (rc)
                {
                    break;
                }

                printf("0x%08x", (uint32_t)data);
                break;

            case P9_XIP_INT64:
                rc = p9_xip_get_scalar(io_image, i_item->iv_id, &data);

                if (rc)
                {
                    break;
                }

                printf("0x%016lx", data);
                break;

            case P9_XIP_STRING:
                rc = p9_xip_get_string(io_image, i_item->iv_id, &s);

                if (rc)
                {
                    break;
                }

                printf("%s", s);
                break;

            case P9_XIP_ADDRESS:
                rc = p9_xip_get_scalar(io_image, i_item->iv_id, &data);

                if (rc)
                {
                    break;
                }

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
    while (0);

    control->index += 1;
    return rc;
}


// Dump the image header, including the section table

int
dumpHeader(void* i_image)
{
    int i;
    P9XipHeader header;
    P9XipSection* section;
    char magicString[9];

    // Dump header information. Since the TOC may not exist we need to get
    // the information from the header explicitly.

    p9_xip_translate_header(&header, (P9XipHeader*)i_image);

    memcpy(magicString, (char*)(&(((P9XipHeader*)i_image)->iv_magic)), 8);
    magicString[8] = 0;

    printf("Magic Number   : 0x%016lx \"%s\"\n",
           header.iv_magic, magicString);
    printf("Header Version : 0x%02x\n", header.iv_headerVersion);
    printf("Link Address   : 0x%016lx\n", header.iv_linkAddress);
    printf("L1 Loader Address   : 0x%08x\n", (uint32_t)header.iv_L1LoaderAddr);
    printf("L2 Loader Address   : 0x%08x\n", (uint32_t)header.iv_L2LoaderAddr);
    printf("Kernel Address   : 0x%08x\n", (uint32_t)header.iv_kernelAddr);
    printf("Image Size     : 0x%08x (%d)\n",
           header.iv_imageSize, header.iv_imageSize);
    printf("Normalized     : %s\n", header.iv_normalized ? "Yes" : "No");
    printf("TOC Sorted     : %s\n", header.iv_tocSorted ? "Yes" : "No");
    printf("Build Date     : %02d/%02d/%04d\n",
           (header.iv_buildDate / 100) % 100,
           header.iv_buildDate % 100,
           header.iv_buildDate / 10000);
    printf("Build Time     : %02d:%02d\n",
           header.iv_buildTime / 100,
           header.iv_buildTime % 100);
    printf("Build User     : %s\n", header.iv_buildUser);
    printf("Build Host     : %s\n", header.iv_buildHost);
    printf("\n");

    printf("Section Table  :   Offset      Size\n");
    printf("\n");

    for (i = 0; i < P9_XIP_SECTIONS; i++)
    {
        section = &(header.iv_section[i]);
        printf("%-16s 0x%08x 0x%08x (%d)\n",
               get_sectionName(header.iv_magic, i),
               section->iv_offset, section->iv_size, section->iv_size);
    }

    printf("\n");

    return 0;
}


// Print a report

int
report(void* io_image, const int i_argc, const char** i_argv)
{
    int rc;
    ReportControl control;
    char errbuf[ERRBUF_SIZE];

    do
    {

        // Basic syntax check : [<regexp>]

        if (i_argc > 1)
        {
            fprintf(stderr, g_usage);
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

            dumpHeader(io_image);
            printf("TOC Report\n\n");
        }

        // Map the TOC with the mapReport() function

        control.index = 0;
        rc = p9_xip_map_toc(io_image, tocListing, (void*)(&control));

        if (rc)
        {
            break;
        }

    }
    while (0);

    return rc;
}


// Set a scalar or vector element values in the image.  The 'i_setv' argument
// indicates set/setv (0/1).

int
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
            fprintf(stderr, g_usage);
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
                    // e.g. in time strings generated by `date +%H%M`, and try to
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

int
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
            fprintf(stderr, g_usage);
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


// strtoul() with application-specific error handling

unsigned long
localStrtoul(const char* s)
{
    unsigned long v;
    char* endptr;

    errno = 0;
    v = strtoul(s, &endptr, 0);

    if ((errno != 0) || (endptr != (s + strlen(s))))
    {
        fprintf(stderr,
                "Error parsing putative integer value : %s\n",
                s);
        exit(1);
    }

    return v;
}


// Append a file to section
int
append(const char* i_imageFile, const int i_imageFd, void* io_image,
       int i_argc, const char** i_argv)
{
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

        if (i_argc != 2)
        {
            fprintf(stderr, g_usage);
            exit(1);
        }

        section = i_argv[0];
        file = i_argv[1];

        p9_xip_translate_header(&header, (P9XipHeader*)io_image);

        // Translate the section name to a section Id
        sectionId = get_sectionId(header.iv_magic, section);

        if (sectionId < 0)
        {
            fprintf(stderr, "Unrecognized section name : '%s;\n", section);
            exit(1);
        }

        // Open and mmap the file to be appended

        fileFd = open(file, O_RDONLY);

        if (fileFd < 0)
        {
            perror("open() of the file to be appended failed : ");
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

        rc = p9_xip_append(newImage, sectionId,
                           appendImage, buf.st_size,
                           newSize, &sectionOffset);

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

// Extract section from a file
int
extract(const char* i_imageFile, const int i_imageFd, void* io_image,
        int i_argc, const char** i_argv)
{
    int fileFd, sectionId, rc;
    void* newImage;
    const char* section;
    const char* file;
    P9XipHeader header;
    P9XipSection* xSection;
    uint32_t size;
    uint32_t offset;

    do
    {

        if (i_argc != 2)
        {
            fprintf(stderr, g_usage);
            exit(1);
        }

        section = i_argv[0];
        file = i_argv[1];

        printf("%s %s\n", section , file);

        p9_xip_translate_header(&header, (P9XipHeader*)io_image);

        sectionId = get_sectionId(header.iv_magic, section);

        if (sectionId < 0)
        {
            fprintf(stderr, "Unrecognized section name : '%s;\n", section);
            exit(1);
        }

        xSection = &(header.iv_section[sectionId]);

        size = xSection->iv_size;
        offset = xSection->iv_offset;

        printf("%-16s 0x%08x 0x%08x (%d)\n",
               section, offset, size, size);

        newImage = malloc(size);

        if (newImage == 0)
        {
            fprintf(stderr, "Can't malloc() a buffer for the new image\n");
            exit(1);
        }

        memcpy(newImage, (void*)((uint64_t)io_image + offset), size);

        fileFd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0755);

        if (fileFd < 0)
        {
            perror("open() of the fixed section : ");
            exit(1);
        }

        rc = write(fileFd, newImage, size);

        if ((rc < 0) || ((uint32_t)rc != size))
        {
            perror("write() of fixed section : ");
            exit(1);
        }

        rc = close(fileFd);

        if (rc)
        {
            perror("close() of fixed section : ");
            exit(1);
        }

    }
    while (0);

    return rc;

}


// Delete 0 or more sections in order.

int
deleteSection(const char* i_imageFile, const int i_imageFd, void* io_image,
              int i_argc, const char** i_argv)
{
    int newImageFd, sectionId, rc, argc;
    const char* section;
    const char** argv;
    void* newImage;
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

            rc = p9_xip_delete_section(newImage, sectionId);

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


int
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
            case P9_XIP_MAGIC_BASE:
                key = (char*)"proc_p9_fabricinit_revision";
                rc = p9_xip_get_string(io_image, key, &revision);
                BOMB_IF_RC;
                BOMB_IF(strncmp(revision, "1.", 2) != 0);
                break;

            case P9_XIP_MAGIC_SEEPROM:
                key = (char*)"";
                // Can't do this test here as the TOC has been stripped
                break;

            case P9_XIP_MAGIC_CENTAUR:
                key = (char*)"cen_p9_initf_revision";
                rc = p9_xip_get_string(io_image, key, &revision);
                BOMB_IF_RC;
                BOMB_IF(strncmp(revision, "1.", 2) != 0);
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
        rc = p9_xip_get_section(io_image, P9_XIP_SECTION_TOC, &section);
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


#ifdef DEBUG_P9_XIP_IMAGE
        printf("\nYou will see an expected warning below "
               "about P9_XIP_WOULD_OVERFLOW\n"
               "It means the TEST is working (not failing)\n\n");
#endif

        // Finally compare against the original

        BOMB_IF(memcmp(io_image, originalImage, imageSize));

    }
    while (0);

    return rc;
}



#ifdef XIP_TOOL_ENABLE_DISSECT

/// Function:  dissectRingSectionTor()
///
/// Brief:  Dissects and summarizes content of a ring section.
///
/// \param[in] i_ringSection    A pointer to a TOR compliant ring section.
///
/// \param[in] i_imageMagicNo   The image's MAGIC number.
///
/// \param[in] i_listingModeId  The listing mode: {short, normal(default), long}.
///
/// Assumptions:
///
int dissectRingSectionTor( void*       i_ringSection,
                           uint64_t    i_imageMagicNo,
                           uint8_t     i_listingModeId )
{
    int         rc = 0;
    uint32_t    i;
    char*       disList = NULL;
    uint32_t    sizeDisLine = 0, sizeList = 0, sizeListMax = 0, sizeListIncr;
    char        lineDis[LINE_SIZE_MAX];
    uint32_t    numDdLevels;
    uint8_t     iDdLevel, ddLevel;
    uint8_t     ppeType;
    uint8_t     ringId;
    RingType_t  ringType;
    uint8_t     ringVariant;
    uint8_t     instanceId;
    void*       ringBlockPtr;
    uint32_t    ringBlockSize;
    char        ringName[32];
    void*       hostRs4Container;
    uint32_t    compressedBits = 0, ringLength = 0;
    double      compressionPct = 0;
    uint32_t    ringSeqNo  = 0; // Ring sequence number

    //
    // Allocate buffer to hold dissected ring info. (Start out with min 10kB buffer size.)
    //
    sizeListIncr = 10 * LINE_SIZE_MAX;
    sizeListMax = sizeListIncr;
    disList = (char*)malloc(sizeListMax);

    if (disList == NULL)
    {
        fprintf( stderr, "ERROR : malloc() failed.\n");
        fprintf( stderr, "\tMore info: %s\n", DIS_ERROR_STRING(g_errorStringsDis, DIS_MEMORY_ERROR));
        return P9_XIP_DISASSEMBLER_ERROR;
    }

    *disList = '\0'; // Make sure the buffer is NULL terminated (though probably not needed.)
    sizeList = 0;

    sizeDisLine = snprintf( lineDis, LINE_SIZE_MAX,
                            "-----------------------------\n"
                            "*       Ring summary        *\n");

    disList = strcat(disList, lineDis);
    sizeList = sizeList + sizeDisLine;

    //
    // Allocate large buffer to hold max length ring block.
    //
    ringBlockSize = RING_BUF_SIZE_MAX;
    ringBlockPtr = malloc(ringBlockSize);

    //
    // Get number of DD levels from TOR structure in ring section
    //
    if (i_imageMagicNo == P9_XIP_MAGIC_HW)
    {
        numDdLevels = htobe32( ( (TorNumDdLevels_t*)i_ringSection )->TorNumDdLevels );
        fprintf( stdout, "numDdLevels (=%d) read from top of ring section.\n", numDdLevels);
    }
    else
    {
        numDdLevels = 1;
        ddLevel = 0xff; // This means it's unknown.
        fprintf( stdout, "Image contains only one DD level set of rings.\n");
    }

    //----------------
    // DD level loop.
    for (iDdLevel = 0; iDdLevel < numDdLevels; iDdLevel++)
    {

        if (i_imageMagicNo == P9_XIP_MAGIC_HW)
        {
            ddLevel = ( ( htobe32( ( ( (TorDdLevelBlock_t*)((uintptr_t)i_ringSection + sizeof(TorNumDdLevels_t)) ) +
                                     iDdLevel )->TorDdLevelAndOffset ) & 0xff000000 ) >> 24 );
        }

        //----------------
        // PPE type loop.
        // - SBE, CME, SGPE
        for (ppeType = 0; ppeType < NUM_PPE_TYPES; ppeType++)
        {

            //--------------------
            // Ring variant loop.
            // - Base, cache, risk, override, overlay
            for (ringVariant = 0; ringVariant < NUM_RING_VARIANTS; ringVariant++)
            {

                //----------------------
                // Unique ring ID loop.
                for (ringId = 0; ringId < NUM_RING_IDS; ringId++)
                {

                    ringType = (RingType_t)(-1);

                    //---------------------------
                    // Chiplet instance ID loop.
                    // - Only loop once if ringId is a common ring.
                    for (instanceId = 0; instanceId <= CHIPLET_ID_MAX && ringType != COMMON; instanceId++)
                    {

                        fprintf( stdout, "Processing:  "
                                 "DD=0x%02x  "
                                 "PPE=%s "
                                 "Variant=%s "
                                 "RingID=%d  "
                                 "InstanceID=0x%02x\n",
                                 ddLevel, ppeTypeName[ppeType], ringVariantName[ringVariant], ringId, instanceId);

                        ringBlockSize = RING_BUF_SIZE_MAX;
                        rc = tor_get_ring( i_ringSection,
                                           i_imageMagicNo,
                                           (RingID)ringId,
                                           ddLevel,
                                           (PpeType_t)ppeType,
                                           ringType,            // IO parm
                                           (RingVariant_t)ringVariant,
                                           instanceId,          // IO parm
                                           SINGLE_RING,
                                           &ringBlockPtr,       // IO parm
                                           ringBlockSize,       // IO parm
                                           ringName );

                        // Gather ring details and print it.
                        //
                        if (rc == IMGBUILD_TGR_RING_FOUND)
                        {

                            // Check ring block size.
                            if ( htobe32(((RingLayout_t*)ringBlockPtr)->sizeOfThis) != ringBlockSize )
                            {
                                fprintf(stderr, "tor_get_ring() was successful and found a ring but "
                                        "sizeOfThis(=0x%08x) != ringBlockSize(=0x%08x) is a bug.\n",
                                        htobe32(((RingLayout_t*)ringBlockPtr)->sizeOfThis), ringBlockSize);
                                exit(1);
                            }

                            ringSeqNo++;

                            // Summarize a few key characteristics of the ring block if "short".
                            if (i_listingModeId == LMID_SHORT)
                            {
                                sizeDisLine = snprintf( lineDis, LINE_SIZE_MAX,
                                                        "-----------------------------\n"
                                                        "%i.\n"
                                                        "ddLevel = 0x%02x\n"
                                                        "ppeType = %s\n"
                                                        "ringName = %s\n"
                                                        "ringVariant = %s\n"
                                                        "instanceId = 0x%02x\n",
                                                        ringSeqNo, ddLevel, ppeTypeName[ppeType], ringName,
                                                        ringVariantName[ringVariant], instanceId );

                                if (sizeDisLine >= LINE_SIZE_MAX)
                                {
                                    fprintf(stderr, "The max print line size, LINE_SIZE_MAX=%d, has been reached.(1)",
                                            LINE_SIZE_MAX);
                                    fprintf(stderr, "You should investigate why this happened before increasing "
                                            "the value of LINE_SIZE_MAX since something might be wrong "
                                            "with the RS4 ring content.");
                                    exit(1);
                                }

                                // Update list buffer and readjust list buffer size, if needed.
                                disList = strcat(disList, lineDis);
                                sizeList = sizeList + sizeDisLine;

                            }

                            // Summarize all characteristics of the ring block if "normal" or "long" (default).
                            if ( i_listingModeId == LMID_NORMAL || i_listingModeId == LMID_LONG )
                            {
                                // Calculate RS4 compression efficiency.
                                hostRs4Container = (void*)( (uintptr_t)ringBlockPtr + sizeof(RingLayout_t) );
                                compressedBits = htobe32(((CompressedScanData*)hostRs4Container)->iv_algorithmReserved) * 4;
                                ringLength = htobe32(((CompressedScanData*)hostRs4Container)->iv_length);
                                compressionPct = (double)compressedBits / (double)ringLength * 100.0;

                                sizeDisLine = snprintf( lineDis, LINE_SIZE_MAX,
                                                        "-----------------------------\n"
                                                        "%i.\n"
                                                        "ddLevel = 0x%02x\n"
                                                        "ppeType = %s\n"
                                                        "ringId = %u\n"
                                                        "ringName = %s\n"
                                                        "ringVariant = %s\n"
                                                        "instanceId = 0x%02x\n"
                                                        "ringBlockSize = 0x%08x\n"
                                                        "RS4 ring size [bits] = %u\n"
                                                        "Raw ring size [bits] = %u\n"
                                                        "Compression [%%]      = %0.2f\n",
                                                        ringSeqNo, ddLevel, ppeTypeName[ppeType], ringId, ringName,
                                                        ringVariantName[ringVariant], instanceId,
                                                        ringBlockSize, compressedBits, ringLength, compressionPct );

                                if (sizeDisLine >= LINE_SIZE_MAX)
                                {
                                    fprintf(stderr, "The max print line size, LINE_SIZE_MAX=%d, has been reached.(2)",
                                            LINE_SIZE_MAX);
                                    fprintf(stderr, "You should investigate why this happened before increasing "
                                            "the value of LINE_SIZE_MAX since something might be wrong "
                                            "with the RS4 ring content.");
                                    exit(1);
                                }

                                // Update list buffer and readjust list buffer size, if needed.
                                disList = strcat(disList, lineDis);
                                sizeList = sizeList + sizeDisLine;

                            }

                            // Dump ring block if "long".
                            if (i_listingModeId == LMID_LONG)
                            {
                                sizeDisLine = snprintf( lineDis, LINE_SIZE_MAX,
                                                        "Binary ring block dump (LE format):\n");
                                disList = strcat(disList, lineDis);
                                sizeList = sizeList + sizeDisLine;

                                if (sizeDisLine >= LINE_SIZE_MAX)
                                {
                                    fprintf(stderr, "The max print line size, LINE_SIZE_MAX=%d, has been reached.(3)",
                                            LINE_SIZE_MAX);
                                    fprintf(stderr, "You should investigate why this happened before increasing "
                                            "the value of LINE_SIZE_MAX since something might be wrong "
                                            "with the RS4 ring content.");
                                    exit(1);
                                }

                                for (i = 0; i < ringBlockSize / 8; i++)
                                {
                                    sizeDisLine = snprintf( lineDis, LINE_SIZE_MAX,
                                                            "%04x: %04x %04x %04x %04x\n",
                                                            i * 8,
                                                            (uint16_t)( htobe64(*((uint64_t*)ringBlockPtr + i)) >> 48),
                                                            (uint16_t)( htobe64(*((uint64_t*)ringBlockPtr + i)) >> 32),
                                                            (uint16_t)( htobe64(*((uint64_t*)ringBlockPtr + i)) >> 16),
                                                            (uint16_t)( htobe64(*((uint64_t*)ringBlockPtr + i))) );

                                    disList = strcat(disList, lineDis);
                                    sizeList = sizeList + sizeDisLine;

                                }
                            }

                            if (sizeList > (sizeListMax - LINE_SIZE_MAX))
                            {
                                sizeListMax = sizeListMax + sizeListIncr;
                                disList = (char*)realloc( (void*)(disList), sizeListMax);
                            }

                            fprintf(stdout, "%s\n", disList);

                        }
                        else if (rc == IMGBUILD_TGR_RING_NOT_FOUND)
                        {
                            fprintf(stdout, "tor_get_ring() returned rc=%d=IMGBUILD_TGR_RING_NOT_FOUND\n", rc);
                        }
                        else if (rc == IMGBUILD_INVALID_INSTANCEID)
                        {
                            fprintf(stdout, "tor_get_ring() returned rc=%d=IMGBUILD_INVALID_INSTANCEID\n", rc);
                        }
                        else if (rc == IMGBUILD_TGR_AMBIGUOUS_API_PARMS)
                        {
                            fprintf(stdout, "tor_get_ring() returned rc=%d=IMGBUILD_TGR_AMBIGUOUS_API_PARMS\n", rc);
                        }
                        else
                        {
                            fprintf(stderr, "tor_get_ring() returned error code rc=%d\n", rc);
                            exit(1);
                        }

                    }  // End of for(instanceId)

                }  // End of for(ringVariant)

            }  // End of for(iRingId)

        }  // End of for(ppeType)

    }  // End of for(iDdLevel)


    sizeDisLine = snprintf( lineDis, LINE_SIZE_MAX,
                            "-----------------------------\n");

    disList = strcat(disList, lineDis);
    sizeList = sizeList + sizeDisLine;

    // Adjust final buffer size, add 1 for NULL char and print it.
    if (disList)
    {
        disList = (char*)realloc( (void*)(disList), sizeList + 1);
        fprintf(stdout, "%s\n", disList);
        free(disList);
    }

    return 0;

}



/// Function:  dissectRingSection()
///
/// Brief:  Processes XIP tool input parms and prepares parameters to be passed
///         to dissectRingSectionTor which does the actual dissection and
///         summarizing of the ring section.
///
/// \param[in] i_image  A pointer to a P9-XIP image in host memory.
///
/// \param[in] i_argc  Additional number of arguments beyond "dissect" keyword.
///
/// \param[in] i_argv  Additional arguments beyond "dissect" keyword.
///
/// Assumptions:
///
int dissectRingSection(void*          i_image,
                       int            i_argc,
                       const char**   i_argv)
{
    int             rc = 0;
    const char*     sectionName;
    const char*     listingModeName = NULL;
    uint8_t         sectionId, listingModeId;
    P9XipHeader     hostHeader;
    P9XipSection    hostSection;
    void*           ringSectionPtr;


    if (i_argc != 1 && i_argc != 2)
    {
        fprintf(stderr, g_usage);
        exit(1);
    }

    if (i_argc == 1)
    {
        sectionName = i_argv[0];
    }
    else
    {
        sectionName = i_argv[0];
        listingModeName = i_argv[1];
    }

    p9_xip_translate_header(&hostHeader, (P9XipHeader*)i_image);

    // Determine P9-XIP ring section ID from the section name, e.g.
    //         .rings    =>  P9_XIP_SECTION_HW_RINGS
    if (strcmp(sectionName, ".rings") == 0)
    {
        if (hostHeader.iv_magic == P9_XIP_MAGIC_SEEPROM)
        {
            sectionId = P9_XIP_SECTION_SBE_RINGS;
        }
        else if (hostHeader.iv_magic == P9_XIP_MAGIC_HW)
        {
            sectionId = P9_XIP_SECTION_HW_RINGS;
        }
        else
        {
            fprintf(stderr, "ERROR: .rings is not a valid section for image w/magic=0x%016lx\n",
                    hostHeader.iv_magic);
            exit(1);
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
            fprintf(stderr, "ERROR: .overrides is not a valid section for image w/magic=0x%016lx\n",
                    hostHeader.iv_magic);
            exit(1);
        }
    }
    else if (strcmp(sectionName, ".overlays") == 0)
    {
        if (hostHeader.iv_magic == P9_XIP_MAGIC_SEEPROM)
        {
            sectionId = P9_XIP_SECTION_SBE_OVERLAYS;
        }
        else
        {
            fprintf(stderr, "ERROR: .overlays is not a valid section for image w/magic=0x%016lx\n",
                    hostHeader.iv_magic);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "ERROR : %s is an invalid ring section name.\n", sectionName);
        fprintf(stderr, "Valid ring <section> names for the 'dissect' function are:\n");
        fprintf(stderr, "\t.rings\n");
        fprintf(stderr, "\t.overrides\n");
        fprintf(stderr, "\t.overlays\n");
        exit(1);
    }

    // Determine mode of listing.
    //
    if ( listingModeName == NULL )
    {
        listingModeId = LMID_NORMAL;
    }
    else if (strcmp(listingModeName, "short") == 0)
    {
        listingModeId = LMID_SHORT;
    }
    else if (strcmp(listingModeName, "normal") == 0)
    {
        listingModeId = LMID_NORMAL;
    }
    else if (strcmp(listingModeName, "long") == 0)
    {
        listingModeId = LMID_LONG;
    }
    else
    {
        fprintf(stderr, "ERROR : %s is an invalid listing mode name.\n", listingModeName);
        fprintf(stderr, "Valid listing mode names the 'dissect' function are:\n");
        fprintf(stderr, "\tshort\n");
        fprintf(stderr, "\tnormal (default if omitted)\n");
        fprintf(stderr, "\tlong\n");
        exit(1);
    }

    // Get ring section.
    //
    rc = p9_xip_get_section( i_image, sectionId, &hostSection);

    if (rc)
    {
        fprintf( stderr, "p9_xip_get_section() failed : %s\n", P9_XIP_ERROR_STRING(g_errorStrings, rc));
        return P9_XIP_DISASSEMBLER_ERROR;
    }

    if (hostSection.iv_offset == 0)
    {
        fprintf( stdout, "Ring section (w/ID=%d) is empty. Nothing to do. Quitting.\n", sectionId);
        exit(1);
    }

    ringSectionPtr = (void*)(hostSection.iv_offset + (uintptr_t)i_image);

    rc = dissectRingSectionTor(ringSectionPtr, hostHeader.iv_magic, listingModeId);

    return rc;

}

#endif



/// Function:  openAndMap()
///
/// Brief:  Opens and mmaps the file.
///
void
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
        exit(1);
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

    if ( !(i_maskIgnores & P9_XIP_IGNORE_ALL) )
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


// Parse and execute a pre-tokenized command

void
command(const char* i_imageFile, const int i_argc, const char** i_argv, const uint32_t i_maskIgnores)
{
    void* image;
    int fd, rc = 0;

    if (strcmp(i_argv[0], "normalize") == 0)
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = normalize(image, i_argc - 1, &(i_argv[1]), i_maskIgnores);

    }
    else if (strcmp(i_argv[0], "set") == 0)
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = set(image, i_argc - 1, &(i_argv[1]), 0);

    }
    else if (strcmp(i_argv[0], "setv") == 0)
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = set(image, i_argc - 1, &(i_argv[1]), 1);

    }
    else if (strcmp(i_argv[0], "get") == 0)
    {

        openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
        rc = get(image, i_argc - 1, &(i_argv[1]), 0);

    }
    else if (strcmp(i_argv[0], "getv") == 0)
    {

        openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
        rc = get(image, i_argc - 1, &(i_argv[1]), 1);

    }
    else if (strcmp(i_argv[0], "report") == 0)
    {

        openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
        rc = report(image, i_argc - 1, &(i_argv[1]));

    }
    else if (strcmp(i_argv[0], "append") == 0)
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = append(i_imageFile, fd, image, i_argc - 1, &(i_argv[1]));

    }
    else if (strcmp(i_argv[0], "extract") == 0)
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = extract(i_imageFile, fd, image, i_argc - 1, &(i_argv[1]));

    }
    else if (strcmp(i_argv[0], "delete") == 0)
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = deleteSection(i_imageFile, fd, image, i_argc - 1,
                           &(i_argv[1]));

    }
    else if (strcmp(i_argv[0], "dissect") == 0)
    {

        openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
#ifdef XIP_TOOL_ENABLE_DISSECT
        rc = dissectRingSection(image, i_argc - 1, &(i_argv[1]));
#else
        fprintf(stderr, "\n");
        fprintf(stderr, "-------------------------------\n");
        fprintf(stderr, " dissect feature not supported \n");
        fprintf(stderr, "-------------------------------\n\n");
        exit(1);
#endif

    }
    else if (strcmp(i_argv[0], "disasm") == 0)
    {

        //openAndMapReadOnly(i_imageFile, &fd, &image, i_maskIgnores);
        //rc = disassembleSection(image, i_argc - 1, &(i_argv[1]));
        fprintf(stderr, "not supported\n");
        exit(1);

    }
    else if (strcmp(i_argv[0], "TEST") == 0)
    {

        openAndMapWritable(i_imageFile, &fd, &image, i_maskIgnores);
        rc = TEST(image, i_argc - 1, &(i_argv[1]));

    }
    else
    {
        fprintf(stderr, g_usage);
        exit(1);
    }

    if (rc)
    {
        fprintf(stderr, "Command failed : %s\n",
                P9_XIP_ERROR_STRING(g_errorStrings, rc));
        exit(1);
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
                    fprintf(stderr, g_usage);
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
        fprintf(stderr, g_usage);
        exit(1);
    }

    command(argv[1], argc - idxArgv, &(argv[idxArgv]), maskIgnores);

    return 0;
}
