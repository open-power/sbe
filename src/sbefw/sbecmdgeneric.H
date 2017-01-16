/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdgeneric.H $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
/*
 * @file: ppe/sbe/sbefw/sbecmdgeneric.H
 *
 * @brief This file contains the SBE Generic command details
 *
 */

#ifndef __SBEFW_SBECMDGENERIC_H
#define __SBEFW_SBECMDGENERIC_H

#include <stdint.h>
#include <p9_sbe_hb_structures.H>

typedef struct sbeStashMemoryPair
{
    // This is coming from p9_sbe_hb_structures.H
    keyAddrPair_t keyValuePairfromHost;

    // Default Constructor to initialize addr/key to 0xFF
    sbeStashMemoryPair()
    {
        for(uint8_t cnt=0; cnt<MAX_ROW_COUNT; cnt++)
        {
            keyValuePairfromHost.addr[cnt] = 0xFFFFFFFFFFFFFFFFULL;
            keyValuePairfromHost.key[cnt] = 0xFF;
        }
    }

    bool updatePair(uint8_t key, uint64_t addr)
    {
        bool l_return = false;
        // Check if Key already exist, if yes update addr there
        for(uint8_t cnt=0; cnt<MAX_ROW_COUNT; cnt++)
        {
            if(keyValuePairfromHost.key[cnt] == key)
            {
                keyValuePairfromHost.addr[cnt] = addr;
                l_return = true;
                break;
            }
        }
        if(false == l_return) // Insert the new key into a free pair
        {
            for(uint8_t cnt=0; cnt<MAX_ROW_COUNT; cnt++)
            {
                if(keyValuePairfromHost.key[cnt] == 0xFF)
                {
                    keyValuePairfromHost.key[cnt] = key;
                    keyValuePairfromHost.addr[cnt] = addr;
                    l_return = true;
                    break;
                }
            }
        }
        return l_return;
    }

    uint64_t fetchStashAddrAttribute()
    {
        return (uint64_t)((uint8_t*)(&keyValuePairfromHost));
    }
}sbeStashMemoryPair_t;

// Message struct to receive the key/addr pair from host
typedef struct stashMsg
{
    uint64_t reserve:56;
    uint64_t key:8;
    uint64_t addr;
}stashMsg_t;

/**
  * @brief retrieve SBE FFDC on request from FSP.  (0xA801)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetFfdc(uint8_t *i_pArg);

/**
  * @brief SBE Generic capabilities  (0xA802)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetCapabilities(uint8_t *i_pArg);

/**
  * @brief SBE Fifo Quiesce (0xA806)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeFifoQuiesce(uint8_t *i_pArg);

/**
  * @brief SBE Psu Set FFDC Address(0xD704)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the Psu access utility
  */
uint32_t sbeSetFFDCAddr(uint8_t *i_pArg);

/**
  * @brief SBE Psu Quiesce (0xD705)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the Psu access utility
  */
uint32_t sbePsuQuiesce(uint8_t *i_pArg);

/**
  * @brief SBE Psu Set System Fabric Map (0xD706)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the Psu access utility
  */
uint32_t sbeSetSystemFabricMap(uint8_t *i_pArg);

/**
  * @brief SBE stash the key addr pair into sbe memory (0xD707)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the Psu access utility
  */
uint32_t sbeStashKeyAddrPair(uint8_t *i_pArg);

#endif // __SBEFW_SBECMDGENERIC_H
