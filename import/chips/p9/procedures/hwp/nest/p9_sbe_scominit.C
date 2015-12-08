/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/nest/p9_sbe_scominit.C $              */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//------------------------------------------------------------------------------
/// @file  p9_sbe_scominit.C
///
/// @brief This procedure contains SCOM based initialization required for
///  fabric configuration & HBI operation
/// *!
/// *!   o Set fabric node/chip ID configuration for all configured
/// *!     chiplets to chip specific values
/// *!   o Establish ADU XSCOM BAR for HBI operation
//------------------------------------------------------------------------------
// *HWP HW Owner        : Girisankar Paulraj <gpaulraj@in.ibm.com>
// *HWP HW Backup Owner : Joe McGill <jmcgill@us.ibm.com>
// *HWP FW Owner         : Thi N. Tran <thi@us.ibm.com>
// *HWP Team             : Nest
// *HWP Level            : 2
// *HWP Consumed by      : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_scominit.H"

#include "p9_misc_scom_addresses.H"
#include "p9_perv_scom_addresses.H"


static fapi2::ReturnCode p9_sbe_scominit_chiplet_cnfg(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
        const i_id_struct& iv_id_struct);

fapi2::ReturnCode p9_sbe_scominit(const
                                  fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    i_id_struct l_id_struct;
    uint64_t l_attr_xscom_bar_addr = 0;
    fapi2::buffer<uint64_t> l_data64;
    auto l_perv_functional_vector = i_target.getChildren<fapi2::TARGET_TYPE_PERV>
                                    (fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_DBG("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_CHIP_ID, i_target,
                           l_id_struct.iv_chip_id));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID, i_target,
                           l_id_struct.iv_group_id));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_SYSTEM_ID, i_target,
                           l_id_struct.iv_system_id));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ADU_XSCOM_BAR_BASE_ADDR, i_target,
                           l_attr_xscom_bar_addr));

    for (auto l_target_chplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_chplt,
                               l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos & 0xF0) == 0)
        {
            FAPI_TRY(p9_sbe_scominit_chiplet_cnfg(l_target_chplt, l_id_struct));
        }
    }

    // Clearing and masking the Local Power bus, Fabric bus and N3 chiplet FIRs/MASK/ACTION registers
    //Setting FIR_MASK register value
    //N3.FIR_MASK = p9SbeScominit::FIRMASK_RESET_VALUE
    FAPI_TRY(fapi2::putScom(i_target, 0x05040002,
                            p9SbeScominit::FIRMASK_RESET_VALUE));
    //Setting LOCAL_FIR_ACTION0 register value
    //N3.LOCAL_FIR_ACTION0 = p9SbeScominit::FIRACT_RESET_VALUE
    FAPI_TRY(fapi2::putScom(i_target, PERV_N3_LOCAL_FIR_ACTION0,
                            p9SbeScominit::FIRACT_RESET_VALUE));
    //Setting LOCAL_FIR_ACTION1 register value
    //N3.LOCAL_FIR_ACTION1 = p9SbeScominit::FIRACT_RESET_VALUE
    FAPI_TRY(fapi2::putScom(i_target, PERV_N3_LOCAL_FIR_ACTION1,
                            p9SbeScominit::FIRACT_RESET_VALUE));
    //Setting XFIR register value
    //N3.XFIR = p9SbeScominit::FIR_RESET_VALUE
    FAPI_TRY(fapi2::putScom(i_target, 0x05040000, p9SbeScominit::FIR_RESET_VALUE));
    //Setting PBAFIRMASK register value
    //N3.PBAFIRMASK = p9SbeScominit::FIRMASK_RESET_VALUE
    FAPI_TRY(fapi2::putScom(i_target, PU_PBAFIRMASK,
                            p9SbeScominit::FIRMASK_RESET_VALUE));
    //Setting PBAFIRACT0 register value
    //N3.PBAFIRACT0 = p9SbeScominit::FIRACT_RESET_VALUE
    FAPI_TRY(fapi2::putScom(i_target, PU_PBAFIRACT0,
                            p9SbeScominit::FIRACT_RESET_VALUE));
    //Setting PBAFIRACT1 register value
    //N3.PBAFIRACT1 = p9SbeScominit::FIRACT_RESET_VALUE
    FAPI_TRY(fapi2::putScom(i_target, PU_PBAFIRACT1,
                            p9SbeScominit::FIRACT_RESET_VALUE));
    //Setting PBAFIR register value
    //N3.PBAFIR = p9SbeScominit::FIR_RESET_VALUE
    FAPI_TRY(fapi2::putScom(i_target, PU_PBAFIR, p9SbeScominit::FIR_RESET_VALUE));

    // Setting ADU XSCOM BASE ADDR register value
    l_data64 = l_attr_xscom_bar_addr;
    l_data64.clearBit<0, 8>();
    l_data64.clearBit<30, 34>();

    FAPI_TRY(fapi2::putScom(i_target, PU_XSCOM_BASE_REG, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief This function configures Fabric chip/system/group ID for functioning chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_id_struct     This structure contains the following parameter
///      i_chip_id --> current chiplet Fabric chip id
///      i_group_id --> current chiplet Fabric group id
///      i_system_id --> current chiplet Fabric system id
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_scominit_chiplet_cnfg(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
        const i_id_struct& i_id_struct)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    //Setting CPLT_CONF0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_CPLT_CONF0, l_data64));
    //CPLT_CONF0.TC_UNIT_GROUP_ID_DC = i_id_struct.iv_group_id
    l_data64.insertFromRight<48, 4>(i_id_struct.iv_group_id);
    //CPLT_CONF0.TC_UNIT_CHIP_ID_DC = i_id_struct.iv_chip_id
    l_data64.insertFromRight<52, 3>(i_id_struct.iv_chip_id);
    //CPLT_CONF0.TC_UNIT_SYS_ID_DC = i_id_struct.iv_system_id
    l_data64.insertFromRight<56, 5>(i_id_struct.iv_system_id);
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CPLT_CONF0, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

i_id_struct::i_id_struct() : iv_chip_id(0), iv_group_id(0), iv_system_id(0)
{
}
