
#include <main.h>



void main(void) {

    int i;

    const char transmit_char = 0xad;

//    int test_rssi;

    volatile int ver,readback;

    unsigned long curr_time;

    volatile int read_data;

    curr_time = 0;

//    printf("Hey There!\n");


    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= 0x01;                          // Set P1.0 to output direction

    setup_millis();

    initSPI();

//    writeReg(0x10,0x24);

//    ver = readReg(0x10);

//    writeReg(0x39,0xda);

//    readback = readReg(0x1);



//    test_rssi = readRSSI(false);

      rfm69_initialize(RF69_915MHZ,0,0);

      setMode(RF69_MODE_RX);

    for(;;) {

//    send(0,(const void*)&transmit_char,1,false);



    P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR



    if(receiveDone())
    {
        read_data = readReg(REG_FIFO);
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
