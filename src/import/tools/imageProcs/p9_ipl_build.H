/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/tools/imageProcs/p9_ipl_build.H $                  */
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

#ifndef __P9_IMAGE_BLD_H
#define __P9_IMAGE_BLD_H

// $TODO - these are currently defined in p9_ring_apply.H - but I cant include
// that file due to other issues - added here till we sort it out.
#define IMGBUILD_SUCCESS                      0  // Successful img build.
#define IMGBUILD_ERR_FILE_ACCESS              2  // Unable to access/open file.
#define IMGBUILD_MEM_MAP_FAILED               3  // Failed to map image to memory location
#define IMGBUILD_NO_RINGS_FOUND               5  // Successful img build but no rings found.
#define IMGBUILD_ERR_MEMORY                   7  // Memory allocation error.
#define IMGBUILD_INVALID_IMAGE               10  // Invalid image.
#define IMGBUILD_ERR_RING_SEARCH             33  // Err assoc w/ring retrieval.
#define IMGBUILD_ERR_GET_SECTION             49  // Err assoc w/getting section ID.
#define IMGBUILD_ERR_APPEND                  51  // Err assoc w/appending to ELF section.
#define IMGBUILD_ERR_SECTION_SIZING          48  // Err assoc w/section sizing.

#endif
