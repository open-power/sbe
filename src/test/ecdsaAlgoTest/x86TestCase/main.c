/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/test/ecdsaAlgoTest/x86TestCase/main.c $                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021                             */
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
#include <stdio.h>
#include "ecverify.h"
#include "ROM.h"
#include <stdint.h>
#include "ppe42_string.h"

int main()
{
    uint8_t read_data[1288];

    printf("ECDSA Testing \n");

    FILE *ptr;
    ptr = fopen("SBE_FW.header","rb");
    fread(read_data,sizeof(read_data),1,ptr);
    fclose(ptr);

   SHA512_t message_hash = {0x8a,0x4d,0x8b,0x81,0xaf,0x46,0x78,0x91,0x7b,0xb3,0x76,0xd9,0x30,0x2e,0x0c,0x8f,0xc2,0xc3,0x80,0x7d,0x39,0x10,0x2e,0xac,0x46,0x96,0x42,0x52,0x02,0x41,0xf2,0xe1,0xf7,0x62,0x30,0x7f,0x52,0x18,0x52,0x56,0xc1,0x84,0x6d,0xe9,0x4c,0x73,0xfa,0x99,0x65,0xee,0x05,0xd4,0xb6,0x82,0x95,0xb2,0x87,0x14,0x8a,0x48,0xf1,0xe0,0x92,0x75};

    ROM_container_raw* container = (ROM_container_raw *)read_data;
    ROM_prefix_header_raw* prefix;
    prefix = (ROM_prefix_header_raw*)&container->prefix;
    ROM_prefix_data_raw* hw_data;
    hw_data = (ROM_prefix_data_raw*)(prefix->ecid + get8(&prefix->ecid_count)*ECID_SIZE);

    if(ec_verify (container->hw_pkey_a, message_hash, hw_data->hw_sig_a)<1)
    {
        printf("Failed to verify signature \n");
    }
    else
    {
        printf("Signature verified \n");
    }

    return 0;
}
