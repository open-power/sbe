/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_tp_chiplet_reset.C $      */
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
/// @file  p9_sbe_tp_chiplet_reset.C
///
/// @brief setup hangcounter 6 for TP chiplet
//------------------------------------------------------------------------------
// *HWP HWP Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HWP Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner         : Brian Silver <bsilver@us.ibm.com>
// *HWP Team             : Perv
// *HWP Level            : 2
// *HWP Consumed by      : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_chiplet_reset.H"

#include "p9_perv_scom_addresses.H"


fapi2::ReturnCode p9_sbe_tp_chiplet_reset(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_DBG("p9_sbe_tp_chiplet_reset: Entering ...");

    FAPI_DBG("Initializing Hangcounter 6 for PRV Cplt");
    //Setting HANG_PULSE_6_REG register value
    //PERV.HANG_PULSE_6_REG = HANG_PULSE_VALUE
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_6_REG,
                            HANG_PULSE_VALUE));

    FAPI_DBG("p9_sbe_tp_chiplet_reset: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
