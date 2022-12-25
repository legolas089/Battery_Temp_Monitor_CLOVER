/*
 * CAN.h
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#ifndef SRC_GETANDSEND_H_
#define SRC_GETANDSEND_H_

#include "stm32f4xx_hal.h"
#include "can.h"


typedef union{
	uint8_t TxData[8];
	uint8_t RxData[8];
	struct{
		unsigned int HighestTemp : 12;
		unsigned int LowestTemp : 12;
		unsigned int MeanTemp : 12;
		unsigned int reserved : 12;
		unsigned int reserved1 : 12;
		unsigned int reserved2: 4;

	}__attribute__((aligned(1),packed)) B;

}BatteryTemp_t;

typedef union{
	uint8_t TxData[8];
	uint8_t RxData[8];
	struct{
		uint8_t prechargeStateSignal1 ;
		uint8_t prechargeStateSignal2 ;
		uint8_t RelayContactSignal1   ;
		uint8_t RelayContactSignal2	  ;
		uint8_t RelayContactSignal3	  ;
		uint8_t TsalSignal			  ;
		uint8_t IMDStatusFrequency	  ;
		uint8_t Reserved			  ;


	}__attribute__((aligned(1),packed)) B;

}BatteryDiagnose_t;

typedef union{
	uint8_t TxData[8];
	uint8_t RxData[8];
	struct{
		unsigned int HighestTemp : 12;
		unsigned int LowestTemp : 12;
		unsigned int MeanTemp : 12;
		uint8_t prechargeStateSignal1 :1 ;
		uint8_t prechargeStateSignal2 :1 ;
		uint8_t RelayContactSignal1   :1 ;
		uint8_t RelayContactSignal2	  :1 ;
		uint8_t RelayContactSignal3	  :1 ;
		uint8_t TsalSignal			  :1 ;
		uint8_t IMDStatusFrequency	  :1 ;
		uint8_t Reserved			  :1 ;
		uint32_t Reserved2		  :20 ;
	}__attribute__((aligned(1),packed)) B;

}BatteryInfo_t;

extern BatteryTemp_t T_BatteryTemp;
extern BatteryDiagnose_t R_BatteryDiagnose;
extern BatteryInfo_t T_BatteryInfo;

extern void GAS_Can_init(void);

extern void GAS_Can_sendMessage(CAN_HandleTypeDef hcan,CAN_TxHeaderTypeDef canHeader, uint8_t Data[]);

extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
//extern void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);
#endif /* SRC_GETANDSEND_H_ */
