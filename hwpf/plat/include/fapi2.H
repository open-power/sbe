///
/// @file fapi2.H
/// @brief top level header for fapi2
///

#ifndef __FAPI2_TOP_LEVEL__
#define __FAPI2_TOP_LEVEL__

// Define which platforms will not have FAPI Return Codes
#undef __noRC__
#if defined (__CME__) || defined (__SGPE__) || defined (__PGPE__)
#define __noRC__
#endif

// Determine if running on a PPE platform
#ifndef __PPE__
#if defined (__SBE__) || defined (__CME__) || defined (__SGPE__) || defined (__PGPE__)
#define __PPE__
#endif
#endif

#include <plat_trace.H>
#include <target.H>
#include <return_code.H>
#include <buffer.H>
#include <hw_access.H>
#include <utils.H>



// In turn includes the needed generated headers (hwp_ffd_classes, etc.)
#include <error_scope.H>
#include <set_sbe_error.H>      // Generated file
#include <plat_attributes.H>
#include <plat_target_utils.H>


#include <fapi2_hwp_executor.H>

// Block of headers not currently in fapi2
#ifdef FAPI2_MISSING_HEADERS
  #include <mvpdAccess.H>
  #include <mbvpdAccess.H>
#endif


#endif // __FAPI2_TOP_LEVEL__






