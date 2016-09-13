/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/ppe/pk/std/std_common.h $      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
#ifndef __STD_COMMON_H__
#define __STD_COMMON_H__

/// \file cme_common.h
/// \brief Common header for standard PPE's
///

#ifndef __ASSEMBLER__
    #include <stdint.h>
#endif

//#include "cmehw_interrupts.h"
#include "pk.h"
#include "std_irq_config.h"

#ifdef __ASSEMBLER__
// *INDENT-OFF*
/// This macro contains standard PPE code for determining what IRQ caused the
/// external exception handler to be invoked by the PPE

/// Check for interrupts pending in the interrupt status register while the IRQ
/// is computed.  The IRQ is expected to be stored in r4. If no IRQ is
/// pending then load the phantom irq # (EXTERNAL_IRQS).
/// 
/// r1, r2, r3, and r13 must not be modified.  All other registers may be used.
///
/// The pk_unified_irq_prty_mask_handler routine MUST return the task priority
/// interrupt vector in d5.
///
    .macro hwmacro_get_ext_irq

#ifdef UNIFIED_IRQ_HANDLER_CME
        // Unified approach.
        _liw        r5, pk_unified_irq_prty_mask_handler
        mtlr        r5
        blrl            // On return, d5 contains task prty irq vec.
        mfsprg      r3, 0 // In case r3 is modified by unified handler, restore to sprg0
#else
        _lvdg       d5, STD_LCL_EISTR    #load the 64bit interrupt status into d5
#endif        
        cntlzw      r4, r5
        cmpwible    r4, 31, call_external_irq_handler   #branch if irq is lt or eq to 31

        ## No IRQ pending in r5.  Try r6.
        ## Note: irq # will be 64 (phantom irq) if no bits were set in either register

        cntlzw  r4, r6
        addi    r4, r4, 32

    .endm

/// Redirect the .hwmacro_irq_cfg_bitmaps macro to call our standard PPE implementation
/// This is called from the ppe42_exceptions.S file.
    .macro .hwmacro_irq_cfg_bitmaps
        .std_irq_cfg_bitmaps
    .endm

// *INDENT-ON*
#endif /* __ASSEMBLER__ */

#endif  /* __STD_COMMON_H__ */
