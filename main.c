
#include <main.h>



void main(void) {

    int i;

    volatile unsigned long time;

    const char transmit_char = 0xad;

//    int test_rssi;

    volatile int ver,readback;

    unsigned long curr_time;

    volatile int read_data;

    unsigned int sample;

    curr_time = 0;

    char hey[] = "HEY";

//    printf("Hey There!\n");


    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= 0x01;                          // Set P1.0 to output direction

    setup_millis();

    initSPI();

//    Init_LCD();

//    showChar(hey[0],pos1);
//    showChar(hey[1],pos2);
//    showChar(hey[2],pos3);

//    writeReg(0x10,0x24);

//    ver = readReg(0x10);

//    writeReg(0x39,0xda);

//    readback = readReg(0x1);



//    test_rssi = readRSSI(false);



      rfm69_initialize(RF69_915MHZ,0,0);

      initialize_adc();

//      setMode(RF69_MODE_RX);

    setMode(RF69_MODE_TX);

    for(;;) {


//    for(i=0;i<10;i++){
//    send(0,(const void*)&transmit_char,1,false);
//    }

//    time=(unsigned long)((1000UL*(unsigned long)RTCPS)>>15);// + (unsigned long)((1000UL*(unsigned long)RTCTIM0));//=millis();

//    ver = readReg(0x10);

    P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR


/*
    if(receiveDone())
    {
        read_data = readReg(REG_FIFO);
    }
*/


    if (ADC12IFGR0 == ADC12IFG11){
        // Grab ADC output value from register ADC12MEM11
        //printf("ADC output: %d\n",ADC12MEM11);
        sample = ADC12MEM11;
        send(0,(const void*)&sample,1,false);
    }



//    i = 10000;                          // SW Delay
//    do i--;
//    while(i != 0);
//
//    }

//    curr_time = millis();

//    while((millis()-curr_time) < 4000);

}
}
