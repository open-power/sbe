/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_load.C $ */
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
#include <p10_contained.H>
#include <p10_contained_sim.H>
#include <p10_contained_qme_sram_xfer.H>

#include <multicast_group_defs.H>
#include <p10_putmempba.H>
#include <fapi2_mem_access.H>
#include <fapi2_subroutine_executor.H>
#include <p10_l3_flush.H>
#include <p10_pm_utils.H>
#include <p10_hcode_image_defines.H>
#include <p10_hcd_memmap_qme_sram.H>

// L3 caches are 4MiB
const uint64_t L3_CACHE_SIZE = (1ull << 20) * 4;

enum memflags : uint32_t
{
    PBA_LCO_OP = (fapi2::SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON
                  | fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA
                  | fapi2::SBE_MEM_ACCESS_FLAGS_LCO_MODE),
    PBA_INJ_OP = (fapi2::SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON
                  | fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA
                  | fapi2::SBE_MEM_ACCESS_FLAGS_CACHE_INJECT_MODE)
};

template<typename T>
static inline T align_8B(const T x)
{
    return (x + (T)7) & ~(T)7;
}

///
/// @brief Wrapper around p10_putmempba
///
/// @param[in] i_chip Reference to chip target
/// @param[in] i_core L3 cache to target for LCO writes (ignored for INJ)
/// @param[in] i_start_addr Start address for write operation
/// @param[in] i_data_bytes Number of bytes pointed to by i_data
/// @param[in] i_data Input data content
/// @param[in] i_mem_flags Flags for PBA operation
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static inline fapi2::ReturnCode putmempba(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core,
        const uint64_t i_start_addr,
        const uint64_t i_data_bytes,
        const uint8_t* i_data,
        const memflags i_mem_flags)
{
    fapi2::ReturnCode rc;
    FAPI_CALL_SUBROUTINE(rc, p10_putmempba, i_chip, i_core, i_start_addr, i_data_bytes,
                         i_data, i_mem_flags);
    return rc;
}

///
/// @brief Wrapper around p10_l3_flush
///
/// @param[in] i_core L3 cache to purge
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static inline fapi2::ReturnCode purgel3(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core)
{
    fapi2::ReturnCode rc;

    if (sim::skip_l3purge())
    {
        // Use FAPI_ERR(...) to force print this message
        FAPI_ERR("Skipping L3 purge");
        return fapi2::FAPI2_RC_SUCCESS;
    }

    FAPI_EXEC_HWP(rc, p10_l3_flush, i_core, L3_FULL_PURGE, 0);
    return rc;
}

///
/// @brief Load QME SRAM with content from HOMER image
///
/// @param[in] i_chip            Reference to chip target
/// @param[in] i_homer_img_bytes Size of the HOMER image to load QME SRAM from in bytes
/// @param[in] i_homer_img       Pointer to HOMER image to load QME SRAM from
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static inline fapi2::ReturnCode load_qme_sram(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
        const uint64_t i_homer_img_bytes,
        const uint8_t* i_homer_img)
{
    using hcodeImageBuild::CpmrHeader_t;
    using hcodeImageBuild::QmeHeader_t;
    // XXX: This runs immediately after the istep 0-4 IPL, ie. this multicast
    //      group includes only EQs with "good" cores. So that means we only
    //      start QMEs in EQs with "good" cores. Should we instead setup the
    //      istep3 MC groups, then restore the istep4 MC groups after loading
    //      the QMEs? The subsequent p10_contained_ipl requires istep 4 MC groups.
    const auto eqs = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);

    const uint8_t* const cpmr = i_homer_img + CPMR_HOMER_OFFSET;
    const CpmrHeader_t* const cpmr_hdr = (CpmrHeader_t*)(cpmr);

    const uint8_t* const cpmr_qme_img = cpmr + revle32(cpmr_hdr->iv_qmeImgOffset);
//    const QmeHeader_t* const cpmr_qme_hdr = (QmeHeader_t*)(cpmr_qme_img + QME_HEADER_IMAGE_OFFSET);

    // 1. Copy QME Hcode section
    {
        const uint64_t* qme_img = (uint64_t*)(cpmr_qme_img);
        const uint32_t qme_len = align_8B(revle32(cpmr_hdr->iv_qmeImgLength));
        FAPI_TRY(qme_sram_xfer_copy(eqs, qme_img, qme_len, QME_SRAM_BASE_ADDR));
    }
    // 2. Copy common rings
    {
        // XXX currently only support STOP2/3 which do not require common rings
        /*
        const uint64_t* ring_img = (uint64_t*)(cpmr + revle32(cpmr_hdr->iv_commonRingOffset));
        const uint32_t ring_len = align_8B(revle32(cpmr_hdr->iv_commonRingLength));
        const uint32_t qme_ring_offset = revle32(cpmr_qme_hdr->g_qme_common_ring_offset);
        FAPI_TRY(qme_sram_xfer_copy(eqs, ring_img, ring_len,
                                    QME_SRAM_BASE_ADDR + qme_ring_offset));
        */
    }
    // 3. Copy SCOM restore
    {
        // XXX currently only support STOP2/3 which do not require SCOM restore
    }
    // 4. Copy instance rings
    {
        // XXX currently only support STOP2/3 which do not require instance rings
    }

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Validate arguments and configuration for cache-/chip-contained IPL
///
/// @param[in] i_cac                Indicate cache-contained IPL
/// @param[in] i_chc                Indicate chip-contained IPL
/// @param[in] i_cache_img_bytes    Size of the cache image to load in bytes (MUST be 128B aligned)
/// @param[in] i_nactive            Number of active caches
/// @param[in] i_nbacking           Number of backing caches
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode validate(const bool& i_cac, const bool& i_chc,
                                  const uint64_t& i_cache_img_bytes,
                                  const fapi2::ATTR_ACTIVE_CORES_NUM_Type i_nactive,
                                  const fapi2::ATTR_BACKING_CACHES_NUM_Type i_nbacking)
{
    FAPI_ASSERT(i_cac ^ i_chc,
                fapi2::P10_CONTAINED_LOAD_NOT_CONTAINED_IPL()
                .set_CAC(i_cac)
                .set_CHC(i_chc),
                "IPL is not one of cache-contained or chip-contained");

    if (i_chc)
    {
        FAPI_ASSERT(i_cache_img_bytes <= (i_nbacking * L3_CACHE_SIZE),
                    fapi2::P10_CONTAINED_LOAD_CACHE_IMG_TOO_BIG_CHC()
                    .set_CACHE_IMG_BYTES(i_cache_img_bytes)
                    .set_BACKING_STORAGE_BYTES(i_nbacking * L3_CACHE_SIZE),
                    "Cache image is too large to fit into backing cache storage");

        FAPI_ASSERT((i_nbacking & (i_nbacking - 1)) == 0,
                    fapi2::P10_CONTAINED_LOAD_NBACKING_NOT_POW2()
                    .set_NBACKING(i_nbacking),
                    "Number of backing caches is not a power of 2");
    }

    if (i_cac)
    {
        FAPI_ASSERT(i_cache_img_bytes <= L3_CACHE_SIZE,
                    fapi2::P10_CONTAINED_LOAD_CACHE_IMG_TOO_BIG_CAC()
                    .set_CACHE_IMG_BYTES(i_cache_img_bytes),
                    "Cache image is too large to fit into an L3 cache");
    }

    FAPI_ASSERT(i_nactive >= 1,
                fapi2::P10_CONTAINED_LOAD_NACTIVE_INVALID()
                .set_NACTIVE(i_nactive),
                "Number of active caches < 1");

fapi_try_exit:
    return fapi2::current_err;
}

extern "C" {
    fapi2::ReturnCode p10_contained_load(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                         const uint64_t& i_cache_img_start_addr,
                                         const uint64_t& i_cache_img_bytes,
                                         const uint8_t* i_cache_img,
                                         const uint64_t& i_homer_img_bytes,
                                         const uint8_t* i_homer_img)
    {
        bool chc;
        bool cac;
        int ncores = 0;
        fapi2::ATTR_CHIP_UNIT_POS_Type corenum;
        fapi2::ATTR_ACTIVE_CORES_VEC_Type active_bvec;
        fapi2::ATTR_ACTIVE_CORES_NUM_Type nactive;
        fapi2::ATTR_BACKING_CACHES_NUM_Type nbacking;

        FAPI_TRY(is_cac_ipl(cac));
        FAPI_TRY(is_chc_ipl(chc));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_VEC, i_chip, active_bvec));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_NUM, i_chip, nactive));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BACKING_CACHES_NUM, i_chip, nbacking));

        FAPI_TRY(validate(cac, chc, i_cache_img_bytes, nactive, nbacking));

        FAPI_INF("chip_contained[%d] cache_contained[%d]", chc, cac);
        FAPI_INF("nactive[%02d] nbacking[%02d] active[x%08x]", nactive, nbacking,
                 active_bvec);

        if (chc && (i_homer_img != NULL && i_homer_img_bytes > 0))
        {
            FAPI_INF("Loading QME image(s)");
            FAPI_TRY(load_qme_sram(i_chip, i_homer_img_bytes, i_homer_img));
        }

        if (i_cache_img == NULL && i_cache_img_bytes == 0)
        {
            FAPI_INF("No cache image, skipping PBA-LCO load");
            return fapi2::FAPI2_RC_SUCCESS;
        }


        for (auto const& core : i_chip.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, core, corenum));

            if (!is_active_core(corenum, active_bvec))
            {
                continue;
            }

            ++ncores;

            if (chc)
            {
                FAPI_INF("PBA-LCO cache image bytes[%09d] @ core[#%02d] L3 @ offset[%09d]",
                         i_cache_img_bytes, corenum, i_cache_img_start_addr);
                FAPI_TRY(putmempba(i_chip, core, i_cache_img_start_addr, i_cache_img_bytes,
                                   i_cache_img, PBA_LCO_OP));

                FAPI_INF("Purging core[#%02d] L3 to backing caches", corenum);
                FAPI_TRY(purgel3(core));

                break;
            }

            FAPI_INF("core[%02d/%02d]: PBA-LCO cache img bytes[%09d] @ core[#%02d] L3",
                     ncores, nactive, i_cache_img_bytes, corenum);
            FAPI_TRY(putmempba(i_chip, core, 0, i_cache_img_bytes, i_cache_img,
                               PBA_LCO_OP));

            if (ncores == nactive)
            {
                break;
            }
        }

    fapi_try_exit:
        return fapi2::current_err;
    }
} // extern "C"
