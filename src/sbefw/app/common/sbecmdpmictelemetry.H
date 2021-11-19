/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbecmdpmictelemetry.H $                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
#ifndef __SBE_CMD_PMIC_TELEMETRY__
#define __SBE_CMD_PMIC_TELEMETRY__

// Structure to parse arguments for PMIC Health Check chipop (0xAC, 0x01)
typedef struct
{
    uint32_t targetType     :16;    // target type as per sbeTargetTypes_t.
                                    // Only TARGET_OCMB_CHIP supported for this chipop.
    uint32_t reserved       :8;
    uint32_t targetInstance :8;     // Target intstance number. value between 0-15.

    /**
      * @brief Validate target type.
      *
      * @return true incase of valid target type, else false.
      */
    bool validateTargetType()
    {
        return (targetType == TARGET_OCMB_CHIP)?true:false;
    }

    /**
      * @brief Validate OCMB target instance.
      *
      * @return true incase of valid instance, else false.
      */
    bool validateInstance()
    {
        return (targetInstance >= 0 && targetInstance < OCMB_TARGET_COUNT)?true:false;
    }

    /**
      * @brief Validate functional state of the target requested.
      *
      * @return true incase of target is functional, else false.
      */
    bool validateFunctional()
    {
        return plat_getOCMBTargetHandleByInstance<fapi2::TARGET_TYPE_OCMB_CHIP>
            (targetInstance).fields.functional;
    }
} sbePmicHealthCheckCmdHdr_t;

// Structure to parse arguments for PMIC Health Check chipop (0xD7, 0x0C)
typedef struct
{
      sbePmicHealthCheckCmdHdr_t pmicDataMsg;
      uint32_t reserved     :32;
      uint64_t hbAddress    :64;
} psu2SbePmicTelemetry_t;


/**
  * @brief check the health status of PMIC in DIMM. (0xAC01)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbePmicHealthCheck( uint8_t *i_pArg );


/**
 *  * @brief check the PSU health status of PMIC in DIMM. (0xD70C)
 *
 *  @param[in] i_pArg Buffer to be passed to the function (not used as of now)
 *
 *  @return    Rc from the PSU access utility
 */
uint32_t sbePsuPmicHealthCheck( uint8_t *i_pArg );
#endif // __SBE_CMD_PMIC_TELEMETRY__
