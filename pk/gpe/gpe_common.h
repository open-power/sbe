#ifndef __GPE_COMMON_H__
#define __GPE_COMMON_H__

//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file gpe_common.h
/// \brief Common header for GPE
///
/// This header is maintained as part of the PK port for GPE, but needs to be
/// physically present in the PMX area to allow dropping PMX code as a whole
/// to other teams.

// -*- WARNING: This file is maintained as part of PK.  Do not edit in -*-
// -*- the PMX area as your edits will be lost.                         -*-

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif

#include "occhw_common.h"

/// Each GPE instance has it's own interrupt status register these macros
/// are added for convenience in accessing the correct register
#define GPE_GISR0(instance_id) (OCB_G0ISR0 + (instance_id * 8))
#define GPE_GISR1(instance_id) (OCB_G0ISR1 + (instance_id * 8))

#ifdef __ASSEMBLER__
/// This macro contains GPE specific code for determining what IRQ caused the
/// external exception handler to be invoked by the PPE

/// Check for interrupts pending in status register 0 while the IRQ is
/// computed.  The IRQ is expected to be stored in r4.  If no IRQ is
/// pending then load the phantom irq # (EXTERNAL_IRQS).
    .macro hwmacro_get_ext_irq
        
        _lwzi       %r3, %r3, GPE_GISR0(APPCFG_OCC_INSTANCE_ID)
        cntlzw      %r4, %r3
        cmpwible    %r4, 31, external_irq_found   #branch if irq is lt or eq to 31
        
        ## No IRQ pending in interrupt set 0.  Try set 1.
        ## Note: irq # will be 64 (EXTERNAL_IRQS) if no bits were set in either register
        
        _lwzi       %r3, %r3, GPE_GISR1(APPCFG_OCC_INSTANCE_ID)
        cntlzw      %r4, %r3
        addi        %r4, %r4, 32

    .endm


/// Redirect the .hwmacro_irq_cfg_bitmaps macro to call our macro that is common for both
/// GPE's and the 405 inside the OCC complex.  This is called from the ppe42_exceptions.S
/// file.
    .macro .hwmacro_irq_cfg_bitmaps
        .occhw_irq_cfg_bitmaps
    .endm
#endif /* __ASSEMBLER__ */

#endif  /* __GPE_COMMON_H__ */
