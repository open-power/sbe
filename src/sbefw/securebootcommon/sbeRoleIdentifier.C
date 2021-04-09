/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/sbeRoleIdentifier.C $              */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021                             */
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
#include "sbeRoleIdentifier.H"
#include "sbemtrace.H"
#include "sbestates.H"
#include "target.H"
#include "plat_utils.H"
#include "plat_hw_access.H"
#include <p10_scom_perv_9.H>
#include <p10_scom_perv_e.H>

using namespace fapi2;

uint32_t checkSbeRole()
{
#define SBEM_FUNC "checkSbeRole "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    uint32_t sbeRole = SBE_ROLE_SLAVE; //1
    do
    {
        fapi2::buffer<uint64_t> cbsreg;
        fapi2::buffer<uint64_t> scratchreg8;
        fapi2::buffer<uint64_t> scratchreg6;
        rc = getscom_abs (scomt::perv::FSXCOMP_FSXLOG_CBS_ENVSTAT_RO, &cbsreg());
        SBE_INFO(SBE_FUNC "CBS_ENVSTAT_REG value is 0x%08X %08X", ((cbsreg >> 32) & 0xFFFFFFFF), (cbsreg & 0xFFFFFFFF));

        //Read Scratch register 8.
        rc = getscom_abs (scomt::perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_8_RW, &scratchreg8());
        SBE_INFO(SBE_FUNC "SCRTATCH_REG 8 is 0x%08X %08X",
            ((scratchreg8 >> 32) & 0xFFFFFFFF), (scratchreg8 & 0xFFFFFFFF));

        if(scratchreg8.getBit<5>())
        {
            //Read Scratch Reg 6.
            rc = getscom_abs (scomt::perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_6_RW, &scratchreg6());
            SBE_INFO(SBE_FUNC "SCRATCH_REG 6 is 0x%08X %08X",
                      ((scratchreg6 >> 32) & 0xFFFFFFFF), (scratchreg6 & 0xFFFFFFFF));
            if ( scratchreg6.getBit<24>() )
            {
                sbeRole = SBE_ROLE_MASTER;
                if( !((cbsreg >> 32) & 0x8000000) )
                {
                    SBE_INFO(SBE_FUNC "SBE Role is Secondary");
                    sbeRole = SBE_ROLE_SLAVE;
                }
                else
                {
                    SBE_INFO(SBE_FUNC "SBE Role is Master");
                }
            }
            else
            {
               //ALT master
               if( ((cbsreg >> 32) & 0x8000000) )
                {   
                    SBE_INFO(SBE_FUNC "SBE Role is Alt-Master");
                    sbeRole = SBE_ROLE_ALT_MASTER;
                }
                else
                {
                    SBE_INFO(SBE_FUNC "SBE Role is Secondary");
                }
            }
        }
        else
        {
            if( (cbsreg >> 32) & 0x8000000 )
            {
                SBE_INFO(SBE_FUNC "SBE Role is Master");
                sbeRole = SBE_ROLE_MASTER;
            }
            else
            {
                SBE_INFO(SBE_FUNC "SBE Role is Secondary");
            }
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return sbeRole;
    #undef SBEM_FUNC
}
