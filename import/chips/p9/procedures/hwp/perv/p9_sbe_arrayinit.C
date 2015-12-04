/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_arrayinit.C $             */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//------------------------------------------------------------------------------
/// @file  p9_sbe_arrayinit.C
///
/// @brief array init procedure to be called with any chiplet target except TP,EP,EC
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_arrayinit.H"

#include "p9_perv_scom_addresses.H"
#include "p9_perv_sbe_cmn.H"


enum P9_SBE_ARRAYINIT_Private_Constants
{
    LOOP_COUNTER = 0x0000000000042FFF,
    REGIONS_EXCEPT_VITAL = 0x7FF,
    REGIONS_FOR_PERV = 0x400,
    SCAN_TYPES_EXCEPT_TIME_GPTR_REPR = 0xDCF,
    SELECT_EDRAM = 0x0,
    SELECT_SRAM = 0x1,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

static fapi2::ReturnCode p9_sbe_arrayinit_scan0_and_arrayinit_module_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_arrayinit(const
                                   fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    for (auto l_chplt_trgt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        FAPI_INF("Call proc_sbe_arryinit_scan0_and_arrayinit_module_function");
        FAPI_TRY(p9_sbe_arrayinit_scan0_and_arrayinit_module_function(l_chplt_trgt));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief --Run arrayinit on all enabled chiplets
/// --Scan flush 0 to all rings except GPTR, Time, Repair on all enabled chiplets
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_arrayinit_scan0_and_arrayinit_module_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    bool l_read_reg = false;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    FAPI_INF("Check for chiplet enable");
    //Getting NET_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL0, l_data64));
    l_read_reg = l_data64.getBit<0>();  //l_read_reg = NET_CTRL0.CHIPLET_ENABLE

    if ( l_read_reg )
    {
        FAPI_INF("run array_init module for all chiplet except TP, EC, EP");
        FAPI_TRY(p9_perv_sbe_cmn_array_init_module(i_target_chiplet,
                 REGIONS_EXCEPT_VITAL, LOOP_COUNTER, SELECT_SRAM, SELECT_EDRAM,
                 START_ABIST_MATCH_VALUE));

        FAPI_INF("Check  for SRAM ABIST done");
        //Getting CPLT_STAT0 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_CPLT_STAT0, l_data64));
        //l_read_reg = CPLT_STAT0.SRAM_ABIST_DONE_DC
        l_read_reg = l_data64.getBit<0>();

        FAPI_ASSERT(l_read_reg,
                    fapi2::ABIST_DONE_ERR()
                    .set_READ_ABIST_DONE(l_read_reg),
                    "ERROR:ABIST DONE BIT NOT SET ");

        FAPI_INF("run scan0 module for region except vital and scan types except GPTR TIME REPR all chiplets except TP, EC, EP");
        FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chiplet, REGIONS_EXCEPT_VITAL,
                                              SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
