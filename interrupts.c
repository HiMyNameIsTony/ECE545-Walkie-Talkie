/*
 * interrupts.c
 *
 *  Created on: Oct 6, 2017
 *      Author: Dylan
 */

#include<main.h>


void noInterrupts(){
    __disable_interrupt();
}

void interrupts(){
    __enable_interrupt();
}

inline void maybeInterrupts()

{

        // Only reenable interrupts if we're not being called from the ISR

        if (!_inISR) interrupts();

}
