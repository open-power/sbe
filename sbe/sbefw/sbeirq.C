/*
 * @file: ppe/sbe/sbefw/sbeirq.C
 *
 * @brief This sets up and registers SBE ISRs
 *
 */

#include "sbeexeintf.H"
#include "sbeirq.H"
#include "sbetrace.H"
#include "assert.h"

extern sbeIntrHandle_t g_sbeIntrSource ;

////////////////////////////////////////////////////////////////
// @brief:     SBE control loop ISR:
//               - FIFO new data available
//               - FIFO reset request
//               - PSU new data available
//
// @param[in]  i_pArg - Unused
// @param[in]  i_irq  - IRQ number as defined in sbeirq.h
//
////////////////////////////////////////////////////////////////
void sbe_interrupt_handler (void *i_pArg, PkIrqId i_irq)
{
    #define SBE_FUNC " sbe_interrupt_handler "
    SBE_ENTER(SBE_FUNC"i_irq=[0x%02X]",i_irq);

    int l_rc = 0;

    switch (i_irq)
    {
        case SBE_IRQ_HOST_PSU_INTR:
            g_sbeIntrSource.setIntrSource(SBE_INTERFACE_PSU);
            break;

        case SBE_IRQ_SBEFIFO_DATA:
            g_sbeIntrSource.setIntrSource(SBE_INTERFACE_FIFO);
            pk_irq_disable(SBE_IRQ_SBEFIFO_RESET);
            break;

        case SBE_IRQ_SBEFIFO_RESET:
            g_sbeIntrSource.setIntrSource(SBE_INTERFACE_FIFO_RESET);
            pk_irq_disable(SBE_IRQ_SBEFIFO_DATA);
            break;

        default:
            SBE_ERROR(SBE_FUNC"Unknown IRQ, assert");
            assert(0);
            break;
    }
    // Mask the interrupt
    pk_irq_disable(i_irq);

    // Unblock the command receiver thread
    l_rc = pk_semaphore_post(&g_sbeSemCmdRecv);
    if (l_rc)
    {
        // If we received an error while posting the semaphore,
        // unmask the interrupt back and assert
        SBE_ERROR(SBE_FUNC"pk_semaphore_post failed, rc=[%d]", l_rc);
        pk_irq_enable(i_irq);
        assert(!l_rc);
    }
    #undef SBE_FUNC
}

////////////////////////////////////////////////////////////////
// See sbeexeintf.h for more details
////////////////////////////////////////////////////////////////
int sbeIRQSetup (void)
{
    #define SBE_FUNC " sbeIRQSetup "
    int l_rc = 0;
    PkIrqId l_irq;

    // Disable the relevant IRQs while we set them up
    pk_irq_disable(SBE_IRQ_HOST_PSU_INTR);
    pk_irq_disable(SBE_IRQ_SBEFIFO_DATA);
    pk_irq_disable(SBE_IRQ_SBEFIFO_RESET);

    do
    {
        // Register the IRQ handler with PK

        // PSU New data available interrupt
        l_irq = SBE_IRQ_HOST_PSU_INTR;
        l_rc = pk_irq_handler_set(l_irq, sbe_interrupt_handler, NULL);
        if(l_rc)
        {
            break;
        }

        // FIFO New data available interrupt
        l_irq = SBE_IRQ_SBEFIFO_DATA;
        l_rc = pk_irq_handler_set(l_irq, sbe_interrupt_handler, NULL);
        if(l_rc)
        {
            break;
        }

        // FIFO Reset request
        l_irq = SBE_IRQ_SBEFIFO_RESET;
        l_rc = pk_irq_handler_set(l_irq, sbe_interrupt_handler, NULL);
        if(l_rc)
        {
            break;
        }

        // Enable the IRQ
        pk_irq_enable(SBE_IRQ_SBEFIFO_RESET);
        pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);
        pk_irq_enable(SBE_IRQ_HOST_PSU_INTR);
    } while(false);

    if (l_rc)
    {
        SBE_ERROR (SBE_FUNC"pk_irq_handler_set failed, IRQ=[0x%02X], "
            "rc=[%d]", l_irq, l_rc);
    }

    return l_rc;
    #undef SBE_FUNC
}

