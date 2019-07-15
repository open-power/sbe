/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/target.C $                                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2020                        */
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


#include <fapi2.H>
#include <assert.h>
#include <fapi2_target.H>
#include <plat_target_utils.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>

#ifndef __SBEFW_SEEPROM__
// Global Vector containing ALL targets.  This structure is referenced by
// fapi2::getChildren to produce the resultant returned vector from that
// call.
std::vector<fapi2::plat_target_handle_t> G_vec_targets;

//Defined a Global variable to store HRMOR in PIBMEM, Alias for HRMOR Attr
uint64_t g_HRMOR = 0;

// Global variable for fixed section in pibmem
G_sbe_attrs_t G_sbe_attrs;

fapi2attr::SystemAttributes_t*    G_system_attributes_ptr;
fapi2attr::ProcChipAttributes_t*  G_proc_chip_attributes_ptr;
fapi2attr::PervAttributes_t*      G_perv_attributes_ptr;
fapi2attr::CoreAttributes_t*      G_core_attributes_ptr;
fapi2attr::EQAttributes_t*        G_eq_attributes_ptr;
fapi2attr::EXAttributes_t*        G_ex_attributes_ptr;
fapi2attr::OCMBChipAttributes_t*  G_ocmb_chip_attributes_ptr;

#else // __SBEFW_SEEPROM__
extern uint64_t g_HRMOR;

extern std::vector<fapi2::plat_target_handle_t> G_vec_targets;

// Global variable for fixed section in pibmem
extern G_sbe_attrs_t G_sbe_attrs;

extern fapi2attr::SystemAttributes_t*    G_system_attributes_ptr;
extern fapi2attr::ProcChipAttributes_t*  G_proc_chip_attributes_ptr;
extern fapi2attr::PervAttributes_t*      G_perv_attributes_ptr;
extern fapi2attr::CoreAttributes_t*      G_core_attributes_ptr;
extern fapi2attr::EQAttributes_t*        G_eq_attributes_ptr;
extern fapi2attr::EXAttributes_t*        G_ex_attributes_ptr;
extern fapi2attr::OCMBChipAttributes_t* G_ocmb_chip_attributes_ptr;

// For PhyP system, HRMOR is set to 128MB, which is multiple of 64MB Granule * 2
// For OPAL system, HRMOR needs to be lower than 4GB, so that HB reloading
// doesn't stamp on the OPAL/HostLinux Data .Its made lower than 4GB instead of 
// 4GB inorder to make it fit it into the scratch register. 
// 64MB Granule * 62 = 4096MB, 62 is the multipler
#define HRMOR_FOR_SPLESS_MODE 0xF8000000ull //3968 * 1024 * 1024

#endif // else __SBEFW_SEEPROM__

namespace fapi2
{
#ifdef __SBEFW_SEEPROM__
extern fapi2::ReturnCode
    plat_TargetPresent( fapi2::Target<fapi2::TARGET_TYPE_PERV> & i_chiplet_target,
                                    bool & b_present);

    ReturnCode plat_AttrInit()
    {
        union deviceId l_deviceId;
        uint64_t l_temp_hrmor = 0;
        uint8_t fusedMode = 0;
        fapi2::buffer<uint64_t> l_tempReg = 0;
        fapi2::buffer<uint64_t> l_scratch8Reg = 0;
        fapi2::buffer<uint8_t> l_read1 = 0;
        fapi2::buffer<uint8_t> l_read2 = 0;
        fapi2::buffer<uint8_t> l_read3 = 0;
        fapi2::buffer<uint16_t> l_read4 = 0;
        fapi2::buffer<uint32_t> l_read5 = 0;
        fapi2::buffer<uint64_t> l_deviceIdReg = 0;
        uint8_t l_riskLvl  = 0;
        bool l_isSlave = false;
        uint8_t l_smfConfig = 0;
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chipTarget =
            plat_getChipTarget();
        const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
        const size_t SCRATCH_PROC_CHIP_MEM_TO_USE_VALID_BIT = 0;
        const size_t SCRATCH_PROC_CHIP_MEM_TO_USE_GROUP_ID_STARTBIT = 1;
        const size_t SCRATCH_PROC_CHIP_MEM_TO_USE_CHIP_ID_STARTBIT = 4;
        const size_t ATTR_PROC_MEM_TO_USE_GROUP_ID_STARTBIT = 0;
        const size_t ATTR_PROC_MEM_TO_USE_CHIP_ID_STARTBIT = 3;
        const size_t ATTR_PROC_FABRIC_GROUP_ID_LENGTH   = 3;
        const size_t ATTR_PROC_FABRIC_CHIP_ID_LENGTH    = 3;
        uint8_t l_proc_chip_mem_to_use_valid = 0;
        uint8_t l_proc_chip_mem_to_use_group_id = 0;
        uint8_t l_proc_chip_mem_to_use_chip_id = 0;
        bool l_proc_chip_mem_to_use_set = false;
        fapi2::buffer<uint8_t> l_proc_chip_mem_to_use_attr = 0;
        uint8_t isSpMode = 0;
        bool isHostOpal = true;

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RISK_LEVEL, FAPI_SYSTEM, l_riskLvl));
        //Getting SCRATCH_REGISTER_8 register value
        FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_8_SCOM,
                                l_scratch8Reg));

        //Getting CBS_CS register value
        FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_CBS_CS_SCOM,
                                l_tempReg));

        l_read1.writeBit<7>(l_tempReg.getBit<4>());

        FAPI_DBG("Setting ATTR_SECURITY_ENABLE with the SAB state");
        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_SECURITY_ENABLE, FAPI_SYSTEM, l_read1));

        if ( l_scratch8Reg.getBit<0>() )
        {
            FAPI_DBG("Reading Scratch_reg1");
            //Getting SCRATCH_REGISTER_1 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_1_SCOM,
                                    l_tempReg));

            l_tempReg.extract<0, 6>(l_read1);
            l_tempReg.extract<8, 24>(l_read5);

            FAPI_DBG("Setting up ATTR_EQ_GARD, ATTR_EC_GARD");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_EQ_GARD, l_chipTarget, l_read1));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_EC_GARD, l_chipTarget, l_read5));

            l_read1 = 0;
            l_read5 = 0;
        }
        if ( l_scratch8Reg.getBit<1>() )
        {
            uint8_t l_ob0PllBucket = 0;
            uint8_t l_ob1PllBucket = 0;
            uint8_t l_ob2PllBucket = 0;
            uint8_t l_ob3PllBucket = 0;
            uint8_t l_ndlMeshctrlSetup = 0x0;

            FAPI_DBG("Reading Scratch_reg2");
            //Getting SCRATCH_REGISTER_2 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_2_SCOM,
                                    l_tempReg));

            l_tempReg.extractToRight<0, 16>(l_read4);
            l_tempReg.extractToRight<24, 2>(l_ob0PllBucket);
            l_tempReg.extractToRight<26, 2>(l_ob1PllBucket);
            l_tempReg.extractToRight<28, 2>(l_ob2PllBucket);
            l_tempReg.extractToRight<30, 2>(l_ob3PllBucket);
            l_tempReg.extractToRight<21, 3>(l_read1);

            // Workaround to handle backward compatibilty
            // Old drivers will keep MBX OBUS PLL bucket value as zero. So
            // change it to 1 to make old drivers compatible with new SBE
            // image
            if( 0 == l_ob0PllBucket )
            {
                l_ob0PllBucket = 1;
            }

            if( 0 == l_ob1PllBucket )
            {
                l_ob1PllBucket = 1;
            }

            if( 0 == l_ob2PllBucket )
            {
                l_ob2PllBucket = 1;
            }

            if( 0 == l_ob3PllBucket )
            {
                l_ob3PllBucket = 1;
            }

            FAPI_DBG("Setting up ATTR_I2C_BUS_DIV_REF");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_I2C_BUS_DIV_REF, l_chipTarget, l_read4));

            l_tempReg.extractToRight<16, 4>(l_ndlMeshctrlSetup);
            l_ndlMeshctrlSetup = (~l_ndlMeshctrlSetup) & 0x0F;
            FAPI_DBG("Setting up ATTR_NDL_MESHCTRL_SETUP");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NDL_MESHCTRL_SETUP, l_chipTarget, l_ndlMeshctrlSetup));

            FAPI_DBG("Setting up ATTR_MC_PLL_BUCKET");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_MC_PLL_BUCKET, FAPI_SYSTEM, l_read1));

            FAPI_DBG("Setting up ATTR_OBX_PLL_BUCKET");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OB0_PLL_BUCKET, l_chipTarget, l_ob0PllBucket));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OB1_PLL_BUCKET, l_chipTarget, l_ob1PllBucket));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OB2_PLL_BUCKET, l_chipTarget, l_ob2PllBucket));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OB3_PLL_BUCKET, l_chipTarget, l_ob3PllBucket));
        }

        if ( l_scratch8Reg.getBit<2>() )
        {
            uint8_t isMpIpl = 0;
            FAPI_DBG("Reading Scratch_reg3");
            //Getting SCRATCH_REGISTER_3 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_3_SCOM,
                                    l_tempReg));

            l_tempReg.extractToRight<2, 1>(isMpIpl);

            FAPI_DBG("Setting up ATTR_IS_MPIPL");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_IS_MPIPL, FAPI_SYSTEM, isMpIpl));

            l_tempReg.extractToRight<3, 1>(isSpMode);
            l_tempReg.extractToRight<15, 1>(isHostOpal);
            FAPI_INF("isSpMode=%d and isHostOpal=%d ",isSpMode,isHostOpal);
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_IS_SP_MODE,l_chipTarget, isSpMode));
            l_tempReg.extractToRight<28, 4>(l_riskLvl);
        }

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_SP_MODE,l_chipTarget, isSpMode));
        //If its BMC based system force the HRMOR to be 0xF8000000ull for both
        //OPAL and PHYP payload
        if( (isSpMode == false) && !SBE::isSimicsRunning())
        {
            FAPI_INF("FSP Less system HRMOR set as=0x%.8x",HRMOR_FOR_SPLESS_MODE);
            uint64_t hrmor = HRMOR_FOR_SPLESS_MODE;
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET,
                        FAPI_SYSTEM, hrmor));
        }
        //If FSP based system and payload is of the type OPAL then set the HRMOR
        //as 0xF8000000ull
        if(( isSpMode == true) && (!SBE::isSimicsRunning()))
        {
            if( isHostOpal == true)
            {
               FAPI_INF("OPAL based system HRMOR set as=0x%.8x",HRMOR_FOR_SPLESS_MODE);
               uint64_t tempHRMOR = HRMOR_FOR_SPLESS_MODE;
               FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET,
                        FAPI_SYSTEM,tempHRMOR ));
            }
        }

        if ( l_scratch8Reg.getBit<3>() )
        {
            uint8_t l_cpFilterBypass = 0;
            uint8_t l_ssFilterBypass = 0;
            uint8_t l_ioFilterBypass = 0;
            uint8_t l_dpllBypass = 0;
            uint8_t l_nestMemXOPciBypass = 0;
            uint8_t l_attrObusRatio = 0;

            FAPI_DBG("Reading Scratch_Reg4");
            //Getting SCRATCH_REGISTER_4 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_4_SCOM,
                                    l_tempReg));

            l_tempReg.extractToRight<0, 16>(l_read4);
            l_tempReg.extractToRight<16, 1>(l_cpFilterBypass);
            l_tempReg.extractToRight<17, 1>(l_ssFilterBypass);
            l_tempReg.extractToRight<18, 1>(l_ioFilterBypass);
            l_tempReg.extractToRight<19, 1>(l_dpllBypass);
            l_tempReg.extractToRight<20, 1>(l_nestMemXOPciBypass);
            l_tempReg.extractToRight<21, 1>(l_attrObusRatio);
            l_tempReg.extractToRight<24, 8>(l_read1);

            FAPI_DBG("Setting up PLL bypass attributes");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_CP_FILTER_BYPASS, l_chipTarget, l_cpFilterBypass));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_SS_FILTER_BYPASS, l_chipTarget, l_ssFilterBypass));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_IO_FILTER_BYPASS, l_chipTarget, l_ioFilterBypass));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_DPLL_BYPASS, l_chipTarget, l_dpllBypass));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_NEST_MEM_X_O_PCI_BYPASS, l_chipTarget, l_nestMemXOPciBypass));
            FAPI_DBG("Setting up ATTR_BOOT_FREQ_MULT, ATTR_NEST_PLL_BUCKET");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_BOOT_FREQ_MULT, l_chipTarget, l_read4));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_NEST_PLL_BUCKET, FAPI_SYSTEM, l_read1));

            FAPI_DBG("Setting up ATTR_OBUS_RATIO_VALUE");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OBUS_RATIO_VALUE, l_chipTarget, l_attrObusRatio));

            l_read1 = 0;
            l_read4 = 0;
        }

        if ( l_scratch8Reg.getBit<4>() )
        {
            uint8_t l_forceAllCores = 0;
            uint8_t l_disableHbblVectors = 0;
            uint32_t l_pllMux = 0;
            uint8_t l_mcSyncMode = 0;
            uint8_t l_slowPciRefClock = 0;

            FAPI_DBG("Reading Scratch_reg5");
            //Getting SCRATCH_REGISTER_5 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_5_SCOM,
                                    l_tempReg));

            l_tempReg.extract<1, 1, 7>(l_forceAllCores);

            if (l_tempReg.getBit<2>())
            {
                l_riskLvl = 1;
            }

            if (l_tempReg.getBit<3>())
            {
                l_disableHbblVectors = fapi2::ENUM_ATTR_DISABLE_HBBL_VECTORS_TRUE;
            }
            else
            {
                l_disableHbblVectors = fapi2::ENUM_ATTR_DISABLE_HBBL_VECTORS_FALSE;
            }

            l_tempReg.extract<12, 20, 0>(l_pllMux);

            l_tempReg.extract<4, 1, 7>(l_mcSyncMode);

            if (l_tempReg.getBit<5>())
            {
                l_slowPciRefClock = fapi2::ENUM_ATTR_DD1_SLOW_PCI_REF_CLOCK_NORMAL;
            }
            else
            {
                l_slowPciRefClock = fapi2::ENUM_ATTR_DD1_SLOW_PCI_REF_CLOCK_SLOW;
            }

            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_SYS_FORCE_ALL_CORES, FAPI_SYSTEM, l_forceAllCores));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_DISABLE_HBBL_VECTORS, FAPI_SYSTEM, l_disableHbblVectors));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_MC_SYNC_MODE, l_chipTarget, l_mcSyncMode));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_DD1_SLOW_PCI_REF_CLOCK, FAPI_SYSTEM, l_slowPciRefClock));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_CLOCK_PLL_MUX, l_chipTarget, l_pllMux));
        }

        if ( l_scratch8Reg.getBit<5>() )
        {
            uint8_t l_pumpMode = fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_NODE;

            FAPI_DBG("Reading Scratch_reg6");
            //Getting SCRATCH_REGISTER_6 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_6_SCOM,
                                    l_tempReg));

            l_tempReg.extractToRight<SCRATCH_PROC_CHIP_MEM_TO_USE_VALID_BIT,
                              1>(l_proc_chip_mem_to_use_valid);
            if (l_proc_chip_mem_to_use_valid)
            {
                l_proc_chip_mem_to_use_group_id = 0;
                l_proc_chip_mem_to_use_chip_id = 0;
                l_tempReg.extractToRight<SCRATCH_PROC_CHIP_MEM_TO_USE_GROUP_ID_STARTBIT,
                                  ATTR_PROC_FABRIC_GROUP_ID_LENGTH>(l_proc_chip_mem_to_use_group_id);
                l_tempReg.extractToRight<SCRATCH_PROC_CHIP_MEM_TO_USE_CHIP_ID_STARTBIT,
                                  ATTR_PROC_FABRIC_CHIP_ID_LENGTH>(l_proc_chip_mem_to_use_chip_id);
                l_proc_chip_mem_to_use_set = true;
            }

            l_read1 = 0;
            l_isSlave = l_tempReg.getBit<24>();

            if ( !l_isSlave )  // 0b0 == master
            {
                FAPI_DBG("Reading DEVICE_ID_REG value");
                FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_DEVICE_ID_REG, l_deviceIdReg));

                if (!l_deviceIdReg.getBit<40>())
                {
                    l_read1.setBit<7>();
                }
            }

            if (l_tempReg.getBit<23>())
            {
                l_pumpMode = fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_GROUP;
            }
            else
            {
                l_pumpMode = fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_NODE;
            }

            l_tempReg.extractToRight<26, 3>(l_read2);
            l_tempReg.extractToRight<29, 3>(l_read3);

            l_smfConfig = l_tempReg.getBit<16>();
            FAPI_DBG("Setting up SMF CONFIG");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_SMF_CONFIG,
                                    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                                    l_smfConfig));

            FAPI_DBG("Setting up PUMP MODE");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_FABRIC_PUMP_MODE,
                                    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                                    l_pumpMode));

            FAPI_DBG("Setting up MASTER_CHIP, FABRIC_GROUP_ID and CHIP_ID");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_SBE_MASTER_CHIP, l_chipTarget,
                                   l_read1));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_FABRIC_GROUP_ID, l_chipTarget,
                                   l_read2));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_FABRIC_CHIP_ID, l_chipTarget,
                                   l_read3));

            l_tempReg.extractToRight<17, 3>(l_read2);
            l_tempReg.extractToRight<20, 3>(l_read3);

            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_EFF_FABRIC_GROUP_ID, l_chipTarget,
                                   l_read2));
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_EFF_FABRIC_CHIP_ID, l_chipTarget,
                                   l_read3));
        }
        if (!l_proc_chip_mem_to_use_set)
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID,
                                   l_chipTarget,
                                   l_proc_chip_mem_to_use_group_id));
        }

        l_proc_chip_mem_to_use_attr.insertFromRight
                                    <ATTR_PROC_MEM_TO_USE_GROUP_ID_STARTBIT,
                                     ATTR_PROC_FABRIC_GROUP_ID_LENGTH>
                                    (l_proc_chip_mem_to_use_group_id);
        l_proc_chip_mem_to_use_attr.insertFromRight
                                    <ATTR_PROC_MEM_TO_USE_CHIP_ID_STARTBIT,
                                     ATTR_PROC_FABRIC_CHIP_ID_LENGTH>
                                    (l_proc_chip_mem_to_use_chip_id);

        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_MEM_TO_USE,
                                l_chipTarget,
                                l_proc_chip_mem_to_use_attr));


        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_RISK_LEVEL, FAPI_SYSTEM,
                               l_riskLvl));
        FAPI_TRY(getscom_abs(PERV_DEVICE_ID_REG, &l_deviceId.iv_deviceIdReg));
        fusedMode = (uint8_t)l_deviceId.iv_fusedMode;
        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_FUSED_CORE_MODE,
                                fapi2::Target<TARGET_TYPE_SYSTEM>(),
                                fusedMode));

        { // scope initializer to resolve compile issues

        // if bit 17 of PERV_SB_CS_SCOM is set, set attribute
        // which suggest backup seeprom is selected for boot.
        FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SB_CS_SCOM,
                                l_tempReg));
        fapi2::buffer<uint8_t> attrSeepromSlct = 0;

        if( l_tempReg.getBit(PERV_SB_CS_SELECT_SECONDARY_SEEPROM))
        {
            attrSeepromSlct.setBit<7>();
        }

        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_BACKUP_SEEPROM_SELECT,
                    l_chipTarget, attrSeepromSlct));
        } // end of scope initializer

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET, FAPI_SYSTEM, l_temp_hrmor));
        g_HRMOR = l_temp_hrmor;

fapi_try_exit:
        return fapi2::current_err;
    }
#endif //__SBEFW_SEEPROM__

#ifndef __SBEFW_SEEPROM__

    // Get the plat target handle by chiplet number - For PERV targets
    template<>
    plat_target_handle_t plat_getTargetHandleByChipletNumber<TARGET_TYPE_PERV>(
            const uint8_t i_chipletNumber)
    {
        uint32_t l_idx = 0;

        if((i_chipletNumber > 0) &&
           (i_chipletNumber < (EQ_CHIPLET_OFFSET + EQ_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - NEST_GROUP1_CHIPLET_OFFSET) +
                NEST_GROUP1_TARGET_OFFSET;
        }
        else if((i_chipletNumber >= CORE_CHIPLET_OFFSET) &&
                (i_chipletNumber < (CORE_CHIPLET_OFFSET + CORE_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - CORE_CHIPLET_OFFSET) +
                CORE_TARGET_OFFSET;
        }
        else
        {
            assert(false);
        }
        return G_vec_targets[l_idx];
    }

    // Get the plat target handle by chiplet number - For EQ targets
    template<>
    plat_target_handle_t plat_getTargetHandleByChipletNumber<TARGET_TYPE_EQ>(
            const uint8_t i_chipletNumber)
    {
        assert(((i_chipletNumber >= EQ_CHIPLET_OFFSET) &&
                (i_chipletNumber < (EQ_CHIPLET_OFFSET + EQ_TARGET_COUNT))))

        uint32_t l_idx = (i_chipletNumber - EQ_CHIPLET_OFFSET) +
                EQ_TARGET_OFFSET;
        return G_vec_targets[l_idx];
    }

    // Get the plat target handle by chiplet number - For CORE targets
    template<>
    plat_target_handle_t plat_getTargetHandleByChipletNumber<TARGET_TYPE_CORE>(
            const uint8_t i_chipletNumber)
    {
        assert(((i_chipletNumber >= CORE_CHIPLET_OFFSET) &&
                (i_chipletNumber < (CORE_CHIPLET_OFFSET + CORE_TARGET_COUNT))));

        uint32_t l_idx = (i_chipletNumber - CORE_CHIPLET_OFFSET) +
                 CORE_TARGET_OFFSET;

        return G_vec_targets[l_idx];
    }

    // Get the plat target handle by chiplet number - For EX targets
    template<>
    plat_target_handle_t plat_getTargetHandleByChipletNumber<TARGET_TYPE_EX>(
            const uint8_t i_chipletNumber)
    {
        assert(((i_chipletNumber >= CORE_CHIPLET_OFFSET) &&
                (i_chipletNumber < (CORE_CHIPLET_OFFSET + CORE_TARGET_COUNT))));

        uint32_t l_idx = ((i_chipletNumber - CORE_CHIPLET_OFFSET) / 2) +
                 EX_TARGET_OFFSET;

        return G_vec_targets[l_idx];
    }

    // Get plat target handle by instance number - For EX targets
    template <>
    plat_target_handle_t plat_getTargetHandleByInstance<TARGET_TYPE_EX>(
            const uint8_t i_targetNum)
    {
        assert(i_targetNum < EX_TARGET_COUNT);

        return G_vec_targets[i_targetNum + EX_TARGET_OFFSET];
    }


    TargetType plat_target_handle_t::getFapiTargetType() const
    {
        TargetType l_targetType = TARGET_TYPE_NONE;
        switch(fields.type)
        {
            case PPE_TARGET_TYPE_PROC_CHIP:
                l_targetType = TARGET_TYPE_PROC_CHIP;
                break;
            case PPE_TARGET_TYPE_MCS:
                l_targetType = TARGET_TYPE_MCS;
                break;
            case PPE_TARGET_TYPE_MI:
                l_targetType = TARGET_TYPE_MI;
                break;
            case PPE_TARGET_TYPE_PHB:
                l_targetType = TARGET_TYPE_PHB;
                break;
            case PPE_TARGET_TYPE_CORE | PPE_TARGET_TYPE_PERV:
                l_targetType = TARGET_TYPE_CORE;
                break;
            case PPE_TARGET_TYPE_EQ | PPE_TARGET_TYPE_PERV:
                l_targetType = TARGET_TYPE_EQ;
                break;
            case PPE_TARGET_TYPE_EX:
                l_targetType = TARGET_TYPE_EX;
                break;
            case PPE_TARGET_TYPE_PERV:
                l_targetType = TARGET_TYPE_PERV;
                break;
            case PPE_TARGET_TYPE_SYSTEM:
                l_targetType = TARGET_TYPE_SYSTEM;
                break;
            case PPE_TARGET_TYPE_MCBIST | PPE_TARGET_TYPE_PERV:
                l_targetType = TARGET_TYPE_MCBIST;
                break;
            case PPE_TARGET_TYPE_MC | PPE_TARGET_TYPE_PERV:
                l_targetType = TARGET_TYPE_MC;
                break;
            case PPE_TARGET_TYPE_NONE:
            case PPE_TARGET_TYPE_ALL:
            default:
                assert(false);
                break;
        }
        return l_targetType;
    }

    plat_target_handle_t plat_target_handle_t::getParent(
            const TargetType i_parentType) const
    {
        plat_target_handle_t l_handle;
        switch(i_parentType)
        {
            case TARGET_TYPE_PROC_CHIP:
                l_handle = G_vec_targets[CHIP_TARGET_OFFSET];
                break;
            case TARGET_TYPE_PERV:
                assert(fields.type & PPE_TARGET_TYPE_PERV);
                l_handle = *this;
                break;
            case TARGET_TYPE_EX:
                assert(fields.type & PPE_TARGET_TYPE_CORE);
                l_handle = G_vec_targets
                    [(fields.type_target_num / EX_PER_QUAD) + EX_TARGET_OFFSET];
                break;
            case TARGET_TYPE_EQ:
                assert(fields.type &
                       (PPE_TARGET_TYPE_EX | PPE_TARGET_TYPE_CORE));
                {
                    uint32_t l_perQuad = (fields.type & PPE_TARGET_TYPE_EX) ?
                                         EX_PER_QUAD : CORES_PER_QUAD;
                    l_handle = G_vec_targets
                        [(fields.type_target_num / l_perQuad) +
                         EQ_TARGET_OFFSET];
                }
                break;
            default:
                assert(false);
        }
        return l_handle;
    }

    void plat_target_handle_t::getChildren(const TargetType i_parentType,
                                           const TargetType i_childType,
                                           const plat_target_type_t i_platType,
                                           const TargetState i_state,
                                           std::vector<plat_target_handle>
                                           &o_children) const
    {
        uint32_t l_childPerChiplet = 0;
        uint32_t l_childTargetOffset = 0;
        uint32_t l_loopCount = G_vec_targets.size();
        TargetType l_targetType = i_parentType;

        if((i_parentType & ~(TARGET_TYPE_PROC_CHIP)) != 0)
        {
            // For composite targets, if multicast, treat as PROC_CHIP, else
            // treat as other target
            if(this->fields.is_multicast)
            {
                l_targetType = TARGET_TYPE_PROC_CHIP;
            }
            else
            {
                l_targetType =
                    static_cast<TargetType>(l_targetType & ~(TARGET_TYPE_PROC_CHIP));
            }
        }

        // EQ ==> EX
        if((l_targetType == TARGET_TYPE_EQ) && (i_childType == TARGET_TYPE_EX))
        {
            l_childPerChiplet = EX_PER_QUAD;
            l_childTargetOffset = EX_TARGET_OFFSET;
            l_loopCount = l_childPerChiplet;
        }

        // EQ ==> EC
        if((l_targetType == TARGET_TYPE_EQ) && (i_childType == TARGET_TYPE_CORE))
        {
            l_childPerChiplet = CORES_PER_QUAD;
            l_childTargetOffset = CORE_TARGET_OFFSET;
            l_loopCount = l_childPerChiplet;
        }

        // EX ==> EC
        if((l_targetType == TARGET_TYPE_EX) && (i_childType == TARGET_TYPE_CORE))
        {
            l_childPerChiplet = CORES_PER_EX;
            l_childTargetOffset = CORE_TARGET_OFFSET;
            l_loopCount = l_childPerChiplet;
        }
        // else it is TARGET_TYPE_PROC_CHIP ==> anything, and we iterate over
        // all the targets

        for(uint32_t i = 0; i < l_loopCount; ++i)
        {
            plat_target_handle_t l_temp =
                G_vec_targets.at((this->fields.type_target_num *
                            l_childPerChiplet) + l_childTargetOffset + i);
            if ((l_temp.fields.type & i_platType) == i_platType)
            {
                switch (i_state)
                {
                    case TARGET_STATE_PRESENT:
                        if (l_temp.fields.present)
                        {
                            o_children.push_back(l_temp);
                        }
                        break;
                    case TARGET_STATE_FUNCTIONAL:
                        if (l_temp.fields.functional)
                        {
                            o_children.push_back(l_temp);
                        }
                        break;
                    default:
                        assert(false);
                }
            }
        }
    }

    void plat_target_handle_t::getChildren(const TargetFilter i_filter,
                                           const TargetState i_state,
                                           std::vector<plat_target_handle_t>
                                           &o_children) const
    {
        static const uint64_t mask = 1;

        // Walk the bits in the input target filter. For every bit, at
        // position x, that is set, x can be used as an index into our global
        // target vector (indexed by chiplet number)
        for (uint32_t l_idx = 0;
                l_idx < sizeof(TargetFilter) * 8;
                ++l_idx)
        {
            if (i_filter & (mask << (((sizeof(TargetFilter)*8)-1) - l_idx)))
            {
                plat_target_handle_t l_targetHandle = G_vec_targets.at(l_idx + NEST_GROUP1_CHIPLET_OFFSET);

                if(l_targetHandle.fields.type & PPE_TARGET_TYPE_PERV) // Can be an assertion?
                {
                    switch (i_state)
                    {
                        case TARGET_STATE_PRESENT:
                            if(l_targetHandle.fields.present)
                            {
                                o_children.push_back(l_targetHandle);
                            }
                            break;
                         case TARGET_STATE_FUNCTIONAL:
                            if(l_targetHandle.fields.functional)
                            {
                                o_children.push_back(l_targetHandle);
                            }
                            break;
                        default:
                            break;
                     }
                 }
             }
         }
    }

    #ifndef __noRC__
    ReturnCode current_err;
    #endif

     fapi2::ReturnCode plat_PervPGTargets(const fapi2::Target<fapi2::TARGET_TYPE_PERV> & i_target,
                                          bool & o_present)
     {
        o_present = false;
        uint16_t attr_value = 0;
        FAPI_ATTR_GET(fapi2::ATTR_PG,
                i_target,
                attr_value);
        FAPI_DBG("Target: 0x%08X, ATTR_PG value = %x", static_cast<uint32_t>(i_target.get().value), attr_value);
        if (0 == (attr_value & 0x1000))
        {
            o_present = true;
        }
        return fapi2::FAPI2_RC_SUCCESS;
    }

    /// @brief Function to determine if pervsaive target within a chip is
    ///     present and, thus, considered functional per PG attributes
    fapi2::ReturnCode
    plat_TargetPresent( fapi2::Target<fapi2::TARGET_TYPE_PERV> & i_chiplet_target,
                        bool & b_present)
    {

        uint8_t l_chipName = fapi2::ENUM_ATTR_NAME_NONE;

        FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_NAME, plat_getChipTarget(), l_chipName));

        // TODO via RTC 164026
        // In nimbus all pervasive chiplets (non quad, non core), are present
        // other than OBUS1 and OBUS2. In cumulus all chiplets are present.
        // Though on field parts, all chiplets which are present should be
        // functional. But in lab when we can get partial good parts, its
        // possible that few chiplets are not functional. So we need to
        // differentiate between present versus functional chiplets.
        // We need to see if we need to use same strategy for cores/caches as
        // well.
        // Also in current code we are hard coding the chiplets. We need to use
        // attribute to differentiate between nimbus versus cumulus config.
        static const size_t OBUS1 = 10;
        static const size_t OBUS2 = 11;
        if((OBUS1 == i_chiplet_target.getChipletNumber()) ||
           (OBUS2 == i_chiplet_target.getChipletNumber()))
        {
            if((fapi2::ENUM_ATTR_NAME_CUMULUS == l_chipName) ||
               (fapi2::ENUM_ATTR_NAME_AXONE == l_chipName))
            {
                static_cast<plat_target_handle_t&>((i_chiplet_target.operator()())).setPresent();
            }
        }
        else if( i_chiplet_target.getChipletNumber() < EQ_CHIPLET_OFFSET )
        {
            static_cast<plat_target_handle_t&>((i_chiplet_target.operator()())).setPresent();
        }
        // Find the PERV target number in the partial good initialization
        // array

        FAPI_TRY(plat_PervPGTargets(i_chiplet_target, b_present));

        if (b_present)
        {
            if( i_chiplet_target.getChipletNumber() >=  EQ_CHIPLET_OFFSET )
            {
                static_cast<plat_target_handle_t&>((i_chiplet_target.operator()())).setPresent();
            }
            static_cast<plat_target_handle_t&>((i_chiplet_target.operator()())).setFunctional(true);
        }
        else
        {
            FAPI_DBG("Perv target NOT present (nor functional): chiplet_number = %d", i_chiplet_target.getChipletNumber());
        }

        FAPI_DBG("Target present = %u, Target functional = %u",
            static_cast<plat_target_handle_t>(i_chiplet_target.get()).getPresent(),
            static_cast<plat_target_handle_t>(i_chiplet_target.get()).getFunctional());

fapi_try_exit:
        return fapi2::current_err;
    }

#endif // not __SBEFW_SEEPROM__

#ifdef __SBEFW_SEEPROM__

    /// @brief Function to initialize the G_targets vector based on partial good
    ///      attributes ///  this will move to plat_target.H formally
    fapi2::ReturnCode plat_TargetsInit()
    {
        bool b_present = false;
        uint8_t l_chipName = fapi2::ENUM_ATTR_NAME_NONE;
        plat_target_handle_t l_platHandle;


        // Copy fixed section from SEEPROM to PIBMEM
        G_sbe_attrs.G_system_attrs = G_system_attributes;
        G_sbe_attrs.G_proc_chip_attrs = G_proc_chip_attributes;
        G_sbe_attrs.G_perv_attrs = G_perv_attributes;
        G_sbe_attrs.G_core_attrs = G_core_attributes;
        G_sbe_attrs.G_eq_attrs = G_eq_attributes;
        G_sbe_attrs.G_ex_attrs = G_ex_attributes;
        G_sbe_attrs.G_ocmb_chip_attrs = G_ocmb_chip_attributes;

        // Initialise global attribute pointers
        G_system_attributes_ptr = &(G_sbe_attrs.G_system_attrs);
        G_proc_chip_attributes_ptr = &(G_sbe_attrs.G_proc_chip_attrs);
        G_perv_attributes_ptr = &(G_sbe_attrs.G_perv_attrs);
        G_core_attributes_ptr = &(G_sbe_attrs.G_core_attrs);
        G_eq_attributes_ptr = &(G_sbe_attrs.G_eq_attrs);
        G_ex_attributes_ptr = &(G_sbe_attrs.G_ex_attrs);
        G_ocmb_chip_attributes_ptr = &(G_sbe_attrs.G_ocmb_chip_attrs);


        std::vector<fapi2::plat_target_handle_t>::iterator tgt_iter;
        uint32_t l_beginning_offset;

        FAPI_DBG("Platform target initialization.  Target Count = %u", TARGET_COUNT);
        /*
         * Initialize all entries to NULL
         */
        for (uint32_t i = 0; i < TARGET_COUNT; ++i)
        {
            G_vec_targets.push_back((fapi2::plat_target_handle_t)0x0);
        }

        /*
         * Chip Target is the first one
         */
        l_beginning_offset = CHIP_TARGET_OFFSET;

        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> chip_target((createPlatTargetHandle<fapi2::TARGET_TYPE_PROC_CHIP>(0)));
        G_vec_targets.at(l_beginning_offset) = (fapi2::plat_target_handle_t)(chip_target.get());

        // Initialize platform attributes. Needs to be after the chip target is
        // created.
        FAPI_TRY(plat_AttrInit());

        FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_NAME, plat_getChipTarget(), l_chipName));

        /*
         * Nest Targets - group 1
         */
        l_beginning_offset = NEST_GROUP1_TARGET_OFFSET;
        for (uint32_t i = 0; i < NEST_GROUP1_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PERV> target_name((createPlatTargetHandle<fapi2::TARGET_TYPE_PERV>(i)));

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(target_name, b_present));

            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(target_name.get());
        }

        /*
         * Memory Controller Synchronous (MCBIST/MC) Targets
         */

        // Note: MCBIST/MC have the same offset and counts, so the loop just
        // uses MCBIST constants
        l_beginning_offset = MCBIST_TARGET_OFFSET;
        for (uint32_t i = 0; i < MCBIST_TARGET_COUNT; ++i)
        {
            if(fapi2::ENUM_ATTR_NAME_CUMULUS == l_chipName || fapi2::ENUM_ATTR_NAME_AXONE == l_chipName)
            {
                l_platHandle = createPlatTargetHandle<fapi2::TARGET_TYPE_MC>(i);
            }
            else if(fapi2::ENUM_ATTR_NAME_NIMBUS == l_chipName)
            {
                l_platHandle = createPlatTargetHandle<fapi2::TARGET_TYPE_MCBIST>(i);
            }
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv(l_platHandle);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(l_perv, b_present));

            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(l_perv.get());
        }

        /*
         * Nest Targets - group 2
         */
        l_beginning_offset = NEST_GROUP2_TARGET_OFFSET;
        for (uint32_t i = NEST_GROUP2_TARGET_OFFSET;
                i < (NEST_GROUP2_TARGET_OFFSET + NEST_GROUP2_TARGET_COUNT); ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PERV> target_name((createPlatTargetHandle<fapi2::TARGET_TYPE_PERV>(i - 1)));

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(target_name, b_present));

            G_vec_targets.at(i) = (fapi2::plat_target_handle_t)(target_name.get());
        }

        /*
         * Cache (EQ) Targets
         */
        l_beginning_offset = EQ_TARGET_OFFSET;
        for (uint32_t i = 0; i < EQ_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_EQ> target_name((createPlatTargetHandle<fapi2::TARGET_TYPE_EQ>(i)));
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv = target_name.getParent<fapi2::TARGET_TYPE_PERV>();

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(l_perv, b_present));

            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(l_perv.get());
        }

        /*
         * Core (EC) Targets
         */

        l_beginning_offset = CORE_TARGET_OFFSET;
        for (uint32_t i = 0; i < CORE_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_CORE> target_name((createPlatTargetHandle<fapi2::TARGET_TYPE_CORE>(i)));
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv = target_name.getParent<fapi2::TARGET_TYPE_PERV>();

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(l_perv, b_present));

            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(l_perv.get());
        }

        /*
         * EX Targets
         */

        l_beginning_offset = EX_TARGET_OFFSET;
        for (uint32_t i = 0; i < EX_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_EX> target_name((createPlatTargetHandle<fapi2::TARGET_TYPE_EX>(i)));

            fapi2::Target<fapi2::TARGET_TYPE_EQ> l_parent = target_name.getParent<fapi2::TARGET_TYPE_EQ>();

            // Get the parent EQ's ATTR_PG
            uint16_t l_eqAttrPg = 0;
            FAPI_ATTR_GET(fapi2::ATTR_PG, l_parent.getParent<TARGET_TYPE_PERV>(), l_eqAttrPg);

            // Check if this EX's L2 and L3 regions are marked "good"
            if(0 == (i % EX_PER_QUAD))
            {
                // Bits 6 and 8 need to be 0
                l_eqAttrPg &= 0x0280;
            }
            else
            {
                // Bits 7 and 9 need to be 0
                l_eqAttrPg &= 0x0140;
            }

            if(0 == l_eqAttrPg)
            {
                static_cast<plat_target_handle_t&>(target_name.operator ()()).setPresent();
                static_cast<plat_target_handle_t&>(target_name.operator ()()).setFunctional(true);
            }
            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(target_name.get());
        }

        /*
         * MCS/MI Targets
         */

        // Note: MCS/MI have the same offset and counts, so the loop just uses
        // MCS constants
        l_beginning_offset = MCS_TARGET_OFFSET;
        for (uint32_t i = 0; i < MCS_TARGET_COUNT; ++i)
        {
            if(fapi2::ENUM_ATTR_NAME_CUMULUS == l_chipName || fapi2::ENUM_ATTR_NAME_AXONE == l_chipName)
            {
                l_platHandle = createPlatTargetHandle<fapi2::TARGET_TYPE_MI>(i);
            }
            else if(fapi2::ENUM_ATTR_NAME_NIMBUS == l_chipName)
            {
                l_platHandle = createPlatTargetHandle<fapi2::TARGET_TYPE_MCS>(i);
            }

            fapi2::Target<fapi2::TARGET_TYPE_PERV>
                l_nestTarget((plat_getTargetHandleByChipletNumber<TARGET_TYPE_PERV>(N3_CHIPLET - (MCS_PER_MCBIST * (i / MCS_PER_MCBIST)))));

            uint16_t l_attrPg = 0;

            FAPI_ATTR_GET(fapi2::ATTR_PG, l_nestTarget, l_attrPg);

            if(0 == (i / MCS_PER_MCBIST))
            {
                // Bit 10 needs to be 0 for MCS 0, 1
                l_attrPg &= 0x0020;
            }
            else
            {
                // Bit 9 needs to be 0 for MCS 2, 3
                l_attrPg &= 0x0040;
            }

            if(0 == l_attrPg)
            {
                l_platHandle.setPresent();
                l_platHandle.setFunctional(true);
            }

            G_vec_targets.at(l_beginning_offset+i) = l_platHandle;
        }

        /*
         * PHB Targets
         */
        l_beginning_offset = PHB_TARGET_OFFSET;
        for (uint32_t i = 0; i < PHB_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PHB> target_name(createPlatTargetHandle<fapi2::TARGET_TYPE_PHB>(i));

            fapi2::Target<fapi2::TARGET_TYPE_PERV>
                l_pciTarget((plat_getTargetHandleByChipletNumber<TARGET_TYPE_PERV>(target_name.getChipletNumber())));

            constexpr uint16_t l_pciPgArray[] = {0xE1FD, 0xE0FD, 0xE07D};
            uint16_t l_attrPg = 0;

            FAPI_ATTR_GET(fapi2::ATTR_PG, l_pciTarget, l_attrPg);

            if(l_pciPgArray[target_name.getChipletNumber() - PCI0_CHIPLET] == l_attrPg)
            {
                static_cast<plat_target_handle_t&>(target_name.operator ()()).setPresent();
                static_cast<plat_target_handle_t&>(target_name.operator ()()).setFunctional(true);
            }

            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(target_name.get());
        }


fapi_try_exit:
        return fapi2::current_err;
    }

#endif // __SBEFW_SEEPROM__
#ifndef __SBEFW_SEEPROM__

    /// @brief Function to initialize the G_targets vector based on partial good
    ///        attributes
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> plat_getChipTarget()
    {

        // Get the chip specific target
        return ((fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>)G_vec_targets.at(0));
    }

    /// @brief Function to apply any gard records set (via
    //  ATTR_EQ_GARD/ATTR_EC_GARD) to mark corresponding targets non functional
    ReturnCode plat_ApplyGards()
    {
        uint8_t l_eqGards = 0;
        uint32_t l_ecGards = 0;
        static const uint32_t l_mask = 0x80000000;
        bool l_coreGroupNonFunctional = true;
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = plat_getChipTarget();

        // Read the EQ and EC gard attributes from the chip target
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_EQ_GARD, l_chip, l_eqGards));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_EC_GARD, l_chip, l_ecGards));

        FAPI_DBG("ATTR_EQ_GARD:: 0x%08x", l_eqGards);
        FAPI_DBG("ATTR_EC_GARD:: 0x%08x", l_ecGards);

        // Iterate over the bits in EQ and EC gards, if set, mark the
        // corresponding target non-functional
        for(uint32_t l_idx = 0; l_idx < EQ_TARGET_COUNT; ++l_idx)
        {
            if((l_mask >> l_idx) & (((uint32_t)(l_eqGards)) << 24))
            {
                FAPI_DBG("Making %d'th EQ non-functional", l_idx);
                // EQ chiplet l_idx is to be marked non-functional
                fapi2::Target<fapi2::TARGET_TYPE_EQ> l_target = G_vec_targets.at(l_idx + EQ_TARGET_OFFSET);
                static_cast<plat_target_handle_t&>(l_target.operator ()()).setFunctional(false);
                G_vec_targets.at(l_idx + EQ_TARGET_OFFSET) = l_target.get();
            }
        }

        for(uint32_t l_idx = 0; l_idx < CORE_TARGET_COUNT; ++l_idx)
        {
            if((l_mask >> l_idx) & (l_ecGards))
            {
                FAPI_DBG("Making %d'th EC non-functional", l_idx);
                // EC chiplet l_idx is to be marked non-functional
                fapi2::Target<fapi2::TARGET_TYPE_CORE> l_target = G_vec_targets.at(l_idx + CORE_TARGET_OFFSET);
                static_cast<plat_target_handle_t&>(l_target.operator ()()).setFunctional(false);
                G_vec_targets.at(l_idx + CORE_TARGET_OFFSET) = l_target.get();
            }
            else
            {
                l_coreGroupNonFunctional = false;
            }
            if(0 == ((l_idx + 1) % CORES_PER_EX))
            {
                if(true == l_coreGroupNonFunctional)
                {
                    // All cores of this group are non-functional. Mark the EX
                    // non-functional too.
                    G_vec_targets.at((l_idx / CORES_PER_EX) + EX_TARGET_OFFSET).fields.functional = false;
                }
                // Reset ex non-functional flag for the next group
                l_coreGroupNonFunctional = true;
            }
        }
fapi_try_exit:
        return fapi2::current_err;
    }
#endif //not __SBEFW_SEEPROM__
} // fapi2
