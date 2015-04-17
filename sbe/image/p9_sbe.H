#ifndef __P9_SBE_H
#define __P9_SBE_H

/// \file p9_sbe.H
/// \brief A header to be included into all SBE assembler files and C++
/// hardware procedures that interact with SBE images.
///
/// This header #include-s all generally useful headers required for SBE code
/// development as well as defining other needed facilities.  Note that any
/// sub-includes must be done in an assembler-safe way.

#include "sbe_common.H"
//#include "p9_scom_addresses.H"
//#include "p9_istep_num.H"

// The #include of fapiHwpReturnCodes.H must be made here (instead of in
// sbe_common.H) to guarantee that the P9-local copy is used.
//#include "fapiHwpReturnCodes.H"

#ifdef __ASSEMBLER__

        // p9_sbe.H 'recursively' inserts itself into any hook code extracted
        // from an assembly context that includes p9_sbe.H.

	##`#include "p9_sbe.H"

#endif  // __ASSEMBLER__

#endif  // __P9_SBE_H
