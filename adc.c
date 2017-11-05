#include <msp430.h>
#include <stdio.h>


void initialize_adc(void);
void start_adc_sampling(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WatchDog Time
  initialize_adc();
  start_adc_sampling();

    while(1){
    		if (ADC12IFGR0 == ADC12IFG11){
    			// Grab ADC output value from register ADC12MEM11
    			//printf("ADC output: %d\n",ADC12MEM11);
    		}
    }
  return 0;
}

// Function to configure and initialize ADC
void initialize_adc(void){
	  ADC12CTL0 &= ~ADC12ENC;                   // Disable ADC12 before doing any configurations
	  ADC12CTL0 = ADC12SHT1_8 + ADC12ON + ADC12MSC;        // Set sample time and set continuous sampling (multiple samples conv)
	  ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2;     // Enable sampling timer (pulse mode) and Repeat-single-channel mode
	  ADC12CTL3 = ADC12CSTARTADD0 + ADC12CSTARTADD1 + ADC12CSTARTADD3;   // Change conversion start address to 0xB (i,e channel 11)
	  ADC12MCTL11 = ADC12VRSEL_0 + ADC12INCH_11;   // Set ADC sample channel to Microphone connected to P9.3, analog pin: A11
	  ADC12IER0 = ADC12IE11; 		// Enables the interrupt request for ADC12IFG11 bit.
	  ADC12CTL0 |= ADC12ENC;  // Set ADC12ENC to enable conversion
}

void start_adc_sampling(void){
    ADC12CTL0 |= ADC12SC;
}
