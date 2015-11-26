/* IBM_PROLOG_BEGIN_TAG                                                 */
/* This is an automatically generated prolog.                           */
/*                                                                      */
/* $Source: $                                                           */
/*                                                                      */
/* OpenPOWER HostBoot Project                                           */
/*                                                                      */
/* Contributors Listed Below - COPYRIGHT 2012,2014                      */
/* [+] International Business Machines Corp.                            */
/*                                                                      */
/*                                                                      */
/* Licensed under the Apache License, Version 2.0 (the "License");      */
/* you may not use this file except in compliance with the License.     */
/* You may obtain a copy of the License at                              */
/*                                                                      */
/* http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                      */
/* Unless required by applicable law or agreed to in writing, software  */
/* distributed under the License is distributed on an "AS IS" BASIS,    */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      */
/* implied. See the License for the specific language governing         */
/* permissions and limitations under the License.                       */
/*                                                                      */
/* IBM_PROLOG_END_TAG                                                   */


#include <fapi2.H>
#include <plat_target_pg_attributes.H>

// Global Vector containing ALL targets.  This structure is referenced by
// fapi2::getChildren to produce the resultant returned vector from that
// call.
std::vector<fapi2::plat_target_handle_t> G_vec_targets;

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


    // Not a fan of the switch technique;  I would prefer an array lookup
    // but the attritute lookup is done via compile time macros that are
    // resolved to attribute Ids (hashes).
     fapi2::ReturnCode plat_PervPGTargets(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target,
                                          const fapi2::TargetTypes_t i_chiplet_num,
                                          bool & o_present)
     {

        o_present = false;
        uint16_t attr_value = 0;
        switch (i_chiplet_num)
        {
            case 0x00:   // Nop
                break;
            case 0x01:
                FAPI_ATTR_GET(ATTR_PG_PRV, i_target, *(&attr_value));
                FAPI_DBG("ATTR_PG_PRV value = %x", attr_value);
                break;
            case 0x02:
                FAPI_ATTR_GET(ATTR_PG_N0, i_target, attr_value);
                FAPI_DBG("ATTR_PG_N0 value = %x", attr_value);
                break;
            case 0x03:
                FAPI_ATTR_GET(ATTR_PG_N1, i_target, attr_value);
                FAPI_DBG("ATTR_PG_N1 value = %x", attr_value);
                break;
            case 0x04:
                FAPI_ATTR_GET(ATTR_PG_N2, i_target, attr_value);
                FAPI_DBG("ATTR_PG_N2 value = %x", attr_value);
                break;
            case 0x05:
                FAPI_ATTR_GET(ATTR_PG_N3, i_target, attr_value);
                FAPI_DBG("ATTR_PG_N0 value = %x", attr_value);
                break;
            case 0x06:
                FAPI_ATTR_GET(ATTR_PG_XB, i_target, attr_value);
                FAPI_DBG("ATTR_PG_XB value = %x", attr_value);
                break;
            case 0x07:
                FAPI_ATTR_GET(ATTR_PG_MC01, i_target, attr_value);
                FAPI_DBG("ATTR_PG_MC01 value = %x", attr_value);
                break;
            case 0x08:
                FAPI_ATTR_GET(ATTR_PG_MC23, i_target, attr_value);
                FAPI_DBG("ATTR_PG_MC23 value = %x", attr_value);
                break;
            case 0x09:
                FAPI_ATTR_GET(ATTR_PG_OB0, i_target, attr_value);
                FAPI_DBG("ATTR_PG_OB0 value = %x", attr_value);
                break;
            case 0x0A:
                FAPI_ATTR_GET(ATTR_PG_OB1, i_target, attr_value);
                FAPI_DBG("ATTR_PG_OB1 value = %x", attr_value);
                break;
            case 0x0B:
                FAPI_ATTR_GET(ATTR_PG_OB2, i_target, attr_value);
                FAPI_DBG("ATTR_PG_OB2 value = %x", attr_value);
                break;
            case 0x0C:
                FAPI_ATTR_GET(ATTR_PG_OB3, i_target, attr_value);
                FAPI_DBG("ATTR_PG_OB3 value = %x", attr_value);
                break;
            case 0x0D:
                FAPI_ATTR_GET(ATTR_PG_PCI0, i_target, attr_value);
                FAPI_DBG("ATTR_PG_PCI0 value = %x", attr_value);
                break;
            case 0x0E:
                FAPI_ATTR_GET(ATTR_PG_PCI1, i_target, attr_value);
                FAPI_DBG("ATTR_PG_PCI1 value = %x", attr_value);
                break;
            case 0x0F:
                FAPI_ATTR_GET(ATTR_PG_PCI2, i_target, attr_value);
                FAPI_DBG("ATTR_PG_PCI2 value = %x", attr_value);
                break;
            case 0x10:
                FAPI_ATTR_GET(ATTR_PG_EQ0, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EQ0 value = %x", attr_value);
                break;
            case 0x11:
                FAPI_ATTR_GET(ATTR_PG_EQ1, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EQ1 value = %x", attr_value);
                break;
            case 0x12:
                FAPI_ATTR_GET(ATTR_PG_EQ2, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EQ2 value = %x", attr_value);
                break;
            case 0x13:
                FAPI_ATTR_GET(ATTR_PG_EQ3, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EQ3 value = %x", attr_value);
                break;
            case 0x14:
                FAPI_ATTR_GET(ATTR_PG_EQ4, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EQ4 value = %x", attr_value);
                break;
            case 0x15:
                FAPI_ATTR_GET(ATTR_PG_EQ5, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EQ5 value = %x", attr_value);
                break;
            case 0x20:
                FAPI_ATTR_GET(ATTR_PG_EC00, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC00 value = %x", attr_value);
                break;
            case 0x21:
                FAPI_ATTR_GET(ATTR_PG_EC01, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC01 value = %x", attr_value);
                break;
            case 0x22:
                FAPI_ATTR_GET(ATTR_PG_EC02, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC02 value = %x", attr_value);
                break;
            case 0x23:
                FAPI_ATTR_GET(ATTR_PG_EC03, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC03 value = %x", attr_value);
                break;
            case 0x24:
                FAPI_ATTR_GET(ATTR_PG_EC04, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC04 value = %x", attr_value);
                break;
            case 0x25:
                FAPI_ATTR_GET(ATTR_PG_EC05, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC05 value = %x", attr_value);
                break;
            case 0x26:
                FAPI_ATTR_GET(ATTR_PG_EC06, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC06 value = %x", attr_value);
                break;
            case 0x27:
                FAPI_ATTR_GET(ATTR_PG_EC07, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC07 value = %x", attr_value);
                break;
            case 0x28:
                FAPI_ATTR_GET(ATTR_PG_EC08, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC08 value = %x", attr_value);
                break;
            case 0x29:
                FAPI_ATTR_GET(ATTR_PG_EC09, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC09 value = %x", attr_value);
                break;
            case 0x2A:
                FAPI_ATTR_GET(ATTR_PG_EC10, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC10 value = %x", attr_value);
                break;
            case 0x2B:
                FAPI_ATTR_GET(ATTR_PG_EC11, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC11 value = %x", attr_value);
                break;
            case 0x2C:
                FAPI_ATTR_GET(ATTR_PG_EC12, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC12 value = %x", attr_value);
                break;
            case 0x2D:
                FAPI_ATTR_GET(ATTR_PG_EC13, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC13 value = %x", attr_value);
                break;
            case 0x2E:
                FAPI_ATTR_GET(ATTR_PG_EC14, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC14 value = %x", attr_value);
                break;
            case 0x2F:
                FAPI_ATTR_GET(ATTR_PG_EC15, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC15 value = %x", attr_value);
                break;
            case 0x30:
                FAPI_ATTR_GET(ATTR_PG_EC16, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC16 value = %x", attr_value);
                break;
            case 0x31:
                FAPI_ATTR_GET(ATTR_PG_EC17, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC17 value = %x", attr_value);
                break;
            case 0x32:
                FAPI_ATTR_GET(ATTR_PG_EC18, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC18 value = %x", attr_value);
                break;
            case 0x33:
                FAPI_ATTR_GET(ATTR_PG_EC19, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC19 value = %x", attr_value);
                break;
            case 0x34:
                FAPI_ATTR_GET(ATTR_PG_EC20, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC20 value = %x", attr_value);
                break;
            case 0x35:
                FAPI_ATTR_GET(ATTR_PG_EC21, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC21 value = %x", attr_value);
                break;
            case 0x36:
                FAPI_ATTR_GET(ATTR_PG_EC22, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC22 value = %x", attr_value);
                break;
            case 0x37:
                FAPI_ATTR_GET(ATTR_PG_EC23, i_target, attr_value);
                FAPI_DBG("ATTR_PG_EC23 value = %x", attr_value);
                break;
            default:
                FAPI_ERR("PervPGTargets:  invalid chiplet number %u", i_chiplet_num);
        }

        if (attr_value & 0xC000)
        {
            o_present = true;
        }

        return fapi2::FAPI2_RC_SUCCESS;

    }

    /// @brief Function to determine if pervsaive target within a chip is
    ///     present and, thus, considered functional per PG attributes
    template<fapi2::TargetType K>
    fapi2::ReturnCode
    plat_TargetPresent( fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_chip_target,
                        fapi2::Target<K> & i_chiplet_target,
                        bool & b_present)
    {

        // Find the PERV target number in the partial good initialization
        // array
        fapi2::ChipletNumber_t chiplet_number = i_chiplet_target.getChipletNumber();

        FAPI_TRY(plat_PervPGTargets(i_chip_target, chiplet_number, b_present));

        if (b_present)
        {
            i_chiplet_target.setPresent();
            i_chiplet_target.setFunctional(true);
        }
        else
        {
            FAPI_DBG("Perv target NOT present (nor functional): chiplet_number = %d",
                        chiplet_number);
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
        uint32_t c = 0;

        // Initialise global attribute pointers
        G_system_attributes_ptr = &G_system_attributes;
        G_proc_chip_attributes_ptr = &G_proc_chip_attributes;
        G_perv_attributes_ptr = &G_perv_attributes;
        G_core_attributes_ptr = &G_core_attributes;
        G_eq_attributes_ptr = &G_eq_attributes;
        G_ex_attributes_ptr = &G_ex_attributes;


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
        G_vec_targets.at(l_beginning_offset) = revle64((fapi2::plat_target_handle_t)(chip_target.get()));

        /*
         * Nest Targets - group 1
         */
        l_beginning_offset = NEST_GROUP1_TARGET_OFFSET;
        for (uint32_t i = 0; i < NEST_GROUP1_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_PERV> target_name((fapi2::plat_target_handle_t)i);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(chip_target, target_name, b_present));

            G_vec_targets.at(l_beginning_offset+i) = revle64((fapi2::plat_target_handle_t)(target_name.get()));
        }

        /*
         * Memory Controller Synchronous (MCS) Targets
         */

        l_beginning_offset = MCS_TARGET_OFFSET;
        for (uint32_t i = 0; i < MCS_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_MCS> target_name((fapi2::plat_target_handle_t)i);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(chip_target, target_name, b_present));

            G_vec_targets.at(l_beginning_offset+i) = revle64((fapi2::plat_target_handle_t)(target_name.get()));

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
            FAPI_TRY(plat_TargetPresent(chip_target, target_name, b_present));

            G_vec_targets.at(i) = revle64((fapi2::plat_target_handle_t)(target_name.get()));
        }

        /*
         * Cache (EQ) Targets
         */
        l_beginning_offset = EQ_TARGET_OFFSET;
        for (uint32_t i = 0; i < EQ_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_EQ> target_name((fapi2::plat_target_handle_t)i);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(chip_target, target_name, b_present));

            G_vec_targets.at(l_beginning_offset+i) = revle64((fapi2::plat_target_handle_t)(target_name.get()));
        }

        /*
         * Core (EC) Targets
         */

        l_beginning_offset = CORE_TARGET_OFFSET;
        for (uint32_t i = 0; i < CORE_TARGET_COUNT; ++i)
        {
            fapi2::Target<fapi2::TARGET_TYPE_CORE> target_name((fapi2::plat_target_handle_t)i);

            // Determine if the chiplet is present and, thus, functional
            // via partial good attributes
            FAPI_TRY(plat_TargetPresent(chip_target, target_name, b_present));

            G_vec_targets.at(l_beginning_offset+i) = revle64((fapi2::plat_target_handle_t)(target_name.get()));
        }

        // Trace all entries
        for (auto tgt_iter : G_vec_targets)
        {
            FAPI_DBG("Trace hi word G_vec_targets[%u] value=%08X",
                       c, (uint32_t)(tgt_iter>>32));
            ++c;
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
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = plat_getChipTarget();

        // Read the EQ and EC gard attributes from the chip target
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_EQ_GARD, l_chip, l_eqGards));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_EC_GARD, l_chip, l_ecGards));

        FAPI_DBG("ATTR_EQ_GARD:: 0x%08x", l_eqGards);
        FAPI_DBG("ATTR_EC_GARD:: 0x%08x", l_ecGards);

        // Normalize the values read from the attributes (they are right aligned)
        l_eqGards <<= ((sizeof(l_eqGards) * 8) - EQ_TARGET_COUNT);
        l_ecGards <<= ((sizeof(l_ecGards) * 8) - CORE_TARGET_COUNT);

        // Iterate over the bits in EQ and EC gards, if set, mark the
        // corresponding target non-functional
        // TODO: Need to mark corresponding EX targets non-functional when we
        // start supporting EX targets in the global target vector
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
                // EC chiplet l_idx is to be marked non-functional
                fapi2::Target<fapi2::TARGET_TYPE_CORE> l_target = G_vec_targets.at(l_idx + CORE_TARGET_OFFSET);
                l_target.setFunctional(false);
                G_vec_targets.at(l_idx + CORE_TARGET_OFFSET) = l_target.get();
            }
        }
fapi_try_exit:
        return fapi2::current_err;
    }

} // fapi2
