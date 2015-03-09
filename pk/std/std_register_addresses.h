#ifndef __STD_REGISTER_ADDRESSES_H__
#define __STD_REGISTER_ADDRESSES_H__

//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2015
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file std_register_addresses.h
/// \brief Symbolic addresses for a standard PPE

#include "pk.h"

// Define the base address for the external interrupt controller registers
// This can be overridden in the pk_app_cfg.h file
#ifndef STD_EIC_BASE
#define STD_EIC_BASE 0xC0000000
#endif

// Define the base address for the PPE mode registers
// This can be overridden in the pk_app_cfg.h file
#ifndef STD_PMR_BASE
#define STD_PMR_BASE 0xC0000100
#endif

// Note: This list only contains registers that are needed by PK.  If
// an application requires other registers, it should define them
// elsewhere (i.e., cme_register_addresses.h)
#define STD_LCL_EISR        (STD_EIC_BASE + 0x0000)
#define STD_LCL_EISR_OR     (STD_EIC_BASE + 0x0010)
#define STD_LCL_EISR_CLR    (STD_EIC_BASE + 0x0018)
#define STD_LCL_EIMR        (STD_EIC_BASE + 0x0020)
#define STD_LCL_EIMR_OR     (STD_EIC_BASE + 0x0030)
#define STD_LCL_EIMR_CLR    (STD_EIC_BASE + 0x0038)
#define STD_LCL_EIPR        (STD_EIC_BASE + 0x0040)
#define STD_LCL_EIPR_OR     (STD_EIC_BASE + 0x0050)
#define STD_LCL_EIPR_CLR    (STD_EIC_BASE + 0x0058)
#define STD_LCL_EITR        (STD_EIC_BASE + 0x0060)
#define STD_LCL_EITR_OR     (STD_EIC_BASE + 0x0070)
#define STD_LCL_EITR_CLR    (STD_EIC_BASE + 0x0078)
#define STD_LCL_EISTR       (STD_EIC_BASE + 0x0080)
#define STD_LCL_EINR        (STD_EIC_BASE + 0x00a0)

#define STD_LCL_TSEL        (STD_PMR_BASE + 0x0000)
#define STD_LCL_TBR         (STD_PMR_BASE + 0x0040)

#endif // __STD_REGISTER_ADDRESSES_H__

