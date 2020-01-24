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
    REGION_VITL            = 0x1000000000000000,
    MMA_REGION_MASK        = 0x0001E00000000000,
    SCAN_HEADER            = 0xA5A55A5A00000000,
    RUN_ABIST_DELAY_NS     = 10000,
    RUN_ABIST_DELAY_SIM    = 50000,
    RUN_ABIST_TIMEOUT      = 100,
    CLEANUP_ABIST_CYCLES   = 0x1000,
    CLEANUP_ABIST_TIMEOUT  = 16,
    MAX_CARE_WORDS         = 100,
    NUM_FLUSH_CYCLES       = 16,  // 4 should be plenty but let's go for broke
};

/**
 * @brief Set up a chiplet for fast array dump
 * @param[in] i_target_chiplet The chiplet to prepare
 * @param[in] i_clock_regions  The clock regions to set up,
 *                             formatted like bits 4:15 of SCAN_REGION_TYPE
 * @param[in] i_setup_flush    If true, set up for initial flush cycles, where ARY clocks are
 *                             excluded and the ABIST engines are not being started.
 *                             If false, set up for the real thing.
 * @return FAPI2_RC_SUCCESS if success, else error code.
 */
static fapi2::ReturnCode p10_sbe_fastarray_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const uint64_t i_clock_regions,
    const uint64_t i_bist_regions,
    const bool i_setup_flush)
{
    // Five-bit value for OPCG_CAPT registers. Bits are SL, NSL, ARY, SE, FCE in that order
    // For flush cycles, we set up SL+NSL, for actual dumps SL+NSL+ARY
    const uint8_t l_capt_value = i_setup_flush ? 0x18 : 0x1C;
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
    .writeBit<CLK_REGION_SEL_THOLD_ARY>(!i_setup_flush);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, CLK_REGION, l_buf), "Failed to set up clock regions");

    l_buf = i_bist_regions;
    l_buf.setBit<BIST_TC_SRAM_ABIST_MODE_DC>()
    .writeBit<BIST_TC_BIST_START_TEST_DC>(!i_setup_flush);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, BIST, l_buf), "Failed to set up BIST register");

    l_buf.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_REG1, l_buf), "Failed to clear OPCG_REG1");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_REG2, l_buf), "Failed to clear OPCG_REG2");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_CAPT3, l_buf), "Failed to clear OPCG_CAPT3");

    l_buf.flush<0>()
    .insertFromRight<OPCG_CAPT1_COUNT, OPCG_CAPT1_COUNT_LEN>(1)
    .insertFromRight<OPCG_CAPT1_SEQ_01, OPCG_CAPT1_SEQ_01_LEN>(l_capt_value);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, OPCG_CAPT1, l_buf), "Failed to set up OPCG_CAPT1");

    l_buf.flush<0>()
    .insertFromRight<OPCG_CAPT2_14_01ODD, OPCG_CAPT2_14_01ODD_LEN>(l_capt_value);
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
        uint32_t l_timeout = RUN_ABIST_TIMEOUT;

        while (--l_timeout)
        {
            FAPI_TRY(fapi2::getScom(i_target_chiplet, CPLT_STAT0, l_buf), "Failed to read Chiplet Status 0 Register");

            if (l_buf.getBit<CPLT_STAT0_CC_CTRL_OPCG_DONE_DC>())
            {
                break;
            }

            fapi2::delay(RUN_ABIST_DELAY_NS, RUN_ABIST_DELAY_SIM);
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
        uint32_t l_timeout = CLEANUP_ABIST_TIMEOUT;

        do
        {
            FAPI_TRY(p10_sbe_fastarray_run_abist_cycles(i_target_chiplet, CLEANUP_ABIST_CYCLES),
                     "Failed to clock ABIST to completion");
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

/**
 * @brief expand a 32bit ring address into a 64bit value for the clock controller's SCAN_REGION_TYPE register
 *
 * If the ring address belongs to a core ring, the result will be shifted to match a core target's core select.
 *
 * @param[in] i_target a PERV or CORE target; used for adapting the result to a core select value
 * @param[in] i_ring_address the 32bit ring address
 * @return The 64bit SCAN_REGION_TYPE value
 */
static uint64_t expand_ring_address(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_CORE > & i_target,
    const uint32_t i_ring_address)
{
    const uint32_t l_chiplet_id   =   ((i_ring_address & 0xFF000000) >> 24 );
    const uint32_t l_core_select  =   (l_chiplet_id >= 0x20) ? i_target.getCoreSelect() : 0x8;
    // Multiplication with l_core_select puts copies of l_core_select everywhere a bit is set in the input region vector
    // Since core 0 is 0x8 we need to shift left by three positions less
    const uint32_t l_scan_region  =   l_core_select *
                                      (((i_ring_address & 0x0000FFF0) | ((i_ring_address & 0x00F00000) >> 20)) << (13 - 3));
    const uint32_t l_scan_encode  =   i_ring_address & 0x0000000F;
    const uint32_t l_scan_type    =   (l_scan_encode == 0xF) ? 0x9000 : (0x8000 >> l_scan_encode);
    return ((uint64_t)l_scan_region << 32) | l_scan_type;
}

/**
 * @brief Translate a clock region value into a BIST region value, apply workarounds
 *
 * Encapsulates the workaround for HW514994 by enabling ECL2 for BIST if MMA is selected for clocking.
 *
 * @param[in] i_target The target we're trying to BIST
 * @param[in] i_ring_address Ring address of the fastarray control stream
 * @param[in] i_clock_region Pre-masked clock region value without scan type bits
 * @return The value for the BIST register
 */
static uint64_t translate_abist_region(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_CORE > & i_target,
    const uint32_t i_ring_address,
    const uint64_t i_clock_region)
{
    if (((i_ring_address >> 24) >= 0x20) && (i_clock_region & MMA_REGION_MASK))
    {
        uint8_t l_core_mma_abist_linked = false;
        const auto l_proc_target = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
        // Ignore query errors and just default to no workaround
        FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_MMA_CORE_ABIST_LINKED, l_proc_target, l_core_mma_abist_linked);

        if (l_core_mma_abist_linked)
        {
            // In P10 DD1 the MMA ABIST is controlled by the ECL2 ABIST enable signal, so scoot the bits over there
            return i_clock_region << 10;
        }
    }

    return i_clock_region;
}

fapi2::ReturnCode p10_sbe_fastarray(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_CORE > & i_target,
    fapi2::hwp_data_istream&  i_instructions,
    fapi2::hwp_data_ostream&  o_dump_data)
{
    const auto l_perv_target = i_target.getParent<fapi2::TARGET_TYPE_PERV>();
    uint32_t l_header, l_care_bits[MAX_CARE_WORDS];

    // Open new scope so we can FAPI_TRY() across initializers at leisure
    {
        // Load the ring address from the stream and translate it
        FAPI_TRY(i_instructions.get(l_header));
        const uint64_t l_scan_region_type = expand_ring_address(i_target, l_header);
        const uint64_t l_clock_region = l_scan_region_type & 0xFFFFFFFF00000000;
        const uint64_t l_abist_region = translate_abist_region(i_target, l_header, l_clock_region);

        // At this point there may still be pending write pulses stuck inside the array-internal
        // pipelines. Enabling ABIST mode will make sure no new writes enter the pipelines, but
        // any existing writes must be flushed.
        // At the lowest level the actual array writes pass through ARY clocked latches, so we
        // can drain those pipelines by clocking SL+NSL but not ARY. Thus, before we start dumping,
        // run some cycles without starting the ABIST engine and without enabling ARY clocks.
        FAPI_TRY(p10_sbe_fastarray_setup(l_perv_target, l_clock_region, l_abist_region, true));
        FAPI_TRY(p10_sbe_fastarray_run_abist_cycles(l_perv_target, NUM_FLUSH_CYCLES));

        // Now we can set up for the real thing, turn on the ABIST engines and enable ARY.
        FAPI_TRY(p10_sbe_fastarray_setup(l_perv_target, l_clock_region, l_abist_region, false));

        uint32_t l_row = 0;

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
                FAPI_TRY(p10_sbe_fastarray_run_abist_cycles(l_perv_target, l_ncycles));
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

                FAPI_TRY(p10_sbe_fastarray_run_abist_cycles(l_perv_target, 1));
                FAPI_TRY(p10_sbe_fastarray_row(l_perv_target, l_scan_region_type, l_care_stream, l_dumped_bits));
            }

            FAPI_TRY(l_dumped_bits.flush());
        }

        FAPI_TRY(p10_sbe_fastarray_cleanup(l_perv_target, l_clock_region));
    }

fapi_try_exit:
    return fapi2::current_err;
}
