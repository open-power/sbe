/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_tp_enable_ridi.C $        */
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
/// @file  p9_sbe_tp_enable_ridi.C
///
/// @brief enables ridi bits in RC regs after scan initialize and start clock the pervasive chiplet
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner         : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team             : Perv
// *HWP Level            : 2
// *HWP Consumed by      : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_enable_ridi.H"

#include "perv_scom_addresses.H"


fapi2::ReturnCode p9_sbe_tp_enable_ridi(const
                                        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    FAPI_INF("Enable Recievers, Drivers DI1 & DI2");
    //Setting ROOT_CTRL1 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL1_SCOM, l_data64));
    l_data64.setBit<19>();  //PIB.ROOT_CTRL1.TP_RI_DC_B = 1
    l_data64.setBit<20>();  //PIB.ROOT_CTRL1.TP_DI1_DC_B = 1
    l_data64.setBit<21>();  //PIB.ROOT_CTRL1.TP_DI2_DC_B = 1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL1_SCOM, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
