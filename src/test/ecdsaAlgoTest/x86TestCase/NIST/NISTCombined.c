/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/test/ecdsaAlgoTest/x86TestCase/NIST/NISTCombined.c $      */
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

    FILE *ptrTest;
    ptrTest = fopen("NIST_test_vectors.bin","rb");

    uint8_t testPublicKey[132];
    uint8_t testSignature[132];
    uint8_t testMessageHash[64];
    int i,passed=0, failed=0;
    for(i=0;i<10;i++){

    fread(testMessageHash ,sizeof(uint8_t), 64, ptrTest);
    fread(testPublicKey, sizeof(uint8_t), 132, ptrTest);
    fread(testSignature, sizeof(uint8_t), 132, ptrTest);

    if(ec_verify(testPublicKey , testMessageHash, testSignature )<1)
    {
        printf("NIST TC %d FAILED CASE !\n", i+1);
        failed+=1;
    }
    else
    {   printf("NIST TC %d PASSED CASE !\n", i+1);
        passed+=1;
    }
 }
    printf("\n \n *************************************\n\n%d tescases passed\n%d testcases failed\n\n*********************************\n\n", passed, failed );
    return 0;
}
