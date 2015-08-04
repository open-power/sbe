/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: hwpf/fapi2/include/plat/plat_error_scope.H $                  */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2012,2015                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
/**
 * @file plat_error_scope.H
 * @brief platform definitions which create a scope for automatic error handling
 */

#ifndef __FAPI2_PLAT_ERROR_SCOPE__
#define __FAPI2_PLAT_ERROR_SCOPE__

/// @cond
#define PLAT_FAPI_TRY_NO_TRACE( __operation__ )                         \
    if ((fapi2::current_err = (__operation__)) != fapi2::FAPI2_RC_SUCCESS) \
    {                                                                   \
        goto fapi_try_exit;                                             \
    }

#define PLAT_FAPI_TRY_TRACE( __operation__, ... )                       \
    if ((fapi2::current_err = (__operation__)) != fapi2::FAPI2_RC_SUCCESS) \
    {                                                                   \
        FAPI_ERR(__VA_ARGS__);                                          \
        goto fapi_try_exit;                                             \
    }

///
/// @brief Assert a conditional is true.
/// If it is not, the FFDC gathering function is called and the
/// trace is output as a FAPI error trace.
/// @param[in] __conditional__ the condition to assert
/// @param[in] __ffdc__ the FFDC gathering function
/// @param[in] ... varargs, as input to FAPI_ERR
///
#define PLAT_FAPI_ASSERT( __conditional__, __ffdc__, ... )      \
    if (! (__conditional__))                                    \
    {                                                           \
        (__ffdc__).execute();                                   \
        FAPI_ERR(__VA_ARGS__);                                  \
        goto fapi_try_exit;                                     \
    }
/// @endcond

#endif
