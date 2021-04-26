/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevsecuritysetting.C $                */
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
#include "sbevsecuritysetting.H"
#include "ppe42_scom.h"
#include "p10_scom_perv.H"
#include "sbeglobals.H"
#include "p10_sbe_scratch_regs.H"
#include "sbevtrace.H"

void sbevSetSecureAccessBit(uint32_t i_secureHdrStatus, uint32_t i_secureHdrFlags)
{
    #define SBEV_FUNC " sbemSetSecureAccessBit "
    SBEV_ENTER(SBEV_FUNC);
    uint32_t rc = 0;
    fapi2::buffer<uint64_t> cbs_cs_reg;
    fapi2::buffer<uint64_t> scratch_reg3;
    fapi2::buffer<uint64_t> scratch_reg8;
    fapi2::buffer<uint64_t> temp_lfr_reg;
    sbe_local_LFR lfr_reg;
    uint32_t security_mode = 1;
    Target<TARGET_TYPE_PROC_CHIP> target =  plat_getChipTarget();

    do
    {
        //Skip evaluating SAB in MPIPL Path only evaluate in cold Ipl path
        // Fetch LFR
        rc = getscom_abs_wrap (&target, 0xc0002040, &temp_lfr_reg());
        SBEV_INFO(SBEV_FUNC "LFR Reg before update [0x%08X %08X]",
             ((temp_lfr_reg >> 32) & 0xFFFFFFFF), (temp_lfr_reg & 0xFFFFFFFF));
        if(temp_lfr_reg.getBit<14>())
        {
            SBEV_INFO(SBEV_FUNC "Not evaluating SAB in MPIPL Path");
            break;
        }

        //FAPI_ATTR_GET(fapi2::ATTR_SECURITY_MODE, FAPI_SYSTEM, security_mode));
        //The above attribute will come directly from the parsing the boot seeprom
        //secure header xip section flags to determine if the SBE should allow
        //secure_mode to be disabled based on the scratch register request
        if(i_secureHdrStatus == 0)  //Secure Header verification failed, security disablement not allowed .
        {
            SBEV_INFO(SBEV_FUNC "Disabling security not allowed since SBE_FW Secure Header Failed");
            security_mode = 1;
        }
        else //Secure Header verification passed
        {
            // Check for SB header flag (0x00080000) that is only possible in imprint signed images
            if ((0x00080000 & i_secureHdrFlags) != 0)
            {
                security_mode = 0;
                SBEV_INFO(SBEV_FUNC "Secure Header valid and security disablement allowed");
            }
            else
            {
                security_mode = 1;
                SBEV_INFO(SBEV_FUNC "Secure Header valid and security disablement NOT allowed");
            }
        }
        // 1 == Secure mode == Disabling security not allowed
        // 0 == Unsecure mode == Disabling security allowed via request in scratch register

        rc = getscom_abs_wrap (&target, scomt::perv::FSXCOMP_FSXLOG_CBS_CS, &cbs_cs_reg());
        if(rc)
        {
            SBEV_ERROR("Failed to read CBS_Control_status register");
            pk_halt();
        }
        SBEV_INFO(SBEV_FUNC "CBS_Control_status register [0x%08X %08X]",
                ((cbs_cs_reg >> 32) & 0xFFFFFFFF), (cbs_cs_reg & 0xFFFFFFFF));

        rc = getscom_abs_wrap (&target, scomt::perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_8_RW, &scratch_reg8());
        if(rc)
        {
            SBEV_ERROR("Failed to read Scratch register8");
            pk_halt();
        }
        SBEV_INFO(SBEV_FUNC "Scratch register8 [0x%08X %08X]",
             ((scratch_reg8 >> 32) & 0xFFFFFFFF), (scratch_reg8 & 0xFFFFFFFF));

        // Updating Secure mode bit into LFR Bit 19, so that Boot Seeprom can
        // fetch it from here again and interpret what needs to be set for
        // securityOverride mode and allowAttrOverrides, these needs to be
        // passed to hostboot loader
        lfr_reg.secure_mode = security_mode;
        PPE_STVD(0xc0002050, lfr_reg);
        rc = getscom_abs_wrap (&target, 0xc0002040, &temp_lfr_reg());
        SBEV_INFO(SBEV_FUNC "LFR Reg after update [0x%08X %08X]",
              ((temp_lfr_reg >> 32) & 0xFFFFFFFF), (temp_lfr_reg & 0xFFFFFFFF));

        /*
         * Jumper SMD - 2801:Bit5 -> Sampled state of Secure Mode Disable c4 pin (SMD)
         * SBE Security override  -> ATTR_SECURITY_MODE, 1 enable, 0 read mailbox
         * Scratch Disable Security -> 5003A bit6, Disable Security
         |---------------------------------------------------------------------------------------|
         |Jumper val/SMD Value   SBE Security override     Scratch Disable Security   Result     |
         |---------------------------------------------------------------------------------------|
         |1 (request disable)    x (don't care)            x(don't care)              Non-secure |
         |0 (don't req disable)  x (invalid scratch 3)     x                          Secure     |
         |0                      1 (don't check mbox)      x                          Secure     |
         |0                      0 (check mbox -dev only)  0 (don't ask for disable)  Secure     |
         |0                      0                         1 (ask for disable)        Non-secure |
         |---------------------------------------------------------------------------------------|
         */

        // This will make sense only if C4 pin status on SMD bit is secure i.e. 0
        if(cbs_cs_reg.getBit<5>() == 0)
        {
            SBEV_INFO("2801:Bit5[%d] 2801:Bit4[%d]", cbs_cs_reg.getBit<5>(), cbs_cs_reg.getBit<4>());
            if( !(security_mode) && (scratch_reg8.getBit<SCRATCH3_REG_VALID_BIT>()) )
            {
                SBEV_INFO(SBEV_FUNC "Reading mailbox Scratch Register3 Bit6 to "
                        "check for external security override request");

                rc = getscom_abs_wrap (&target, scomt::perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_3_RW, &scratch_reg3());
                if(rc)
                {
                    SBEV_ERROR("Failed to read Scratch register3");
                    pk_halt();
                }
                SBEV_INFO("Scratch register3 [0x%08X %08X] Bit6[%d] Bit7[%d]",
                        ((scratch_reg3 >> 32) & 0xFFFFFFFF),(scratch_reg3 & 0xFFFFFFFF),
                        scratch_reg3.getBit<6>(), scratch_reg3.getBit<7>());
                cbs_cs_reg.writeBit<scomt::perv::FSXCOMP_FSXLOG_CBS_CS_SECURE_ACCESS_BIT>(!scratch_reg3.getBit<6>());
            }
            else
            {
                // Enable Secure mode
                SBEV_INFO(SBEV_FUNC "Either Security Mode or Scratch8 is not set, Setting SAB by default");
                cbs_cs_reg.setBit<scomt::perv::FSXCOMP_FSXLOG_CBS_CS_SECURE_ACCESS_BIT>();
            }
        }
        SBEV_INFO(SBEV_FUNC "Updating the SAB Bit4[%d]", cbs_cs_reg.getBit<4>());
        rc = putscom_abs_wrap (&target, scomt::perv::FSXCOMP_FSXLOG_CBS_CS, cbs_cs_reg());
        if(rc)
        {
            SBEV_ERROR("Failed to write CBS_Control_status register");
            pk_halt();
        }
    }while(0);
    SBEV_EXIT(SBEV_FUNC);
}
