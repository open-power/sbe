/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/target.C $                                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2019                        */
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

// Global variable for fixed section in pibmem
G_sbe_attrs_t G_sbe_attrs;

fapi2attr::SystemAttributes_t*    G_system_attributes_ptr;
fapi2attr::ProcChipAttributes_t*  G_proc_chip_attributes_ptr;
fapi2attr::PervAttributes_t*      G_perv_attributes_ptr;
fapi2attr::CoreAttributes_t*      G_core_attributes_ptr;
fapi2attr::EQAttributes_t*        G_eq_attributes_ptr;
fapi2attr::EXAttributes_t*        G_ex_attributes_ptr;

#else // __SBEFW_SEEPROM__
extern std::vector<fapi2::plat_target_handle_t> G_vec_targets;

// Global variable for fixed section in pibmem
extern G_sbe_attrs_t G_sbe_attrs;

extern fapi2attr::SystemAttributes_t*    G_system_attributes_ptr;
extern fapi2attr::ProcChipAttributes_t*  G_proc_chip_attributes_ptr;
extern fapi2attr::PervAttributes_t*      G_perv_attributes_ptr;
extern fapi2attr::CoreAttributes_t*      G_core_attributes_ptr;
extern fapi2attr::EQAttributes_t*        G_eq_attributes_ptr;
extern fapi2attr::EXAttributes_t*        G_ex_attributes_ptr;

// For PhyP system, HRMOR is set to 128MB, which is multiple of 64MB Granule * 2
// For OPAL system, it needs the HRMOR in the range of 4GB, so that HB reloading
// doesn't stamp on the OPAL/HostLinux Data. 64MB Granule * 64 = 4096MB, 64 is
// the multipler.
#define HRMOR_FOR_SPLESS_MODE 0x100000000ull //4096 * 1024 * 1024

#endif // else __SBEFW_SEEPROM__

namespace fapi2
{

#ifdef __SBEFW_SEEPROM__
const uint8_t MC_UNDEFINED = 0xFF;
uint8_t multicast_group_map[MCGROUP_COUNT];

plat_target_handle_t createPlatMCHandle(const plat_target_type_t i_plat_type, const MulticastGroup i_group, uint8_t i_core_select)
{
    assert((i_plat_type != PPE_TARGET_TYPE_CORE) || (i_core_select != 0));

    plat_target_handle_t l_handle;

    l_handle.fields.is_multicast = 1;
    l_handle.fields.core_select = i_core_select;
    l_handle.fields.mcast_group = multicast_group_map[i_group];
    assert(l_handle.fields.mcast_group != MC_UNDEFINED);
    l_handle.fields.type = i_plat_type;
    l_handle.fields.type_target_num = i_group;

    return l_handle;
}

static inline void clear_mc_map()
{
    for (unsigned int i = 0; i < MCGROUP_COUNT; i++)
    {
        multicast_group_map[i] = MC_UNDEFINED;
    }
}

ReturnCode plat_setMcMap(const std::vector< MulticastGroupMapping > &i_mappings)
{
    clear_mc_map();

    for (auto mapping : i_mappings)
    {
        if (mapping.abstractGroup >= MCGROUP_COUNT || mapping.hwValue > 7)
            return FAPI2_RC_INVALID_PARAMETER;
        multicast_group_map[mapping.abstractGroup] = mapping.hwValue;
    }
    return FAPI2_RC_SUCCESS;
}
#endif // __SBEFW_SEEPROM__

template<TargetType K>
plat_target_handle_t createPlatTargetHandle(const uint32_t i_plat_argument)
{
    static_assert(K != TARGET_TYPE_ALL, "Target instances cannot be of type ALL");
    static_assert(!(K & TARGET_TYPE_MULTICAST), "Cannot create multicast handles here");
    plat_target_handle_t l_handle;

    if(K & TARGET_TYPE_PROC_CHIP)
    {
        l_handle.fields.chiplet_num = 0;
        l_handle.fields.type = PPE_TARGET_TYPE_PROC_CHIP;
        l_handle.fields.type_target_num = 0;
    }
    else if(K & TARGET_TYPE_PERV)
    {
        l_handle.fields.chiplet_num = i_plat_argument;
        l_handle.fields.type = PPE_TARGET_TYPE_PERV;
        l_handle.fields.type_target_num = i_plat_argument;
    }
   
    else if(K & TARGET_TYPE_CORE)
    {
        l_handle.fields.chiplet_num = (i_plat_argument / 4) + EQ_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_CORE;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    
    else if(K & TARGET_TYPE_EQ)
    {
        l_handle.fields.chiplet_num = i_plat_argument + EQ_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_EQ;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    
    else if(K & TARGET_TYPE_EX)
    {
        l_handle.fields.chiplet_num = (i_plat_argument / 2) + EQ_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_EX;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    
    else if(K & TARGET_TYPE_PHB)
    {
        l_handle.fields.chiplet_num = (i_plat_argument / 3) + PHB_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_PHB;
        l_handle.fields.type_target_num = i_plat_argument;
    }
   
    else if(K & TARGET_TYPE_MCS)
    {
        //l_handle.fields.chiplet_num = N3_CHIPLET - (MCS_PER_MCBIST * (i_plat_argument / MCS_PER_MCBIST));
        l_handle.fields.chiplet_num = (i_plat_argument / MCS_PER_MCBIST) + MCBIST_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_MCS;
        l_handle.fields.type_target_num = i_plat_argument;
        l_handle.fields.sat_id = (2 * (i_plat_argument & 1)); 
    }

    else if(K & TARGET_TYPE_MC)
    {
        l_handle.fields.chiplet_num = i_plat_argument + MC_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_MC;
        l_handle.fields.type_target_num = i_plat_argument;
    }

    else if(K & TARGET_TYPE_MCBIST)
    {
        l_handle.fields.chiplet_num = i_plat_argument + MCBIST_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_MCBIST;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    
    else if(K & TARGET_TYPE_PAU)
    {
        l_handle.fields.chiplet_num = i_plat_argument + PAU_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_PAU;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    
    else if(K & TARGET_TYPE_IOHS)
    {
        l_handle.fields.chiplet_num = i_plat_argument + IOHS_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_IOHS;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    
    else if(K & TARGET_TYPE_MI)
    {
        //l_handle.fields.chiplet_num = N3_CHIPLET - (MI_PER_MC * (i_plat_argument / MI_PER_MC));
        l_handle.fields.chiplet_num = (i_plat_argument / MI_PER_MC) + MC_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_MI;
        l_handle.fields.type_target_num = i_plat_argument;
        l_handle.fields.sat_id = (2 * (i_plat_argument & 1));
    }

    return l_handle;
}

#ifdef __SBEFW_SEEPROM__

extern fapi2::ReturnCode
    plat_TargetPresent( fapi2::Target<fapi2::TARGET_TYPE_PERV> & i_chiplet_target,
                                    bool & b_present);

//TODO - Clean-up this function, since most of the things are in
// proc_sbe_attr_setup istep istep 2.2
    ReturnCode plat_AttrInit()
    {
        union
        {
            struct
            {
                uint64_t iv_majorEC : 4;
                uint64_t iv_deviceIdDontCare : 4;
                uint64_t iv_minorEC : 4;
                uint64_t iv_chipId : 8;
                uint64_t iv_deviceIdDontCare2 : 20;
                uint64_t iv_c4Pin : 1;
                uint64_t iv_deviceIdDontCare3 :17;
                uint64_t iv_fusedMode : 1;
                uint64_t iv_deviceIdDontCare4 :5;
            };
            uint64_t iv_deviceIdReg;
        } l_deviceId;

        uint8_t l_chipName = fapi2::ENUM_ATTR_NAME_NONE;
        uint8_t l_ec = 0;
        uint8_t fusedMode = 0;
        fapi2::buffer<uint64_t> l_tempReg = 0;
        fapi2::buffer<uint64_t> l_scratch8Reg = 0;
        fapi2::buffer<uint8_t> l_read1 = 0;
        fapi2::buffer<uint8_t> l_read2 = 0;
        fapi2::buffer<uint8_t> l_read3 = 0;
        fapi2::buffer<uint16_t> l_read4 = 0;
        fapi2::buffer<uint32_t> l_read5 = 0;
        fapi2::buffer<uint64_t> l_deviceIdReg = 0;
        //uint8_t l_riskLvl  = 0;
        bool l_isSlave = false;
        //uint8_t l_smfConfig = 0;
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chipTarget =
            plat_getChipTarget();
        const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
        //const size_t SCRATCH_PROC_CHIP_MEM_TO_USE_VALID_BIT = 0;
        //const size_t SCRATCH_PROC_CHIP_MEM_TO_USE_GROUP_ID_STARTBIT = 1;
        //const size_t SCRATCH_PROC_CHIP_MEM_TO_USE_CHIP_ID_STARTBIT = 4;
        //const size_t ATTR_PROC_MEM_TO_USE_GROUP_ID_STARTBIT = 0;
        //const size_t ATTR_PROC_MEM_TO_USE_CHIP_ID_STARTBIT = 3;
        //const size_t ATTR_PROC_FABRIC_GROUP_ID_LENGTH   = 3;
        //const size_t ATTR_PROC_FABRIC_CHIP_ID_LENGTH    = 3;
        //uint8_t l_proc_chip_mem_to_use_valid = 0;
        //uint8_t l_proc_chip_mem_to_use_group_id = 0;
        //uint8_t l_proc_chip_mem_to_use_chip_id = 0;
        //bool l_proc_chip_mem_to_use_set = false;
        //fapi2::buffer<uint8_t> l_proc_chip_mem_to_use_attr = 0;


        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RISK_LEVEL, FAPI_SYSTEM, l_riskLvl));
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
            //FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_1_SCOM,
            //                        l_tempReg));

            //l_tempReg.extract<0, 31>(l_read1);

            //FAPI_DBG("Setting up ATTR_CORE_GARD");
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_CORE_GARD, l_chipTarget, l_read1));

            l_read1 = 0;
        }
        if ( l_scratch8Reg.getBit<1>() )
        {
#if 0
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
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_I2C_BUS_DIV_REF, l_chipTarget, l_read4));

            l_tempReg.extractToRight<16, 4>(l_ndlMeshctrlSetup);
            l_ndlMeshctrlSetup = (~l_ndlMeshctrlSetup) & 0x0F;
            //FAPI_DBG("Setting up ATTR_NDL_MESHCTRL_SETUP");
            //FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NDL_MESHCTRL_SETUP, l_chipTarget, l_ndlMeshctrlSetup));

            //FAPI_DBG("Setting up ATTR_MC_PLL_BUCKET");
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_MC_PLL_BUCKET, FAPI_SYSTEM, l_read1));

            //FAPI_DBG("Setting up ATTR_OBX_PLL_BUCKET");
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OB0_PLL_BUCKET, l_chipTarget, l_ob0PllBucket));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OB1_PLL_BUCKET, l_chipTarget, l_ob1PllBucket));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OB2_PLL_BUCKET, l_chipTarget, l_ob2PllBucket));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OB3_PLL_BUCKET, l_chipTarget, l_ob3PllBucket));
#endif
        }

        if ( l_scratch8Reg.getBit<2>() )
        {
            uint8_t isMpIpl = 0;
            uint8_t isSpMode = 0;
            FAPI_DBG("Reading Scratch_reg3");
            //Getting SCRATCH_REGISTER_3 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_3_SCOM,
                                    l_tempReg));

            l_tempReg.extractToRight<2, 1>(isMpIpl);

            FAPI_DBG("Setting up ATTR_IS_MPIPL");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_IS_MPIPL, FAPI_SYSTEM, isMpIpl));

            l_tempReg.extractToRight<3, 1>(isSpMode);
            if(!isSpMode && !SBE::isSimicsRunning())
            {
#if 0
                FAPI_DBG("Set up ATTR_HOSTBOOT_HRMOR_OFFSET in SPless mode");
                uint64_t hrmor = HRMOR_FOR_SPLESS_MODE;
                FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET,
                                        FAPI_SYSTEM, hrmor));
#endif
            }
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_IS_SP_MODE, l_chipTarget, isSpMode));
            //l_tempReg.extractToRight<28, 4>(l_riskLvl);
        }

        if ( l_scratch8Reg.getBit<3>() )
        {
#if 0
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
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_CP_FILTER_BYPASS, l_chipTarget, l_cpFilterBypass));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_SS_FILTER_BYPASS, l_chipTarget, l_ssFilterBypass));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_IO_FILTER_BYPASS, l_chipTarget, l_ioFilterBypass));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_DPLL_BYPASS, l_chipTarget, l_dpllBypass));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_NEST_MEM_X_O_PCI_BYPASS, l_chipTarget, l_nestMemXOPciBypass));
            FAPI_DBG("Setting up ATTR_BOOT_FREQ_MULT, ATTR_NEST_PLL_BUCKET");
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_BOOT_FREQ_MULT, l_chipTarget, l_read4));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_NEST_PLL_BUCKET, FAPI_SYSTEM, l_read1));

            FAPI_DBG("Setting up ATTR_OBUS_RATIO_VALUE");
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_OBUS_RATIO_VALUE, l_chipTarget, l_attrObusRatio));

            l_read1 = 0;
            l_read4 = 0;
#endif
        }

        if ( l_scratch8Reg.getBit<4>() )
        {
#if 0
            uint8_t l_disableHbblVectors = 0;
            uint32_t l_pllMux = 0;
            uint8_t l_mcSyncMode = 0;
            //uint8_t l_slowPciRefClock = 0;

            FAPI_DBG("Reading Scratch_reg5");
            //Getting SCRATCH_REGISTER_5 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_5_SCOM,
                                    l_tempReg));

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

            //if (l_tempReg.getBit<5>())
            //{
            //    l_slowPciRefClock = fapi2::ENUM_ATTR_DD1_SLOW_PCI_REF_CLOCK_NORMAL;
            //}
            //else
            //{
            //    l_slowPciRefClock = fapi2::ENUM_ATTR_DD1_SLOW_PCI_REF_CLOCK_SLOW;
            //}

            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_DISABLE_HBBL_VECTORS, FAPI_SYSTEM, l_disableHbblVectors));
            //FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_MC_SYNC_MODE, l_chipTarget, l_mcSyncMode));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_DD1_SLOW_PCI_REF_CLOCK, FAPI_SYSTEM, l_slowPciRefClock));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_CLOCK_PLL_MUX, l_chipTarget, l_pllMux));
#endif
        }

        if ( l_scratch8Reg.getBit<5>() )
        {
            //uint8_t l_pumpMode = fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_NODE;

            FAPI_DBG("Reading Scratch_reg6");
            //Getting SCRATCH_REGISTER_6 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, PERV_SCRATCH_REGISTER_6_SCOM,
                                    l_tempReg));

#if 0
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
#endif
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

            FAPI_DBG("Setting up MASTER_CHIP, FABRIC_GROUP_ID and CHIP_ID");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_SBE_MASTER_CHIP, l_chipTarget,
                                   l_read1));
#if 0
            //if (l_tempReg.getBit<23>())
            //{
            //    l_pumpMode = fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_GROUP;
            //}
            //else
           // {
           //     l_pumpMode = fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_NODE;
            //}

            l_tempReg.extractToRight<26, 3>(l_read2);
            l_tempReg.extractToRight<29, 3>(l_read3);

            //l_smfConfig = l_tempReg.getBit<16>();
            FAPI_DBG("Setting up SMF CONFIG");
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_SMF_CONFIG,
             //                       fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
             //                       l_smfConfig));

            FAPI_DBG("Setting up PUMP MODE");
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_FABRIC_PUMP_MODE,
            //                        fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
            //                        l_pumpMode));

            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_FABRIC_GROUP_ID, l_chipTarget,
            //                       l_read2));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_FABRIC_CHIP_ID, l_chipTarget,
            //                       l_read3));

            l_tempReg.extractToRight<17, 3>(l_read2);
            l_tempReg.extractToRight<20, 3>(l_read3);

            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_EFF_FABRIC_GROUP_ID, l_chipTarget,
            //                       l_read2));
            //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_EFF_FABRIC_CHIP_ID, l_chipTarget,
            //                       l_read3));
#endif        
        }
#if 0
        if (!l_proc_chip_mem_to_use_set)
        {
            //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID,
            //                       l_chipTarget,
            //                       l_proc_chip_mem_to_use_group_id));
        }

        l_proc_chip_mem_to_use_attr.insertFromRight
                                    <ATTR_PROC_MEM_TO_USE_GROUP_ID_STARTBIT,
                                     ATTR_PROC_FABRIC_GROUP_ID_LENGTH>
                                    (l_proc_chip_mem_to_use_group_id);
        l_proc_chip_mem_to_use_attr.insertFromRight
                                    <ATTR_PROC_MEM_TO_USE_CHIP_ID_STARTBIT,
                                     ATTR_PROC_FABRIC_CHIP_ID_LENGTH>
                                    (l_proc_chip_mem_to_use_chip_id);

        //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_MEM_TO_USE,
        //                        l_chipTarget,
        //                        l_proc_chip_mem_to_use_attr));


        //FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_RISK_LEVEL, FAPI_SYSTEM,
        //                       l_riskLvl));
#endif
        FAPI_TRY(getscom_abs(PERV_DEVICE_ID_REG, &l_deviceId.iv_deviceIdReg));
        l_ec = (l_deviceId.iv_majorEC << 4) | (l_deviceId.iv_minorEC);
        switch(l_deviceId.iv_chipId)
        {
            case 0xD9:
                l_chipName = fapi2::ENUM_ATTR_NAME_P10;
                break;
            default:
                FAPI_ERR("Unsupported chip ID: 0x%02X",
                         static_cast<uint8_t>(l_deviceId.iv_chipId));
                assert(false);
        }
        
        fusedMode = (uint8_t)l_deviceId.iv_fusedMode;
        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_NAME, l_chipTarget, l_chipName));
        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_EC, l_chipTarget, l_ec));
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
           (i_chipletNumber < (NEST_CHIPLET_OFFSET + NEST_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - NEST_CHIPLET_OFFSET) +
                NEST_TARGET_OFFSET;
        }
        else if((i_chipletNumber >= PCI_CHIPLET_OFFSET) && 
               (i_chipletNumber < PCI_CHIPLET_OFFSET + PCI_TARGET_COUNT))
        {
            l_idx = (i_chipletNumber - PCI_CHIPLET_OFFSET) +
                PCI_TARGET_OFFSET;
        }
        else if((i_chipletNumber >= MC_CHIPLET_OFFSET) &&
                (i_chipletNumber < (MC_CHIPLET_OFFSET + MC_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - MC_CHIPLET_OFFSET) +
                MC_TARGET_OFFSET;
        }
        else if((i_chipletNumber >= PAU_CHIPLET_OFFSET) &&
                (i_chipletNumber < (PAU_CHIPLET_OFFSET + PAU_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - PAU_CHIPLET_OFFSET) +
                PAU_TARGET_OFFSET;
        }
        else if((i_chipletNumber >= IOHS_CHIPLET_OFFSET) &&
                (i_chipletNumber < (IOHS_CHIPLET_OFFSET + IOHS_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - IOHS_CHIPLET_OFFSET) +
                IOHS_TARGET_OFFSET;
        }
        else if((i_chipletNumber >= EQ_CHIPLET_OFFSET) &&
                (i_chipletNumber < (EQ_CHIPLET_OFFSET + EQ_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - EQ_CHIPLET_OFFSET) +
                EQ_TARGET_OFFSET;
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
        assert(((i_chipletNumber >= EX_CHIPLET_OFFSET) &&
                (i_chipletNumber < (EX_CHIPLET_OFFSET + EX_TARGET_COUNT))));

        uint32_t l_idx = (i_chipletNumber - EX_CHIPLET_OFFSET) +
                 EX_TARGET_OFFSET;

        return G_vec_targets[l_idx];
    }

    // Get the plat target handle by chiplet number - For PAU targets
    template<>
    plat_target_handle_t plat_getTargetHandleByChipletNumber<TARGET_TYPE_PAU>(
            const uint8_t i_chipletNumber)
    {
        assert(((i_chipletNumber >= PAU_CHIPLET_OFFSET) &&
                (i_chipletNumber < (PAU_CHIPLET_OFFSET + PAU_TARGET_COUNT))));

        uint32_t l_idx = (i_chipletNumber - PAU_CHIPLET_OFFSET) +
                 PAU_TARGET_OFFSET;

        return G_vec_targets[l_idx];
    }

    // Get the plat target handle by chiplet number - For IOHS targets
    template<>
    plat_target_handle_t plat_getTargetHandleByChipletNumber<TARGET_TYPE_IOHS>(
            const uint8_t i_chipletNumber)
    {
        assert(((i_chipletNumber >= IOHS_CHIPLET_OFFSET) &&
                (i_chipletNumber < (IOHS_CHIPLET_OFFSET + IOHS_TARGET_COUNT))));

        uint32_t l_idx = (i_chipletNumber - IOHS_CHIPLET_OFFSET) +
                 IOHS_TARGET_OFFSET;

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

    // Get plat target handle by instance number - For Core targets
    template <>
    plat_target_handle_t plat_getTargetHandleByInstance<TARGET_TYPE_CORE>(
            const uint8_t i_targetNum)
    {
        assert(i_targetNum < CORE_TARGET_COUNT);

        return G_vec_targets[i_targetNum + CORE_TARGET_OFFSET];
    }


    TargetType plat_target_handle_t::getFapiTargetType() const
    {
        TargetType l_targetType = TARGET_TYPE_NONE;
        switch(fields.type)
        {
            case PPE_TARGET_TYPE_PROC_CHIP:
                l_targetType = TARGET_TYPE_PROC_CHIP;
                break;
            case PPE_TARGET_TYPE_PHB:
                l_targetType = TARGET_TYPE_PHB;
                break;
            case PPE_TARGET_TYPE_CORE:
                l_targetType = TARGET_TYPE_CORE;
                break;
            case PPE_TARGET_TYPE_EQ:
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
            case PPE_TARGET_TYPE_MC:
                l_targetType = TARGET_TYPE_MC;
                break;
            case PPE_TARGET_TYPE_MCBIST:
                l_targetType = TARGET_TYPE_MCBIST;
                break;
            case PPE_TARGET_TYPE_PAU:
                l_targetType = TARGET_TYPE_PAU;
                break;
            case PPE_TARGET_TYPE_IOHS:
                l_targetType = TARGET_TYPE_IOHS;
                break;
            case PPE_TARGET_TYPE_MCS:
                l_targetType = TARGET_TYPE_MCS;
                break;
            case PPE_TARGET_TYPE_MI:
                l_targetType = TARGET_TYPE_MI;
                break;
            case PPE_TARGET_TYPE_NONE:
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
                assert(isPerv());
                l_handle = *this;
                break;
            case TARGET_TYPE_EQ:
                assert(fields.type == PPE_TARGET_TYPE_CORE);
                {
                    l_handle = G_vec_targets[(fields.type_target_num / CORES_PER_QUAD) + EQ_TARGET_OFFSET];
                }
                break;
            default:
                assert(false);
        }
        return l_handle;
    }

    // TODO - Need to check this
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
        buffer<uint64_t> l_enabledTargets = 0xFFFFFFFFFFFFFFFFULL;
        TargetType l_targetType = i_parentType;

        if(i_parentType & TARGET_TYPE_MULTICAST)
        {
            if (this->fields.is_multicast)
            {
                // If a real multicast target, loop over all targets in the chip
                // but filter for multicast group members.
                l_targetType = TARGET_TYPE_PROC_CHIP;
                getScom(Target<TARGET_TYPE_PERV | TARGET_TYPE_MULTICAST, MULTICAST_BITX>(*this),
                        0xF0001, l_enabledTargets);
            }
            else
            {
                // Otherwise just return this one target.
                o_children.push_back(*this);
            }
        }
        else if((i_parentType & ~(TARGET_TYPE_PROC_CHIP)) != 0)
        {
            // For composite targets, treat as other target
            l_targetType =
                static_cast<TargetType>(l_targetType & ~(TARGET_TYPE_PROC_CHIP));
        }

        // EQ ==> EC
        if((l_targetType == TARGET_TYPE_EQ) && (i_childType == TARGET_TYPE_CORE))
        {
            l_childPerChiplet = CORES_PER_QUAD;
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
            if ((l_temp.isType(i_platType)) && (l_enabledTargets.getBit(i)))
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

    
    // TODO - Need to check this
    void plat_target_handle_t::getChildren(
                const TargetFilter i_filter,
                const TargetState i_state,
                std::vector<plat_target_handle_t> &o_children) const
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
                plat_target_handle_t l_targetHandle = G_vec_targets.at(l_idx);

                if(l_targetHandle.isPerv()) // Can be an assertion?
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

    bool plat_target_handle::isPerv() const
    {
        static const bool l_truth_table[] = {
            false, // PPE_TARGET_TYPE_NONE
            false, // PPE_TARGET_TYPE_PROC_CHIP
            false, // PPE_TARGET_TYPE_MCS
            false,  // PPE_TARGET_TYPE_CORE
            true,  // PPE_TARGET_TYPE_EQ
            false, // PPE_TARGET_TYPE_EX
            true,  // PPE_TARGET_TYPE_PERV
            true,  // PPE_TARGET_TYPE_MCBIST
            false, // PPE_TARGET_TYPE_SYSTEM
            false, // PPE_TARGET_TYPE_PHB
            false, // PPE_TARGET_TYPE_MI
            true,  // PPE_TARGET_TYPE_MC
            true,  // PPE_TARGET_TYPE_PAU
            true, // PPE_TARGET_TYPE_IOHS
            false, // 0x0E
            false, // 0x0F
        };
        return l_truth_table[fields.type];
    }

    #ifndef __noRC__
    ReturnCode current_err;
    #endif

     fapi2::ReturnCode plat_PervPGTargets(const fapi2::Target<fapi2::TARGET_TYPE_PERV> & i_target,
                                          bool & o_present)
     {
        o_present = false;
        uint32_t attr_value = 0;
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

        // TODO - Check if this is required. Should we do all the Perv Targets
        // with PG Record ??
        if( i_chiplet_target.getChipletNumber() < EQ_CHIPLET_OFFSET )
        {
            static_cast<plat_target_handle_t&>((i_chiplet_target.operator()())).setPresent();
            static_cast<plat_target_handle_t&>((i_chiplet_target.operator()())).setFunctional(true);
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

        // Initialize multicast group mappings to "undefined"
        clear_mc_map();

        // Copy fixed section from SEEPROM to PIBMEM
        G_sbe_attrs.G_system_attrs = G_system_attributes;
        G_sbe_attrs.G_proc_chip_attrs = G_proc_chip_attributes;
        G_sbe_attrs.G_perv_attrs = G_perv_attributes;
        G_sbe_attrs.G_core_attrs = G_core_attributes;
        G_sbe_attrs.G_eq_attrs = G_eq_attributes;
        G_sbe_attrs.G_ex_attrs = G_ex_attributes;

        // Initialise global attribute pointers
        G_system_attributes_ptr = &(G_sbe_attrs.G_system_attrs);
        G_proc_chip_attributes_ptr = &(G_sbe_attrs.G_proc_chip_attrs);
        G_perv_attributes_ptr = &(G_sbe_attrs.G_perv_attrs);
        G_core_attributes_ptr = &(G_sbe_attrs.G_core_attrs);
        G_eq_attributes_ptr = &(G_sbe_attrs.G_eq_attrs);
        G_ex_attributes_ptr = &(G_sbe_attrs.G_ex_attrs);


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
         * Nest Targets
         */
        l_beginning_offset = NEST_TARGET_OFFSET;
        for (uint32_t i = 0; i < NEST_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PERV> target_name((createPlatTargetHandle<fapi2::TARGET_TYPE_PERV>(i + NEST_CHIPLET_OFFSET)));

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(target_name, b_present));

            G_vec_targets.at(i + l_beginning_offset) = (fapi2::plat_target_handle_t)(target_name.get());
        }

        /*
         * PCI Targets
         */

        l_beginning_offset = PCI_TARGET_OFFSET;
        for (uint32_t i = 0; i < PCI_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PERV> target_name((createPlatTargetHandle<fapi2::TARGET_TYPE_PERV>(i + PCI_CHIPLET_OFFSET)));

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(target_name, b_present));

            G_vec_targets.at(i + l_beginning_offset) = (fapi2::plat_target_handle_t)(target_name.get());
        }

        /*
         * Note: MCBIST/MC have the same offset and counts, so the loop just
         *       uses MC constants
         */

        l_beginning_offset = MC_TARGET_OFFSET;
        for (uint32_t i = 0; i < MC_TARGET_COUNT; ++i)
        {
            l_platHandle = createPlatTargetHandle<fapi2::TARGET_TYPE_MC>(i);
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv(l_platHandle);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            //FAPI_TRY(plat_TargetPresent(target_name, b_present));
            FAPI_TRY(plat_TargetPresent(l_perv, b_present));

            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(l_perv.get());
        }

        /*
         * PAU Targets
         */

        l_beginning_offset = PAU_TARGET_OFFSET;
        for (uint32_t i = 0; i < PAU_TARGET_COUNT; ++i)
        {
            l_platHandle = createPlatTargetHandle<fapi2::TARGET_TYPE_PAU>(i);
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv(l_platHandle);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            //FAPI_TRY(plat_TargetPresent(target_name, b_present));
            FAPI_TRY(plat_TargetPresent(l_perv, b_present));

            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(l_perv.get());
        }

        /*
         * IOHS Targets
         */

        l_beginning_offset = IOHS_TARGET_OFFSET;
        for (uint32_t i = 0; i < IOHS_TARGET_COUNT; ++i)
        {
            l_platHandle = createPlatTargetHandle<fapi2::TARGET_TYPE_IOHS>(i);
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv(l_platHandle);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            //FAPI_TRY(plat_TargetPresent(target_name, b_present));
            FAPI_TRY(plat_TargetPresent(l_perv, b_present));

            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(l_perv.get());
        }

        /*
         * Cache (EQ) Targets
         */
        l_beginning_offset = EQ_TARGET_OFFSET;
        for (uint32_t i = 0; i < EQ_TARGET_COUNT; ++i)
        {
            l_platHandle = createPlatTargetHandle<fapi2::TARGET_TYPE_EQ>(i);
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv(l_platHandle);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            //FAPI_TRY(plat_TargetPresent(target_name, b_present));
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
            
            // Get the parent CORE's ATTR_PG
            // TODO - Fix this
            static_cast<plat_target_handle_t&>(target_name.operator ()()).setPresent();
            static_cast<plat_target_handle_t&>(target_name.operator ()()).setFunctional(true);

            G_vec_targets.at(l_beginning_offset+i) = (fapi2::plat_target_handle_t)(target_name.get());
        }

        /*
         * EX Targets
         */
        l_beginning_offset = EX_TARGET_OFFSET;
        for (uint32_t i = 0; i < EX_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_EX> target_name((createPlatTargetHandle<fapi2::TARGET_TYPE_EX>(i)));

            //fapi2::Target<fapi2::TARGET_TYPE_EQ> l_parent = target_name.getParent<fapi2::TARGET_TYPE_EQ>();

            // Get the parent EQ's ATTR_PG
            // TODO - Fix this

            static_cast<plat_target_handle_t&>(target_name.operator ()()).setPresent();
            static_cast<plat_target_handle_t&>(target_name.operator ()()).setFunctional(true);
            
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
            l_platHandle = createPlatTargetHandle<fapi2::TARGET_TYPE_MI>(i);

            fapi2::Target<fapi2::TARGET_TYPE_PERV>
                l_nestTarget((plat_getTargetHandleByChipletNumber<TARGET_TYPE_PERV>( (i/MCS_PER_MCBIST) + MCBIST_CHIPLET_OFFSET)));

            uint32_t l_attrPg = 0;

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
                l_pciTarget((plat_getTargetHandleByChipletNumber<TARGET_TYPE_PERV>(target_name.getChipletNumber()/3)));

            // Get the PHB's ATTR_PG
            static_cast<plat_target_handle_t&>(target_name.operator ()()).setPresent();
            static_cast<plat_target_handle_t&>(target_name.operator ()()).setFunctional(true);

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
#if 0
        uint8_t l_eqGards = 0;
        uint32_t l_ecGards = 0;
        static const uint32_t l_mask = 0x80000000;
        bool l_coreGroupNonFunctional = true;
        //fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = plat_getChipTarget();

        // Read the EQ and EC gard attributes from the chip target
        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_EQ_GARD, l_chip, l_eqGards));
        //FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_EC_GARD, l_chip, l_ecGards));

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
#endif
        return fapi2::current_err;
    }
#endif //not __SBEFW_SEEPROM__
} // fapi2
