/*
 * i2c_master.h
 *
 *  Created on: Nov 8, 2017
 *      Author: Tony
 */


#define SDA_PIN 0x01                                  // msp430fr6989 eUCB1SDA pin
#define SCL_PIN 0x02                                  // msp430fr6989 eUCB1SCL pin

void I2C_RX_init( char slave_address, int prescale );
void I2C_TX_init( char slave_address, int prescale );

void DAC_RX( char *RX_data_ptr );
void DAC_TX_FM( short DAC_data );
void DAC_TX( short DAC_data, char WRITE_EEPROM );

char I2C_slave_present( void );
char I2C_not_ready( void );
