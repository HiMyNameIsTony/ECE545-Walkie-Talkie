/*
 * spi.c
 *
 *  Created on: Oct 1, 2017
 *      Author: Dylan
 */


void initSPI(){

    P4SEL0 |= BIT2 | BIT3; //spi mode for these pins
    P4SEL1 &= ~(BIT2 | BIT3);

    P1SEL0 &= ~BIT5;
    P1SEL1 |= BIT5;

    P1OUT |= BIT4;
    P1DIR |= BIT4;

    UCA0CTLW0 = UCSWRST;
    UCA0CTLW0 |= UCCKPH | UCMSB | UCMST | UCSYNC; // 3-pin, 8-bit SPI master
    UCA0CTL1 |= UCSSEL__SMCLK; // SMCLK
    UCA0BR0 |= 0x02; // /2
    UCA0BR1 = 0; //
    UCA0MCTLW = 0; // No modulation
    UCA0CTLW0 &= ~UCSWRST; // **Initialize USCI state machine**

}

void writeReg(char addr ,char write_val){
    select();

    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = addr | 0x80; // Send addr to slave (set MSB high for a write)

    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = write_val; // Send data to slave

    unselect();
}

void writeMultipleReg(char addr,char* data, unsigned int num_bytes){

    select();

    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = addr | 0x80; // Send addr over SPI to Slave(assert MSB high for write)


    for (uint8_t i = 0; i < num_bytes; i++){

        while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
        UCA0TXBUF = data[i]; // Send data over SPI to Slave

    }


    unselect();

}

char readReg(char addr){

    char read_data;

    select();

    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = addr; // Send addr over SPI to Slave
    while (!(UCA0IFG & UCRXIFG)); // USCI_A0 RX Received?
    read_data = UCA0RXBUF; // (dummy read)


    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = 0x00; // (dummy write)
    while (!(UCA0IFG & UCRXIFG)); // USCI_A0 RX Received?
    read_data = UCA0RXBUF; // Store received data

    unselect();

    return read_data;
}

void select(){

    P1OUT &= (~BIT4);
}

void unselect(){
    P1OUT |= (BIT4);
}
