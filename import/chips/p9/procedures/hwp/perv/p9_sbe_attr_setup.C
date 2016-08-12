/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/perv/p9_sbe_attr_setup.C $     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
//------------------------------------------------------------------------------
/// @file  p9_sbe_attr_setup.C
///
/// @brief Read scratch Regs, update ATTR
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_attr_setup.H"

#include <p9_perv_scom_addresses.H>

fapi2::ReturnCode p9_sbe_attr_setup(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_scratch_reg = 0;
    fapi2::buffer<uint64_t> l_read_scratch8 = 0;
    fapi2::buffer<uint8_t> l_read_1 = 0;
    fapi2::buffer<uint8_t> l_read_2 = 0;
    fapi2::buffer<uint8_t> l_read_3 = 0;
    fapi2::buffer<uint16_t> l_read_4 = 0;
    fapi2::buffer<uint32_t> l_read_5 = 0;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::buffer<uint64_t> l_data64;
    bool sbe_slave_chip = false;
    fapi2::buffer<uint64_t> l_read_device_reg = 0;
    FAPI_INF("p9_sbe_attr_setup: Entering ...");

    FAPI_DBG("Read Scratch8 for validity of Scratch register");
    //Getting SCRATCH_REGISTER_8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_8_SCOM,
                            l_read_scratch8)); //l_read_scratch8 = PIB.SCRATCH_REGISTER_8

    //set_security_acess
    {
        fapi2::buffer<uint64_t> l_read_reg;

        FAPI_DBG("Reading ATTR_SECURITY_MODE");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SECURITY_MODE, FAPI_SYSTEM, l_read_1));

        if ( l_read_1.getBit<7>() == 0 )
        {
            FAPI_DBG("Clear Security Access Bit");
            //Setting CBS_CS register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_CBS_CS_SCOM, l_data64));
            l_data64.clearBit<4>();  //PIB.CBS_CS.CBS_CS_SECURE_ACCESS_BIT = 0
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CBS_CS_SCOM, l_data64));
        }

        //Getting CBS_CS register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_CBS_CS_SCOM,
                                l_read_reg)); //l_read_reg = PIB.CBS_CS

        l_read_1 = 0;
        l_read_1.writeBit<7>(l_read_reg.getBit<4>());

        FAPI_DBG("Setting ATTR_SECURITY_ENABLE with the SAB state");
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SECURITY_ENABLE, FAPI_SYSTEM, l_read_1));

    }
    //read_scratch1_reg
    {
        if ( l_read_scratch8.getBit<0>() )
        {
            FAPI_DBG("Reading Scratch_reg1");
            //Getting SCRATCH_REGISTER_1 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_1_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_1

            l_read_scratch_reg.extract<0, 6>(l_read_1);
            l_read_scratch_reg.extract<8, 24>(l_read_5);

            FAPI_DBG("Setting up ATTR_EQ_GARD, ATTR_EC_GARD");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_EQ_GARD, i_target_chip, l_read_1));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_EC_GARD, i_target_chip, l_read_5));

            l_read_1 = 0;
            l_read_4 = 0;
        }
    }
    //read_scratch2_reg
    {
        if ( l_read_scratch8.getBit<1>() )
        {
            FAPI_DBG("Reading Scratch_reg2");
            //Getting SCRATCH_REGISTER_2 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_2_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_2

            l_read_scratch_reg.extractToRight<0, 16>(l_read_4);

            FAPI_DBG("Setting up ATTR_I2C_BUS_DIV_REF");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_I2C_BUS_DIV_REF, i_target_chip, l_read_4));
        }
    }

    //read_scratch4_reg
    {
        if ( l_read_scratch8.getBit<3>() )
        {
            FAPI_DBG("Reading Scratch_Reg4");
            //Getting SCRATCH_REGISTER_4 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_4_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_4

            l_read_scratch_reg.extractToRight<0, 16>(l_read_4);
            l_read_scratch_reg.extractToRight<24, 8>(l_read_1);

            FAPI_DBG("Setting up ATTR_BOOT_FREQ_MULT, ATTR_NEST_PLL_BUCKET");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_BOOT_FREQ_MULT, i_target_chip, l_read_4));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NEST_PLL_BUCKET, FAPI_SYSTEM, l_read_1));

            l_read_1 = 0;
            l_read_4 = 0;
        }
    }
    //read_scratch5_reg
    {
        if ( l_read_scratch8.getBit<4>() )
        {
            FAPI_DBG("Reading Scratch_reg5");
            //Getting SCRATCH_REGISTER_5 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_5_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_5

            if (l_read_scratch_reg.getBit<0>())
            {
                l_read_1 = fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED;
            }
            else
            {
                l_read_1 = fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL;
            }

            l_read_2.writeBit<7>(l_read_scratch_reg.getBit<1>());

            if (l_read_scratch_reg.getBit<2>())
            {
                l_read_3 = fapi2::ENUM_ATTR_RISK_LEVEL_TRUE;
            }
            else
            {
                l_read_3 = fapi2::ENUM_ATTR_RISK_LEVEL_FALSE;
            }

            FAPI_DBG("Setting up SYSTEM_IPL_PHASE, RISK_LEVEL, SYS_FORCE_ALL_CORES");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_read_1));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SYS_FORCE_ALL_CORES, FAPI_SYSTEM,
                                   l_read_2));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_RISK_LEVEL, FAPI_SYSTEM, l_read_3));

            l_read_1 = 0;
            l_read_2  = 0;
            l_read_3 = 0;

            if (l_read_scratch_reg.getBit<3>())
            {
                l_read_1 = fapi2::ENUM_ATTR_DISABLE_HBBL_VECTORS_TRUE;
            }
            else
            {
                l_read_1 = fapi2::ENUM_ATTR_DISABLE_HBBL_VECTORS_FALSE;
            }

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_DISABLE_HBBL_VECTORS, FAPI_SYSTEM,
                                   l_read_1));

            l_read_1 = 0;
        }
    }
    //read_scratch6_reg
    {
        if ( l_read_scratch8.getBit<5>() )
        {
            FAPI_DBG("Reading Scratch_reg6");
            //Getting SCRATCH_REGISTER_6 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_6_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_6

            l_read_1 = 0;
            sbe_slave_chip = l_read_scratch_reg.getBit<24>();

            if ( !sbe_slave_chip )  // 0b0 == master
            {
                FAPI_DBG("Reading DEVICE_ID_REG value");
                FAPI_TRY(fapi2::getScom(i_target_chip, PERV_DEVICE_ID_REG, l_read_device_reg));

                if (!l_read_device_reg.getBit<40>())
                {
                    l_read_1.setBit<7>();
                }
            }

            l_read_scratch_reg.extractToRight<26, 3>(l_read_2);
            l_read_scratch_reg.extractToRight<29, 3>(l_read_3);

            FAPI_DBG("Setting up MASTER_CHIP, FABRIC_GROUP_ID and CHIP_ID");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_SBE_MASTER_CHIP, i_target_chip,
                                   l_read_1));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_FABRIC_GROUP_ID, i_target_chip,
                                   l_read_2));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_PROC_FABRIC_CHIP_ID, i_target_chip,
                                   l_read_3));

        }
    }

    FAPI_INF("p9_sbe_attr_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
