/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/heap.H $                               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2022,2024                        */
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

#pragma once

#include <stdint.h>

class Heap {
   private:
    // @brief - iv_heap_top : top/end of available heap space (highest addr
    // available)
    uint32_t iv_heap_top;
    // @brief - iv_scratch_bottom : bottom/start of used scratch space (lower
    // addrs), from heap top to lower addresses as more scratch space is used
    uint32_t iv_scratch_bottom;
    // @brief - iv_heap_midline : top of used pakstack space, grows from heap bottom
    // to higher addresses as more pakstack space is used
    uint32_t iv_heap_midline;
    // @brief - iv_heap_bottom : bottom of available heap space (lowest addr
    // available)
    uint32_t iv_heap_bottom;

    /**
     * @brief Constructor for the heap class, assign heap start address and size
     *
     * Making this private since this will be a singleton class.
     */
    Heap(uint32_t i_heap_start, uint32_t i_heap_size);

   public:
    /**
     * @brief Allocation flags for the scratch allocator
     */
    enum alloc_flags {
        AF_NONE = 0,     ///< Empty flags, no special request
        AF_PERSIST = 1,  ///< The allocated memory must persist across
                         ///< scratch_free_all() calls
    };

    /**
     * @brief  Initialize memory space
     */
    void initialize();

    /**
     * @brief Return a pointer to the heap midline address
     */
    uint32_t* getHeapMidlinePtr();

    /**
     * @brief Allocate scratch memory from the top of the heap space
     *
     * Scratch space is allocated from the top of the free heap space going
     * down, and the maximum total amount of memory that can be allocated is
     * limited by the space allocated by the linker (minus engdata images and so
     * on).
     *
     * @param i_size    Amount of bytes requested; will be rounded up to a
     * multiple of 8
     * @param i_flags   Options for the allocation request
     * @return A pointer to the allocated scratch space, or
     *         NULL if not enough space was available
     */
    void* scratch_alloc(const uint32_t i_size,
                        const alloc_flags i_flags = AF_NONE);

    /**
     * @brief Allocate scratch memory from the top of the heap space and clear
     * the allocated memory function internally calling scratch_alloc methods
     * @param i_size    Amount of bytes requested; will be rounded up to a
     * multiple of 8
     * @param i_flags   Options for the allocation request
     * @return A pointer to the allocated scratch space, or
     *         NULL if not enough space was available
     */
    void* scratch_calloc(const uint32_t i_size,
                         const alloc_flags i_flags = AF_NONE);

    /**
     * @brief Free a chunk of scratch memory
     *
     * @param ptr The block of memory to free
     */
    void scratch_free(const void* i_ptr);

    /**
     * @brief Determine whether or not a pointer has been allocated from scratch memory
     * @param i_ptr The pointer to query
     * @return true if the pointer comes from scratch space
     */
    bool is_scratch_pointer(const void *i_ptr);

    /**
     * @brief Free all scratch memory
     */
    void scratch_free_all();

    /**
     * @brief Function to get the current free heap space in bytes
     *
     * @return size_t Currently available free heap space in bytes
     */
    size_t getFreeHeapSize();

    /**
     * @brief Static method to get the singleton instance
     */
    static Heap& get_instance();

   private:
    /**
     * @brief Update the scratch bottom mark
     *
     * Walk the linked list of blocks starting at the very bottom
     * and rewind the bottom pointer to the bottommost non-freed block
     * (or the top of scratch).
     *
     * @param i_unwind_all If true, non-freed blocks will also be removed
     */
    void scratch_unwind(bool i_unwind_all);

    /**
     * @brief Get the flags and next pointer for the bottommost scratch block
     *
     * This also checks for scratch space consistency and halts if the
     * chain of blocks is corrupted.
     *
     * @return A 64 bit value with the flags in the left 32 bits
     *         and the next pointer in the right 32 bits
     */
    uint64_t scratch_bottom_header();
};

