/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: hwpf/src/plat/target.C $                                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2016                        */
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

namespace fapi2
{

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

        // Find the PERV target number in the partial good initialization
        // array
        FAPI_TRY(plat_PervPGTargets(i_chiplet_target, b_present));

        if (b_present)
        {
            i_chiplet_target.setPresent();
            i_chiplet_target.setFunctional(true);
        }
        else
        {
            FAPI_DBG("Perv target NOT present (nor functional): chiplet_number = %d",
                        i_chiplet_target.getChipletNumber());
        }

        FAPI_DBG("Target present = %u, Target functional = %u",
            i_chiplet_target.getPresent(),
            i_chiplet_target.getFunctional());

fapi_try_exit:
        return fapi2::current_err;
    }


    /// @brief Function to initialize the G_targets vector based on partial good
    ///      attributes ///  this will move to plat_target.H formally
    fapi2::ReturnCode plat_TargetsInit()
    {
        bool b_present = false;

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

        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> chip_target((fapi2::plat_target_handle_t)0);
        G_vec_targets.at(l_beginning_offset) = revle32((fapi2::plat_target_handle_t)(chip_target.get()));

        /*
         * Nest Targets - group 1
         */
        l_beginning_offset = NEST_GROUP1_TARGET_OFFSET;
        for (uint32_t i = 0; i < NEST_GROUP1_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PERV> target_name((fapi2::plat_target_handle_t)i);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(target_name, b_present));

            G_vec_targets.at(l_beginning_offset+i) = revle32((fapi2::plat_target_handle_t)(target_name.get()));
        }

        /*
         * Memory Controller Synchronous (MCBIST) Targets
         */

        l_beginning_offset = MCBIST_TARGET_OFFSET;
        for (uint32_t i = 0; i < MCBIST_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_MCBIST> target_name((fapi2::plat_target_handle_t)i);
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv = target_name.getParent<fapi2::TARGET_TYPE_PERV>();

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(l_perv, b_present));

            G_vec_targets.at(l_beginning_offset+i) = revle32((fapi2::plat_target_handle_t)(l_perv.get()));

        }

        /*
         * Nest Targets - group 2
         */
        l_beginning_offset = NEST_GROUP2_TARGET_OFFSET;
        for (uint32_t i = NEST_GROUP2_TARGET_OFFSET;
                i < (NEST_GROUP2_TARGET_OFFSET + NEST_GROUP2_TARGET_COUNT); ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PERV> target_name((fapi2::plat_target_handle_t)(i - 1));

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(target_name, b_present));

            G_vec_targets.at(i) = revle32((fapi2::plat_target_handle_t)(target_name.get()));
        }

        /*
         * Cache (EQ) Targets
         */
        l_beginning_offset = EQ_TARGET_OFFSET;
        for (uint32_t i = 0; i < EQ_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_EQ> target_name((fapi2::plat_target_handle_t)i);
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv = target_name.getParent<fapi2::TARGET_TYPE_PERV>();

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(l_perv, b_present));

            G_vec_targets.at(l_beginning_offset+i) = revle32((fapi2::plat_target_handle_t)(l_perv.get()));
        }

        /*
         * Core (EC) Targets
         */

        l_beginning_offset = CORE_TARGET_OFFSET;
        for (uint32_t i = 0; i < CORE_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_CORE> target_name((fapi2::plat_target_handle_t)i);
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv = target_name.getParent<fapi2::TARGET_TYPE_PERV>();

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(l_perv, b_present));

            G_vec_targets.at(l_beginning_offset+i) = revle32((fapi2::plat_target_handle_t)(l_perv.get()));
        }

        /*
         * EX Targets
         */

        l_beginning_offset = EX_TARGET_OFFSET;
        for (uint32_t i = 0; i < EX_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_EX> target_name((fapi2::plat_target_handle_t)i);

            fapi2::Target<fapi2::TARGET_TYPE_EQ> l_parent = target_name.getParent<fapi2::TARGET_TYPE_EQ>();

            // Get the parent EQ's ATTR_PG
            uint32_t l_eqAttrPg = 0;
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
                target_name.setPresent();
                target_name.setFunctional(true);
            }
            G_vec_targets.at(l_beginning_offset+i) = revle32((fapi2::plat_target_handle_t)(target_name.get()));
        }

        /*
         * MCS Targets
         */

        l_beginning_offset = MCS_TARGET_OFFSET;
        for (uint32_t i = 0; i < MCS_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_MCS> target_name((fapi2::plat_target_handle_t)i);

            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_nestTarget((plat_getTargetHandleByChipletNumber(N3_CHIPLET - (MCS_PER_MCBIST * (i / MCS_PER_MCBIST)))));

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
                target_name.setPresent();
                target_name.setFunctional(true);
            }

            G_vec_targets.at(l_beginning_offset+i) = revle32((fapi2::plat_target_handle_t)(target_name.get()));
        }


fapi_try_exit:
        return fapi2::current_err;
    }

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
                l_target.setFunctional(false);
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
                l_target.setFunctional(false);
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

    /// @brief Function to return a platform target handle, given the chiplet
    //         number
    //  @param i_chipletNumber The chiplet number of the target
    //  @return Platform target handle
    //  @note The caller can use the platform target handle to construct a
    //        Target of it's choice. Ex:
    //  fapi2::Target<fapi2::TARGET_TYPE_CORE>
    //  l_core(plat_getTargetHandleByChipletNumber(0x20);
    plat_target_handle_t plat_getTargetHandleByChipletNumber(
            const uint8_t i_chipletNumber)
    {
        assert(((i_chipletNumber > 0) &&
                (i_chipletNumber < (EQ_CHIPLET_OFFSET + EQ_TARGET_COUNT))) ||
                ((i_chipletNumber >= CORE_CHIPLET_OFFSET) &&
                (i_chipletNumber < (CORE_CHIPLET_OFFSET + CORE_TARGET_COUNT))));

        uint32_t l_idx = 0;

        if(i_chipletNumber < (EQ_CHIPLET_OFFSET + EQ_TARGET_COUNT))
        {
            l_idx = (i_chipletNumber - NEST_GROUP1_CHIPLET_OFFSET) +
                NEST_GROUP1_TARGET_OFFSET;
        }
        else
        {
            l_idx = (i_chipletNumber - CORE_CHIPLET_OFFSET) +
                CORE_TARGET_OFFSET;
        }
        return G_vec_targets[l_idx];
    }

} // fapi2
