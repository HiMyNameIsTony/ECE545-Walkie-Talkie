/*
 * spi.c
 *
 *  Created on: Oct 1, 2017
 *      Author: Dylan
 */

#include <main.h>

void initSPI(){

    P4SEL0 |= BIT2 | BIT3; //spi mode for these pins
    P4SEL1 &= ~(BIT2 | BIT3);

    P1SEL0 &= ~BIT5;
    P1SEL1 |= BIT5;

//    P1OUT |= BIT4;
//    P1DIR |= BIT4;

    P2OUT  |= BIT0;
    P2DIR  |= BIT0;

    UCA0CTLW0 = UCSWRST;
    UCA0CTLW0 |= UCCKPH | UCMSB | UCMST | UCSYNC; // 3-pin, 8-bit SPI master
    UCA0CTL1 |= UCSSEL__SMCLK; // SMCLK
    UCA0BR0 |= 0x10;//0x02; // /2
    UCA0BR1 = 0; //
    UCA0MCTLW = 0; // No modulation
    UCA0CTLW0 &= ~UCSWRST; // **Initialize USCI state machine**

}

void writeReg(char addr ,char write_val){

    volatile char dummy_data;

    select();

    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = addr | 0x80; // Send addr to slave (set MSB high for a write)
    while (!(UCA0IFG & UCRXIFG)); // USCI_A0 RX Received?
    dummy_data = UCA0RXBUF; // (dummy read)


    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = write_val; // Send data to slave
    while (!(UCA0IFG & UCRXIFG)); // USCI_A0 RX Received?
    dummy_data = UCA0RXBUF; // (dummy read)

    unselect();
}

void writeMultipleReg(char addr,char* data, unsigned int num_bytes){

    volatile char dummy_data;

    char i;

    select();

    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = addr | 0x80; // Send addr over SPI to Slave(assert MSB high for write)
    while (!(UCA0IFG & UCRXIFG)); // USCI_A0 RX Received?
    dummy_data = UCA0RXBUF; // (dummy read)


    for (i = 0; i < num_bytes; i++){

        while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
        UCA0TXBUF = data[i]; // Send data over SPI to Slave
        while (!(UCA0IFG & UCRXIFG)); // USCI_A0 RX Received?
        dummy_data = UCA0RXBUF; // (dummy read)

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


void send_addr(char addr){

    volatile char read_data;

    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = addr; // Send addr over SPI to Slave
    while (!(UCA0IFG & UCRXIFG)); // USCI_A0 RX Received?
    read_data = UCA0RXBUF; // (dummy read)


}

char read_single(){

    char read_data;


    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = 0x00; // (dummy write)
    while (!(UCA0IFG & UCRXIFG)); // USCI_A0 RX Received?
    read_data = UCA0RXBUF; // Store received data

    return read_data;
}


void select(){

    noInterrupts();

    //P1OUT &= (~BIT4);
    P2OUT &= (~BIT0);
}

void unselect(){
    //P1OUT |= (BIT4);
    P2OUT |= BIT0;

    maybeInterrupts();
}


