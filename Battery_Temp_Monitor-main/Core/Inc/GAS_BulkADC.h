/*
 * GAS_BulkADC.h
 *
 *  Created on: Oct 30, 2021
 *      Author: aswww
 */

#ifndef INC_GAS_BULKADC_H_
#define INC_GAS_BULKADC_H_

#include "GAS_Spi.h"
#include "GAS_Can.h"

extern void GAS_BulkADC_run_100ms(uint8_t CScount);
extern void GAS_BulkADC_select(uint8_t CScount);
#endif /* INC_GAS_BULKADC_H_ */
