//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file ppe42_core.c
/// \brief The final bits of PK runtime code required to complete the PPE42
/// port. 
///
/// The entry points in this file are considered 'core' routines that will
/// always be present during runtime in any PK application.

#define __PPE42_CORE_C__

#include "pk.h"

typedef union
{
    uint64_t    value;
    struct
    {
        uint32_t dec_start;
        uint32_t dec_change_tag;
    };
}ppe42_timebase_data_t;

ppe42_timebase_data_t ppe42_tb_data = {0};
PkTimebase  ppe42_64bit_timebase = 0;


#if 0
/// Get the emulated 64-bit timebase 
///

PkTimebase
pk_timebase_get(void) 
{
    PkTimebase          tb;
    uint32_t            dec_start;
    uint32_t            dec;
    uint32_t            time_since_last_update;
    PkMachineContext    ctx;

    pk_critical_section_enter(&ctx);

    tb = ppe42_64bit_timebase;
    dec_start = ppe42_dec_start;
    dec = mfspr(SPRN_DEC);

    pk_critical_section_exit(&ctx);

    time_since_last_update = dec_start - dec;

    return (tb + time_since_last_update);
}
#endif

/// Set the 64-bit timebase in an critical section
///
/// It is assumed that the caller knows what they are doing; e.g., is aware of
/// what may happen when time warps as a result of this call.

void
pk_timebase_set(PkTimebase timebase) 
{
    PkMachineContext ctx;

    pk_critical_section_enter(&ctx);

    ppe42_64bit_timebase = timebase;
    ppe42_tb_data.dec_start = 0;
    ppe42_tb_data.dec_change_tag++;

    //This will cause TSR[DIS] to be set on the next timer tick.
    mtspr(SPRN_DEC, 0);

    pk_critical_section_exit(&ctx);
}
    

/// Enable interrupt preemption
///
/// This API can only be called from an interrupt context.  Threads will
/// always be preempted by interrupts unless they explicitly disable
/// interrupts with the \c pk_interrupt_disable() API. It is legal to call
/// this API redundantly.
///
/// Be careful when enabling interrupt handler preemption that the interrupt
/// being handled does not/can not trigger again, as this could rapidly lead
/// to stack overflows.
///
/// Return values other then PK_OK (0) are errors; see \ref pk_errors
///
/// \retval 0 Successful completion
///
/// \retval -PK_ILLEGAL_CONTEXT The API call was not made from an interrupt
/// context. 

int
pk_interrupt_preemption_enable()
{
    if (PK_ERROR_CHECK_API) {
        PK_ERROR_UNLESS_ANY_INTERRUPT_CONTEXT();
    }

    wrteei(1);

    return PK_OK;
}
        

/// Disable interrupt preemption
///
/// This API can only be called from an interrupt context.  Threads will
/// always be preempted by interrupts unless they explicitly disable
/// interrupts with the \c pk_interrupt_disable() API.  It is legal to call
/// this API redundantly.
///
/// Return values other then PK_OK (0) are errors; see \ref pk_errors
///
/// \retval 0 Successful completion
///
/// \retval -PK_ILLEGAL_CONTEXT The API call was not made from an interrupt
/// context. 

int
pk_interrupt_preemption_disable()
{
    if (PK_ERROR_CHECK_API) {
        PK_ERROR_UNLESS_ANY_INTERRUPT_CONTEXT();
    }

    wrteei(0);

    return PK_OK;
}
        

#if PK_TIMER_SUPPORT

// The tickless kernel timer mechanism for PPE42
//
// This routine must be called from a critical section.
//
// Tickless timeouts are provided by programming the PIT timer based on when
// the next timeout will occur.  If the timeout is for the end of time there's
// nothing to do - PK does not use auto-reload mode so no more PIT interrupts
// will be arriving.  Otherwise, if the timeout is longer than the 32-bit PIT
// timer can handle, we simply schedule the timeout for 2**32 - 1 and
// __pk_timer_handler() will keep rescheduling it until it finally occurs.
// If the \a timeout is in the past, we schedule the PIT interrupt for 1 tick
// in the future in accordance with the PK specification.

void
__pk_schedule_hardware_timeout(PkTimebase timeout)
{
    PkTimebase       now;
    uint32_t         new_dec;
    PkMachineContext ctx;
    uint32_t         dec;

    if (timeout != PK_TIMEBASE_MAX) {

        pk_critical_section_enter(&ctx);

        now = pk_timebase_get();

        if (timeout <= now) {
            new_dec = 1;
        } else if ((timeout - now) > 0xffff0000) {
            new_dec = 0xffff0000;
        } else {
            new_dec = timeout - now;
        }

        //read and write the DEC back-to-back so that we loose as little time
        //as possible
        dec = mfspr(SPRN_DEC);
        mtspr(SPRN_DEC, new_dec);

        //update our 64bit accumulator with how much time has advanced since
        //we last changed it.
        ppe42_64bit_timebase += ppe42_tb_data.dec_start - dec;

        //update our start time so we know how much time has advanced since
        //this update of the accumulator
        ppe42_tb_data.dec_start = new_dec;
        ppe42_tb_data.dec_change_tag++;

        pk_critical_section_exit(&ctx);
    }
}



#endif  /* PK_TIMER_SUPPORT */

#undef __PPE42_CORE_C__
