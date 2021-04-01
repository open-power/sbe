/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/target.C $                                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2021                        */
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
#if !defined(__SBEMFW_MEASUREMENT__) && !defined(__SBEVFW_VERIFICATION__)
#include <plat_target_utils.H>
#include <p10_scom_pibms.H>
#include <p10_scom_perv_9.H>
#endif


#define PERV_SB_CS_SELECT_SECONDARY_SEEPROM 17
#define N1_PG_NMMU1_BIT                     17

#define PSU_REGS_OCMB_ENGINE_OFFSET    16
#define PSU_REGS_OCMB_DEVADDR_OFFSET   17
#define PSU_REGS_OCMB_FUNCSTATE_OFFSET 18

#if defined __SBEFW_PIBMEM__
// Global Vector containing ALL targets.  This structure is referenced by
// fapi2::getChildren to produce the resultant returned vector from that
// call.
std::vector<fapi2::plat_target_handle_t> G_vec_targets;

// Global variable for fixed section in pibmem
G_sbe_attrs_t G_sbe_attrs;

fapi2attr::SystemAttributes_t*    G_system_attributes_ptr;
fapi2attr::ProcChipAttributes_t*  G_proc_chip_attributes_ptr;
fapi2attr::OCMBChipAttributes_t*  G_ocmb_chip_attributes_ptr;
fapi2attr::PervAttributes_t*      G_perv_attributes_ptr;
fapi2attr::CoreAttributes_t*      G_core_attributes_ptr;
fapi2attr::EQAttributes_t*        G_eq_attributes_ptr;
#endif // __SBEFW_PIBMEM__

#if defined __SBEFW_SEEPROM__
extern std::vector<fapi2::plat_target_handle_t> G_vec_targets;

// Global variable for fixed section in pibmem
extern G_sbe_attrs_t G_sbe_attrs;

extern fapi2attr::SystemAttributes_t*    G_system_attributes_ptr;
extern fapi2attr::ProcChipAttributes_t*  G_proc_chip_attributes_ptr;
extern fapi2attr::PervAttributes_t*      G_perv_attributes_ptr;
extern fapi2attr::CoreAttributes_t*      G_core_attributes_ptr;
extern fapi2attr::EQAttributes_t*        G_eq_attributes_ptr;
extern fapi2attr::OCMBChipAttributes_t*  G_ocmb_chip_attributes_ptr;

// For PhyP system, HRMOR is set to 128MB, which is multiple of 64MB Granule * 2
// For OPAL system, it needs the HRMOR in the range of 4GB, so that HB reloading
// doesn't stamp on the OPAL/HostLinux Data. 64MB Granule * 62 = 3968MB, 64 is
// the multipler.
#define HRMOR_FOR_SPLESS_MODE 0xF8000000ull //3968 * 1024 * 1024

#endif // __SBEFW_SEEPROM__

#if !defined(__SBEMFW_MEASUREMENT__)  && !defined(__SBEVFW_VERIFICATION__)
//TODO - This will be removed once this address is
//define in P10 scom definition.
#define EXPORT_REGL_STATUS 0x10009ull
#endif  // not __SBEMFW_MEASUREMENT__ or __SBEVFW_VERIFICATION__

namespace fapi2
{

#ifndef __SBEFW_SEEPROM__
  #ifndef __noRC__
    ReturnCode current_err;
  #endif
#endif // not __SBEFW_SEEPROM__

#if defined __SBEFW_SEEPROM__
const uint8_t MC_UNDEFINED = 0xFF;
uint8_t multicast_group_map[MCGROUP_COUNT];

plat_target_handle_t createPlatMCHandle(const plat_target_type_t i_plat_type, const MulticastGroup i_group, uint8_t i_core_select)
{
    assert((i_plat_type != PPE_TARGET_TYPE_CORE) || (i_core_select != 0));

    plat_target_handle_t l_handle;

    l_handle.fields.is_multicast = 1;
    l_handle.fields.core_select = i_core_select;
    l_handle.fields.mcast_group = multicast_group_map[i_group];
    assert(multicast_group_map[i_group] != MC_UNDEFINED);
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

#if !defined(__SBEMFW_MEASUREMENT__) && !defined(__SBEVFW_VERIFICATION__)
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
        l_handle.fields.core_select = (1 << ( 3 - (i_plat_argument % 4)));
    }
    else if(K & TARGET_TYPE_EQ)
    {
        l_handle.fields.chiplet_num = i_plat_argument + EQ_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_EQ;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    else if(K & TARGET_TYPE_PHB)
    {
        l_handle.fields.chiplet_num = (i_plat_argument / 3) + PEC_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_PHB;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    else if(K & TARGET_TYPE_PEC)
    {
        l_handle.fields.chiplet_num = i_plat_argument + PEC_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_PEC;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    else if(K & TARGET_TYPE_MC)
    {
        l_handle.fields.chiplet_num = i_plat_argument + MC_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_MC;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    else if(K & TARGET_TYPE_PAUC)
    {
        l_handle.fields.chiplet_num = i_plat_argument + PAUC_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_PAUC;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    else if(K & TARGET_TYPE_PAU)
    {
        l_handle.fields.chiplet_num = (i_plat_argument/2) + PAUC_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_PAU;
        l_handle.fields.type_target_num = i_plat_argument;
        l_handle.fields.ring_id = (i_plat_argument == 5 || i_plat_argument == 7) ? 4 : 2;
    }
    else if(K & TARGET_TYPE_IOHS)
    {
        l_handle.fields.chiplet_num = i_plat_argument + IOHS_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_IOHS;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    else if(K & TARGET_TYPE_MI)
    {
        l_handle.fields.chiplet_num = (i_plat_argument / MI_PER_MC) + MC_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_MI;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    else if(K & TARGET_TYPE_NMMU)
    {
        l_handle.fields.chiplet_num = i_plat_argument + NEST_CHIPLET_OFFSET;
        l_handle.fields.type = PPE_TARGET_TYPE_NMMU;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    else if(K & TARGET_TYPE_OCMB_CHIP)
    {
        l_handle.fields.chiplet_num = i_plat_argument;
        l_handle.fields.type = PPE_TARGET_TYPE_OCMB;
        l_handle.fields.type_target_num = i_plat_argument;
    }
    else
    {
        STATIC_COMPILE_ERROR("Unhandled target type");
    }

    l_handle.fields.functional = true;
    return l_handle;
}
#endif // not __SBEMFW_MEASUREMENT__ or __SBEVFW_VERIFICATION__

#if defined __SBEFW_SEEPROM__
//TODO - Clean-up this function, since most of the things are in
// proc_sbe_attr_setup istep istep 2.2
    ReturnCode plat_AttrInit()
    {
// TODO - Need to cleanup, Device Id is not available, instead we have read
// Export Control Status Register and Environment Status Register to fetch
// EC Level, Fused Mode and SBE Master bit
        uint8_t fusedMode = 0;
        fapi2::buffer<uint64_t> l_tempReg = 0;
        fapi2::buffer<uint64_t> l_scratch8Reg = 0;
        fapi2::buffer<uint8_t> l_read1 = 0;
        fapi2::buffer<uint8_t> l_read2 = 0;
        fapi2::buffer<uint8_t> l_read3 = 0;
        fapi2::buffer<uint16_t> l_read4 = 0;
        fapi2::buffer<uint32_t> l_read5 = 0;
        fapi2::buffer<uint64_t> l_ctrlReg = 0;
        fapi2::buffer<uint64_t> cbs_envstat_reg = 0;
        fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type isMaster = false;
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chipTarget = plat_getChipTarget();
        const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
        FAPI_TRY(fapi2::getScom(l_chipTarget, MAILBOX_SCRATCH_REG_8,
                                l_scratch8Reg));

        //Getting CBS_CS register value
        FAPI_TRY(fapi2::getScom(l_chipTarget, MAILBOX_CBS_CTRL_STATUS,
                                l_tempReg));

        l_read1.writeBit<7>(l_tempReg.getBit<4>());

        FAPI_DBG("Setting ATTR_SECURITY_ENABLE with the SAB state");
        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_SECURITY_ENABLE, FAPI_SYSTEM, l_read1));


        if ( l_scratch8Reg.getBit<2>() )
        {
            uint8_t isMpIpl = 0;
            uint8_t isSpMode = 0;
            FAPI_DBG("Reading Scratch_reg3");
            //Getting SCRATCH_REGISTER_3 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, MAILBOX_SCRATCH_REG_3,
                                    l_tempReg));

            l_tempReg.extractToRight<2, 1>(isMpIpl);

            FAPI_DBG("Setting up ATTR_IS_MPIPL");
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_IS_MPIPL, FAPI_SYSTEM, isMpIpl));

            l_tempReg.extractToRight<3, 1>(isSpMode);
            FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_IS_SP_MODE, l_chipTarget, isSpMode));
        }

    
        //Update the ATTR_PROC_SBE_MASTER_CHIP based on the Scratch Register 6 and 
        //FSXCOMP_FSXLOG_CBS_ENVSTAT_RO(0x50004) register.
        //Read FSXCOMP_FSXLOG_CBS_ENVSTAT_RO value
        FAPI_TRY(fapi2::getScom(l_chipTarget, scomt::perv::FSXCOMP_FSXLOG_CBS_ENVSTAT_RO,
                                cbs_envstat_reg));
        FAPI_INF("CBS_ENVSTAT Reg [0x%08X]", (cbs_envstat_reg >>32));
        //Check SCRATCH_REGISTER_6 register value is valid
        if ( l_scratch8Reg.getBit<5>() )
        {
            FAPI_DBG("Reading Scratch_reg6");
            //Getting SCRATCH_REGISTER_6 register value
            FAPI_TRY(fapi2::getScom(l_chipTarget, MAILBOX_SCRATCH_REG_6 ,
                                    l_tempReg));

            //Scratch bit 24 indicates Master if SET
            if ( l_tempReg.getBit<24>() ) 
            {
                isMaster = true;
                //Override based on the C4 PIN in the regegister FSXCOMP_FSXLOG_CBS_ENVSTAT_RO
                if( !((cbs_envstat_reg >> 32) & 0x8000000) )
                {
                    // C4 overrides Master as slave
                    isMaster = false;
                    FAPI_INF("Overriding the Proc:0x%.8x as SLAVE chip based on C4 Pin",
                              l_chipTarget.get());
                }
            }
         }
         else //SCRATCH_REGISTER_6 register value is invalid, determine
         {
            FAPI_INF("Scratch Register 6 is not valid,depend on FSXCOMP_FSXLOG_CBS_ENVSTAT_RO"
                     "to determine master/slave PROC" );
            if( (cbs_envstat_reg >> 32) & 0x8000000 )
            {
                isMaster = true;
            }
         }
         //Update the ATTR_PROC_SBE_MASTER_CHIP
         FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_PROC_SBE_MASTER_CHIP, l_chipTarget,isMaster));

        FAPI_TRY(fapi2::getScom(l_chipTarget, EXPORT_REGL_STATUS, l_ctrlReg));
        fusedMode = static_cast<uint8_t>(l_ctrlReg.getBit<10>());
        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_FUSED_CORE_MODE,
                                fapi2::Target<TARGET_TYPE_SYSTEM>(),
                                fusedMode));

        { // scope initializer to resolve compile issues

        // if bit 17 of MAILBOX_CBS_SELFBOOT_CTRL_STATUS is set, set attribute
        // which suggest backup seeprom is selected for boot.
        FAPI_TRY(fapi2::getScom(l_chipTarget, MAILBOX_CBS_SELFBOOT_CTRL_STATUS,
                                l_tempReg));
        fapi2::buffer<uint8_t> attrSeepromSlct = 0;

        if( l_tempReg.getBit(PERV_SB_CS_SELECT_SECONDARY_SEEPROM))
        {
            attrSeepromSlct.setBit<7>();
        }
        //TODO: Read LFR SBE local reg and set ATTR_BACKUP_SEEPROM_SELECT
        FAPI_TRY(PLAT_ATTR_INIT(fapi2::ATTR_BACKUP_SEEPROM_SELECT,
                    l_chipTarget, attrSeepromSlct));
        } // end of scope initializer
fapi_try_exit:
        return fapi2::current_err;
    }
#endif //__SBEFW_SEEPROM__

#if defined __SBEFW_PIBMEM__

    // Get the plat target handle by chiplet number - For PERV targets
    template<>
    plat_target_handle_t plat_getTargetHandleByChipletNumber<TARGET_TYPE_PERV>(
            const uint8_t i_chipletNumber)
    {
        uint32_t l_idx = 0;

        if(i_chipletNumber == TP_CHIPLET_OFFSET)
        {
            l_idx = TP_CHIPLET_OFFSET;
        }
        else  if((i_chipletNumber >= NEST_CHIPLET_OFFSET) &&
           (i_chipletNumber < (NEST_CHIPLET_OFFSET + NEST_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - NEST_CHIPLET_OFFSET) +
                NEST_TARGET_OFFSET;
        }
        else if((i_chipletNumber >= PEC_CHIPLET_OFFSET) &&
               (i_chipletNumber < PEC_CHIPLET_OFFSET + PEC_TARGET_COUNT))
        {
            l_idx = (i_chipletNumber - PEC_CHIPLET_OFFSET) +
                PEC_TARGET_OFFSET;
        }
        else if((i_chipletNumber >= MC_CHIPLET_OFFSET) &&
                (i_chipletNumber < (MC_CHIPLET_OFFSET + MC_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - MC_CHIPLET_OFFSET) +
                MC_TARGET_OFFSET;
        }
        else if((i_chipletNumber >= PAUC_CHIPLET_OFFSET) &&
                (i_chipletNumber < (PAUC_CHIPLET_OFFSET + PAUC_TARGET_COUNT)))
        {
            l_idx = (i_chipletNumber - PAUC_CHIPLET_OFFSET) +
                PAUC_TARGET_OFFSET;
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

    // Get the plat target handle by chiplet number - For PAUC targets
    template<>
    plat_target_handle_t plat_getTargetHandleByChipletNumber<TARGET_TYPE_PAUC>(
            const uint8_t i_chipletNumber)
    {
        assert(((i_chipletNumber >= PAUC_CHIPLET_OFFSET) &&
                (i_chipletNumber < (PAUC_CHIPLET_OFFSET + PAUC_TARGET_COUNT))));

        uint32_t l_idx = (i_chipletNumber - PAUC_CHIPLET_OFFSET) +
                 PAUC_TARGET_OFFSET;

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

    // Get plat target handle by instance number - For Core targets
    template <>
    plat_target_handle_t plat_getTargetHandleByInstance<TARGET_TYPE_CORE>(
            const uint8_t i_targetNum)
    {
        assert(i_targetNum < CORE_TARGET_COUNT);

        return G_vec_targets[i_targetNum + CORE_TARGET_OFFSET];
    }

    // Get plat target handle by instance number - For OCMB targets
    template <>
    plat_target_handle_t plat_getOCMBTargetHandleByInstance<TARGET_TYPE_OCMB_CHIP>(
            const uint8_t i_targetNum)
    {
        assert(i_targetNum < OCMB_TARGET_COUNT);

        return G_vec_targets[i_targetNum + OCMB_TARGET_OFFSET];
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
            case PPE_TARGET_TYPE_PERV:
                l_targetType = TARGET_TYPE_PERV;
                break;
            case PPE_TARGET_TYPE_SYSTEM:
                l_targetType = TARGET_TYPE_SYSTEM;
                break;
            case PPE_TARGET_TYPE_MC:
                l_targetType = TARGET_TYPE_MC;
                break;
            case PPE_TARGET_TYPE_PEC:
                l_targetType = TARGET_TYPE_PEC;
                break;
            case PPE_TARGET_TYPE_PAUC:
                l_targetType = TARGET_TYPE_PAUC;
                break;
            case PPE_TARGET_TYPE_PAU:
                l_targetType = TARGET_TYPE_PAU;
                break;
            case PPE_TARGET_TYPE_IOHS:
                l_targetType = TARGET_TYPE_IOHS;
                break;
            case PPE_TARGET_TYPE_MI:
                l_targetType = TARGET_TYPE_MI;
                break;
            case PPE_TARGET_TYPE_NMMU:
                l_targetType = TARGET_TYPE_NMMU;
                break;
            case PPE_TARGET_TYPE_OCMB :
                l_targetType = TARGET_TYPE_OCMB_CHIP;
                break;
            case PPE_TARGET_TYPE_NONE:
            default:
                assert(false);
                break;
        }
        return l_targetType;
    }

    static void loopTargetsByChiplet(
        const int i_first, const int i_count,
        const buffer<uint64_t> &i_enabled,
        const bool i_include_nonfunctional,
        std::vector<plat_target_handle> &o_children)
    {
        for (int i = i_first; i < i_first + i_count; i++)
        {
            const auto &l_tgt = G_vec_targets[i];
            if ((i_include_nonfunctional || l_tgt.fields.functional)
                && i_enabled.getBit(l_tgt.fields.chiplet_num))
            {
                o_children.push_back(l_tgt);
            }
        }
    }

    static void loopTargetsByInstance(
        const int i_first, const int i_count,
        const buffer<uint64_t> &i_enabled,
        const bool i_include_nonfunctional,
        std::vector<plat_target_handle> &o_children)
    {
        for (int i = i_first; i < i_first + i_count; i++)
        {
            const auto &l_tgt = G_vec_targets[i];
            if ((i_include_nonfunctional || l_tgt.fields.functional)
                && i_enabled.getBit(l_tgt.fields.type_target_num))
            {
                o_children.push_back(l_tgt);
            }
        }
    }

    void plat_target_handle_t::getMulticastChildren(
        const bool i_include_nonfunctional,
        std::vector<plat_target_handle> &o_children) const
    {
        if (!this->fields.is_multicast)
        {
            // Trivial case where we're already a unicast target of the requested type - return just this target
            o_children.push_back(*this);
            return;
        }

        // If a real multicast target, loop over all targets in the chip
        // but filter for multicast group members.
        fapi2::buffer<uint64_t> l_enabledTargets;
        Target<TARGET_TYPE_PERV | TARGET_TYPE_MULTICAST, MULTICAST_BITX> l_tmpTarget(*this);
        l_tmpTarget().fields.core_select = 0; // Strip core select in case it's a core target; we want to target the EQ for this
        getScom(l_tmpTarget, 0xF0001, l_enabledTargets);

        if (this->fields.type != PPE_TARGET_TYPE_CORE)
        {
            // Non-core (i.e. chiplet) target -> loop over all PERV targets, match chiplet ID
            loopTargetsByChiplet(PERV_TARGET_OFFSET, PERV_TARGET_COUNT,
                                 l_enabledTargets, i_include_nonfunctional, o_children);
        }
        else
        {
            // Special handling for core targets: take core_select into account
            buffer<uint64_t> l_enabledCores = 0;

            for (uint32_t i = 0; i < CORE_TARGET_COUNT; i++)
            {
                // Select a core if its EQ is part of the MC group and its ID is selected in core_select
                l_enabledCores.writeBit((l_enabledTargets.getBit(32 + (i >> 2))
                                         && (this->fields.core_select & (8 >> (i & 3)))), i);
            }

            loopTargetsByInstance(CORE_TARGET_OFFSET, CORE_TARGET_COUNT,
                                  l_enabledCores, i_include_nonfunctional, o_children);
        }
    }

    void plat_target_handle_t::getChipletChildren(
        const plat_target_type_t i_child_type,
        const bool i_include_nonfunctional,
        std::vector<plat_target_handle> &o_children) const
    {
        for (uint32_t i = NON_CHIPLET_TARGET_OFFSET; i < TARGET_COUNT; i++)
        {
            plat_target_handle_t &l_target = G_vec_targets[i];
            if ((l_target.fields.type == i_child_type) &&
                (l_target.fields.chiplet_num == fields.chiplet_num) &&
                (i_include_nonfunctional || l_target.fields.functional))
            {
                o_children.push_back(l_target);
            }
        }
    }

    void plat_target_handle_t::getProcChildren(
        const plat_target_type_t i_child_type,
        const bool i_include_nonfunctional,
        std::vector<plat_target_handle> &o_children) const
    {
        for (uint32_t i = PERV_TARGET_OFFSET; i < TARGET_COUNT; i++)
        {
            plat_target_handle_t &l_target = G_vec_targets[i];
            if ((l_target.fields.type == i_child_type) &&
                (i_include_nonfunctional || l_target.fields.functional))
            {
                o_children.push_back(l_target);
            }
        }
    }

    void plat_target_handle_t::getChildren(
        const TargetFilter i_filter,
        const bool i_include_nonfunctional,
        std::vector<plat_target_handle_t> &o_children) const
    {
        const fapi2::buffer<__underlying_type(TargetFilter)> l_filter = i_filter;

        // Walk the bits in the input target filter. For every bit, at
        // position x, that is set, x can be used as an index into our global
        // target vector (indexed by chiplet number)
        for (uint32_t i = 0; i < PERV_TARGET_COUNT; i++)
        {
            plat_target_handle_t &l_target = G_vec_targets[i + PERV_TARGET_OFFSET];
            if (l_filter.getBit(i) && (i_include_nonfunctional || l_target.fields.functional))
            {
                o_children.push_back(l_target);
            }
         }
    }

    /// @brief Function to initialize the G_targets vector with functional state
    ///        basis the PG Attribute
    ReturnCode plat_UpdateFunctionalState()
    {
        // Loop over all PERV targets and disable any target sharing the same
        // chiplet ID if the entire chiplet is bad.
        // This will take care of MC+MI, PEC+PHB and IOHS
        for (uint32_t i = 0; i < PERV_TARGET_COUNT; i++)
        {
            plat_target_handle &ref = G_vec_targets[i + PERV_TARGET_OFFSET];
            fapi2::Target<fapi2::TARGET_TYPE_PERV> perv = ref;
            fapi2::ATTR_PG_Type pg;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, perv, pg));
            for (plat_target_handle &target : G_vec_targets)
            {
                if (target.fields.chiplet_num == ref.fields.chiplet_num)
                {
                    target.setFunctional(pg != 0xFFFFFFFF);
                    break;
                }
            }
        }

        // EQs are always functional, find the logical Cores Target
        // and update functional/non-functional state
        for(uint32_t i=0; i<EQ_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PERV> eqPerv = G_vec_targets.at(i + EQ_TARGET_OFFSET);
            fapi2::buffer<fapi2::ATTR_PG_Type> pg;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, eqPerv, pg));
            for(uint32_t j=0; j<CORES_PER_QUAD; ++j)
            {
                G_vec_targets[j + CORE_TARGET_OFFSET + i * CORES_PER_QUAD].setFunctional(!pg.getBit(PARTIAL_GOOD_CORE0 + j));
            }
        }

        // PAU chiplets are always functional, but check the individual PAUs
        for (uint32_t i = 0; i < PAUC_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PERV> paucPerv = G_vec_targets[i + PAUC_TARGET_OFFSET];
            fapi2::buffer<fapi2::ATTR_PG_Type> pg;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, paucPerv, pg));

            for (int j = 0; j < PAU_PER_PAUC; j++)
            {
                // Sometimes a table is faster and smaller than code :)
                static const uint8_t pau_pg_map[PAUC_TARGET_COUNT][PAU_PER_PAUC] = {
                    { PAU_TARGET_OFFSET + 0, 0 },
                    { PAU_TARGET_OFFSET + 1, 0 },
                    { PAU_TARGET_OFFSET + 2, PAU_TARGET_OFFSET + 3 },
                    { PAU_TARGET_OFFSET + 4, PAU_TARGET_OFFSET + 5 }
                };

                const uint8_t l_pau_id = pau_pg_map[i][j];
                if (l_pau_id)
                {
                    G_vec_targets[l_pau_id].setFunctional(!pg.getBit(PARTIAL_GOOD_PAU0 + j));
                }
            }
        }

        {
            // NMMU0 is should always be functional.
            // Set NMMU1 Functional State basis Nest1 Perv chiplet bit 17
            fapi2::Target<fapi2::TARGET_TYPE_PERV> nest1Perv = G_vec_targets[NEST_TARGET_OFFSET + 1];
            fapi2::buffer<fapi2::ATTR_PG_Type> pg;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, nest1Perv, pg));
            G_vec_targets[NMMU_TARGET_OFFSET + 1].setFunctional(!pg.getBit(N1_PG_NMMU1_BIT));
        }
    fapi_try_exit:
        return fapi2::current_err;
    }

#endif // __SBEFW_PIBMEM__

#if defined __SBEFW_SEEPROM__

    /// @brief Function to initialize the G_targets vector based on partial good
    ///      attributes ///  this will move to plat_target.H formally
    fapi2::ReturnCode plat_TargetsInit()
    {
        uint8_t l_chipName = fapi2::ENUM_ATTR_NAME_NONE;
        plat_target_handle_t l_platHandle;

        //TODO: Remove once UpdateOCMBTarget chip-op is enabled.
        //This data will from that chip-op.
        uint8_t mReg[24] = {0x00, 0x1A, 0x2B, 0x39, 0x4C, 0x5D, 0x68, 0x77,
                             0x8F, 0x92, 0xA1, 0xBE, 0xC3, 0xD6, 0xE5, 0xF4,
                             0x03, 0x40, 0xFF, 0xFF, 0x0, 0x0, 0x0, 0x0};
        uint8_t *ocmbParam = mReg;

        // Initialize multicast group mappings to "undefined"
        clear_mc_map();

        // Copy fixed section from SEEPROM to PIBMEM
        G_sbe_attrs.G_system_attrs = G_system_attributes;
        G_sbe_attrs.G_proc_chip_attrs = G_proc_chip_attributes;
        G_sbe_attrs.G_ocmb_chip_attrs = G_ocmb_chip_attributes;
        G_sbe_attrs.G_perv_attrs = G_perv_attributes;
        G_sbe_attrs.G_core_attrs = G_core_attributes;
        G_sbe_attrs.G_eq_attrs = G_eq_attributes;

        // Initialise global attribute pointers
#ifndef DFT
        G_system_attributes_ptr = &(G_sbe_attrs.G_system_attrs);
        G_proc_chip_attributes_ptr = &(G_sbe_attrs.G_proc_chip_attrs);
        G_ocmb_chip_attributes_ptr = &(G_sbe_attrs.G_ocmb_chip_attrs);
        G_perv_attributes_ptr = &(G_sbe_attrs.G_perv_attrs);
        G_core_attributes_ptr = &(G_sbe_attrs.G_core_attrs);
        G_eq_attributes_ptr = &(G_sbe_attrs.G_eq_attrs);
#else
		// For DFT PIBMEM contained, use the attributes in the image header
		// at the top of the image instead of the second header appended
		// further into the image
        G_system_attributes_ptr = &(G_system_attributes);
        G_proc_chip_attributes_ptr = &(G_proc_chip_attributes);
        G_perv_attributes_ptr = &(G_perv_attributes);
        G_core_attributes_ptr = &(G_core_attributes);
        G_eq_attributes_ptr = &(G_eq_attributes);
#endif	

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
        G_vec_targets[l_beginning_offset] = createPlatTargetHandle<fapi2::TARGET_TYPE_PROC_CHIP>(0);

        // Initialize platform attributes. Needs to be after the chip target is
        // created.
        FAPI_TRY(plat_AttrInit());
        FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_NAME, plat_getChipTarget(), l_chipName));

        /*
         * TP Target
         */
        l_beginning_offset = TP_TARGET_OFFSET;
        {
            G_vec_targets[l_beginning_offset] = createPlatTargetHandle<fapi2::TARGET_TYPE_PERV>(TP_CHIPLET_OFFSET);
        }

        /*
         * Nest Targets
         */
        l_beginning_offset = NEST_TARGET_OFFSET;
        for (uint32_t i = 0; i < NEST_TARGET_COUNT; ++i)
        {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_PERV>(i + NEST_CHIPLET_OFFSET);
        }

        /*
         * PEC Targets
         */
        l_beginning_offset = PEC_TARGET_OFFSET;
        for (uint32_t i = 0; i < PEC_TARGET_COUNT; ++i)
        {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_PEC>(i);
        }

        /*
         * Note: MC have the same offset and counts, so the loop just
         *       uses MC constants
         */
        l_beginning_offset = MC_TARGET_OFFSET;
         for (uint32_t i = 0; i < MC_TARGET_COUNT; ++i)
        {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_MC>(i);
        }

        /*
         * PAUC Targets
         */
        l_beginning_offset = PAUC_TARGET_OFFSET;
        for (uint32_t i = 0; i < PAUC_TARGET_COUNT; ++i)
        {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_PAUC>(i);
        }

        /*
         * IOHS Targets
         */
        l_beginning_offset = IOHS_TARGET_OFFSET;
         for (uint32_t i = 0; i < IOHS_TARGET_COUNT; ++i)
        {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_IOHS>(i);
        }

        /*
         * Cache (EQ) Targets
         */
        l_beginning_offset = EQ_TARGET_OFFSET;
        for (uint32_t i = 0; i < EQ_TARGET_COUNT; ++i)
         {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_EQ>(i);
        }

        /*
         * Core (EC) Targets
         */
        l_beginning_offset = CORE_TARGET_OFFSET;
        for (uint32_t i = 0; i < CORE_TARGET_COUNT; ++i)
        {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_CORE>(i);
        }

        /*
         * MI Targets
         */
        l_beginning_offset = MI_TARGET_OFFSET;
        for (uint32_t i = 0; i < MI_TARGET_COUNT; ++i)
        {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_MI>(i);
        }

        /*
         * PHB Targets
         */
        l_beginning_offset = PHB_TARGET_OFFSET;
        for (uint32_t i = 0; i < PHB_TARGET_COUNT; ++i)
        {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_PHB>(i);
        }

        /*
         * PAU Targets
         */
        l_beginning_offset = PAU_TARGET_OFFSET;
        for (uint32_t i = 0; i < PAU_TARGET_COUNT; ++i)
        {
            const int pau_unit = (i == 0) ? i : i + 2;
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_PAU>(pau_unit);
        }

        /*
         * NMMU Targets
         */
        l_beginning_offset = NMMU_TARGET_OFFSET;
        for (uint32_t i = 0; i < NMMU_TARGET_COUNT; ++i)
        {
            G_vec_targets[l_beginning_offset + i] = createPlatTargetHandle<fapi2::TARGET_TYPE_NMMU>(i);
        }

        plat_OCMBTargetsInit(ocmbParam);

        // Debug Code - Don't Remove
        //for(uint32_t i=0;i< TARGET_COUNT;i++)
        //{
        //    FAPI_ERR("G_vec_targets.at[%d]=0x%.8x",i,(uint32_t)(G_vec_targets[i].value));
        //}

fapi_try_exit:
        return fapi2::current_err;
    }

    void plat_OCMBTargetsInit(uint8_t * ocmbParams)
    {
        do
        {
            if(ocmbParams == NULL)
            {
                SBE_ERROR("OCMB Parameters can not be NULL");
                break;
            }
            //Get the Engine from the ocmbParams.
            uint8_t engine = *(ocmbParams + PSU_REGS_OCMB_ENGINE_OFFSET);
            uint8_t devAddr = *(ocmbParams + PSU_REGS_OCMB_DEVADDR_OFFSET);
            uint16_t funcState = *((uint16_t *)(ocmbParams + PSU_REGS_OCMB_FUNCSTATE_OFFSET));
            FAPI_IMP("Engine : 0x%02X, Device Address: 0x%02X, Func State 0x%04X",
                  engine, devAddr, funcState);
            // Update the G_VEC for the OCMB targets.
            for(uint32_t i = 0; i < OCMB_TARGET_COUNT; i++ )
            {
                //Get the instance and port number; Instance:Port 4:4
                FAPI_IMP("OCMB Param[%d] is 0x%02X", i, *(ocmbParams + i));
                uint8_t instance = (*(ocmbParams + i)) >> 4;
                FAPI_IMP("instance is 0x%02X", instance);
                uint8_t functionalSate = (funcState >> (OCMB_TARGET_COUNT - i -1)) & 0x1;
                FAPI_IMP("functional state is 0x%02X", functionalSate);
                uint8_t port = (*(ocmbParams + i)) & 0xF;
                FAPI_IMP("port is 0x%02X", port);
                uint8_t targetType = PPE_TARGET_TYPE_OCMB & 0x0F;
                //create OCMB target.
                plat_target_handle ocmbTarget;
                ocmbTarget.value = 0;
                ocmbTarget.fields.chiplet_num = instance;
                ocmbTarget.fields.port = port;
                ocmbTarget.fields.engine = engine;
                ocmbTarget.fields.devAddr = devAddr;
                ocmbTarget.fields.functional = functionalSate;
                ocmbTarget.fields.type = targetType;
                FAPI_IMP("OCMB target created is 0x%08X", ocmbTarget);
                G_vec_targets[OCMB_TARGET_OFFSET + i] = ocmbTarget;
            }
            for(uint32_t i=0;i< TARGET_COUNT;i++)
            {
                FAPI_DBG("G_vec_targets.at[%d]=0x%.8x",i,(uint32_t)(G_vec_targets[i].value));
            }
        }while(0);
    }

#endif // __SBEFW_SEEPROM__
#if defined __SBEFW_PIBMEM__

    /// @brief Function to initialize the G_targets vector based on partial good
    ///        attributes
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> plat_getChipTarget()
    {

        // Get the chip specific target
        return ((fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>)G_vec_targets.at(CHIP_TARGET_OFFSET));
    }

#endif // __SBEFW_PIBMEM__

#if defined(__SBEMFW_MEASUREMENT__) || defined(__SBEVFW_VERIFICATION__)
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> plat_getChipTarget()
    {
         // Get the chip default specific target
         return ((fapi2::plat_target_handle_t)0x0);
    }
    TargetType plat_target_handle_t::getFapiTargetType() const
    {
         // Get the chip specific target
         return (TARGET_TYPE_PROC_CHIP);
    }
#endif // end of __SBEMFW_MEASUREMENT__ or __SBEVFW_VERIFICATION__

} // fapi2
