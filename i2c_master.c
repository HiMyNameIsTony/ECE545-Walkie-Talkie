/*
 * i2c_master.c
 *
 *  Created on: Nov 8, 2017
 *      Author: Tony
 */
#include <msp430.h>
#include <i2c_master.h>

char *RX_Data;
char TX_Data[3];
char TX_Data_i;

/*****************************************************************\
 * This function initializes the USCI_B1 for Master receive I2C  *
 * operations.                                                   *
 *                                                               *
 * IN:  slave_address   => 7-bit, right-justified slave address  *
 *      prescale        => SMCLK divider                         *
\*****************************************************************/
void I2C_RX_init( char slave_address, int prescale ) {
    P4SEL0 &= ~(SDA_PIN | SCL_PIN);             // Assign I2C pins to USCI_B1
    P4SEL1 |= SDA_PIN | SCL_PIN;
    UCB1CTLW0 = UCSWRST;                        // put USB1 reset state
    UCB1CTLW0 |= UCMODE_3 | UCMST | UCSYNC;     // I2C master mode
    UCB1CTLW0 |= UCSSEL_2;                      // select the SMCLK
    UCB1CTLW1 = UCASTP_2;                       // auto-gen a stop after last byte RX
    UCB1BRW = prescale;                         // baudrate = SMCLK / prescale
    UCB1I2CSA = slave_address;                  // configure slave address
    UCB1CTLW0 &= ~UCSWRST;                      // clear reset register
    UCB1IE |= UCNACKIE | UCRXIE0;               // receive and NACK interrupt enable
}

/*****************************************************************\
 * This function initializes the USCI_B1 for Master transmit I2C *
 * operations.                                                   *
 *                                                               *
 * IN:  slave_address   => 7-bit, right-justified slave address  *
 *      prescale        => SMCLK divider                         *
\*****************************************************************/
void I2C_TX_init( char slave_address, int prescale ) {
    P4SEL0 &= ~SDA_PIN & ~SCL_PIN;              // Assign I2C pins to USCI_B1
    P4SEL1 |= SDA_PIN | SCL_PIN;
    UCB1CTLW0 = UCSWRST;                        // put USB1 reset state
    UCB1CTLW0 |= UCMODE_3 | UCMST | UCSYNC;     // I2C master mode
    UCB1CTLW0 |= UCSSEL_2;                      // select the SMCLK
    UCB1CTLW1 = UCASTP_2 | UCTR;                // TX mode and auto-gen stop
    UCB1BRW = prescale;                         // baudrate = SMCLK / prescale
    UCB1I2CSA = slave_address;                  // configure slave address
    UCB1CTLW0 &= ~UCSWRST;                      // clear reset register
    UCB1IE |= UCNACKIE | UCTXIE0;               // transmit and NACK interrupt enable
}

/*****************************************************************\
 * This function receives 5 bytes of data from the DAC.          *
 *                                                               *
 * IN:  RX_data_ptr     => pointer to data storage of RX         *
\*****************************************************************/
void DAC_RX( char *RX_data_ptr ) {
    RX_Data = RX_data_ptr;                      // gotta know where to put data
                                                // from the ISR
    UCB1TBCNT = 5;                              // set the number of bytes to be
                                                // RX'd before auto-gen'd stop
    UCB1CTLW0 |= UCTXSTT;                       // start condition
    while( UCB1CTLW0 & UCTXSTT );
    while( I2C_not_ready() );
}

/*****************************************************************\
 * This function sends the DAC a 12-bit payload for output using *
 * "Fast mode" meaning it only sends 2 bytes vice 3.             *
 *                                                               *
 * IN:  DAC_data    => 12-bit payload to be sent                 *
\*****************************************************************/
void DAC_TX_FM( short DAC_data ) {
    TX_Data[0] = ( DAC_data >> 8 ) & 0x0F;      // top 4 bits of first byte must be zero
                                                // bottom 4 bits are top 4 of 12-bit DAC
    TX_Data[1] = DAC_data & 0xFF;               // second byte is bottom 8 bits
    TX_Data_i = 0;
    UCB1TBCNT = 2;                              // set the number of bytes to be
                                                // TX'd before auto-gen'd stop
    UCB1CTLW0 |= UCTR | UCTXSTT;                // start condition
    while( UCB1CTLW0 & UCTXSTT );
    while( I2C_not_ready() );
}

/*****************************************************************\
 * This function sends the DAC a 12-bit payload for output.      *
 *                                                               *
 * IN:  DAC_data        => 12-bit payload to be sent             *
 *      WRITE_EEPROM    => non-zero value will write DAC_data to *
 *                         both DAC output and EEPROM            *
\*****************************************************************/
void DAC_TX( short DAC_data, char WRITE_EEPROM ) {
    if( WRITE_EEPROM ) {
        TX_Data[0] = 0x60;                      // first byte is command data
    } else {
        TX_Data[0] = 0x40;                      // first byte is command data
    }
    TX_Data[1] = ( DAC_data >> 4 ) & 0xFF;      // second byte is DAC_data[11:4]
    TX_Data[2] = ( DAC_data << 4 ) & 0xF0;      // third byte DAC_data[3:0] shifted 4 left
    TX_Data_i = 0;
    UCB1TBCNT = 3;                              // set the number of bytes to be
                                                // TX'd before auto-gen'd stop
    UCB1CTLW0 |= UCTR | UCTXSTT;                // start condition
    while( UCB1CTLW0 & UCTXSTT );
    while( I2C_not_ready() );
}

/*****************************************************************\
 * This  function can be used to see if a given slave address is *
 * present, using the slave_address from either init function.   *
 *                                                               *
 * OUT: char       => 0: slave is not present                    *
 *                    1: slave is present                        *
\*****************************************************************/
char I2C_slave_present( void ) {
    if(!UCB1I2CSA) return 0;
    char rtn_val;                               // temp vars
    __disable_interrupt();
    UCB1CTLW0 |= UCTXSTT | UCTXSTP;             // send just the address
    while( UCB1CTLW0 & UCTXSTT );               // wait for Start to clear
    rtn_val = !(UCB1IFG & UCNACKIFG);           // store NACK flag
    if(!rtn_val) {                              // if slave not there...
        UCB1CTLW0 |= UCTXSTP;                   // ...send stop again
        while( (UCB1IFG & UCSTPIFG) );          // wait for it to finish
        UCB1IFG &= ~UCNACKIFG;                  // clear the NACK flag
    }
    __enable_interrupt();
    return rtn_val;                             // return 0 if NACK'd

}

/*****************************************************************\
 * This function can be used to see if the I2C bus is currently  *
 * in use.                                                       *
 *                                                               *
 * OUT: unsigned char   => zero: I2C bus is idle                 *
 *                         non-zero: I2C bus is busy             *
\*****************************************************************/
char I2C_not_ready( void ) {
    return (UCB1STATW & UCBBUSY);
}


#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR( void ) {
    switch( __even_in_range( UCB1IV, USCI_I2C_UCBIT9IFG ) ) {
        case USCI_I2C_UCNACKIFG:                // Vector 4: NACKIFG
            UCB1CTLW0 |= UCTXSTP;               // send stop if NACK
            break;

        case USCI_I2C_UCSTTIFG:  break;         // Vector 6: STTIFG

        case USCI_I2C_UCSTPIFG:  break;         // Vector 8: STPIFG

        case USCI_I2C_UCRXIFG0:                 // Vector 22: RXIFG0
            *RX_Data = UCB1RXBUF;               // read a byte from the buffer
            RX_Data++;                          // then increment the ptr
            break;

        case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
            UCB1TXBUF = TX_Data[TX_Data_i];     // read a byte from the buffer
            TX_Data_i++;                        // then increment the ptr
            break;

        default: break;
    }
}
