#ifndef __SBEFW_SBE_TRACE_H
#define __SBEFW_SBE_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pk_api.h"
#include "trac_interface.h"

#ifdef __cplusplus
}
#endif

#define  SBE_ENTER_MRK ">>"
#define  SBE_EXIT_MRK "<<"
#define  SBE_ERR_MRK "E>"
#define  SBE_INF_MRK "I>"

#define SBE_TRACE(args...) PK_TRACE(args)
#define SBE_ENTER(args...) PK_TRACE(SBE_ENTER_MRK"" args)
#define SBE_EXIT(args...) PK_TRACE(SBE_EXIT_MRK"" args)
#define SBE_ERROR(args...) PK_TRACE(SBE_ERR_MRK"" args)
#define SBE_INFO(args...) PK_TRACE(SBE_INF_MRK"" args)

//Debug traces
#define  SBE_FW_DEBUG
#ifdef SBE_FW_DEBUG
#define  SBE_DEBUG_MRK "D>"
#define SBE_DEBUG(args...) PK_TRACE(SBE_DEBUG_MRK"" args)
#else
#define SBE_DEBUG(args...)
#endif  //SBE_FW_DEBUG

#endif // __SBEFW_SBE_TRACE_H
