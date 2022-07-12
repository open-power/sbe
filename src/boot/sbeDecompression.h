/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/sbeDecompression.h $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2022                        */
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

#define MAX_BIT_COUNT (356*1024 / 128) // Assuming 356 kb maximum 
                                       // base image size

enum
{
    DECOMPRESS_RC_INVALID_ARGUMENT = 1,
    DECOMPRESS_RC_FILE_OVERLAPPING = 2,
    DECOMPRESS_RC_BIT_COUNT_OVERFLOW = 3,
};

/**
 * @brief Decompress the content from compBuffer to decompBuffer
 *
 * @param[in]       *compBuffer Pointer to start of compressed data
 *
 * @param[in,out]   **decompBuffer Pointer to start address of decompress buffer
 *
 * @return 0 if success, else return non-zero error code.
 */
uint32_t decompress(uint8_t *compBuffer, uint8_t **_decompBuffer)
{
    #define SBEV_FUNC " decompress "
    uint32_t bitArray[MAX_BIT_COUNT];
    uint32_t dict[256];
    uint32_t rc = 0;
    uint8_t *decompBuffer = *(_decompBuffer);
    do
    {
        // we are assuming compBuffer is at end of the pibmem and decompBuffer is
        //      at start of pibmem. This assumption is required to check the overlap
        //      of decompBuffer with compBuffer
        if(((uint32_t)compBuffer) < ((uint32_t)decompBuffer))
        {
            SBEV_ERROR (SBEV_FUNC "FAILED : compBuffer(0x%08X) is less than decompBuffer(0x%08X)",
                            (uint32_t)compBuffer, (uint32_t)decompBuffer);
            rc = DECOMPRESS_RC_INVALID_ARGUMENT;
            break;
        }
        uint32_t iCount = 0;
        int32_t jCount = 0;

        for(iCount = 0; iCount < 256; iCount++)
        {
           uint64_t temp = *(uint64_t *)compBuffer;
           dict[iCount] = (uint32_t)(((temp) & 0xFFFFFFFF00000000LL) >> 32);
           ++iCount;
           dict[iCount] = (uint32_t)(((temp) & 0xFFFFFFFFLL));
           compBuffer = compBuffer + 8;
        }

        //Get the file size.
        uint64_t compfileSize = *((uint64_t * )compBuffer);
        compBuffer = compBuffer + 8;
        uint32_t quo = compfileSize / 128;
        uint32_t rem = compfileSize % (128);
        uint32_t bitCount;
        if(rem == 0)
            bitCount = quo;
        else
            bitCount = quo + 1;

        if(bitCount > (MAX_BIT_COUNT - 1)) // -1, since we are doing ++iCount inside for loop
        {
            SBEV_ERROR (SBEV_FUNC "FAILED : bitCount(0x%08X) greater than MAX_BIT_COUNT", bitCount);
            rc = DECOMPRESS_RC_BIT_COUNT_OVERFLOW;
            break;
        }

        for(iCount = 0; iCount < bitCount; iCount++)
        {
            uint64_t temp = *(uint64_t *)compBuffer;
            bitArray[iCount] = (uint32_t)(((temp) & 0xFFFFFFFF00000000LL) >> 32);
            ++iCount;
            bitArray[iCount] = (uint32_t)((temp) & 0xFFFFFFFFLL);
            compBuffer = compBuffer + 8;
        }

        uint64_t eightByte = *(uint64_t *)compBuffer;
        uint8_t *ptr = (uint8_t *)&eightByte;
        compBuffer = compBuffer + 8;
        uint32_t kCount = 0;

        for(iCount = 0; iCount < bitCount ; iCount++)
        {
            //Extract a bit from 32 bit integer.
            int32_t j = 31;

            if((iCount == bitCount - 1) && (rem != 0))
                j = rem/4 -1;

            for (jCount = j; jCount >= 0; jCount--)
            {
                if((uint32_t)compBuffer <= (uint32_t)decompBuffer)
                {
                    SBEV_ERROR (SBEV_FUNC "FAILED : overlapping compBuffer(0x%08X) with decompBuffer(0x%08X)",
                            (uint32_t)compBuffer, (uint32_t)decompBuffer);
                    rc = DECOMPRESS_RC_FILE_OVERLAPPING;
                    break;
                }
                char isCompressed = (bitArray[iCount] >> jCount) & 1;
                if(isCompressed == 1)
                {
                    //Read a byte.
                    uint8_t index = *(ptr + kCount);
                    ++kCount;
                    if(kCount == 8)
                    {
                        eightByte = *(uint64_t *)compBuffer;
                        compBuffer = compBuffer + 8;
                        kCount = 0;
                    }
                    uint32_t value = dict[index];
                    uint32_t * pTemp = (uint32_t *) decompBuffer;
                    *pTemp = value;
                    decompBuffer = decompBuffer + 4;
                }
                else
                {
                    //Read four bytes.
                    uint8_t i;
                    for(i = 0; i < 4; i++)
                    {
                        *decompBuffer = *(ptr + kCount);
                        ++kCount;
                        if(kCount == 8)
                        {
                            eightByte = *(uint64_t *)compBuffer;
                            compBuffer = compBuffer + 8;
                            kCount = 0;
                        }
                        ++decompBuffer;
                    }
                }// else
            }// loop ends for (jCount = j; jCount >= 0; jCount--)

            if(rc)
            {
                break;
            }
        }//loops ends for (iCount = 0; iCount < bitCount ; iCount++)

        // return the end address
        *_decompBuffer = decompBuffer;
    } while(0);
   return rc;
   #undef SBEV_FUNC
}

