/*
 * GAS_SPI.c
 *
 *  Created on: Mar 5, 2021
 *      Author: aswww
 *      		InitusNovus
 *      		TODO: SPI PINs
 */

#include "GAS_Spi.h"

//uint16_t MCP3204[8];

uint16_t GAS_Spi_TransmitReceive(unsigned short data){

	uint8_t buff[3];


	//TODO: CS select
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);

	buff[0] = 0x06|((data & 0x07)>>2);
	buff[1] = ((data&0x07)<<6);
	buff[2] = 0;

	HAL_SPI_TransmitReceive(&hspi3,buff,buff,3,1000);
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);


	return (uint16_t)((buff[1] & 0x0f)<<8) | buff[2];

}

void GAS_Spi_getAll(uint16_t output[]){
	for(unsigned short i=0;i<8;i++){

		output[i] = GAS_Spi_TransmitReceive(i);

	}
}



