//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file base_main.c
/// \brief base program that creates and starts a thread
///
/// This file is a placeholder code in order to compile. I will be replaced in future by the code placed into the .text section.

extern "C" {
#include "pk.h"
#include "pk_trace.h"
#include "base_ppe_main.h"
#include "sbe_xip_image.h"
}
#define KERNEL_STACK_SIZE   256
#define MAIN_THREAD_STACK_SIZE  256

uint8_t     G_kernel_stack[KERNEL_STACK_SIZE];
uint8_t     G_main_thread_stack[MAIN_THREAD_STACK_SIZE];
PkThread    G_main_thread;

// A simple thread that just increments a local variable and sleeps
void main_thread(void* arg)
{

    while(1)
    {

        pk_sleep(PK_SECONDS(1));

    }
}


// The main function is called by the boot code (after initializing some
// registers)
int main(int argc, char **argv)
{
    // initializes kernel data (stack, threads, timebase, timers, etc.)
    pk_initialize((PkAddress)G_kernel_stack,
                  KERNEL_STACK_SIZE,
                  0,
                  500000000);


    //Initialize the thread control block for G_main_thread
    pk_thread_create(&G_main_thread,
                      (PkThreadRoutine)main_thread,
                      (void*)NULL,
                      (PkAddress)G_main_thread_stack,
                      (size_t)MAIN_THREAD_STACK_SIZE,
                      (PkThreadPriority)1);



    //Make G_main_thread runnable
    pk_thread_resume(&G_main_thread);

    // Start running the highest priority thread.
    // This function never returns
    pk_start_threads();
    
    return 0;
}
