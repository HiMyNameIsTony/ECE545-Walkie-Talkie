
#include <main.h>

unsigned int sample_num;
unsigned int rx_sample[150];
unsigned char sample[150];
volatile int dummy;


void main(void) {


    const char slave_address = 0x62;                // MCP4725 I2C address
    const int PRESCALE = 51;//13;//80;//13;//2;//13;                        // baudrate = SMCLK / PRESCALE = 5.33MHz/13 = 410Hz

//    printf("Hey There!\n");


    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= 0x81;                          // Set P1.0 to output direction

    P1REN |= BIT2;

    P1OUT |= BIT2;

    FRCTL0 = 0xA500 + NWAITS0;

    CSCTL0 = CSKEY;                                 // unlock clock settings
    CSCTL1 = DCORSEL + DCOFSEL_4;//DCOFSEL_6;//DCOFSEL_4;                             // set DCO to 5.33MHz
    CSCTL3 = DIVS_0 | DIVM_0;                       // set SMCLK and MCLK to 5.33MHz/1
    CSCTL0_H = 0;                                   // lock clock settings


    setup_millis();

    initSPI();

 //   Init_LCD();

 //   showChar(hey[0],pos1);
 //   showChar(hey[1],pos2);
 //   showChar(hey[2],pos3);





      rfm69_initialize(RF69_915MHZ,0,0);//1,0xda);

      initialize_adc();




      __enable_interrupt();
      I2C_TX_init( slave_address, PRESCALE );         // init transmissions to slave at a rate of ~400kHz


//        setMode(RF69_MODE_TX);
//      setMode(RF69_MODE_RX);
   //P1OUT = 0;

   setPowerLevel(10);



    for(;;)
    {
//        transmit();
//        receive();

        if(!(P1IN & BIT2))
        {
            //setMode(RF69_MODE_TX);
            //while ((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady
            transmit();
        }else
        {
//            setMode(RF69_MODE_RX);
            //while ((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady
            receive();
        }

    }
}

void transmit()
{

    char i=0;

    //setMode(RF69_MODE_TX);

    while(!(P1IN & BIT2))
    {

        while(sample_num < 60)
        {

            if (ADC12IFGR0 == ADC12IFG11)
            {
                // Grab ADC output value from register ADC12MEM11
                sample[sample_num] = ADC12MEM11;
                dummy = ADC12MEM11;


                //while(readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_FIFOFULL);
                //   writeReg(REG_FIFO, sample[0]);

                sample_num ++;

                P1OUT ^= 0x81;

            }

        }

        if(sample_num == 60){
        send(0x0,sample,60,false);
        }

    //    i^=1;

        //printf("%x\n",readReg(REG_IRQFLAGS1));
        //printf("%x\n",readReg(REG_IRQFLAGS2));

        sample_num =0;

    }
//
//    sample_num =0;

}


void receive()
{
    while(P1IN & BIT2)
    {
        unsigned int byte_num=0;

        //    setMode(RF69_MODE_RX);

        if(receiveDone())
        {

            select();
            send_addr(REG_FIFO);

            dummy = read_single();
            dummy = read_single();
            dummy = read_single();
            dummy = read_single();
            dummy = read_single();
            dummy = read_single();
            dummy = read_single();

            for(byte_num=0;byte_num<52;byte_num++)
            {

                rx_sample[sample_num] = read_single();

                sample_num++;
            }

            unselect();
        }



        if(sample_num>0)
        {
            if (ADC12IFGR0 == ADC12IFG11)
            {
             dummy = ADC12MEM11;

             DAC_TX_FM(rx_sample[sample_num]<<4);

             sample_num--;

        //          if(rx_sample[sample_num] == 255)
        //              P1OUT ^= 0x81;
        //          else
        //              P1OUT = 0x0;
            }

        }

        if(sample_num == 0)
        {
           DAC_TX_FM(2048);
        }
  }

    sample_num = 0;
    writeReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00); // DIO0 is "Packet Sent"
    writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
    while (!canSend()) receiveDone();
    setMode(RF69_MODE_STANDBY);
}


