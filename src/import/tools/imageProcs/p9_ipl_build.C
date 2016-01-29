/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/tools/imageProcs/p9_ipl_build.C $                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
/*------------------------------------------------------------------------------*/
/* *! TITLE : p9_ipl_build.C                                                    */
/* *! DESCRIPTION : Copies RS4 delta ring states from unsigned HW image to DD-  */
//                  specific PNOR SBE image.
/* *! OWNER NAME : Michael Olsen                  cmolsen@us.ibm.com            */
//
/* *! EXTENDED DESCRIPTION :                                                    */
//
/* *! USAGE : To build -                                                        */
//              buildecmdprcd -C "p9_image_help_base.C" -c "p9_xip_image.c" p9_ipl_build.C
//
/* *! ASSUMPTIONS :                                                             */
//    - sysPhase=0 is assumed which puts the SBE image together for IPL.
//
/* *! COMMENTS :                                                                */
//    - .strings and .toc are removed as "a unit" before validating.
//
/*------------------------------------------------------------------------------*/

#include <p9_image_help.H>

#define THIS_HELP  ("\nUSAGE:\n\tp9_ipl_build  -help [anything]\n\t  or\n" \
                    "\tp9_ipl_build \n" \
                    "\t\t<Input/output SBE image file>\n" \
                    "\t\t<Input HW reference image file\n" \
                    "\t\t<DD level [hex value]>\n" )

//  main() input parms:
//  arg1:   Input/output SBE image file
//  arg2:   Input HW image file
//  arg3:   DD level [hex value]
int main( int argc, char* argv[])
{
    int         rc = 0;
    char*        fnImageSbe, *fnImageHw;
    uint32_t    ddLevel = 0;
    uint32_t    fdImageHw = 0;
    struct stat stbuf;
    uint32_t    sizeImageHw;
    void*        imageHw;

    P9_XIP_ERROR_STRINGS(g_errorStrings);

    // ==========================================================================
    // Convert input parms from char to <type>
    // ==========================================================================
    MY_DBG("\n--> Processing Input Parameters...\n");

    if (argc < 4 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--h") == 0)
    {
        MY_INF("%s\n", THIS_HELP);
        return 1;
    }

    // Convert input parms from char to <type>
    //
    fnImageSbe = argv[1];
    fnImageHw  = argv[2];
    ddLevel    = strtol(argv[3], NULL, 16);

    MY_INF("  Input/output SBE image fn = %s\n", fnImageSbe);
    MY_INF("  Input HW image fn         = %s\n", fnImageHw);
    MY_INF("  DD level                  = 0x%02x\n", ddLevel);

    // ==========================================================================
    // Memory map HW reference image.
    // ==========================================================================
    MY_DBG("Memory map HW ref image.\n");
    fdImageHw = open(fnImageHw, O_RDONLY);

    if (fstat(fdImageHw, &stbuf) != 0)
    {
        MY_ERR("Could not fstat the HW ref image file.\n");
        return 1;
    }

    sizeImageHw = stbuf.st_size;
    imageHw = mmap(0, sizeImageHw, PROT_READ, MAP_SHARED, fdImageHw, 0);

    if (imageHw == MAP_FAILED)
    {
        MY_ERR("mmap() of HW ref image failed.\n");
        return 1;
    }

    // ...validate image.
    rc = p9_xip_validate(imageHw, sizeImageHw);

    if (rc)
    {
        MY_ERR("p9_xip_validate() of HW ref image failed: %s\n", P9_XIP_ERROR_STRING(g_errorStrings, rc));
        return 1;
    }

    // Update the SBE image.
    //
    MY_INF("Updating the SBE image... \n");

    rc = ipl_build( fnImageSbe,
                    imageHw,
                    ddLevel );

    if (rc == IMGBUILD_SUCCESS)
    {
        MY_INF("SBE image build was SUCCESSFUL.\n");
    }
    else if (rc == IMGBUILD_RING_SEARCH_EOS_NO_MATCH)
    {
        MY_INF("SBE image build was SUCCESSFUL but no RS4 rings appended (rc=%i).\n", rc);
    }
    else
    {
        MY_ERR("SBE image build was UNSUCCESSFUL (rc=%i).\n", rc);
        return 1;
    }

    close(fdImageHw);

    return rc;

}



//  Parameter list:
//  char      *io_fnImageSbe:   Filename of SBE I/O image
//  void      *i_imageHw:       Pointer to memory mapped HW Reference image
//  uint32_t  i_ddLevel:        DD level
//
int ipl_build( char*      io_fnImageSbe,
               void*      i_imageHw,
               uint32_t  i_ddLevel )
{
    int             rc = 0, rcLoc = 0, rcSearch = 0, countRings = 0;
    void*            ringBuffer = NULL;
    uint32_t        ringBlockSize = 0;
    void*            nextRing = NULL;
    uint8_t         iRingType = 0;
    uint8_t         xipSectionId = 0;
    uint8_t         bDone = 0;

//CMO-When removing sections, mmap SBE image file here first.

#if 0  //CMO-for now.. we probably need different impl for p9
    uint32_t        sizeImage = 0;
    P9XipSection    xipSection;

    // ==========================================================================
    // First, remove all unnecessary sections in the output image.
    // ==========================================================================
    //
    rcLoc1 = sbe_xip_delete_section( i_imageOut, P9_XIP_SECTION_HBBL);
    rcLoc2 = sbe_xip_image_size(i_imageOut, &sizeImage);
    rcLoc =  sbe_xip_validate(i_imageOut, sizeImage);

    if (rcLoc1 || rcLoc2 || rcLoc)
    {
        fprintf(stderr, "_delete_section(.hbbl) (rcLoc1=%i), _image_size() (rcLoc2=%i) and/or _validate() (rcLoc=%i) failed.\n",
                rcLoc1, rcLoc2, rcLoc);
        return IMGBUILD_ERR_SECTION_DELETE;
    }

    fprintf(stdout, "Image size (after .hbbl delete): %i\n", sizeImage);

    rcLoc1 = sbe_xip_delete_section( i_imageOut, P9_XIP_SECTION_RINGS);
    rcLoc2 = sbe_xip_image_size(i_imageOut, &sizeImage);
    rcLoc =  sbe_xip_validate(i_imageOut, sizeImage);

    if (rcLoc1 || rcLoc2 || rcLoc)
    {
        fprintf(stderr,
                "_delete_section(.rings) (rcLoc1=%i), _image_size() (rcLoc2=%i) and/or _validate() (rcLoc=%i) failed.\n", rcLoc1,
                rcLoc2, rcLoc);
        return IMGBUILD_ERR_SECTION_DELETE;
    }

    fprintf(stdout, "Image size (after .rings delete): %i\n", sizeImage);

    rcLoc1 = sbe_xip_delete_section( i_imageOut, P9_XIP_SECTION_OVERLAYS);
    rcLoc2 = sbe_xip_image_size(i_imageOut, &sizeImage);
    rcLoc =  sbe_xip_validate(i_imageOut, sizeImage);

    if (rcLoc1 || rcLoc2 || rcLoc)
    {
        fprintf(stderr,
                "_delete_section(.overlays) (rcLoc1=%i), _image_size() (rcLoc2=%i) and/or _validate() (rcLoc=%i) failed.\n", rcLoc1,
                rcLoc2, rcLoc);
        return IMGBUILD_ERR_SECTION_DELETE;
    }

    fprintf(stdout, "Image size (after .overlays delete): %i\n", sizeImage);

    rcLoc1 = sbe_xip_delete_section( i_imageOut, P9_XIP_SECTION_PIBMEM0);
    rcLoc2 = sbe_xip_image_size(i_imageOut, &sizeImage);
    rcLoc =  sbe_xip_validate(i_imageOut, sizeImage);

    if (rcLoc1 || rcLoc2 || rcLoc)
    {
        fprintf(stderr,
                "_delete_section(.pibmem0) (rcLoc1=%i), _image_size() (rcLoc2=%i) and/or _validate() (rcLoc=%i) failed.\n", rcLoc1,
                rcLoc2, rcLoc);
        return IMGBUILD_ERR_SECTION_DELETE;
    }

    fprintf(stdout, "Image size (after .pibmem0 delete): %i\n", sizeImage);

    rcLoc1 = sbe_xip_delete_section( i_imageOut, P9_XIP_SECTION_HALT);
    rcLoc2 = sbe_xip_image_size(i_imageOut, &sizeImage);
    rcLoc =  sbe_xip_validate(i_imageOut, sizeImage);

    if (rcLoc1 || rcLoc2 || rcLoc)
    {
        fprintf(stderr, "_delete_section(.halt) (rcLoc1=%i), _image_size() (rcLoc2=%i) and/or _validate() (rcLoc=%i) failed.\n",
                rcLoc1, rcLoc2, rcLoc);
        return IMGBUILD_ERR_SECTION_DELETE;
    }

    fprintf(stdout, "Image size (after .halt delete): %i\n", sizeImage);

    rcLoc1 = sbe_xip_delete_section( i_imageOut, P9_XIP_SECTION_STRINGS);
    rcLoc2 = sbe_xip_delete_section( i_imageOut, P9_XIP_SECTION_TOC);
    sbe_xip_image_size(i_imageOut, &sizeImage);
    rcLoc =  sbe_xip_validate(i_imageOut, sizeImage);

    if (rcLoc1 || rcLoc2 || rcLoc)
    {
        fprintf(stderr,
                "_delete_section(.strings) (rcLoc1=%i), _delete_section(.toc) (rcLoc2=%i) and/or _validate() (rcLoc=%i) failed.\n",
                rcLoc1, rcLoc2, rcLoc);
        return IMGBUILD_ERR_SECTION_DELETE;
    }

    fprintf(stdout, "Image size (after .strings and .toc delete): %i\n", sizeImage);

    rcLoc1 = sbe_xip_delete_section( i_imageOut, P9_XIP_SECTION_DATA);
    rcLoc2 = sbe_xip_image_size(i_imageOut, &sizeImage);
    rcLoc =  sbe_xip_validate(i_imageOut, sizeImage);

    if (rcLoc1 || rcLoc2 || rcLoc)
    {
        fprintf(stderr, "_delete_section(.data) (rcLoc1=%i), _image_size() (rcLoc2=%i) and/or _validate() (rcLoc=%i) failed.\n",
                rcLoc1, rcLoc2, rcLoc);
        return IMGBUILD_ERR_SECTION_DELETE;
    }

    fprintf(stdout, "Image size (after .data delete): %i\n", sizeImage);

    rcLoc1 = sbe_xip_delete_section( i_imageOut, P9_XIP_SECTION_TEXT);
    rcLoc2 = sbe_xip_image_size(i_imageOut, &sizeImage);
    rcLoc =  sbe_xip_validate(i_imageOut, sizeImage);

    if (rcLoc1 || rcLoc2 || rcLoc)
    {
        fprintf(stderr, "_delete_section(.text) (rcLoc1=%i), _image_size() (rcLoc2=%i) and/or _validate() (rcLoc=%i) failed.\n",
                rcLoc1, rcLoc2, rcLoc);
        return IMGBUILD_ERR_SECTION_DELETE;
    }

    fprintf(stdout, "Image size (after .text delete): %i\n", sizeImage);


    // ==========================================================================
    // Re-append .pibmem0
    // ==========================================================================
    rc = sbe_xip_get_section( i_imageSbe, P9_XIP_SECTION_PIBMEM0, &xipSection);

    if (rc)
    {
        MY_INF("ERROR : sbe_xip_get_section() failed: %s", P9_XIP_ERROR_STRING(g_errorStrings, rc));
        MY_INF("Probable cause:");
        MY_INF("\tThe section (=P9_XIP_SECTION_PIBMEM0=%i) was not found.", P9_XIP_SECTION_RINGS);
        return IMGBUILD_ERR_KEYWORD_NOT_FOUND;
    }

    rc = sbe_xip_append(     i_imageOut,
                             P9_XIP_SECTION_PIBMEM0,
                             (void*)((uintptr_t)i_imageSbe + xipSection.iv_offset),
                             xipSection.iv_size,
                             i_sizeImageOutMax,
                             0);

    if (rc)
    {
        MY_INF("sbe_xip_append() failed: %s", P9_XIP_ERROR_STRING(g_errorStrings, rc));
        return IMGBUILD_ERR_APPEND;
    }

#endif // #if 0

//CMO-When removing sections, close the mmapped SBE image file here.

    for (iRingType = 0; iRingType < RING_SECTION_ID_SIZE; iRingType++)
    {

        xipSectionId = RING_SECTION_ID[iRingType];
        nextRing = NULL;
        bDone = 0;

        /****************************************************************************
         *                          SEARCH LOOP - Begin                                *
         ****************************************************************************/

        do
        {

            MY_DBG("nextRing (at top)=0x%016lx\n", (uint64_t)nextRing);

            ringBlockSize = FIXED_RING_BUF_SIZE;
            ringBuffer = malloc(ringBlockSize);

            if (!ringBuffer)
            {
                MY_ERR("malloc() for fixed ring buffer failed.\n");
                exit(1);
            }

            // ==========================================================================
            // Get ring layout from HW ref image
            // ==========================================================================
            rcLoc = get_ring_from_image( i_imageHw,
                                         i_ddLevel,
                                         0,             // sysPhase = IPL-SBE image build
                                         ringBuffer,    // Contains copy of RS4 ring, incl layout
                                         &ringBlockSize,// Contains ring block size on return
                                         &nextRing,     // Points to next ring (may not need for P9)
                                         xipSectionId );
            rcSearch = rcLoc;

            if ( rcSearch != IMGBUILD_RING_SEARCH_MATCH &&
                 rcSearch != IMGBUILD_RING_SEARCH_EOS_MATCH &&
                 rcSearch != IMGBUILD_RING_SEARCH_EOS_NO_MATCH)
            {
                MY_ERR("ERROR : Error during retrieval of delta rings from the image (rcSearch=%i).\n", rcSearch);
                MY_ERR("No further RS4 rings will be appended to the IPL image.\n");
                MY_ERR("The IPL image is incomplete.\n");
                return IMGBUILD_ERR_INCOMPLETE_IMG_BUILD;
            }

            if ( rcSearch == IMGBUILD_RING_SEARCH_MATCH ||
                 rcSearch == IMGBUILD_RING_SEARCH_EOS_MATCH )
            {
                MY_DBG("Retrieving ring was successful.\n");
                countRings++;
            }

            // Check if we're done due to failure to find any more rings.
            //
            if ( rcSearch == IMGBUILD_RING_SEARCH_EOS_NO_MATCH )
            {
                MY_INF("Number of RS4 rings appended to ring section (ID=%i): %i\n", xipSectionId, countRings);
                rc = IMGBUILD_SUCCESS;
                bDone = 1;
            }

            if (!bDone)
            {
                rc = append_ring_to_image( io_fnImageSbe,
                                           ringBuffer,
                                           ringBlockSize );
                free(ringBuffer);

                if (rc)
                {
                    MY_ERR("ring_section_append() failed: rc=%i   Stopping.\n", rc);
                    exit(1);
                }
                else
                {
                    MY_DBG("Image update successful.\n");
                }

            } // End of if(!bDone)

            // Check if we're done due to having reached EOS of ring section and found a ring.
            //
            if ( rcSearch == IMGBUILD_RING_SEARCH_EOS_MATCH )
            {
                MY_INF("Number of RS4 rings appended to ring section (ID=%i): %i\n", xipSectionId, countRings);
                rc = IMGBUILD_SUCCESS;
                bDone = 1;
            }


        }
        while (!bDone);

        /***************************************************************************
         *    SEARCH LOOP - End
         ***************************************************************************/

        MY_INF("Done adding rings to ring section ID = %i\n", xipSectionId);

    } // End of for(iRingType=...) loop

    exit(1);
    return rc;
}
