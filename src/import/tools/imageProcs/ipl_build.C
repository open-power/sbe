/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/tools/imageProcs/ipl_build.C $                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
/*----------------------------------------------------------------------------*/
/* *! TITLE : p9_ipl_build.C                                                  */
/* *! DESCRIPTION : Copies RS4 delta ring states from unsigned HW image to DD */
/*                  specific PNOR SBE image.                                  */
/* *! OWNER NAME : Michael Olsen cmolsen@us.ibm.com                           */
/*                                                                            */
/* *! EXTENDED DESCRIPTION :                                                  */
/*                                                                            */
/* *! USAGE : p9_ipl_build <sbe image> <unsigend hw image> <dd level>         */
/*                                                                            */
/* *! ASSUMPTIONS :                                                           */
/*    - sysPhase=0 is assumed which puts the SBE image together for IPL.      */
/*                                                                            */
/* *! COMMENTS :                                                              */
/*----------------------------------------------------------------------------*/
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <dirent.h>

#include "ipl_build.H"
#include <p10_ipl_image.H>
#include <p10_tor.H>
#include <p10_infrastruct_help.H>


///
/// @brief retrieve a block of DD level rings from the unsigned hw image
///
/// @param[in]  i_hwImage - pointer to a an unsigned hw image.
/// @param[in]  i_ddLevel - DD level of rings to append
/// @param[out] o_ringBlock - DD level block of rings from the hw image.
/// @param[out] o_blockSize - size of ring block returned
///
/// @return IMGBUILD_SUCCESS, or failure value.
///
int get_dd_level_rings_from_sbe_image( char* i_hwImage,
                                       uint32_t i_ddLevel,
                                       void** o_ringBlock,
                                       uint32_t& o_blockSize )
{
    int rc = IMGBUILD_SUCCESS;

    P9XipSection l_ringsSection;
    *o_ringBlock = NULL;
    o_blockSize = 0;

    rc = p9_xip_get_sub_section( i_hwImage,
                                 P9_XIP_SECTION_HW_SBE,
                                 P9_XIP_SECTION_SBE_RINGS,
                                 &l_ringsSection,
                                 i_ddLevel);

    if (rc)
    {
        MY_ERR("ERROR: p9_xip_get_sub_section failed w/rc=0x%08x getting DD(=0x%x) specific"
               " ring section\n",
               i_ddLevel, (uint32_t)rc);
        rc = IMGBUILD_ERR_XIP_GET_SECTION;
        return rc;
    }

    // Check if any content in .rings
    if (l_ringsSection.iv_size == 0)
    {
        MY_ERR("Ring section size in SBE image is zero for ddLevel=0x%x."
               " We need rings here to proceed.\n", i_ddLevel);
        rc = IMGBUILD_EMPTY_RING_SECTION;
        return rc;
    }

    // Make a pointer to the start of the rings section
    *o_ringBlock = (void*)(i_hwImage + l_ringsSection.iv_offset);
    o_blockSize = l_ringsSection.iv_size;

    MY_DBG("o_blockSize = %d\n", o_blockSize);
    MY_DBG("*o_ringBlock = %p\n", *o_ringBlock);

    return rc;
};


///
/// @brief appends a block of rings to the previously un-populated .rings
///        section of the P9 SBE image
///
///  the passed in image pointer should point to an in memory sbe image
///  callers should ensure that the location is large enough for the existing
///  image and the new ring section.
///
/// @param[in]   io_sbeImage - pointer to an unsigned SBE image
/// @param[i/o]  io_sbeImageSize - size of image after section has been appended
/// @param[i]    i_ringBlock - pointer to a block of rings to be appended to
//                             the sbe image
/// @param[i]    i_blockSize - size of the block of rings to append.
///
/// @return 0 on success non-zero on failure
///
int append_ring_block_to_image( char* io_sbeImage,
                                size_t& io_sbeImageSize,
                                char* i_ringBlock,
                                uint32_t i_blockSize )
{
    uint32_t unused_parm = 0;
    int rc = IMGBUILD_SUCCESS;

    // Append block of rings to the sbe image in Memory
    rc = p9_xip_append( io_sbeImage,
                        P9_XIP_SECTION_SBE_RINGS,
                        i_ringBlock,
                        i_blockSize,
                        io_sbeImageSize,
                        &unused_parm,
                        0 );

    if(rc)
    {
        MY_ERR("error appending ring section = %d\n", rc);
        rc = IMGBUILD_ERR_XIP_APPEND;
    }

    MY_DBG("i_ringBlock = %p\n", i_ringBlock);

    return rc;
};

///
/// @brief Create an SBE image customized with DD level rings
///
/// @param[in]  i_fnSbeImage - File name of SBE image
/// @param[in]  i_hwImage - pointer to a memory mapped Unsigned hardware image
/// @param[i]   i_ddLevel - DD level of rings to append to the SBE image
///
int ipl_build( char* i_fnSbeImage,
               void* i_hwImage,
               uint32_t i_ddLevel )
{

    char* sbeImage    = NULL;
    void* l_ringBlock = NULL;
    int   rc          = 0;


    std::ifstream  sbeImageFile;

    sbeImageFile.open(i_fnSbeImage, std::ios::binary | std::ios::in | std::ios::out);


    if (!sbeImageFile)
    {
        MY_ERR("Failed to open %s\n", i_fnSbeImage);
        rc = IMGBUILD_ERR_FILE_ACCESS;
    }
    else
    {
        do
        {
            // get a filebuf pointer to make it easy to work with
            std::filebuf* pbuf = sbeImageFile.rdbuf();

            // get the file size
            std::size_t sbeImageSize = pbuf->pubseekoff(0,
                                       sbeImageFile.end, sbeImageFile.in);

            pbuf->pubseekpos(0, sbeImageFile.in);

            // allocate some space to hold the file data
            sbeImage =  (char*)malloc(sbeImageSize);

            if(sbeImage == NULL)
            {
                MY_ERR("Failed to allocate memory for the SBE image\n");
                rc = IMGBUILD_ERR_MEMORY;
                break;
            }

            bzero(sbeImage, sbeImageSize);

            // copy the SBE image into memory
            pbuf->sgetn(sbeImage, sbeImageSize);

            // Validate the SBE image
            rc = p9_xip_validate(sbeImage, sbeImageSize);

            if(rc)
            {
                MY_ERR("The SBE image failed validation w/rc = %d", rc);
                rc = IMGBUILD_XIP_INVALID_IMAGE;
                break;
            }

            uint32_t    l_blockSize = 0;

            char* hwImagePtr = static_cast<char*>(i_hwImage);

            rc = get_dd_level_rings_from_sbe_image( hwImagePtr,
                                                    i_ddLevel,
                                                    &l_ringBlock,
                                                    l_blockSize );

            if(rc == IMGBUILD_SUCCESS)
            {
                // update our SBE image size to include the new block of rings
                // and make sure it's 8 byte aligned
                sbeImageSize += ((l_blockSize + 7) / 8) * 8;

                // grow our workspace
                void* tmp = realloc(sbeImage, sbeImageSize);

                if(tmp == NULL)
                {
                    MY_ERR("error resizing workspace..giving up errno=%d", errno);
                    rc = IMGBUILD_ERR_MEMORY;
                    break;
                }

                // use the new, larger space
                sbeImage = static_cast<char*>(tmp);

                rc = append_ring_block_to_image( sbeImage,
                                                 sbeImageSize,
                                                 (char*)l_ringBlock,
                                                 l_blockSize );

                if(rc == IMGBUILD_SUCCESS)
                {
                    MY_INF("SUCCESS! DD specific SBE image created successfully!\n");
                    // rewind to the beginning of the original file and write this
                    // into it.
                    pbuf->pubseekpos(0, sbeImageFile.in);
                    pbuf->sputn(sbeImage, sbeImageSize);
                }
                else
                {
                    MY_ERR("creating dd specific SBE image failed rc=%d\n", rc);
                }
            }
        }
        while(0);

        free(sbeImage);

        sbeImageFile.close();
    }

    return rc;
}


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
    int         rc = IMGBUILD_SUCCESS;
    char*       fnSbeImage, *fnHwImage;
    uint32_t    ddLevel = 0;
    uint32_t    fdHwImage = 0;
    struct stat stbuf;
    uint32_t    sizeHwImage;
    void*       hwImage;

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
    fnSbeImage = argv[1];
    fnHwImage  = argv[2];
    ddLevel    = strtol(argv[3], NULL, 16);

    MY_INF("  Input/output SBE image fn = %s\n", fnSbeImage);
    MY_INF("  Input HW image fn         = %s\n", fnHwImage);
    MY_INF("  DD level                  = %#02x\n", ddLevel);

    // Memory map HW image.
    fdHwImage = open(fnHwImage, O_RDONLY);

    if (fstat(fdHwImage, &stbuf) != 0)
    {
        MY_ERR("Could not fstat the HW image file.\n");
        return 1;
    }

    sizeHwImage = stbuf.st_size;
    hwImage = mmap(0, sizeHwImage, PROT_READ, MAP_SHARED, fdHwImage, 0);

    if (hwImage == MAP_FAILED)
    {
        MY_ERR("mmap() of HW image failed.\n");
        rc = IMGBUILD_MEM_MAP_FAILED;
    }
    else
    {
        do
        {
            // Validate image.
            rc = p9_xip_validate(hwImage, sizeHwImage);

            if (rc)
            {
                MY_ERR("p9_xip_validate() of HW image failed: rc=%d\n", rc);
                rc = IMGBUILD_XIP_INVALID_IMAGE;
                break;
            }

            // Update the SBE image.
            rc = ipl_build( fnSbeImage,
                            hwImage,
                            ddLevel );

            if (rc == IMGBUILD_SUCCESS)
            {
                MY_INF("SBE image build was SUCCESSFUL.\n");
            }
            else
            {
                MY_ERR("SBE image build was UNSUCCESSFUL (rc=%i).\n", rc);
            }
        }
        while(0);

        munmap(hwImage, sizeHwImage);
    }

    close(fdHwImage);

    return rc;
}
