/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/utils/p10_putRingUtils.C $               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
/// @file   p10_putRingUtils.C
/// @brief  Headers and Constants used by rs4 decompression and
/// ring SCAN/ROTATE functionality
///
// *HWP HW Owner:       Greg Still  <stillgs@us.ibm.com>
// *HWP FW Owner:       Prem S Jha  <premjha2@in.ibm.com>
// *HWP Team:           PM
// *HWP Level:          2
// *HWP Consumed by:    SBE

// *INDENT-OFF*
#include <p10_putRingUtils.H>
#include <p10_scom_eq_2.H>
#include <p10_scom_perv_0.H>
#include <p10_scom_perv_1.H>
#include <p10_scom_perv_7.H>
#include <p10_scom_perv_8.H>
#include <p10_scom_perv_2.H>
#include <p10_scom_perv_d.H>
#include <p10_scom_perv_e.H>
#include <p10_scom_perv_4.H>
#include <p10_scom_perv_c.H>
#include <p10_scan_compression.H>
#include <p10_hcd_common.H>

#ifdef __PUTRING_TEST_
#include <map>
#endif

namespace RS4
{

enum
{
    OPCG_ALIGN_ADDRESS          =   0x00030001,
    OPCG_REG0_ADDRESS           =   0x00030002,
    OPCG_REG1_ADDRESS           =   0x00030003,
    OPCG_REG2_ADDRESS           =   0x00030004,
    SCAN_REGION_TYPE_ADDRESS    =   0x00030005,
    CLK_REGION_ADDRESS          =   0x00030006,
    OPCG_CAPT1_ADDRESS          =   0x00030010,
    OPCG_CAPT2_ADDRESS          =   0x00030011,
    OPCG_CAPT3_ADDRESS          =   0x00030012,
    ROTATE_ADDRESS_REG          =   0x00039000,
    ROTATE_ADDRESS_NSL_REG      =   0x0003A000,
    ROTATE_ADDRESS_SL_REG       =   0x0003C000,
    SCAN64_ADDRESS              =   0x0003E000,
    CHECK_WORD_REG_ADDRESS      =   0x0003F040,
    NET_CTRL0_WAND              =   0x000F0041,
    NET_CTRL0_WOR               =   0x000F0042,
    MAX_RING_LIST               =   512,
    SCAN_HEADER_DATA            =   0xa5a5a5a5a5a5a5a5ull,
    MASK_RESERVE_REGION_BIT     =   0x0FFFFFFFFFFFFFFFull,
    SUPER_CHIPLET_BASE_ID       =   0x20,
    SUPER_CHIPLET_MAX_ID        =   0x27,
    SELECT_ALL_ECL              =   0x07800000,
    SELECT_ALL_L3               =   0x00780000,
    ENABLE_PARALLEL_SCAN        =   0x40000000,
    SHIFT_TO_BIT_ECL0           =   8,
    SHIFT_TO_BIT_L30            =   12,
    SHIFT_TO_BIT_MMA0           =   18,
    SCAN_REGION_ECL0            =   0x04000000,
    SCAN_REGION_L30             =   0x00400000,
    SCAN_REGION_MMA0            =   0x00010000,
    CHIPLET_MASK                =   0xff000000,
    SUPER_CHIPLET_MASK          =   0x20000000,
    MULTICAST_BIT               =   0x40000000,
};

}// namespace RS4

#ifdef __PUTRING_TEST_
std::map <uint8_t, uint64_t> g_ringScanRegionMap;
#endif

void initScanRegionTest()
{

#ifdef __PUTRING_TEST_

    g_ringScanRegionMap[perv_fure]              =   0x0800000000009000ul;
    g_ringScanRegionMap[perv_occ_gptr]          =   0x0900000000002000ul;
    g_ringScanRegionMap[perv_occ_repr]          =   0x0900000000002000ul;
    g_ringScanRegionMap[perv_occ_time]          =   0x0900000000000100ul;
    g_ringScanRegionMap[sbe_fure]               =   0x0400000000009000ul;
    g_ringScanRegionMap[sbe_gptr]               =   0x0400000000002000ul;
    g_ringScanRegionMap[sbe_repr]               =   0x0400000000000200ul;
    g_ringScanRegionMap[sbe_time]               =   0x0400000000000100ul;
    g_ringScanRegionMap[occ_fure]               =   0x0100000000009000ul;
    g_ringScanRegionMap[perv_dpll_func]         =   0x000C000000008000ul;
    g_ringScanRegionMap[perv_dpll_bndy]         =   0x000C000000000080ul;
    g_ringScanRegionMap[perv_dpll_gptr]         =   0x000C000000002000ul;
    g_ringScanRegionMap[perv_pll_func]          =   0x0002000000008000ul;
    g_ringScanRegionMap[perv_pll_bndy]          =   0x0002000000000080ul;
    g_ringScanRegionMap[perv_pll_gptr]          =   0x0002000000002000ul;

    g_ringScanRegionMap[n0_fure]                =   0x0C80000000009000ul;
    g_ringScanRegionMap[n0_gptr]                =   0x0C80000000002000ul;
    g_ringScanRegionMap[n0_repr]                =   0x0C80000000000200ul;
    g_ringScanRegionMap[n0_time]                =   0x0C80000000000100ul;
    g_ringScanRegionMap[n1_fure]                =   0x0AA0000000009000ul;
    g_ringScanRegionMap[n1_gptr]                =   0x0AA0000000002000ul;
    g_ringScanRegionMap[n1_repr]                =   0x0AA0000000000200ul;
    g_ringScanRegionMap[n1_time]                =   0x0AA0000000000100ul;
    g_ringScanRegionMap[n1_nmmu1_fure]          =   0x0040000000009000ul;
    g_ringScanRegionMap[n1_nmmu1_gptr]          =   0x0040000000002000ul;
    g_ringScanRegionMap[n1_nmmu1_repr]          =   0x0040000000000200ul;
    g_ringScanRegionMap[n1_nmmu1_time]          =   0x0040000000000100ul;

    g_ringScanRegionMap[pci_fure]               =   0x0BFC000000009000ul;
    g_ringScanRegionMap[pci_gptr]               =   0x0BFC000000002000ul;
    g_ringScanRegionMap[pci_repr]               =   0x0BFC000000000200ul;
    g_ringScanRegionMap[pci_time]               =   0x0BFC000000000100ul;
    g_ringScanRegionMap[pci_ph5_fure]           =   0x0400000000009000ul;
    g_ringScanRegionMap[pci_ph5_gptr]           =   0x0400000000002000ul;
    g_ringScanRegionMap[pci_ph5_repr]           =   0x0400000000000200ul;
    g_ringScanRegionMap[pci_ph5_time]           =   0x0400000000000100ul;
    g_ringScanRegionMap[pci_pll_func]           =   0x0002000000000080ul;
    g_ringScanRegionMap[pci_pll_bndy]           =   0x0002000000002000ul;
    g_ringScanRegionMap[pci_pll_gptr]           =   0x0002000000000010ul;

    g_ringScanRegionMap[mc_fure]                =   0x09E0000000009000ul;
    g_ringScanRegionMap[mc_gptr]                =   0x09E0000000002000ul;
    g_ringScanRegionMap[mc_repr]                =   0x09E0000000000200ul;
    g_ringScanRegionMap[mc_time]                =   0x09E0000000000100ul;
    g_ringScanRegionMap[mc_emo_fure]            =   0x0400000000009000ul;
    g_ringScanRegionMap[mc_emo_gptr]            =   0x0400000000002000ul;
    g_ringScanRegionMap[mc_emo_repr]            =   0x0400000000000200ul;
    g_ringScanRegionMap[mc_emo_time]            =   0x0400000000000100ul;
    g_ringScanRegionMap[mc_pll_func]            =   0x0002000000000080ul;
    g_ringScanRegionMap[mc_pll_bndy]            =   0x0002000000002000ul;
    g_ringScanRegionMap[mc_pll_bndy_bucket_0]   =   0x0002000000002000ul;
    g_ringScanRegionMap[mc_pll_bndy_bucket_1]   =   0x0002000000002000ul;
    g_ringScanRegionMap[mc_pll_bndy_bucket_2]   =   0x0002000000002000ul;
    g_ringScanRegionMap[mc_pll_bndy_bucket_3]   =   0x0002000000002000ul;
    g_ringScanRegionMap[mc_pll_bndy_bucket_4]   =   0x0002000000002000ul;
    g_ringScanRegionMap[mc_pll_gptr]            =   0x0002000000000010ul;

    g_ringScanRegionMap[pau0_fure]              =   0x0860000000009000ul;
    g_ringScanRegionMap[pau0_gptr]              =   0x0860000000002000ul;
    g_ringScanRegionMap[pau0_repr]              =   0x0860000000000200ul;
    g_ringScanRegionMap[pau0_time]              =   0x0860000000000100ul;
    g_ringScanRegionMap[pau0_pau0_fure]         =   0x0400000000009000ul;
    g_ringScanRegionMap[pau0_pau0_gptr]         =   0x0400000000002000ul;
    g_ringScanRegionMap[pau0_pau0_repr]         =   0x0400000000000200ul;
    g_ringScanRegionMap[pau0_pau0_time]         =   0x0400000000000100ul;

    g_ringScanRegionMap[pau1_fure]              =   0x0860000000009000ul;
    g_ringScanRegionMap[pau1_gptr]              =   0x0860000000002000ul;
    g_ringScanRegionMap[pau1_repr]              =   0x0860000000000200ul;
    g_ringScanRegionMap[pau1_time]              =   0x0860000000000100ul;
    g_ringScanRegionMap[pau1_pau3_fure]         =   0x0400000000009000ul;
    g_ringScanRegionMap[pau1_pau3_gptr]         =   0x0400000000002000ul;
    g_ringScanRegionMap[pau1_pau3_repr]         =   0x0400000000000200ul;
    g_ringScanRegionMap[pau1_pau3_time]         =   0x0400000000000100ul;


    g_ringScanRegionMap[pau2_fure]              =   0x0860000000009000ul;
    g_ringScanRegionMap[pau2_gptr]              =   0x0860000000002000ul;
    g_ringScanRegionMap[pau2_repr]              =   0x0860000000000200ul;
    g_ringScanRegionMap[pau2_time]              =   0x0860000000000100ul;
    g_ringScanRegionMap[pau2_pau4_fure]         =   0x0400000000009000ul;
    g_ringScanRegionMap[pau2_pau4_gptr]         =   0x0400000000002000ul;
    g_ringScanRegionMap[pau2_pau4_repr]         =   0x0400000000000200ul;
    g_ringScanRegionMap[pau2_pau4_time]         =   0x0400000000000100ul;
    g_ringScanRegionMap[pau2_pau5_fure]         =   0x0200000000009000ul;
    g_ringScanRegionMap[pau2_pau5_gptr]         =   0x0200000000002000ul;
    g_ringScanRegionMap[pau2_pau5_repr]         =   0x0200000000000200ul;
    g_ringScanRegionMap[pau2_pau5_time]         =   0x0200000000000100ul;

    g_ringScanRegionMap[pau3_fure]              =   0x0860000000009000ul;
    g_ringScanRegionMap[pau3_gptr]              =   0x0860000000002000ul;
    g_ringScanRegionMap[pau3_repr]              =   0x0860000000000200ul;
    g_ringScanRegionMap[pau3_time]              =   0x0860000000000100ul;
    g_ringScanRegionMap[pau3_pau6_fure]         =   0x0400000000009000ul;
    g_ringScanRegionMap[pau3_pau6_gptr]         =   0x0400000000002000ul;
    g_ringScanRegionMap[pau3_pau6_repr]         =   0x0400000000000200ul;
    g_ringScanRegionMap[pau3_pau6_time]         =   0x0400000000000100ul;


    g_ringScanRegionMap[pau3_pau7_fure]         =   0x0200000000009000ul;
    g_ringScanRegionMap[pau3_pau7_gptr]         =   0x0200000000002000ul;
    g_ringScanRegionMap[pau3_pau7_repr]         =   0x0200000000000200ul;
    g_ringScanRegionMap[pau3_pau7_time]         =   0x0200000000000100ul;


    g_ringScanRegionMap[iohs_fure]              =   0x0C00000000009000ul;
    g_ringScanRegionMap[iohs_gptr]              =   0x0C00000000002000ul;
    g_ringScanRegionMap[iohs_repr]              =   0x0C00000000000200ul;
    g_ringScanRegionMap[iohs_time]              =   0x0C00000000000100ul;
    g_ringScanRegionMap[iohs_ndl_fure]          =   0x0080000000009000ul;
    g_ringScanRegionMap[iohs_ndl_gptr]          =   0x0080000000002000ul;
    g_ringScanRegionMap[iohs_ndl_repr]          =   0x0080000000000200ul;
    g_ringScanRegionMap[iohs_ndl_time]          =   0x0080000000000100ul;
    g_ringScanRegionMap[iohs_pdl_fure]          =   0x0040000000009000ul;
    g_ringScanRegionMap[iohs_pdl_gptr]          =   0x0040000000002000ul;
    g_ringScanRegionMap[iohs_pdl_repr]          =   0x0040000000000200ul;
    g_ringScanRegionMap[iohs_pdl_time]          =   0x0040000000000100ul;
    g_ringScanRegionMap[iohs_pll_func]          =   0x0002000000000080ul;
    g_ringScanRegionMap[iohs_pll_bndy]          =   0x0002000000002000ul;
    g_ringScanRegionMap[iohs_pll_bndy_bucket_0] =   0x0002000000002000ul;
    g_ringScanRegionMap[iohs_pll_bndy_bucket_1] =   0x0002000000002000ul;
    g_ringScanRegionMap[iohs_pll_bndy_bucket_2] =   0x0002000000002000ul;
    g_ringScanRegionMap[iohs_pll_bndy_bucket_3] =   0x0002000000002000ul;
    g_ringScanRegionMap[iohs_pll_bndy_bucket_4] =   0x0002000000002000ul;
    g_ringScanRegionMap[iohs_pll_gptr]          =   0x0002000000000010ul;

    g_ringScanRegionMap[eq_fure]                =   0x0806000000009000ul;
    g_ringScanRegionMap[eq_gptr]                =   0x0806000000002000ul;
    g_ringScanRegionMap[eq_repr]                =   0x0806000000002000ul;
    g_ringScanRegionMap[eq_time]                =   0x0806000000000100ul;
    g_ringScanRegionMap[eq_cmsk]                =   0x0806000000000020ul;
    g_ringScanRegionMap[eq_inex]                =   0x0806000000000010ul;
    g_ringScanRegionMap[ec_cl2_fure]            =   0x0400000000009000ul;
    g_ringScanRegionMap[ec_cl2_gptr]            =   0x0400000000002000ul;
    g_ringScanRegionMap[ec_cl2_repr]            =   0x0400000000002000ul;
    g_ringScanRegionMap[ec_cl2_time]            =   0x0400000000000100ul;
    g_ringScanRegionMap[ec_cl2_cmsk]            =   0x0400000000000020ul;
    g_ringScanRegionMap[ec_cl2_inex]            =   0x0400000000000010ul;
    g_ringScanRegionMap[ec_mma_fure]            =   0x0001000000009000ul;
    g_ringScanRegionMap[ec_mma_gptr]            =   0x0001000000002000ul;
    g_ringScanRegionMap[ec_mma_repr]            =   0x0001000000000200ul;
    g_ringScanRegionMap[ec_mma_time]            =   0x0001000000000100ul;
    g_ringScanRegionMap[ec_mma_cmsk]            =   0x0001000000000020ul;
    g_ringScanRegionMap[ec_mma_inex]            =   0x0001000000000010ul;
    g_ringScanRegionMap[ec_l3_fure]             =   0x0040000000009000ul;
    g_ringScanRegionMap[ec_l3_gptr]             =   0x0040000000002000ul;
    g_ringScanRegionMap[ec_l3_repr]             =   0x0040000000000200ul;
    g_ringScanRegionMap[ec_l3_time]             =   0x0040000000000100ul;
    g_ringScanRegionMap[ec_l3_cmsk]             =   0x0040000000000020ul;
    g_ringScanRegionMap[ec_l3_inex]             =   0x0040000000000010ul;

    #endif
}

using namespace RS4;
extern "C"
{

///
/// @brief Return verbatim data from the RS4 string
/// @param[in] i_rs4Str         The RS4 scan string
/// @param[in] i_nibbleIndx     Index into RS4 string pointed to by i_rs4Str
/// @param[in] i_nibbleCount    The count of nibbles that need to be put in the return value.
/// @return big-endian-indexed double word
///
uint64_t rs4_get_verbatim( const uint8_t* i_rs4Str,
                           const uint32_t i_nibbleIndx,
                           const uint8_t i_nibbleCount )
{
    uint8_t l_byte;
    uint8_t l_nibble;
    uint64_t l_doubleWord   =   0;
    const uint32_t* l_data  =   NULL;
    uint32_t l_cnt          =   0;
    uint32_t l_offset       =   0;
    uint32_t l_index        =   i_nibbleIndx;
    rs4_data_t l_rs4_data;

    for( uint8_t i = 1; i <= i_nibbleCount; i++ )
    {
        l_cnt = l_index >> 1;

        if ( !( l_index %  8 ) || ( i == 1 ) )
        {
            l_offset    =   l_cnt - ( l_cnt % 4 );
            l_data      =   reinterpret_cast<uint32_t*> ( const_cast<uint8_t*>( i_rs4Str ) + l_offset );
            l_rs4_data.iv_data  =   *l_data;
        }

        l_byte      =   l_rs4_data.fields.val[l_cnt % 4];
        l_nibble    =   ( l_index % 2 ) ? ( l_byte & 0x0f ) : ( l_byte >> 4 );

        uint64_t l_tempDblWord = l_nibble;
        l_tempDblWord <<=   ( 64 - ( 4 * i ) );
        l_doubleWord   |=   l_tempDblWord;
        l_index++;
    }

    return l_doubleWord;
}

//-------------------------------------------------------------------------------------------------------

fapi2::ReturnCode putRegister(const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                              const RingType_t i_ringType,
                              const uint32_t i_chipletMask,
                              const uint32_t i_scomAddress,
                              const fapi2::buffer<uint64_t> i_scomData)
{
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_target_proc = i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP> ();

    uint32_t l_scomAddress = i_scomAddress;

    if ( INSTANCE_RING == i_ringType )
    {
        l_scomAddress |= i_chipletMask;

        FAPI_TRY( fapi2::putScom( l_target_proc, l_scomAddress, i_scomData ),
                  "Instance: putRegister failed" );
    }
    else
    {
        if ( SUPER_CHIPLET_MASK == i_chipletMask )
        {
            fapi2::Target<fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST> l_target_eq =
                i_target.getParent<fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST >();
            l_scomAddress |= i_chipletMask;

            FAPI_TRY( fapi2::putScom( l_target_eq, l_scomAddress, i_scomData ),
                      "EQ Common: putRegister failed" );
        }
        else
        {
            if (fapi2::TARGET_TYPE_PERV != i_target.getType() )
            {
                fapi2::ATTR_SCAN_CHIPLET_OVERRIDE_Type l_scan_chiplet_override =  fapi2::ENUM_ATTR_SCAN_CHIPLET_OVERRIDE_NONE;

                FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_SCAN_CHIPLET_OVERRIDE, l_target_proc, l_scan_chiplet_override ) );

                if ( fapi2::ENUM_ATTR_SCAN_CHIPLET_OVERRIDE_NONE == l_scan_chiplet_override )
                {
                    l_scomAddress |= i_chipletMask;
                }
                else
                {
                    l_scomAddress |= ( l_scan_chiplet_override << 24 );
                }
            }

            FAPI_TRY( fapi2::putScom( i_target, l_scomAddress, i_scomData ),
                      "Non EQ Common: putRegister failed" );
        }
    }

fapi_try_exit:
    return fapi2::current_err;
}

//-------------------------------------------------------------------------------------------------------

fapi2::ReturnCode getRegister(const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                              const RingType_t i_ringType,
                              const uint32_t i_chipletMask,
                              const uint32_t i_scomAddress,
                              const bool i_and_not_comp,
                              fapi2::buffer<uint64_t> & o_scomData)
{
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_target_proc = i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP> ();

    uint32_t l_scomAddress = i_scomAddress;

    if ( INSTANCE_RING == i_ringType )
    {
        l_scomAddress |= i_chipletMask;
        FAPI_TRY( fapi2::getScom( l_target_proc, l_scomAddress, o_scomData ),
                  "Instance: getRegister failed" );
    }
    else
    {
        if ( SUPER_CHIPLET_MASK == i_chipletMask )
        {
            l_scomAddress |= i_chipletMask;

            if ( i_and_not_comp )
            {
                fapi2::Target< fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2:: MULTICAST_AND > l_target_eq_and =
                    i_target.getParent<fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST >();

                FAPI_TRY( fapi2::getScom( l_target_eq_and, l_scomAddress, o_scomData ),
                          "EQ Common: getRegister (and) failed" );

            }
            else
            {
                fapi2::Target< fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2:: MULTICAST_COMPARE > l_target_eq_comp =
                    i_target.getParent<fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST>();

                FAPI_TRY( fapi2::getScom( l_target_eq_comp, l_scomAddress, o_scomData ),
                          "EQ Common: getRegister (comp) failed" );
            }
        }
        else
        {
            if (fapi2::TARGET_TYPE_PERV != i_target.getType() )
            {
                fapi2::ATTR_SCAN_CHIPLET_OVERRIDE_Type l_scan_chiplet_override =  fapi2::ENUM_ATTR_SCAN_CHIPLET_OVERRIDE_NONE;

                FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_SCAN_CHIPLET_OVERRIDE, l_target_proc, l_scan_chiplet_override ) );

                if ( fapi2::ENUM_ATTR_SCAN_CHIPLET_OVERRIDE_NONE == l_scan_chiplet_override )
                {
                    l_scomAddress |= i_chipletMask;
                }
                else
                {
                    l_scomAddress |= ( l_scan_chiplet_override << 24 );
                }
            }

            if ( i_and_not_comp )
            {
                fapi2::Target< fapi2::TARGET_TYPE_ALL_MC, fapi2:: MULTICAST_AND > l_target_mc_and = i_target;

                FAPI_TRY( fapi2::getScom( l_target_mc_and, l_scomAddress, o_scomData ),
                          "Non EQ Common: getRegister (and) failed" );
            }
            else
            {
                fapi2::Target< fapi2::TARGET_TYPE_ALL_MC, fapi2:: MULTICAST_COMPARE > l_target_mc_comp = i_target;

                FAPI_TRY( fapi2::getScom( l_target_mc_comp, l_scomAddress, o_scomData ),
                          "Non EQ Common: getRegister (comp) failed" );
            }

        }
    }

fapi_try_exit:
    return fapi2::current_err;
}

//-------------------------------------------------------------------------------------------------------

bool isInstanceRing( const uint16_t i_ringId )
{
    bool l_instanceRing = false;

    if( INSTANCE_RING_MARK & ( RING_PROPERTIES[(uint8_t)i_ringId].idxRing ) )
    {
        l_instanceRing  =   true;
    }

    return l_instanceRing;
}

//-------------------------------------------------------------------------------------------------------

uint64_t decodeScanRegionData( const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                               const uint32_t i_ringAddress)
{
    uint32_t l_chipletId    =   ((i_ringAddress & 0xFF000000) >> 24 );
    uint64_t l_value        =   0;
    uint32_t l_scan_region  =   ( ( i_ringAddress & 0x0000FFF0 ) |
                                  ( ( i_ringAddress & 0x00F00000 ) >> 20 ) ) << 13;
    uint32_t l_coreSelect   =   0;
    uint32_t l_coreSetBitCount = 0;

    //Fastinit support
    //if bit 12 is set in the ring address then set bit 0 in scan region data
    l_scan_region = i_ringAddress & 0x00080000 ? l_scan_region | 0x8000000000000000 :
                    l_scan_region;

    if( ( l_chipletId >= SUPER_CHIPLET_BASE_ID ) && ( l_chipletId <= SUPER_CHIPLET_MAX_ID ) )
    {
        l_coreSelect    =   i_target.getCoreSelect( );
        l_coreSetBitCount = __builtin_popcount(l_coreSelect);

        do
        {
            if( !l_coreSelect )
            {
               // got a eq target. it must be eq_* ring
               break;
            }

            if( SCAN_REGION_MMA0 & l_scan_region )
            {
                //MMA common rings are scanned individually.
                //We need to deselect mma region bit that is set above
                //because we need to set the right mma region thru coreselect.
                l_scan_region &= ~SCAN_REGION_MMA0;
                l_scan_region |= ( l_coreSelect << SHIFT32(SHIFT_TO_BIT_MMA0));
                break;
            }
            if (l_coreSetBitCount > 1)
            {
                l_scan_region |=  ENABLE_PARALLEL_SCAN; //Enabling parallel scan for all functional core region
            }

            //we have core target as input
            if( SCAN_REGION_ECL0 & l_scan_region)
            {
                l_scan_region &= ~SCAN_REGION_ECL0;
                l_scan_region |= ( l_coreSelect << SHIFT32(SHIFT_TO_BIT_ECL0));
                break;
            }

            if( SCAN_REGION_L30 & l_scan_region)
            {
                l_scan_region &= ~SCAN_REGION_L30;
                l_scan_region |= ( l_coreSelect << SHIFT32(SHIFT_TO_BIT_L30));
                break;
            }

        }while(0);
    }

    //Inserting scan type value starting from bit position 48
    uint32_t l_scan_type    =   0x00008000 >> ( i_ringAddress & 0x0000000F );

    // Exception: If scantype is 0x0f then we need special handling.

    if ( ( i_ringAddress & 0x0000000F ) == 0xF )
    {
        //Setting bit 48 and 51
        l_scan_type     =   0x00009000;
    }

    l_value             =   l_scan_region;
    l_value             =   ( l_value << 32 ) |  l_scan_type;

    return l_value;
}

//-------------------------------------------------------------------------------------------------------

///
/// @brief Decode an unsigned integer from a 4-bit octal stop code.
/// @param[in]  i_rs4Str        The RS4 scan string
/// @param[in]  i_nibbleIndx    Index into i_rs4Str that has the stop-code
/// @param[out] o_numRotate     No.of rotates decoded from the stop-code.
/// @return The number of nibbles decoded.
///
uint64_t stop_decode( const uint8_t* i_rs4Str,
                      uint32_t i_nibbleIndx,
                      uint64_t& o_numRotate )
{
    uint64_t l_numNibblesParsed     =   0; // No.of nibbles that make up the stop-code
    uint64_t l_numNonZeroNibbles    =   0;
    const uint32_t* l_data          =   NULL;
    uint32_t l_cnt                  =   0;
    uint32_t l_offset               =   0;
    rs4_data_t l_rs4_data;
    uint8_t l_nibble;
    uint8_t l_byte;

    do
    {
        l_cnt = i_nibbleIndx >> 1;

        if ( !( i_nibbleIndx %  8 ) || ( l_numNibblesParsed == 0 ) )
        {
            l_offset    =   l_cnt  - ( l_cnt % 4 );
            l_data      =   reinterpret_cast< uint32_t* > ( const_cast<uint8_t*>( i_rs4Str ) + l_offset );
            l_rs4_data.iv_data = *l_data;
        }

        l_byte      =   l_rs4_data.fields.val[l_cnt % 4];
        l_nibble    =   (i_nibbleIndx % 2) ? (l_byte & 0x0f) : (l_byte >> 4);
        l_numNonZeroNibbles     =   (l_numNonZeroNibbles * 8) + (l_nibble & 0x07);
        i_nibbleIndx++;
        l_numNibblesParsed++;

    }
    while( ( l_nibble & 0x08 ) == 0 );

    o_numRotate     =   l_numNonZeroNibbles;

    return l_numNibblesParsed;
}

//---------------------------------------------------------------------------------------------

fapi2::ReturnCode standardScan(
                        const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                        opType_t i_operation,
                        uint64_t i_opVal,
                        uint32_t i_chipletMask,
                        uint64_t i_scanData,
                        const RingType_t i_ringType,
                        const uint16_t i_ringId,
                        const OpMode_t i_opMode )
{
    FAPI_INF(">> standardScan");
    using namespace scomt;
    using namespace eq;
    using namespace perv;

    uint32_t l_scomAddress  =   0;
    fapi2::ReturnCode l_rc  =   fapi2::FAPI2_RC_SUCCESS;
    uint32_t l_max_poll_attempts = 1000;

    do
    {
        // **************
        // Scan or Rotate
        // **************
        if( ROTATE == i_operation )
        {
            // Setup Scom Address for rotate operation
            l_scomAddress                   =   ROTATE_ADDRESS_REG;
            const uint64_t l_maxRotates     =   0xFFFFF;
            uint64_t l_rotateCount          =   i_opVal;
            uint32_t l_numRotateScoms       =   1; // 1 - We need to do atleast one scom

            if( i_opVal > l_maxRotates )
            {
                l_numRotateScoms = (i_opVal / l_maxRotates);
                l_rotateCount = l_maxRotates;
            }

            // Scom Data needs to have the no.of rotates in the bits 12-31
            l_rotateCount <<= 32;

            for( uint32_t i = 0; i < (l_numRotateScoms + 1); i++ )
            {
                if( i == l_numRotateScoms )
                {
                    if( i_opVal <= l_maxRotates )
                    {
                        break;
                    }

                    l_rotateCount = ( i_opVal % l_maxRotates );
                    l_rotateCount <<= 32;
                }

                FAPI_INF("l_rotateCount %u", l_rotateCount);
                fapi2::buffer<uint64_t> l_scomData( l_rotateCount );

                FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, l_scomAddress, l_scomData ),
                          "ROTATE for %d, failed", i_opVal );

                // Check OPCG_DONE status
                uint32_t  l_OPCGAddress = 0x00000100;

                uint32_t l_attempts = l_max_poll_attempts;

                while( l_attempts > 0 )
                {
                    l_attempts--;

                    fapi2::buffer<uint64_t> l_opcgStatus;

                    FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, l_OPCGAddress, true, l_opcgStatus ),
                              "Failure during OPCG Check" );

                    if( ( l_opcgStatus.getBit( perv::CPLT_STAT0_CC_CTRL_OPCG_DONE_DC ) ) ||
                        ( CRONTEST_MODE == i_opMode ) )
                    {
                        FAPI_INF("OPCG_DONE set");
                        break;
                    }

                    // delays pulled from Cronus p9/zT code
                    if (l_attempts == (l_max_poll_attempts-1))
                    {
                        uint64_t l_rotate_cycles = (l_rotateCount >> 32);
                        FAPI_TRY( fapi2::delay( l_rotate_cycles * 3333, l_rotate_cycles * 16 ) );
                    }

                    else
                    {
                        FAPI_TRY( fapi2::delay( 1000000, 10000) );
                    }
                }

                if( 0 == l_attempts )
                {
                    l_rc = fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;
                    FAPI_ERR("Max attempts exceeded checking OPCG_DONE");
                    FAPI_ASSERT(false,
                                fapi2::P10_PUTRING_OPCG_DONE_TIMEOUT()
                                .set_SCOM_ADDRESS(l_scomAddress)
                                .set_SCOM_DATA(l_scomData)
                                .set_ROTATE_COUNT(l_rotateCount)
                                .set_RING_ID(i_ringId)
                                .set_RETURN_CODE(l_rc),
                                "ROTATE operation failed  due to timeout");
                }
            }// end of for loop
        }
        else if( SCAN == i_operation )
        {
            fapi2::buffer<uint64_t> l_scomData( i_scanData );
            // Setting Scom Address for a 64-bit scan
            l_scomAddress           =    ( SCAN64CONTSCAN | i_chipletMask );
            // Set the scan count to the actual value
            l_scomAddress          |=    (uint32_t)(i_opVal);

            FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, l_scomAddress, l_scomData ),
                      "SCAN for %d, failed", i_opVal );

        } // end of if(SCAN == i_operation)
    }
    while(0);

fapi_try_exit:

    FAPI_INF("<< standardScan");
    return fapi2::current_err;
}

//---------------------------------------------------------------------------------------------

fapi2::ReturnCode verifyHeader( const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
                                const fapi2::RingMode i_ringMode,
                                uint64_t       i_scanRegion,
                                const uint16_t i_ringId,
                                uint32_t       i_chipletMask,
                                const uint32_t i_bitsDecoded,
                                const RingType_t i_ringType,
                                OpMode_t i_opMode )
{
    using namespace scomt;
    using namespace perv;
    fapi2::buffer<uint64_t> l_scomData;
    fapi2::buffer<uint64_t> l_readHeader;
    fapi2::ReturnCode l_rc  =   fapi2::FAPI2_RC_SUCCESS;

    fapi2::delay(1000, 100);

    FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, SCAN64CONTSCAN, false, l_readHeader ) );

    if( CRONTEST_MODE    ==  i_opMode )
    {
        l_readHeader = SCAN_HEADER_DATA;
    }

    FAPI_INF("Got header - %016lx", uint64_t(l_readHeader));

    if( l_readHeader != SCAN_HEADER_DATA )
    {
        FAPI_ERR("Read CHECKWORD (%016x) data incorrect and total bit decoded 0x%016x",
                 uint64_t(l_readHeader), (uint64_t)i_bitsDecoded);
        l_rc = fapi2::FAPI2_RC_PLAT_ERR_RING_HEADER_CHECK;

        FAPI_ASSERT( false,
                     fapi2::P10_PUTRING_CHECKWORD_DATA_MISMATCH()
                     .set_TARGET( i_target )
                     .set_SCOM_ADDRESS( SCAN64CONTSCAN )
                     .set_SCOM_DATA( l_readHeader )
                     .set_BITS_DECODED( i_bitsDecoded )
                     .set_RING_ID( i_ringId )
                     .set_RINGMODE( i_ringMode )
                     .set_RETURN_CODE( l_rc ),
                     "CHECKWORD DATA mismatch" );
    }

    if(( i_scanRegion  >> 32 ) & ENABLE_PARALLEL_SCAN )
    {
        FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, CPLT_STAT0, false, l_scomData ) );

        if( l_scomData.getBit<CPLT_STAT0_CC_CTRL_PARALLEL_SCAN_COMPARE_ERR>() )
        {
            FAPI_ERR( "Parallel Scan Error For Ring 0x%02x ", i_ringId );
            FAPI_ASSERT( false,
                         fapi2::P10_PARALLEL_SCAN_COMPARE_ERR()
                         .set_TARGET( i_target )
                         .set_RING_ID( i_ringId )
                         .set_CPLT_STAT0( l_scomData ),
                         "Failed To Complete Parallel Scan" );
        }
    }

fapi_try_exit:
    return fapi2::current_err;
}

//----------------------------------------------------------------------------------------------------

fapi2::ReturnCode setupClockController(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    const RingType_t i_ringType,
    const uint32_t i_chipletMask,
    const fapi2::RingMode i_ringMode,
    const uint64_t i_scanRegion,
    struct restoreOpcgRegisters& o_OPCGData)
{
    bool l_use_setpulse =
        ((i_ringMode & fapi2::RING_MODE_SET_PULSE_NSL) ||
         (i_ringMode & fapi2::RING_MODE_SET_PULSE_SL) ||
         (i_ringMode & fapi2::RING_MODE_SET_PULSE_ALL));

    // **************************
    // Setup OPCG align
    // **************************

    if ( l_use_setpulse )
    {
        fapi2::buffer<uint64_t> l_opcgAlign;
        FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, OPCG_ALIGN_ADDRESS, true, o_OPCGData.l_opcgAlign ) );

        // bits: 4:7   SNOP_ALIGN(0:3) 5: 8:1
        // bits: 20:31 SNOP_WAIT(0:11)
        //set SNOP Align=8:1 and SNOP Wait=7
        l_opcgAlign = o_OPCGData.l_opcgAlign;
        l_opcgAlign.setBit<5>();
        l_opcgAlign.setBit<7>();
        l_opcgAlign.setBit<29>();
        l_opcgAlign.setBit<30>();
        l_opcgAlign.setBit<31>();

        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_ALIGN_ADDRESS, l_opcgAlign ) );
    }

    // **************************
    // Setup Scan-Type and Region
    // **************************

    FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, SCAN_REGION_TYPE_ADDRESS, i_scanRegion ) );


    // ***************************
    // Setup Clock Region & OPCG regs
    // ***************************

    if ( l_use_setpulse )
    {
        //First 32 bits tells clock region
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, CLK_REGION_ADDRESS, (i_scanRegion & 0xFFFFFFFF00000000ULL ) ) );

        // bit 11 -- RUN_OPCG_ON_UPDATE_DR
        uint64_t l_opcg_reg0 = 0x0010000000000000ULL;
        uint64_t l_opcg_reg1 = 0x0ULL;
        uint64_t l_opcg_reg2 = 0x0ULL;
        // NSL for slow regions
        uint64_t l_opcg_capt1 = 0x0ULL;
        // NSL for fast regions
        uint64_t l_opcg_capt2 = 0x0ULL;
        uint64_t l_opcg_capt3 = 0x0ULL;

        // setup NSL mode
        if ( i_ringMode & fapi2::RING_MODE_SET_PULSE_NSL )
        {
            // NSL for slow regions
            l_opcg_capt1 = 0x1480000000000000;
            // NSL for fast regions
            l_opcg_capt2 = 0x0480000000000000;
        }
        else if ( i_ringMode & fapi2::RING_MODE_SET_PULSE_SL )
        {
            // NSL for slow regions
            l_opcg_capt1 = 0x1800000000000000;
            // NSL for fast regions
            l_opcg_capt2 = 0x0800000000000000;
        }
        else   //set pulse all
        {
            // NSL for slow regions
            l_opcg_capt1 = 0x1E80000000000000;
            // NSL for fast regions
            l_opcg_capt2 = 0x0E80000000000000;
        }

        // save register state
        FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, OPCG_REG0_ADDRESS,  true, o_OPCGData.l_opcgReg0  ) );
        FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, OPCG_REG1_ADDRESS,  true, o_OPCGData.l_opcgReg1  ) );
        FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, OPCG_REG2_ADDRESS,  true, o_OPCGData.l_opcgReg2  ) );
        FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, OPCG_CAPT1_ADDRESS, true, o_OPCGData.l_opcgCapt1 ) );
        FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, OPCG_CAPT2_ADDRESS, true, o_OPCGData.l_opcgCapt2 ) );
        FAPI_TRY( getRegister( i_target, i_ringType, i_chipletMask, OPCG_CAPT3_ADDRESS, true, o_OPCGData.l_opcgCapt3 ) );

        // apply setpulse setup
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_REG0_ADDRESS,  l_opcg_reg0  ) );
        // Maintain SCAN_CLK_USE_EVEN bit 49
        l_opcg_reg1 = o_OPCGData.l_opcgReg1 & 0x0000000000004000ULL;
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_REG1_ADDRESS,  l_opcg_reg1  ) );
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_REG2_ADDRESS,  l_opcg_reg2  ) );
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_CAPT1_ADDRESS, l_opcg_capt1 ) );
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_CAPT2_ADDRESS, l_opcg_capt2 ) );
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_CAPT3_ADDRESS, l_opcg_capt3 ) );

        if ( ( i_ringMode & fapi2::RING_MODE_SET_PULSE_SL  ) ||
             ( i_ringMode & fapi2::RING_MODE_SET_PULSE_ALL ) )
        {
            FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, ROTATE_ADDRESS_SL_REG, 0x0ULL ) );
        }
    }

fapi_try_exit:
    return fapi2::current_err;
}

//----------------------------------------------------------------------------------------------------

fapi2::ReturnCode cleanupClockController(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    const RingType_t i_ringType,
    const uint32_t i_chipletMask,
    const fapi2::RingMode i_ringMode,
    const uint64_t i_scanRegion,
    const struct restoreOpcgRegisters& i_OPCGData)
{
    fapi2::ATTR_CHIP_EC_FEATURE_HW520842_Type l_hw520842;
    bool l_toggle_vitl_clockgating = false;
    auto l_proc_target = i_target.template getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::buffer<uint64_t> l_net_ctrl0_vitl_clkgate = 0x0002000000000000ULL;

    bool l_use_setpulse =
        ((i_ringMode & fapi2::RING_MODE_SET_PULSE_NSL) ||
         (i_ringMode & fapi2::RING_MODE_SET_PULSE_SL) ||
         (i_ringMode & fapi2::RING_MODE_SET_PULSE_ALL));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW520842, l_proc_target, l_hw520842),
             "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_HW520842)");

    l_toggle_vitl_clockgating = l_hw520842 &&
        ((i_scanRegion & (ENABLE_PARALLEL_SCAN << 32)) == (ENABLE_PARALLEL_SCAN << 32));

    if ( l_use_setpulse )
    {
        if ( ( i_ringMode & fapi2::RING_MODE_SET_PULSE_NSL ) ||
             ( i_ringMode & fapi2::RING_MODE_SET_PULSE_ALL ) )
        {
            FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, ROTATE_ADDRESS_NSL_REG, 0x0ULL ) );
        }

        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_ALIGN_ADDRESS, i_OPCGData.l_opcgAlign ) );
    }

    // disable vital clock gating to permit scan region type clearing to reset parallel scan error bits
    if (l_toggle_vitl_clockgating)
    {
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, NET_CTRL0_WOR, l_net_ctrl0_vitl_clkgate ) );
    }

    FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, SCAN_REGION_TYPE_ADDRESS, 0x0ULL ) );

    if (l_toggle_vitl_clockgating)
    {
        l_net_ctrl0_vitl_clkgate = ~l_net_ctrl0_vitl_clkgate;
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, NET_CTRL0_WAND, l_net_ctrl0_vitl_clkgate ) );
    }

    if ( l_use_setpulse )
    {
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, CLK_REGION_ADDRESS, 0x0ULL ) );
        // restore register state
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_REG0_ADDRESS,  i_OPCGData.l_opcgReg0  ) );
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_REG1_ADDRESS,  i_OPCGData.l_opcgReg1  ) );
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_REG2_ADDRESS,  i_OPCGData.l_opcgReg2  ) );
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_CAPT1_ADDRESS, i_OPCGData.l_opcgCapt1 ) );
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_CAPT2_ADDRESS, i_OPCGData.l_opcgCapt2 ) );
        FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, OPCG_CAPT3_ADDRESS, i_OPCGData.l_opcgCapt3 ) );
    }

fapi_try_exit:
    return fapi2::current_err;
}

//----------------------------------------------------------------------------------------------------

fapi2::ReturnCode flushRTIM(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    const RingType_t i_ringType,
    const uint32_t i_chipletMask,
    const fapi2::RingMode i_ringMode,
    const uint64_t i_scanRegion)
{
    // only run RTIM flush if target of scan operation has parallel scan bit set
    if ((i_scanRegion & (ENABLE_PARALLEL_SCAN << 32)) == (ENABLE_PARALLEL_SCAN << 32))
    {
        struct restoreOpcgRegisters l_OPCGData;
        uint64_t l_scanRegion = i_scanRegion;
        // alter type to unused (bndy)
        l_scanRegion &= 0xFFFFFFFFFFFF0000ULL;
        l_scanRegion |= 0x0000000000000080ULL;
        // setup 64-bit writes to zero
        uint32_t l_scomAddress = SCAN64_ADDRESS | 0x40;
        uint64_t l_scomData = 0x0ULL;

        // Prep clock controller for ring scan with modified type
        FAPI_TRY( setupClockController( i_target, i_ringType, i_chipletMask, i_ringMode, l_scanRegion, l_OPCGData ),
                  "Error from setupClockController" );

        // ensure RTIMs in chain are flushed with 0s, skip header check
        for (auto i = 0; i < 4; i++)
        {
            FAPI_TRY( putRegister( i_target, i_ringType, i_chipletMask, l_scomAddress, l_scomData ) );
        }

        // Cleanup clock controller
        FAPI_TRY( cleanupClockController( i_target, i_ringType, i_chipletMask, i_ringMode, l_scanRegion, l_OPCGData ),
                  "Error from cleanupClockController" );
    }
fapi_try_exit:
    return fapi2::current_err;

}

//----------------------------------------------------------------------------------------------------

fapi2::ReturnCode p10_putRingUtils(
    const fapi2::Target<fapi2::TARGET_TYPE_ALL_MC>& i_target,
    const uint8_t* i_rs4,
    bool  i_applyOverride,
    const fapi2::RingMode i_ringMode,
    const RingType_t i_ringType,
    OpMode_t    i_opMode )
{
    FAPI_INF(">> p10_putRingUtils");
    fapi2::ReturnCode l_rc              =   fapi2::FAPI2_RC_SUCCESS;
    CompressedScanData* l_rs4Header     =   (CompressedScanData*) i_rs4;
    const uint8_t* l_rs4Str             =   ( i_rs4 +  sizeof( CompressedScanData ) );
    opType_t l_opType                   =   ROTATE;
    uint64_t l_nibbleIndx               =   0;
    uint64_t l_bitsDecoded              =   0;
    uint32_t l_scanAddr                 =   0;
    uint64_t l_scanRegion               =   0;
    uint16_t l_ringId                   =   0;
    uint8_t l_mask                      =   0x08;
    uint64_t l_scomData                 =   0x0;
    MyBool_t l_bOverride                =   UNDEFINED_BOOLEAN;
    bool l_decompressionDone            =   false;
    uint32_t l_chipletMask              =   0;

    struct restoreOpcgRegisters l_OPCGData;

    initScanRegionTest();

    do
    {
        if( l_rs4Header == NULL )
        {
            l_rc = fapi2::FAPI2_RC_PLAT_RS4_HEADER_DATA_INVALID;
            FAPI_ERR("NULL header data in RS4 image");
            break;
        }

        //Determine Override/flush status
        l_scanAddr      =   rev_32( l_rs4Header->iv_scanAddr );
        l_ringId        =   rev_16(l_rs4Header->iv_ringId);
        l_scanRegion    =   decodeScanRegionData( i_target, l_scanAddr);
        l_chipletMask   =   ( l_scanAddr  & CHIPLET_MASK );


        if ( i_applyOverride )
        {
            if( UNDEFINED_BOOLEAN == i_applyOverride )
            {
                if( ( l_rs4Header->iv_type & RS4_IV_TYPE_OVRD_MASK ) == RS4_IV_TYPE_OVRD_OVRD )
                {
                    l_bOverride     =   true;
                }
                else if( ( l_rs4Header->iv_type & RS4_IV_TYPE_OVRD_MASK ) == RS4_IV_TYPE_OVRD_FLUSH )
                {
                    l_bOverride = false;
                }
            }

            else if( true == i_applyOverride )
            {
                l_bOverride     =   true;
            }
            else
            {
                FAPI_ASSERT( false,
                             fapi2::RS4_BAD_RING_TYPE()
                             .set_RING_TYPE( l_rs4Header->iv_type )
                             .set_RING_ID( l_ringId )
                             .set_INPUT_OVERRIDE( i_applyOverride ),
                             "Bad Ring Type Field In RS4 Container 0x%02x", l_rs4Header->iv_type );
                break;
            }
        }
        else if( !i_applyOverride )
        {
            l_bOverride     =   false;
        }

        if( TEST_MODE == i_opMode )
        {
            l_rc    =   fapi2::FAPI2_RC_SUCCESS;

            #ifdef __PUTRING_TEST_

            if( ( MASK_RESERVE_REGION_BIT & l_scanRegion )    ==  g_ringScanRegionMap[l_ringId] )
            {
                FAPI_INF( "SUCCESS: !!! Ring : 0x%x", l_ringId );
            }
            else
            {
                FAPI_INF( "FAIL: 0x%x Scan Region 0x%016lx", l_ringId, l_scanRegion );
            }

            #endif

            goto  fapi_try_exit;
        }


        // optionally flush RTIM staging
        FAPI_TRY( flushRTIM( i_target, i_ringType, l_chipletMask, i_ringMode, l_scanRegion ),
                  "Error from flushRTIM" );

        // Prep clock controller for ring scan
        FAPI_TRY( setupClockController( i_target, i_ringType, l_chipletMask, i_ringMode, l_scanRegion, l_OPCGData ),
                  "Error from setupClockController" );

        if( TEST_MODE == i_opMode )
        {
            goto fapi_try_exit;
        }

        // Write a 64 bit value for header.
        FAPI_TRY( putRegister( i_target, i_ringType, l_chipletMask, CHECK_WORD_REG_ADDRESS, SCAN_HEADER_DATA ),
                  "Error during writing header %016x", SCAN_HEADER_DATA );

        bool l_skip_64bits = true;

        // Decompress the RS4 string and scan
        do
        {
            if ( l_opType == ROTATE )
            {
                // Determine the no.of ROTATE operations encoded in stop-code
                uint64_t l_count    =   0;
                l_nibbleIndx       +=   stop_decode( l_rs4Str, l_nibbleIndx, l_count );

                // Determine the no.of rotates in bits
                uint64_t l_bitRotates   =   ( 4 * l_count );

                //Need to skip 64bits , because we have already written header
                //data.
                if ( l_skip_64bits && ( l_bitRotates >= SIXTYFOUR_BIT_HEADER ) )
                {
                    l_bitRotates    -=  SIXTYFOUR_BIT_HEADER;
                    l_skip_64bits    =  false;
                }

                l_bitsDecoded   +=  l_bitRotates;

                // Do the ROTATE operation
                if ( l_bitRotates != 0 )
                {
                    FAPI_TRY( standardScan( i_target,
                                            ROTATE,
                                            l_bitRotates,
                                            l_chipletMask,
                                            0,
                                            i_ringType,
                                            l_ringId,
                                            i_opMode ));
                }

                l_opType = SCAN;
            }
            else if( l_opType == SCAN )
            {
                uint8_t l_scanCount = rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                l_nibbleIndx++;

                if ( 0 == l_scanCount )
                {
                    FAPI_INF("SCAN COUNT %u", l_scanCount);
                    break;
                }

                if (( !l_bOverride ) && ( l_scanCount != 0xF ) )  // Process flush ring (and ensure no '0'-writes)
                {
                    l_bitsDecoded += (4 * l_scanCount);

                    // Parse the non-zero nibbles of the RS4 string and
                    // scan them into the ring
                    l_scomData = rs4_get_verbatim( l_rs4Str,
                                                   l_nibbleIndx,
                                                   l_scanCount );
                    l_nibbleIndx += l_scanCount;

                    FAPI_TRY( standardScan( i_target,
                                            SCAN,
                                            ( l_scanCount * 4 ),
                                            l_chipletMask,
                                            l_scomData,
                                            i_ringType,
                                            l_ringId,
                                            i_opMode ) );
                }
                else    // Process override ring (plus the occasional flush ring with '0'-write bits)
                {
                    if( 0xF == l_scanCount ) // Process ring with '0'-write bits
                    {
                        i_applyOverride = true;
                        uint8_t l_careMask = rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                        l_nibbleIndx++;
                        uint8_t l_spyData = rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                        l_nibbleIndx++;

                        for( uint8_t i = 0; i < 4; i++ )
                        {
                            l_bitsDecoded   +=  1;
                            l_scomData       =  0x0;

                            if( ( l_careMask & ( l_mask >> i ) ) )
                            {
                                if( ( l_spyData & ( l_mask >> i ) ) )
                                {
                                    l_scomData = 0xFFFFFFFFFFFFFFFF;
                                }

                                l_opType = SCAN;
                            }
                            else
                            {
                                l_opType = ROTATE;
                            }

                            FAPI_TRY( standardScan( i_target,
                                                    l_opType,
                                                    1, // Insert 1 bit
                                                    l_chipletMask,
                                                    l_scomData,
                                                    i_ringType,
                                                    l_ringId,
                                                    i_opMode ) );
                        }
                    }
                    else    // Process ring with only '1'-write bits
                    {
                        for ( uint8_t x = 0; x < l_scanCount; x++ )
                        {
                            // Parse the non-zero nibbles of the RS4 string and
                            // scan them into the ring
                            uint8_t l_data      =    rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                            l_nibbleIndx       +=    1;

                            FAPI_INF ( "VERBATIm l_nibbleIndx %u l_scanCount %u "
                                       "l_bitsDecoded %u", l_nibbleIndx, l_scanCount, l_bitsDecoded );

                            for( uint8_t i = 0; i < 4; i++ )
                            {
                                l_scomData      =    0x0;

                                if( ( l_data & ( l_mask >> i ) ) )
                                {
                                    l_opType    =   SCAN;
                                    l_scomData  =   0xFFFFFFFFFFFFFFFF;
                                }
                                else
                                {
                                    l_opType    =   ROTATE;
                                }

                                FAPI_TRY( standardScan( i_target,
                                                        l_opType,
                                                        1, // Insert 1 bit
                                                        l_chipletMask,
                                                        l_scomData,
                                                        i_ringType,
                                                        l_ringId,
                                                        i_opMode ) );
                            }

                        } // end of looper for bit-parsing a non-zero nibble
                    }
                }

                l_opType = ROTATE;
            } // end of - if(l_opType == SCAN)
        }
        while(1);

        if( l_decompressionDone )
        {
            break;
        }

        // Handle the string termination
        uint8_t l_nibble    =   rs4_get_nibble( l_rs4Str, l_nibbleIndx );
        l_nibbleIndx++;

        if ( l_nibble != 0 )
        {
            if ( ( !l_bOverride ) && !( l_nibble & 0x8 ) ) // Process flush ring (and ensure no '0'-writes))
            {
                l_bitsDecoded += l_nibble;
                l_scomData = rs4_get_verbatim( l_rs4Str,
                                               l_nibbleIndx,
                                               1 ); // return 1 nibble

                FAPI_TRY( standardScan( i_target,
                                        SCAN,
                                        l_nibble & 0x3,
                                        l_chipletMask,
                                        l_scomData,
                                        i_ringType,
                                        l_ringId,
                                        i_opMode ));
            }
            else // Process override ring (plus the occasional flush ring with '0'-write bits)
            {
                if( 0x8 & l_nibble ) // Process ring with '0'-write bits
                {
                    uint8_t l_careMask  =   rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                    l_nibbleIndx++;
                    uint8_t l_spyData   =   rs4_get_nibble(l_rs4Str, l_nibbleIndx);
                    l_nibbleIndx++;

                    for( uint8_t i = 0; i < ( l_nibble & 0x3 ); i++ )
                    {
                        l_bitsDecoded += 1;
                        l_scomData = 0x0;

                        if( ( l_careMask & ( l_mask >> i ) ) )
                        {
                            if( ( l_spyData & ( l_mask >> i ) ) )
                            {
                                l_scomData = 0xFFFFFFFFFFFFFFFF;
                            }

                            l_opType = SCAN;
                        }
                        else
                        {
                            l_opType = ROTATE;
                        }

                        FAPI_TRY( standardScan( i_target,
                                                l_opType,
                                                1, // Insert 1 bit
                                                l_chipletMask,
                                                l_scomData,
                                                i_ringType,
                                                l_ringId,
                                                i_opMode ) );
                    }
                }
                else // Process ring with only '1'-write bits
                {
                    // scan them into the ring
                    uint8_t l_data      =   rs4_get_nibble( l_rs4Str, l_nibbleIndx );
                    l_nibbleIndx       +=   1;

                    for( uint8_t i = 0; i < l_nibble; i++ )
                    {
                        l_scomData      =   0x0;
                        l_bitsDecoded   +=  1;

                        if(( l_data & ( l_mask >> i ) ) )
                        {
                            l_opType    =   SCAN;
                            l_scomData  =   0xFFFFFFFFFFFFFFFF;

                        }
                        else
                        {
                            l_opType    =   ROTATE;
                        }

                        FAPI_TRY( standardScan( i_target,
                                                l_opType,
                                                1, // Insert 1 bit
                                                l_chipletMask,
                                                l_scomData,
                                                i_ringType,
                                                l_ringId,
                                                i_opMode ) );
                    } //end of for
                }
            }
        } // end of if(l_nibble != 0)

        // Verify header
        FAPI_TRY( verifyHeader( i_target, i_ringMode, l_scanRegion,
                                l_ringId, l_chipletMask, l_bitsDecoded,
                                i_ringType, i_opMode  ) );

        // Cleanup clock controller post scan
        FAPI_TRY( cleanupClockController( i_target, i_ringType, l_chipletMask, i_ringMode, l_scanRegion, l_OPCGData ),
                  "Error from cleanupClockController" );

    }
    while( 0 );

fapi_try_exit:

    FAPI_INF( "<< p10_putRingUtils" );
    return fapi2::current_err;
}

};   //extern "C"
