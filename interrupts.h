/*
 * interrupts.h
 *
 *  Created on: Oct 6, 2017
 *      Author: Dylan
 */

#include <main.h>

#ifndef INC_INTERRUPTS_H_
#define INC_INTERRUPTS_H_

void noInterrupts();
void interrupts();
inline void maybeInterrupts();

#endif /* INC_INTERRUPTS_H_ */
