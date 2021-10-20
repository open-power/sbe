/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/test/ecdsaAlgoTest/x86TestCase/ecverify.h $               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
#ifndef __EC_VERIFY_CORE_
#define __EC_VERIFY_CORE_

#include <stdint.h>

#define  EC_PRIMEBITS  521    /* P521 */

#define  EC_STACKTRACE  1       /* debug only; currently, glibc      */
#define  NO_EC_DOUBLE_XY 1      /* do not implement ec_double_xy */

#define EC_HASHBYTES   64     /* SHA-256 */
#define EC_COORDBYTES  66     /* P-521   */

#define  BN_PRIME_MSW        0x1ff
#define  BN_PRIME_MSW_MASK   0x1ff    /* equal, as coincidence, for P521 */
#define  BN_PRIME_MSW_BITS   (EC_PRIMEBITS % BN_BITS)



typedef uint8_t ecc_key_t[2*EC_COORDBYTES];
typedef uint8_t ecc_signature_t[2*EC_COORDBYTES];

typedef uint32_t     bn_t;
typedef uint16_t     hbn_t;         /* half-bignumber */
typedef uint_fast8_t bnindex_t;


#define  BN_BITS   (8*sizeof(bn_t))
#define  HBN_BITS  (8*sizeof(hbn_t))

#define  EC_PRIMEBYTES  ((EC_PRIMEBITS +7) /8)

#define  BN_MAXBIT  (((bn_t) 1) << (BN_BITS -1))

#define  BITS2BN(bits) (((bits) +BN_BITS -1) / BN_BITS)

// we only deal with big numbers of fixed size
#define  NWORDS   BITS2BN( EC_PRIMEBITS )
#define  BNBYTES  (NWORDS*sizeof(bn_t))

#define  BN_MSW(p)     ((p)[0])
#define  BN_LSW(p)     ((p)[ NWORDS-1 ])
#define  bn_is_odd(p)  (1 & BN_LSW(p))

typedef struct {
    bn_t ec_prime[ NWORDS ];
    bn_t ec_order[ NWORDS ];
    bn_t prime_px[ NWORDS ];
    bn_t prime_py[ NWORDS ];
    bn_t ec_order_qn[ NWORDS ];
    bn_t coeff_a[ NWORDS ];
    bn_t coeff_b[ NWORDS ];
} consts_t;



int ec_verify (const unsigned char *publicpt,    /* 2*EC_COORDBYTES */
               const unsigned char *hash,        /*   EC_HASHBYTES  */
               const unsigned char *signature);   /* 2*EC_COORDBYTES */

void bn_read_pt(bn_t *r, const unsigned char *data);
void __attribute__((noinline)) BN_COPY (bn_t *dst, const bn_t *src);
bn_t bn_sub    (bn_t *a, const bn_t *b) ;
void bn_add    (bn_t *a, const bn_t *b) ;
void bn_mul    (bn_t *r, const bn_t *a, const bn_t *b) ;
void bn_modadd (bn_t *a, const bn_t *b) ;
void bn_modsub (bn_t *a, const bn_t *b) ;

int bn_cmp (const bn_t a[NWORDS], const bn_t b[NWORDS]) ;
void bn_modmul_prime (bn_t *a, const bn_t *b);
int ec_multiply (bn_t *x, bn_t *y, bn_t *z, const bn_t *k, const bn_t *k1);
void ec_projective2affine (bn_t *x, bn_t *y, const bn_t *z);


#endif
