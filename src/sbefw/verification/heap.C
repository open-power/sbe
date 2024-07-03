/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/heap.C $                               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2022,2024                        */
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

#include <endian.h>

#include "fapi2.H"
#include "heap.H"
#include "pk_api.h"
#include "sbeglobals.H"
#include "sbevtrace.H"

#define CANARY 0xFEEDB0B0ull
#define CANARY_MASK 0xFFFFFFF0ull

#define BLOCK_FLAG_FREED 1ull
#define BLOCK_FLAG_PERSIST 2ull

Heap& Heap::get_instance() {
    static Heap __attribute__((__aligned__(8)))
    iv_instance(reinterpret_cast<uint32_t>(&_heap_space_start_),
                reinterpret_cast<uint32_t>(&_heap_space_end_) -
                    reinterpret_cast<uint32_t>(&_heap_space_start_));

    return iv_instance;
}

Heap::Heap(uint32_t i_heap_start, uint32_t i_heap_size) {
    iv_heap_top = i_heap_start + i_heap_size;
    iv_scratch_bottom = iv_heap_top;
    iv_heap_midline = i_heap_start;
    iv_heap_bottom = i_heap_start;
}

void Heap::initialize() {
    SBEV_INFO(" Available heap to start at addr: 0x%X, of size: 0x%X",
             iv_heap_midline, iv_heap_top - iv_heap_midline);
}

uint32_t* Heap::getHeapMidlinePtr() {
    return reinterpret_cast<uint32_t*>(iv_heap_midline);
}

void* Heap::scratch_alloc(const uint32_t i_size, const alloc_flags i_flags) {
    enum scratch_alloc_rc {
        RC_SUCCESS = 0,
        RC_OUT_OF_SPACE,
        RC_INVALID_REQ_FOR_PERSISTANT,
    };

    const uint32_t rounded_size = (i_size + 7) & ~7;
    const bool l_persist = i_flags & AF_PERSIST;
    scratch_alloc_rc l_rc = RC_SUCCESS;
    uint32_t new_bottom;
    PkMachineContext ctx;

    uint32_t l_heap_midline, l_scratch_bottom;

    // The following operations must be done atomically
    pk_critical_section_enter(&ctx);

    // Following loop is implemented such a way that
    //   1. its light weight
    //   2. no other heavy functions are called
    //   3. no return statement
    //
    // since this is inside a critical section.
    do {
        // storing following member variable to locals, so that it can be
        //   logged outside this loop
        l_heap_midline = iv_heap_midline;
        l_scratch_bottom = iv_scratch_bottom;

        new_bottom = iv_scratch_bottom - rounded_size - 8;
        if (new_bottom < iv_heap_midline) {
            l_rc = RC_OUT_OF_SPACE;
            break;
        }

        // We only allow persistent blocks if they are either the first
        // allocation or the previous allocation was also persistent. This
        // ensures that all persistent blocks are clumped up near the top of
        // memory, preventing fragmentation.
        if (l_persist &&
            !((iv_scratch_bottom == iv_heap_top)
              // scratch_bottom_header() has a call to SBE_ERR, but this is not
              // a concern for
              //   delaying critical section, since it will HALT the SBE after
              //   the trace.
              || ((scratch_bottom_header() >> 32) & BLOCK_FLAG_PERSIST))) {
            l_rc = RC_INVALID_REQ_FOR_PERSISTANT;
            break;
        }

        uint64_t header = (CANARY | (l_persist ? BLOCK_FLAG_PERSIST : 0))
                              << 32 |
                          iv_scratch_bottom;
        *(uint64_t*)new_bottom = header;
        iv_scratch_bottom = new_bottom;
    } while (0);
    // exit the critical section
    pk_critical_section_exit(&ctx);

    // The midline, determined by the pakstack usage, is the limit on what can
    // be allocated
    if (l_rc == RC_OUT_OF_SPACE) {
        SBEV_ERROR(
            "scratch_alloc: Out of scratch space. rounded_size=0x%X limit=0x%X "
            "bottom=0x%X",
            rounded_size, iv_heap_midline, iv_scratch_bottom);
        return NULL;
    }

    if (l_rc == RC_INVALID_REQ_FOR_PERSISTANT) {
        SBEV_ERROR(
            "scratch_alloc: Persistent blocks must be allocated before any "
            "temporary blocks.");
        return NULL;
    }

    SBEV_INFO(
        "scratch_alloc: rounded_size=0x%X limit=0x%X old_bottom=0x%X "
        "new_bottom=0x%X",
        rounded_size, l_heap_midline, l_scratch_bottom, new_bottom);

    return (void*)(new_bottom + 8);
}

void* Heap::scratch_calloc(const uint32_t i_size, const alloc_flags i_flags) {
    void* scratchPtr = scratch_alloc(i_size, i_flags);
    if (scratchPtr) {
        memset(scratchPtr, 0x00, i_size);
    }
    return scratchPtr;
}

void Heap::scratch_free(const void* i_ptr) {
    if (i_ptr != NULL) {
        // Grab the header, which is one word below the allocated block pointer
        uint64_t* const pheader = (uint64_t*)i_ptr - 1;
        const uint64_t header = *pheader;
        const uint32_t canary = (header >> 32) & CANARY_MASK;
        const uint32_t flags = (header >> 32) & ~CANARY_MASK;

        if (canary != CANARY) {
            SBEV_ERROR(
                "scratch_free: Block header corrupted, halting. ptr=%p "
                "header=0x%08X%08X",
                i_ptr, header >> 32, header & 0xFFFFFFFF);
            PK_PANIC(SBE::PANIC_ASSERT);
        }

        if (flags & BLOCK_FLAG_FREED) {
            SBEV_ERROR(
                "scratch_free: Double free detected, halting. ptr=%p "
                "header=0x%08X%08X",
                i_ptr, header >> 32, header & 0xFFFFFFFF);
            PK_PANIC(SBE::PANIC_ASSERT);
        }

        // Mark the block as freed.
        // For persistent blocks we can leave BLOCK_FLAG_PERSIST in place since
        // BLOCK_FLAG_FREED takes precedence.
        SBE_DEBUG("scratch_free: marking ptr=%p as freed", i_ptr);
        *pheader = header | (BLOCK_FLAG_FREED << 32);

        // Drop all blocks that became droppable now
        scratch_unwind(false);
    }
}

bool Heap::is_scratch_pointer(const void* i_ptr) {
    return ((uintptr_t)i_ptr >= iv_heap_midline) &&
           ((uintptr_t)i_ptr < iv_heap_top);
}

void Heap::scratch_free_all() { scratch_unwind(true); }

uint64_t Heap::scratch_bottom_header() {
    const uint64_t header = *(uint64_t*)iv_scratch_bottom;
    const uint32_t canary = (header >> 32) & CANARY_MASK;

    if (canary != CANARY) {
        SBEV_ERROR(
            "scratch: Chain of blocks corrupted, halting. ptr=%p "
            "header=0x%08X%08X",
            iv_scratch_bottom, header >> 32, header & 0xFFFFFFFF);
        PK_PANIC(SBE::PANIC_ASSERT);
    }

    return header & ~(CANARY_MASK << 32);
}

void Heap::scratch_unwind(bool i_unwind_all) {
    PkMachineContext ctx;

    // The following operations must be done atomically
    pk_critical_section_enter(&ctx);
    while (iv_scratch_bottom < iv_heap_top) {
        const uint64_t header = scratch_bottom_header();
        const uint32_t flags = header >> 32;

        if ((flags & BLOCK_FLAG_FREED) ||
            (i_unwind_all && !(flags & BLOCK_FLAG_PERSIST))) {
            uint32_t new_bottom = header & 0xFFFFFFFF;
            // SBE_DEBUG("scratch_unwind: unwind old_bottom=0x%X
            // new_bottom=0x%X",
            //           iv_scratch_bottom, new_bottom);
            iv_scratch_bottom = new_bottom;
        } else {
            // We found a block that is not freed -> done
            break;
        }
    }
    // exit the critical section
    pk_critical_section_exit(&ctx);
}

size_t Heap::getFreeHeapSize() {
    size_t freeHeapSpace = iv_scratch_bottom - iv_heap_midline;
    if (freeHeapSpace > 8) {
        freeHeapSpace -= 0x8;
        SBE_DEBUG("Available free heap space is 0x%08x", freeHeapSpace);
    } else {
        freeHeapSpace = 0x0;
        SBEV_INFO("No free heap space");
    }

    return freeHeapSpace;
}
