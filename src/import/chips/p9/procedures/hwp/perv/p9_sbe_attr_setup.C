/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_attr_setup.C $ */
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

enum P9_SETUP_SBE_CONFIG_scratch4
{
    // Scratch4 reg bit definitions
    ATTR_OBUS_RATIO_VALUE_BIT          = 21,
    ATTR_EQ_GARD_STARTBIT = 0,
    ATTR_EQ_GARD_LENGTH = 6,
    ATTR_EC_GARD_STARTBIT = 8,
    ATTR_EC_GARD_LENGTH = 24,
    ATTR_I2C_BUS_DIV_REF_STARTBIT = 0,
    ATTR_I2C_BUS_DIV_REF_LENGTH = 16,
    ATTR_BOOT_FLAGS_STARTBIT = 0,
    ATTR_BOOT_FLAGS_LENGTH = 32,
    ATTR_PROC_FABRIC_GROUP_ID_STARTBIT = 26,
    ATTR_PROC_FABRIC_GROUP_ID_LENGTH = 3,
    ATTR_PROC_FABRIC_CHIP_ID_STARTBIT = 29,
    ATTR_PROC_FABRIC_CHIP_ID_LENGTH = 3,
    ATTR_CC_IPL_BIT = 0,
    ATTR_INIT_ALL_CORES_BIT = 1,
    ATTR_RISK_LEVEL_BIT = 2,
    ATTR_DISABLE_HBBL_VECTORS_BIT = 3,
    ATTR_MC_SYNC_MODE_BIT = 4,
    ATTR_PLL_MUX_STARTBIT = 12,
    ATTR_PLL_MUX_LENGTH = 20,

    // Scratch4 reg bit definitions
    ATTR_BOOT_FREQ_MULT_STARTBIT       = 0,
    ATTR_BOOT_FREQ_MULT_LENGTH         = 16,
    ATTR_NEST_PLL_BUCKET_STARTBIT      = 24,
    ATTR_NEST_PLL_BUCKET_LENGTH        = 8,
    ATTR_CP_FILTER_BYPASS_BIT          = 16,
    ATTR_SS_FILTER_BYPASS_BIT          = 17,
    ATTR_IO_FILTER_BYPASS_BIT          = 18,
    ATTR_DPLL_BYPASS_BIT               = 19,
    ATTR_NEST_MEM_X_O_PCI_BYPASS_BIT   = 20,
};

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
    fapi2::buffer<uint32_t> l_read_6 = 0;
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
            l_read_5 = 0;
        }
        else
        {
            FAPI_DBG("Read Scratch_reg1");
            //Getting SCRATCH_REGISTER_1 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_1_SCOM,
                                    l_read_scratch_reg));

            FAPI_DBG("Reading ATTR_EQ_GARD, ATTR_EC_GARD");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_EQ_GARD, i_target_chip, l_read_1));
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_EC_GARD, i_target_chip, l_read_5));

            l_read_1.extractToRight< 0, ATTR_EQ_GARD_LENGTH >(l_read_2);
            l_read_5.extractToRight< 0, ATTR_EC_GARD_LENGTH >(l_read_6);

            l_read_scratch_reg.insertFromRight< ATTR_EQ_GARD_STARTBIT, ATTR_EQ_GARD_LENGTH >(l_read_2);
            l_read_scratch_reg.insertFromRight< ATTR_EC_GARD_STARTBIT, ATTR_EC_GARD_LENGTH >(l_read_6);

            FAPI_DBG("Setting up value of Scratch_reg1");
            //Setting SCRATCH_REGISTER_1 register value

            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_SCRATCH_REGISTER_1_SCOM,
                                    l_read_scratch_reg));
            l_read_scratch8.setBit<0>();
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
        else
        {
            FAPI_DBG("Reading Scratch_reg2");
            //Getting SCRATCH_REGISTER_2 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_2_SCOM,
                                    l_read_scratch_reg));

            FAPI_DBG("Reading ATTR_I2C_BUS_DIV_REF");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_I2C_BUS_DIV_REF, i_target_chip, l_read_4));

            l_read_scratch_reg.insertFromRight< ATTR_I2C_BUS_DIV_REF_STARTBIT, ATTR_I2C_BUS_DIV_REF_LENGTH >(l_read_4);

            FAPI_DBG("Setting up value of Scratch_reg2");
            //Setting SCRATCH_REGISTER_2 register value
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_SCRATCH_REGISTER_2_SCOM,
                                    l_read_scratch_reg));
            l_read_scratch8.setBit<1>();
        }
    }

    //read_scratch3_reg
    {
        uint8_t l_is_mpipl = 0;
        uint8_t l_is_sp_mode = 0;

        if ( l_read_scratch8.getBit<2>() )
        {
            FAPI_DBG("Reading Scratch_reg3");
            //Getting SCRATCH_REGISTER_3 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_3_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_

            l_read_scratch_reg.extractToRight<2, 1>(l_is_mpipl);

            FAPI_DBG("Setting up ATTR_IS_MPIPL");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_IS_MPIPL, FAPI_SYSTEM, l_is_mpipl));

            l_read_scratch_reg.extractToRight<3, 1>(l_is_sp_mode);

            FAPI_DBG("Setting up ATTR_IS_SP_MODE");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_IS_SP_MODE, i_target_chip, l_is_sp_mode));
        }
        else
        {
            FAPI_DBG("Reading Scratch_reg3");
            //Getting SCRATCH_REGISTER_3 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_3_SCOM,
                                    l_read_scratch_reg));

            FAPI_DBG("Reading the BOOT_FLAGS");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BOOT_FLAGS, FAPI_SYSTEM, l_read_5));

            l_read_scratch_reg.insertFromRight< ATTR_BOOT_FLAGS_STARTBIT, ATTR_BOOT_FLAGS_LENGTH >(l_read_5);

            FAPI_DBG("Setting up value of Scratch_reg3");
            //Setting SCRATCH_REGISTER_3 register value
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_SCRATCH_REGISTER_3_SCOM,
                                    l_read_scratch_reg));

            l_read_scratch8.setBit<2>();
        }
    }

    //read_scratch4_reg
    {

        uint8_t l_cp_filter_bypass = 0;
        uint8_t l_ss_filter_bypass = 0;
        uint8_t l_io_filter_bypass = 0;
        uint8_t l_dpll_bypass = 0;
        uint8_t l_nest_mem_x_o_pci_bypass = 0;
        uint8_t l_attr_obus_ratio = 0;

        if ( l_read_scratch8.getBit<3>() )
        {

            FAPI_DBG("Reading Scratch_Reg4");
            //Getting SCRATCH_REGISTER_4 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_4_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_4

            l_read_scratch_reg.extractToRight<0, 16>(l_read_4);
            l_read_scratch_reg.extractToRight<16, 1>(l_cp_filter_bypass);
            l_read_scratch_reg.extractToRight<17, 1>(l_ss_filter_bypass);
            l_read_scratch_reg.extractToRight<18, 1>(l_io_filter_bypass);
            l_read_scratch_reg.extractToRight<19, 1>(l_dpll_bypass);
            l_read_scratch_reg.extractToRight<20, 1>(l_nest_mem_x_o_pci_bypass);
            l_read_scratch_reg.extractToRight<ATTR_OBUS_RATIO_VALUE_BIT, 1>(l_attr_obus_ratio);
            l_read_scratch_reg.extractToRight<24, 8>(l_read_1);

            FAPI_DBG("Setting up PLL bypass attributes");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CP_FILTER_BYPASS, i_target_chip, l_cp_filter_bypass));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SS_FILTER_BYPASS, i_target_chip, l_ss_filter_bypass));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_IO_FILTER_BYPASS, i_target_chip, l_io_filter_bypass));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_DPLL_BYPASS, i_target_chip, l_dpll_bypass));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NEST_MEM_X_O_PCI_BYPASS, i_target_chip, l_nest_mem_x_o_pci_bypass));
            FAPI_DBG("Setting up ATTR_BOOT_FREQ_MULT, ATTR_NEST_PLL_BUCKET");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_BOOT_FREQ_MULT, i_target_chip, l_read_4));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NEST_PLL_BUCKET, FAPI_SYSTEM, l_read_1));

            FAPI_DBG("Setting up ATTR_OBUS_RATIO_VALUE");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_OBUS_RATIO_VALUE, i_target_chip, l_attr_obus_ratio));

            l_read_1 = 0;
            l_read_4 = 0;
        }
        else
        {
            FAPI_DBG("Reading Scratch_reg4");
            //Getting SCRATCH_REGISTER_4 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_4_SCOM,
                                    l_read_scratch_reg));

            FAPI_DBG("Reading PLL bypass attributes");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_FILTER_BYPASS, i_target_chip, l_cp_filter_bypass),
                     "Error from FAPI_ATTR_GET (ATTR_CP_FILTER_BYPASS");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SS_FILTER_BYPASS, i_target_chip, l_ss_filter_bypass),
                     "Error from FAPI_ATTR_GET (ATTR_SS_FILTER_BYPASS");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IO_FILTER_BYPASS, i_target_chip, l_io_filter_bypass),
                     "Error from FAPI_ATTR_GET (ATTR_IO_FILTER_BYPASS");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_DPLL_BYPASS, i_target_chip, l_dpll_bypass),
                     "Error from FAPI_ATTR_GET (ATTR_DPLL_BYPASS");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_MEM_X_O_PCI_BYPASS, i_target_chip, l_nest_mem_x_o_pci_bypass),
                     "Error from FAPI_ATTR_GET (ATTR_NEST_MEM_X_O_PCI_BYPASS");

            FAPI_DBG("Reading ATTR_BOOT_FREQ_MULT, ATTR_NEST_PLL_BUCKET");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BOOT_FREQ_MULT, i_target_chip, l_read_4));
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_PLL_BUCKET, FAPI_SYSTEM, l_read_1));

            l_read_scratch_reg.insertFromRight< ATTR_BOOT_FREQ_MULT_STARTBIT, ATTR_BOOT_FREQ_MULT_LENGTH >(l_read_4);
            l_read_scratch_reg.insertFromRight< ATTR_NEST_PLL_BUCKET_STARTBIT, ATTR_NEST_PLL_BUCKET_LENGTH >(l_read_1);

            l_read_scratch_reg.writeBit<ATTR_CP_FILTER_BYPASS_BIT>(l_cp_filter_bypass & 0x1);
            l_read_scratch_reg.writeBit<ATTR_SS_FILTER_BYPASS_BIT>(l_ss_filter_bypass & 0x1);
            l_read_scratch_reg.writeBit<ATTR_IO_FILTER_BYPASS_BIT>(l_io_filter_bypass & 0x1);
            l_read_scratch_reg.writeBit<ATTR_DPLL_BYPASS_BIT>(l_dpll_bypass & 0x1);
            l_read_scratch_reg.writeBit<ATTR_NEST_MEM_X_O_PCI_BYPASS_BIT>(l_nest_mem_x_o_pci_bypass & 0x1);

            // Setting OBUS ratio
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_OBUS_RATIO_VALUE, i_target_chip,
                                   l_attr_obus_ratio));
            l_read_scratch_reg.writeBit<ATTR_OBUS_RATIO_VALUE_BIT>(l_attr_obus_ratio & 0x1);

            FAPI_DBG("Setting up value of Scratch_reg4");
            //Setting SCRATCH_REGISTER_4 register value
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_SCRATCH_REGISTER_4_SCOM,
                                    l_read_scratch_reg));

            l_read_scratch8.setBit<3>();
        }
    }
    //read_scratch5_reg
    {

        uint8_t l_system_ipl_phase = 0;
        uint8_t l_force_all_cores = 0;
        uint8_t l_risk_level = 0;
        uint8_t l_disable_hbbl_vectors = 0;
        uint32_t l_pll_mux = 0;
        uint8_t l_mc_sync_mode = 0;

        if ( l_read_scratch8.getBit<4>() )
        {

            FAPI_DBG("Reading Scratch_reg5");
            //Getting SCRATCH_REGISTER_5 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_5_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_5

            if (l_read_scratch_reg.getBit<0>())
            {
                l_system_ipl_phase = fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED;
            }
            else
            {
                l_system_ipl_phase = fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL;
            }

            l_read_scratch_reg.extract<1, 1, 7>(l_force_all_cores);

            if (l_read_scratch_reg.getBit<2>())
            {
                l_risk_level = fapi2::ENUM_ATTR_RISK_LEVEL_TRUE;
            }
            else
            {
                l_risk_level = fapi2::ENUM_ATTR_RISK_LEVEL_FALSE;
            }

            if (l_read_scratch_reg.getBit<3>())
            {
                l_disable_hbbl_vectors = fapi2::ENUM_ATTR_DISABLE_HBBL_VECTORS_TRUE;
            }
            else
            {
                l_disable_hbbl_vectors = fapi2::ENUM_ATTR_DISABLE_HBBL_VECTORS_FALSE;
            }

            l_read_scratch_reg.extract<4, 1, 7>(l_mc_sync_mode);
            l_read_scratch_reg.extract<12, 20, 0>(l_pll_mux);

            FAPI_DBG("Setting up SYSTEM_IPL_PHASE, RISK_LEVEL, SYS_FORCE_ALL_CORES");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_system_ipl_phase));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SYS_FORCE_ALL_CORES, FAPI_SYSTEM, l_force_all_cores));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_RISK_LEVEL, FAPI_SYSTEM, l_risk_level));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_DISABLE_HBBL_VECTORS, FAPI_SYSTEM, l_disable_hbbl_vectors));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_mc_sync_mode));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CLOCK_PLL_MUX, i_target_chip, l_pll_mux));
        }
        else
        {

            FAPI_DBG("Reading Scratch_reg5");
            //Getting SCRATCH_REGISTER_5 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_5_SCOM,
                                    l_read_scratch_reg));

            FAPI_DBG("Reading control flag attributes");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYSTEM, l_system_ipl_phase));
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYS_FORCE_ALL_CORES, FAPI_SYSTEM, l_force_all_cores));
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RISK_LEVEL, FAPI_SYSTEM, l_risk_level));
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_DISABLE_HBBL_VECTORS, FAPI_SYSTEM, l_disable_hbbl_vectors));
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_mc_sync_mode));

            // set cache contained flag
            if (l_system_ipl_phase == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED)
            {
                l_read_scratch_reg.setBit<ATTR_CC_IPL_BIT>();
            }
            else
            {
                l_read_scratch_reg.clearBit<ATTR_CC_IPL_BIT>();
            }

            // set all cores flag
            if (l_force_all_cores)
            {
                l_read_scratch_reg.setBit<ATTR_INIT_ALL_CORES_BIT>();
            }
            else
            {
                l_read_scratch_reg.clearBit<ATTR_INIT_ALL_CORES_BIT>();
            }

            // set risk level flag
            if (l_risk_level == fapi2::ENUM_ATTR_RISK_LEVEL_TRUE)
            {
                l_read_scratch_reg.setBit<ATTR_RISK_LEVEL_BIT>();
            }
            else
            {
                l_read_scratch_reg.clearBit<ATTR_RISK_LEVEL_BIT>();
            }

            // set disable of HBBL exception vector flag
            if (l_disable_hbbl_vectors == fapi2::ENUM_ATTR_DISABLE_HBBL_VECTORS_TRUE)
            {
                l_read_scratch_reg.setBit<ATTR_DISABLE_HBBL_VECTORS_BIT>();
            }
            else
            {
                l_read_scratch_reg.clearBit<ATTR_DISABLE_HBBL_VECTORS_BIT>();
            }

            // set MC sync mode
            if (l_mc_sync_mode)
            {
                l_read_scratch_reg.setBit<ATTR_MC_SYNC_MODE_BIT>();
            }
            else
            {
                l_read_scratch_reg.clearBit<ATTR_MC_SYNC_MODE_BIT>();
            }

            FAPI_DBG("Reading PLL mux attributes");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_PLL_MUX, i_target_chip, l_pll_mux));
            // set PLL MUX bits
            l_read_scratch_reg.insert<ATTR_PLL_MUX_STARTBIT, ATTR_PLL_MUX_LENGTH, 0>(l_pll_mux);

            FAPI_DBG("Setting up value of Scratch_reg5");
            //Setting SCRATCH_REGISTER_5 register value
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_SCRATCH_REGISTER_5_SCOM,
                                    l_read_scratch_reg));

            l_read_scratch8.setBit<4>();
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
        else
        {
            FAPI_DBG("Reading Scratch_reg6");
            //Getting SCRATCH_REGISTER_6 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_6_SCOM,
                                    l_read_scratch_reg));

            FAPI_DBG("Reading attribute for Hostboot slave bit");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_SBE_MASTER_CHIP, i_target_chip,
                                   l_read_1));

            if ( l_read_1 )
            {
                l_read_scratch_reg.clearBit<24>();
            }
            else
            {
                l_read_scratch_reg.setBit<24>();
            }

            FAPI_DBG("Reading ATTR_PROC_FABRIC_GROUP and CHIP_ID");
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID, i_target_chip,
                                   l_read_1));
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_CHIP_ID, i_target_chip,
                                   l_read_2));

            l_read_scratch_reg.insertFromRight< ATTR_PROC_FABRIC_GROUP_ID_STARTBIT, ATTR_PROC_FABRIC_GROUP_ID_LENGTH >(l_read_1);
            l_read_scratch_reg.insertFromRight< ATTR_PROC_FABRIC_CHIP_ID_STARTBIT, ATTR_PROC_FABRIC_CHIP_ID_LENGTH >(l_read_2);

            FAPI_DBG("Setting up value of Scratch_reg6");
            //Setting SCRATCH_REGISTER_6 register value
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_SCRATCH_REGISTER_6_SCOM,
                                    l_read_scratch_reg));

            l_read_scratch8.setBit<5>();
        }
    }

    FAPI_DBG("Setting Scratch8 for validity of Scratch register");
    //Setting SCRATCH_REGISTER_8 register value
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_SCRATCH_REGISTER_8_SCOM,
                            l_read_scratch8));


    FAPI_INF("p9_sbe_attr_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
