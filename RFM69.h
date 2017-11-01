// **********************************************************************************

// Driver definition for HopeRF RFM69W/RFM69HW/RFM69CW/RFM69HCW, Semtech SX1231/1231H

// **********************************************************************************

// Copyright Felix Rusu 2016, http://www.LowPowerLab.com/contact

// **********************************************************************************

// License

// **********************************************************************************

// This program is free software; you can redistribute it

// and/or modify it under the terms of the GNU General

// Public License as published by the Free Software

// Foundation; either version 3 of the License, or

// (at your option) any later version.

//

// This program is distributed in the hope that it will

// be useful, but WITHOUT ANY WARRANTY; without even the

// implied warranty of MERCHANTABILITY or FITNESS FOR A

// PARTICULAR PURPOSE. See the GNU General Public

// License for more details.

//

// Licence can be viewed at

// http://www.gnu.org/licenses/gpl-3.0.txt

//

// Please maintain this license information along with authorship

// and copyright notices in any redistribution of this code

// **********************************************************************************

#ifndef RFM69_h

#define RFM69_h

//#include <Arduino.h>            // assumes Arduino IDE v1.0 or greater

//#include <main.h>
#include <stdbool.h>


#define RF69_MAX_DATA_LEN       61 // to take advantage of the built in AES/CRC we want to limit the frame size to the internal FIFO size (66 bytes - 3 bytes overhead - 2 bytes crc)

#define RF69_SPI_CS             SS // SS is the SPI slave select pin, for instance D10 on ATmega328



// INT0 on AVRs should be connected to RFM69's DIO0 (ex on ATmega328 it's D2, on ATmega644/1284 it's D2)

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega88) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega88__)

  #define RF69_IRQ_PIN          2

  #define RF69_IRQ_NUM          0

#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)

  #define RF69_IRQ_PIN          2

  #define RF69_IRQ_NUM          2

#elif defined(__AVR_ATmega32U4__)

  #define RF69_IRQ_PIN          3

  #define RF69_IRQ_NUM          0

#elif defined(__arm__)//Use pin 10 or any pin you want

  #define RF69_IRQ_PIN          10

  #define RF69_IRQ_NUM          10

#else

  #define RF69_IRQ_PIN          2

  #define RF69_IRQ_NUM          0

#endif





#define CSMA_LIMIT              -90 // upper RX signal sensitivity threshold in dBm for carrier sense access

#define RF69_MODE_SLEEP         0 // XTAL OFF

#define RF69_MODE_STANDBY       1 // XTAL ON

#define RF69_MODE_SYNTH         2 // PLL ON

#define RF69_MODE_RX            3 // RX MODE

#define RF69_MODE_TX            4 // TX MODE



// available frequency bands

#define RF69_315MHZ            31 // non trivial values to avoid misconfiguration

#define RF69_433MHZ            43

#define RF69_868MHZ            86

#define RF69_915MHZ            91



#define null                  0

#define COURSE_TEMP_COEF    -90 // puts the temperature reading in the ballpark, user can fine tune the returned value

#define RF69_BROADCAST_ADDR 255

#define RF69_CSMA_LIMIT_MS 1000

#define RF69_TX_LIMIT_MS   1000

#define RF69_FSTEP  61.03515625 // == FXOSC / 2^19 = 32MHz / 2^19 (p13 in datasheet)



// TWS: define CTLbyte bits

#define RFM69_CTL_SENDACK   0x80

#define RFM69_CTL_REQACK    0x40



//class RFM69 {
//
//  public:

//    static volatile char DATA[RF69_MAX_DATA_LEN]; // recv/xmit buf, including header & crc bytes
//
//    static volatile char DATALEN;
//
//    static volatile char SENDERID;
//
//    static volatile char TARGETID; // should match _address
//
//    static volatile char PAYLOADLEN;
//
//    static volatile char ACK_REQUESTED;
//
//    static volatile char ACK_RECEIVED; // should be polled immediately after sending a packet with ACK request
//
//    static volatile int RSSI; // most accurate RSSI during reception (closest to the reception)
//
//    static volatile char _mode = RF69_MODE_STANDBY; // should be protected?

      volatile char DATA[RF69_MAX_DATA_LEN]; // recv/xmit buf, including header & crc bytes

      volatile char DATALEN;

      volatile char SENDERID;

      volatile char TARGETID; // should match _address

      volatile char PAYLOADLEN;

      volatile char ACK_REQUESTED;

      volatile char ACK_RECEIVED; // should be polled immediately after sending a packet with ACK request

      volatile int RSSI; // most accurate RSSI during reception (closest to the reception)

      volatile char _mode;// = RF69_MODE_STANDBY; // should be protected?



    //RFM69(char slaveSelectPin=RF69_SPI_CS, char interruptPin=RF69_IRQ_PIN, bool isRFM69HW=false, char interruptNum=RF69_IRQ_NUM) {

//      _slaveSelectPin = slaveSelectPin;

    //}



    bool rfm69_initialize(char freqBand, char ID, char networkID);

    void setAddress(char addr);

    void setNetwork(char networkID);

    bool canSend();

    void send(char toAddress, const void* buffer, char bufferSize, bool requestACK);

    //bool sendWithRetry(char toAddress, const void* buffer, char bufferSize, char retries=2, char retryWaitTime=40); // 40ms roundtrip req for 61byte packets

    bool sendWithRetry(char toAddress, const void* buffer, char bufferSize, char retries, char retryWaitTime); // 40ms roundtrip req for 61byte packets

    bool receiveDone();

    bool ACKReceived(char fromNodeID);

    bool ACKRequested();

    void sendACK(const void* buffer, char bufferSize);

    unsigned long getFrequency();

    void setFrequency(unsigned long freqHz);

    void encrypt(const char* key);

    int readRSSI(bool forceTrigger);

    void promiscuous(bool onOff);

    void setHighPower(bool onOFF); // has to be called after initialize() for RFM69HW

    void setPowerLevel(char level); // reduce/increase transmit power level

    void rf69_sleep();

    void rcCalibration(); // calibrate the internal RC oscillator for use in wide temperature variations - see datasheet section [4.3.5. RC Timer Accuracy]

    static void isr0();

    void interruptHandler();

    void interruptHook(char CTLbyte);

    bool _inISR;

    void sendFrame(char toAddress, const void* buffer, char size, bool requestACK, bool sendACK);


    //static RFM69* selfPointer;

    char _slaveSelectPin;

    char _interruptPin;

    char _interruptNum;

    char _address;

    bool _promiscuousMode;

    char _powerLevel;

    bool _isRFM69HW;


#if defined (SPCR) && defined (SPSR)

    char _SPCR;

    char _SPSR;

#endif



    void receiveBegin();

    void setMode(char mode);

    void setHighPowerRegs(bool onOff);


//};



#endif
