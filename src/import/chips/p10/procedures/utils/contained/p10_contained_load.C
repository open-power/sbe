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
#include <p10_putmempba.H>
#include <fapi2_mem_access.H>
#include <p10_l3_flush.H>

namespace
{

// L3 caches are 4MiB
const uint64_t L3_CACHE_SIZE = (1ull << 20) * 4;
// QME SRAM is 32KiB
const uint64_t QME_SRAM_SIZE = (1ull << 10) * 32;

///
/// @brief Align a 64-bit value to 128B by rounding up
///
/// @param[in] val Value to align to 128B
///
/// @return val aligned to 128B if val != 0 else 0
///
inline uint64_t align128(const uint64_t& val)
{
    return (val + 127) / 128 * 128;
}

enum memflags : uint32_t
{
    PBA_LCO_OP = (fapi2::SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON
                  | fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA
                  | fapi2::SBE_MEM_ACCESS_FLAGS_LCO_MODE),
    PBA_INJ_OP = (fapi2::SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON
                  | fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA
                  | fapi2::SBE_MEM_ACCESS_FLAGS_CACHE_INJECT_MODE)
};

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
inline fapi2::ReturnCode putmempba(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                   const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core,
                                   const uint64_t i_start_addr,
                                   const uint64_t i_data_bytes,
                                   const uint8_t* i_data,
                                   const memflags i_mem_flags)
{
    fapi2::ReturnCode rc;
    FAPI_EXEC_HWP(rc, p10_putmempba, i_chip, i_core, i_start_addr, i_data_bytes,
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
inline fapi2::ReturnCode purgel3(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core)
{
    fapi2::ReturnCode rc;
    FAPI_EXEC_HWP(rc, p10_l3_flush, i_core, L3_FULL_PURGE, 0);
    return rc;
}

///
/// @brief Wrapper around QME block-copy procedure
///
/// @param[in] i_chip Reference to chip target
/// @param[in] i_qme_img_bytes Number of bytes pointed to by i_qme_img
/// @param[in] i_qme_img QME image content
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode blkcpyqme(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                            const uint64_t& i_qme_img_bytes,
                            const uint8_t* i_qme_img)
{
    // TODO RTC:212055 implement QME BCE
    // TODO RTC:212108
    //
    // Should be able to setup the BCE in all QMEs via multicast-write
    // and then check completion using multicast-or (BCE BUSY == 0)
    //
    FAPI_ERR("QME block copy NOT implemented");

fapi_try_exit:
    __attribute__((unused));
    return fapi2::current_err;
}

///
/// @brief Delete QME image via L3 blind-purge
///
/// @param[in] i_core L3 cache to purge
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
inline fapi2::ReturnCode deleteqme(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core)
{
    fapi2::ReturnCode rc;
    FAPI_EXEC_HWP(rc, p10_l3_flush, i_core, L3_FULL_BLIND_PURGE, 0);
    return rc;
}

///
/// @brief Validate arguments and configuration for cache-/chip-contained IPL
///
/// @param[in] i_cac                Indicate cache-contained IPL
/// @param[in] i_chc                Indicate chip-contained IPL
/// @param[in] i_cache_img_bytes    Size of the cache image to load in bytes (MUST be 128B aligned)
/// @param[in] i_qme_img_bytes      Size of the QME image to load in bytes
/// @param[in] i_nactive            Number of active caches
/// @param[in] i_nbacking           Number of backing caches
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode validate(const bool& i_cac, const bool& i_chc,
                           const uint64_t& i_cache_img_bytes,
                           const uint64_t& i_qme_img_bytes,
                           const fapi2::ATTR_ACTIVE_CORES_NUM_Type i_nactive,
                           const fapi2::ATTR_BACKING_CACHES_NUM_Type i_nbacking)
{
    FAPI_ASSERT(i_cac != i_chc,
                fapi2::P10_CONTAINED_LOAD_NOT_CONTAINED_IPL()
                .set_CAC(i_cac)
                .set_CHC(i_chc),
                "IPL is not one of cache-contained or chip-contained");

    FAPI_ASSERT(i_qme_img_bytes <= QME_SRAM_SIZE,
                fapi2::P10_CONTAINED_LOAD_QME_IMG_TOO_BIG()
                .set_QME_IMG_BYTES(i_qme_img_bytes),
                "QME image is too large to fit into QME SRAM");

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

}; // namespace

extern "C" {
    fapi2::ReturnCode p10_contained_load(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                         const uint64_t& i_cache_img_start_addr,
                                         const uint64_t& i_cache_img_bytes,
                                         const uint8_t* i_cache_img,
                                         const uint64_t& i_qme_img_bytes,
                                         const uint8_t* i_qme_img)
    {
        bool chc;
        bool cac;
        int ncores = 0;
        fapi2::ATTR_CHIP_UNIT_POS_Type corenum;
        fapi2::ATTR_ACTIVE_CORES_VEC_Type active_bvec;
        fapi2::ATTR_ACTIVE_CORES_NUM_Type nactive;
        fapi2::ATTR_BACKING_CACHES_NUM_Type nbacking;

        FAPI_TRY(get_contained_ipl_type(cac, chc));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_VEC, i_chip, active_bvec));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_NUM, i_chip, nactive));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BACKING_CACHES_NUM, i_chip, nbacking));

        FAPI_TRY(validate(cac, chc, i_cache_img_bytes, i_qme_img_bytes, nactive,
                          nbacking));

        FAPI_INF("chip_contained[%d] cache_contained[%d]", chc, cac);
        FAPI_INF("nactive[%02d] nbacking[%02d] active[x%08x]", nactive, nbacking,
                 active_bvec);

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
                if (i_qme_img != NULL && i_qme_img_bytes > 0)
                {
                    FAPI_INF("PBA-LCO QME image bytes[%09d] @ core[#%02d] L3",
                             i_qme_img_bytes, corenum);
                    FAPI_TRY(putmempba(i_chip, core, 0, i_qme_img_bytes, i_qme_img,
                                       PBA_LCO_OP));

                    FAPI_INF("Block copying QME image");
                    FAPI_TRY(blkcpyqme(i_chip, i_qme_img_bytes, i_qme_img));

                    FAPI_INF("Blind-purging core[#%02d] L3 to delete QME image",
                             corenum);
                    FAPI_TRY(deleteqme(core));
                }

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
