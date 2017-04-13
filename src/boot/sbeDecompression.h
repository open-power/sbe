/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/sbeDecompression.h $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017                             */
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

uint8_t decompress(uint8_t * compBuffer, uint8_t * decompBuffer)
{
    uint8_t rc = 0;
    do
    {
        uint32_t iCount = 0;
        int32_t jCount = 0;

        uint32_t dict[256];
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

        uint32_t bitArray[bitCount];

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
        }//loops ends for (iCount = 0; iCount < bitCount ; iCount++)
    } while(0);
   return rc;
}

