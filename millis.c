/*
 * millis.c
 *
 *  Created on: Oct 6, 2017
 *      Author: Dylan
 */

#include <main.h>

unsigned long millis(){

    unsigned long msec;

    msec = (1000*(unsigned long)RTCPS)>>15 + (1000*(unsigned long)RTCTIM0);

    return msec;

}

void setup_millis(){


    PJSEL1 &= ~(BIT4 | BIT5);
    PJSEL0 |= BIT4 | BIT5;

//    PM5CTL0 &= ~LOCKLPM5;

    CSCTL0 = CSKEY;
    CSCTL4 &= ~LFXTOFF;

    do
    {
        CSCTL5 &= ~LFXTOFFG;
        SFRIFG1 &= ~OFIFG;

    }while(SFRIFG1 & OFIFG);

    RTCCTL0 = RTCKEY;

    RTCCTL13 |= RTCHOLD;

    RTCCTL13 |= RTCSSEL_2;

    RTCPS1CTL |= RT1SSEL_2;

    RTCCTL13 &= ~RTCMODE;

    RTCPS    = 0;

    RTCTIM0  = 0;

    RTCTIM1  = 0;

    RTCCTL13 &= ~RTCHOLD;

    RTCPS0CTL |= RT0PSDIV_7;

    RTCPS1CTL |= RT1PSDIV_6;

    RTCPS0CTL &= ~RT0PSHOLD;

    RTCPS1CTL &= ~RT1PSHOLD;

    RTCCTL0 &= ~RTCOFIFG;
}
