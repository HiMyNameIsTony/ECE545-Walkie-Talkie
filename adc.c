/*
 * adc.c
 *
 *  Created on: Nov 6, 2017
 *      Author: juancolon
 */

#include <main.h>

// Function to configure and initialize ADC (Repeat-Single-Channel Mode)
void initialize_adc(void){
      ADC12CTL0 &= ~ADC12ENC;                   // Disable ADC12 before doing any configurations
/*
      ADC12CTL0 = ADC12SHT1_8 + ADC12ON + ADC12MSC;        // Set sample time, turn on ADC and set continuous sampling (multiple samples conv)
      ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 + ADC12PDIV__64 + ADC12DIV_7;     // Enable sampling timer (pulse mode) and Repeat-single-channel mode    
*/

      ADC12CTL0 = ADC12SHT1_10 + ADC12ON + ADC12MSC;        // Set sample time, turn on ADC and set continuous sampling (multiple samples conv)
      ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 + ADC12DIV0;                 // Enable sampling timer (pulse mode) and Repeat-single-channel mode

      ADC12CTL2 = ADC12RES__8BIT;                   // 8-bit adc resolution
      ADC12CTL3 = ADC12CSTARTADD_11;   // Change conversion start address to 0xB (i,e channel 11)
      ADC12MCTL11 = ADC12VRSEL_0 + ADC12INCH_11;   // Set ADC sample channel to Microphone connected to P9.3, analog pin: A11
      ADC12CTL0 |= ADC12ENC;  // Set ADC12ENC to enable conversion
      start_adc_sampling();
}

void start_adc_sampling(void){
    ADC12CTL0 |= ADC12SC;
}


