/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_fastarray.C $ */
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
//------------------------------------------------------------------------------
/// @file  p10_sbe_fastarray.C
///
/// @brief SBE part of fast array dump: Sets up the ABIST engines, clocks ABIST
///        cycles and dumps the interesting bits of the FARR ring back to the
///        service processor.
///        The information about which bits are interesting during which ABIST
///        cycle is supplied by the caller.
//------------------------------------------------------------------------------
// *HWP HW Owner        : Joachim Fenkes <fenkes@de.ibm.com>
// *HWP HW Backup Owner : Joe McGill <jmcgill@us.ibm.com>
// *HWP FW Owner        : Raja Das <rajadas2@in.ibm.com>
// *HWP Team            : Chip
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <p10_sbe_fastarray.H>
#include <p10_scom_perv.H>
#include <p10_scan_via_scom.H>

using namespace scomt::perv;

enum sbe_fastarray_constants
{
    REGION_VITL = 0x1000000000000000ULL,
};

/**
 * @brief Set up a chiplet for fast array dump
 * @param[in] i_target_chiplet The chiplet to prepare
 * @param[in] i_clock_regions  The clock regions to set up,
 *                             formatted like bits 4:15 of SCAN_REGION_TYPE
 * @return FAPI2_RC_SUCCESS if success, else error code.
 */
static fapi2::ReturnCode p10_sbe_fastarray_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const uint64_t i_clock_regions)
{
    fapi2::buffer<uint64_t> l_buf;

    /* Set up ABIST engines */
    //    FAPI_TRY(fapi2::putRing(i_target_chiplet, eq_c_fastarray_setup), "Failed to scan in fast array setup");

    /* Set up clock controller to do single BIST pulses */
    FAPI_TRY(fapi2::getScom(i_target_chiplet, OPCG_ALIGN, l_buf), "Failed to read OPCG_ALIGN register");
    l_buf.insertFromRight<OPCG_ALIGN_SNOP_ALIGN, OPCG_ALIGN_SNOP_ALIGN_LEN>(5)
    .insertFromRight<OPCG_ALIGN_SNOP_WAIT, OPCG_ALIGN_SNOP_WAIT_LEN>(7);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_ALIGN, l_buf), "Failed to update OPCG_ALIGN register");

    l_buf = i_clock_regions;
    l_buf.setBit<CLK_REGION_SEL_THOLD_SL>()
    .setBit<CLK_REGION_SEL_THOLD_NSL>()
    .setBit<CLK_REGION_SEL_THOLD_ARY>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, CLK_REGION, l_buf), "Failed to set up clock regions");

    l_buf = i_clock_regions;
    l_buf.setBit<BIST_TC_SRAM_ABIST_MODE_DC>()
    .setBit<BIST_TC_BIST_START_TEST_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, BIST, l_buf), "Failed to set up BIST register");

    l_buf.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_REG1, l_buf), "Failed to clear OPCG_REG1");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_REG2, l_buf), "Failed to clear OPCG_REG2");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_CAPT3, l_buf), "Failed to clear OPCG_CAPT3");

    l_buf.flush<0>()
    .insertFromRight<OPCG_CAPT1_COUNT, OPCG_CAPT1_COUNT_LEN>(1)
    .insertFromRight<OPCG_CAPT1_SEQ_01, OPCG_CAPT1_SEQ_01_LEN>(0x1C);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_CAPT1, l_buf), "Failed to set up OPCG_CAPT1");

    l_buf.flush<0>()
    .insertFromRight<OPCG_CAPT2_14_01ODD, OPCG_CAPT2_14_01ODD_LEN>(0x1C);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_CAPT2, l_buf), "Failed to set up OPCG_CAPT2");

    /* Assert CC_SDIS_DC_N, some arrays don't dump right if we don't set this */
    l_buf.flush<0>().setBit<CPLT_CONF0_CTRL_CC_SDIS_DC_N>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, CPLT_CONF0_WO_OR, l_buf), "Failed to set CC_SDIS_DC_N");

    return fapi2::FAPI2_RC_SUCCESS;

fapi_try_exit:
    return fapi2::current_err;
}

/**
 * @brief Run a number of ABIST cycles
 * @param[in] i_target_chiplet The chiplet target on where we need to run cycles
 * @param[in] i_clockCyclesMinusOne  Number of cycles to run, minus one
 * @return FAPI2_RC_SUCCESS if success, else error code.
 */
static fapi2::ReturnCode p10_sbe_fastarray_run_abist_cycles(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const uint32_t i_clock_cycles)
{
    fapi2::buffer<uint64_t> l_buf = i_clock_cycles - 1;

    l_buf.setBit<OPCG_REG0_OPCG_GO>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_REG0, l_buf), "Failed to clock %d array cycle(s)",
             i_clock_cycles);

    /* If we clocked more than a single cycle, do due diligence and wait for OPCG_DONE */
    if( i_clock_cycles > 1 )
    {
        uint32_t l_timeout = 100;

        while (--l_timeout)
        {
            FAPI_TRY(fapi2::getScom(i_target_chiplet, CPLT_STAT0, l_buf), "Failed to read Chiplet Status 0 Register");

            if (l_buf.getBit<CPLT_STAT0_CC_CTRL_OPCG_DONE_DC>())
            {
                break;
            }

            fapi2::delay(10000, 50000);
        }

        FAPI_ASSERT(l_timeout, fapi2::FASTARRAY_CLOCK_TIMEOUT().set_TARGET(i_target_chiplet),
                    "Clocking ABIST cycles timed out");
    }

    return fapi2::FAPI2_RC_SUCCESS;

fapi_try_exit:
    return fapi2::current_err;

}

/**
 * @brief Clean up a chiplet after a fast array dump
 * @param[in] i_target_chiplet The chiplet to clean up
 * @param[in] i_clock_regions  The clock regions that were previously enabled in fastarray_setup
 * @return FAPI2_RC_SUCCESS if success, else error code.
 */
static fapi2::ReturnCode p10_sbe_fastarray_cleanup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const uint64_t i_clock_regions)
{
    fapi2::buffer<uint64_t> l_buf;

    /* Drop vital fence and core region fences so that we can see the ABIST_DONE signal */
    FAPI_TRY(fapi2::putScom(i_target_chiplet, CPLT_CTRL1_WO_CLEAR, i_clock_regions | REGION_VITL),
             "Failed to drop region fences");

    /* Let ABIST engines run to completion */
    {
        uint32_t l_timeout = 16;

        do
        {
            FAPI_TRY(p10_sbe_fastarray_run_abist_cycles(i_target_chiplet, 0xFFF), "Failed to clock ABIST to completion");
            FAPI_TRY(fapi2::getScom(i_target_chiplet, CPLT_STAT0, l_buf), "Failed to read Chiplet Status 0 Register");
        }
        while (--l_timeout && !l_buf.getBit<CPLT_STAT0_ABIST_DONE_DC>());

        FAPI_ASSERT_NOEXIT(l_timeout != 0, fapi2::FASTARRAY_CLEANUP_NOT_DONE().set_TARGET(i_target_chiplet),
                           "Warning: ABIST_DONE not seen! Your dump is probably fine, but this is unexpected.");
    }

    /* Raise the region fences back up */
    FAPI_TRY(fapi2::putScom(i_target_chiplet, CPLT_CTRL1_WO_OR, i_clock_regions | REGION_VITL),
             "Failed to raise region fences");

    /* Disable ABIST and clock engines so they can cleanly reset */
    l_buf = 0;
    FAPI_TRY(fapi2::putScom(i_target_chiplet, BIST,       l_buf), "Failed to clear BIST reg");
    FAPI_TRY(p10_sbe_fastarray_run_abist_cycles(i_target_chiplet, 0xFF), "Failed to clock ABIST engine reset");

    /* Clean up the clock controller */
    FAPI_TRY(fapi2::putScom(i_target_chiplet, CLK_REGION, l_buf), "Failed to clear clock regions");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_CAPT1, l_buf), "Failed to clear OPCG_CAPT1");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_CAPT2, l_buf), "Failed to clear OPCG_CAPT2");

    /* Clear CC_SDIS_DC_N */
    l_buf.flush<0>().setBit<CPLT_CONF0_CTRL_CC_SDIS_DC_N>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, CPLT_CONF0_WO_CLEAR, l_buf), "Failed to clear CC_SDIS_DC_N");

    return fapi2::FAPI2_RC_SUCCESS;

fapi_try_exit:
    return fapi2::current_err;
}

/**
 * @brief magic value used for the dreaded header check
 */
const uint64_t SCAN_HEADER = 0xA5A55A5A00000000ull;

/**
 * @brief Dump a single row from the FARR ring and return it to the caller
 * @param[in]  i_scan_chiplet     The chiplet that the capture latches can be scanned out of
 * @param[in]  i_scan_region_type The value for the SCAN_REGION_TYPE register that will select the right rings
 * @param[in]  i_bits_of_interest A definition in RS5 format of which bits from the farr chain to return and which to drop
 * @param[out] o_array_bits       A pre-sized buffer that will be filled with the bits of interest
 * @return FAPI2_RC_SUCCESS if success, else error code.
 */
static fapi2::ReturnCode p10_sbe_fastarray_row(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_scan_chiplet,
    uint64_t                                      i_scan_region_type,
    fapi2::hwp_bit_istream&                       i_bits_of_interest,
    fapi2::hwp_bit_ostream&                       o_array_bits)
{
    fapi2::buffer<uint64_t> l_data;
    uint32_t                l_extra_rotate = 0;

    /* Set up scan region & type */
    FAPI_TRY(fapi2::putScom(i_scan_chiplet, SCAN_REGION_TYPE, i_scan_region_type),
             "Failed to set up scan region/type");

    /* Scan in header */
    FAPI_TRY(svs::scan64_put(i_scan_chiplet, SCAN_HEADER, 0), "Failed to write ring header");

    while (true)
    {
        uint32_t l_nbits = 0;
        /* Rotate / 'don't care' bits */
        FAPI_TRY(i_bits_of_interest.get_var_len_int(l_nbits));

        /* Regardless of whether l_nbits is 0 or not, we need to call
         * svs::rotate() here to get the header value into l_data */
        FAPI_TRY(svs::rotate(i_scan_chiplet, l_nbits + l_extra_rotate, l_data));

        if (!l_nbits)
        {
            break;
        }

        /* Scan-out / 'do care' bits */
        FAPI_TRY(i_bits_of_interest.get_var_len_int(l_nbits));

        if (!l_nbits)
        {
            break;
        }

        FAPI_TRY(svs::scan_out(i_scan_chiplet, l_nbits, o_array_bits, l_data, l_extra_rotate));
    }

    /* Header check -- the last svs::rotate() conveniently read it into l_data for us :3 */
    FAPI_ASSERT(l_data == SCAN_HEADER, fapi2::FASTARRAY_HEADER_CHECK_FAILED()
                .set_TARGET(i_scan_chiplet).set_EXPECTED(SCAN_HEADER).set_ACTUAL(l_data));

fapi_try_exit:
    /* Clear scan region & type */
    fapi2::ReturnCode l_rc = fapi2::putScom(i_scan_chiplet, SCAN_REGION_TYPE, 0);

    if (l_rc != fapi2::FAPI2_RC_SUCCESS && fapi2::current_err == fapi2::FAPI2_RC_SUCCESS)
    {
        FAPI_ERR("Failed to clear scan region/type");
        fapi2::current_err = l_rc;
    }

    return fapi2::current_err;
}

fapi2::ReturnCode p10_sbe_fastarray(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    uint64_t                                      i_scan_region_type,
    fapi2::hwp_data_istream&                      i_instructions,
    fapi2::hwp_data_ostream&                      o_dump_data)
{
    const uint64_t l_regions = i_scan_region_type & 0xFFFFFFFF00000000;
    uint32_t l_header, l_care_bits[500];
    uint32_t l_row = 0;

    FAPI_TRY(p10_sbe_fastarray_setup(i_target, l_regions));

    while (true)
    {
        FAPI_INF("Progress: Row %d", l_row);

        // Get the next tuple header
        FAPI_TRY(i_instructions.get(l_header));
        const uint32_t l_ncycles = l_header >> 16, l_nwords = l_header & 0xFFFF;
        l_row += l_ncycles;

        // Copy the header to the output stream
        FAPI_TRY(o_dump_data.put(l_header));

        // No cycles -> we're done!
        if (!l_ncycles)
        {
            break;
        }

        // Cycles but no data -> skip cycles
        if (!l_nwords)
        {
            FAPI_TRY(p10_sbe_fastarray_run_abist_cycles(i_target, l_ncycles));
            continue;
        }

        // Copy the care data into a local buffer (for reuse) and into the output stream
        FAPI_ASSERT(l_nwords <= ARRAY_SIZE(l_care_bits),
                    fapi2::FASTARRAY_CARE_BUFFER_OVERFLOW()
                    .set_BUFFER_SIZE_WORDS(ARRAY_SIZE(l_care_bits))
                    .set_DATA_SIZE_WORDS(l_nwords),
                    "Too many care data words for internal buffer");

        for (uint32_t i = 0; i < l_nwords; i++)
        {
            uint32_t l_data;
            FAPI_TRY(i_instructions.get(l_data));
            FAPI_TRY(o_dump_data.put(l_data));
            l_care_bits[i] = l_data;
        }

        // Dumped bits go into a bit stream
        fapi2::hwp_bit_ostream l_dumped_bits(o_dump_data);

        for (uint32_t i = 0; i < l_ncycles; i++)
        {
            if (i != 0 && (i & 7) == 0)
            {
                FAPI_INF("Progress: Row %d", l_row - l_ncycles + i);
            }

            // Every cycle, reuse the care data we've been sent
            fapi2::hwp_array_istream l_care_array(l_care_bits, l_nwords);
            fapi2::hwp_bit_istream l_care_stream(l_care_array);

            FAPI_TRY(p10_sbe_fastarray_run_abist_cycles(i_target, 1));
            FAPI_TRY(p10_sbe_fastarray_row(i_target, i_scan_region_type, l_care_stream, l_dumped_bits));
        }

        FAPI_TRY(l_dumped_bits.flush());
    }

    FAPI_TRY(p10_sbe_fastarray_cleanup(i_target, l_regions));

fapi_try_exit:
    return fapi2::current_err;
}
