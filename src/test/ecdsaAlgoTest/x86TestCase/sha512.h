/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/test/ecdsaAlgoTest/x86TestCase/sha512.h $                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
#ifndef SHA512_H
#define SHA512_H

#define SHA512_BLOCK_LENGTH 128
#define SHA512_DIGEST_LENGTH 64

#ifndef __PPE__
#include <stdlib.h>
#else
#include <stddef.h>
#endif

typedef uint8_t SHA512_t[SHA512_DIGEST_LENGTH];

typedef uint8_t  sha2_byte;     /* Exactly 1 byte */
typedef uint32_t sha2_word32;   /* Exactly 4 bytes */
typedef uint64_t sha2_word64;   /* Exactly 8 bytes */

typedef struct _SHA512_CTX
{
    uint64_t state[8];
    uint64_t bitcount[2];
    uint8_t buffer[SHA512_BLOCK_LENGTH];
} SHA512_CTX;


void SHA512_Init(SHA512_CTX* context);
void SHA512_Update(SHA512_CTX* context, const sha2_byte *data, size_t len);
void SHA512_Final(SHA512_CTX* context, SHA512_t *result);
void SHA512_Hash(const sha2_byte *data, size_t len, SHA512_t *result);

#endif
