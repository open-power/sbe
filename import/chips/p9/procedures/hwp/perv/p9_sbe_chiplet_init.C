/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_chiplet_init.C $          */
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
/// @file  p9_sbe_chiplet_init.C
///
/// @brief init procedure for all enabled chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_init.H"

#include "p9_perv_scom_addresses.H"


fapi2::ReturnCode p9_sbe_chiplet_init(const
                                      fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    bool l_read_reg = false;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_init: Entering..");

    FAPI_DBG("Check  for XSTOP Bit");
    //Getting INTERRUPT_TYPE_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PIB_INTERRUPT_TYPE_REG, l_data64));
    //l_read_reg = PIB.INTERRUPT_TYPE_REG.CHECKSTOP
    l_read_reg = l_data64.getBit<2>();

    FAPI_ASSERT(!(l_read_reg),
                fapi2::CHECKSTOP_ERR()
                .set_READ_CHECKSTOP(l_read_reg),
                "ERROR:CHECKSTOP BIT GET SET ");

    FAPI_INF("p9_sbe_chiplet_init: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}


