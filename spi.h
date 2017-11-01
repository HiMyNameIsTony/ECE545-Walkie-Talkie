/*
 * spi.h
 *
 *  Created on: Oct 1, 2017
 *      Author: Dylan
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

void initSPI();

void writeReg(char addr ,char write_val);

void writeMultipleReg(char addr,char* data, unsigned int num_bytes);

char readReg(char addr);

void send_addr(char addr);

char read_single();

void select();

void unselect();

#endif /* INC_SPI_H_ */
