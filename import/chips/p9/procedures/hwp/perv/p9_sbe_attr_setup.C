/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_attr_setup.C $            */
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
/// @file  p9_sbe_attr_setup.C
///
/// @brief Read scratch Regs, update ATTR
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner         : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team             : Perv
// *HWP Level            : 2
// *HWP Consumed by      : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_attr_setup.H"

#include "p9_perv_scom_addresses.H"


fapi2::ReturnCode p9_sbe_attr_setup(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_scratch_reg = 0;
    fapi2::buffer<uint64_t> l_read_scratch8 = 0;
    uint8_t l_read_1 = 0;
    uint8_t l_read_2 = 0;
    uint32_t l_read_3 = 0;
    uint32_t l_read_4 = 0;
    uint32_t l_read_5 = 0;
    FAPI_DBG("Entering ...");

    FAPI_INF("Read Scratch8 for validity of Scratch register");
    //Getting SCRATCH_REGISTER_8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_8_SCOM,
                            l_read_scratch8)); //l_read_scratch8 = PIB.SCRATCH_REGISTER_8

    //read_scratch1_reg
    {
        if ( l_read_scratch8.getBit<0>() )
        {
            //Getting SCRATCH_REGISTER_1 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_1_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_1

            l_read_scratch_reg.extractToRight<0, 8>(l_read_1);
            l_read_scratch_reg.extractToRight<8, 24>(l_read_4);

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_EQ_GARD, i_target_chip, l_read_1));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_EC_GARD, i_target_chip, l_read_4));
        }
    }
    //read_scratch2_reg
    {
        if ( l_read_scratch8.getBit<1>() )
        {
            //Getting SCRATCH_REGISTER_2 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_2_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_2

            l_read_scratch_reg.extractToRight<0, 16>(l_read_3);

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_I2C_BUS_DIV_REF, i_target_chip, l_read_3));
        }
    }
    //read_scratch3_reg
    {
        if ( l_read_scratch8.getBit<2>() )
        {
            //Getting SCRATCH_REGISTER_3 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_3_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_3

            l_read_scratch_reg.extractToRight<0, 26>(l_read_4);
            l_read_scratch_reg.extractToRight<26, 3>(l_read_1);
            l_read_scratch_reg.extractToRight<29, 3>(l_read_2);

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_BOOT_FLAGS, i_target_chip, l_read_4));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NODE_POS, i_target_chip, l_read_1));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CHIP_POS, i_target_chip, l_read_2));
        }
    }
    //read_scratch4_reg
    {
        if ( l_read_scratch8.getBit<3>() )
        {
            //Getting SCRATCH_REGISTER_4 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_4_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_4

            l_read_scratch_reg.extractToRight<0, 16>(l_read_3);
            l_read_scratch_reg.extractToRight<24, 8>(l_read_1);

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_BOOT_FREQ, i_target_chip, l_read_3));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_NEST_PLL_BUCKET, i_target_chip, l_read_1));
        }
    }
    //read_scratch5_reg
    {
        if ( l_read_scratch8.getBit<4>() )
        {
            //Getting SCRATCH_REGISTER_5 register value
            FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_5_SCOM,
                                    l_read_scratch_reg)); //l_read_scratch_reg = PIB.SCRATCH_REGISTER_5

            l_read_scratch_reg.extractToRight<0, 16>(l_read_3);
            l_read_scratch_reg.extractToRight<16, 16>(l_read_5);

            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_VCS_BOOT_VOLTAGE, i_target_chip, l_read_3));
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_VDD_BOOT_VOLTAGE, i_target_chip, l_read_5));
        }
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
