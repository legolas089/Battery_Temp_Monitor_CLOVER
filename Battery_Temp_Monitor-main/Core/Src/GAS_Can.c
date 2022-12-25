/*
 * CAN.c
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#include <GAS_Can.h>
#include <stdio.h>

CAN_FilterTypeDef sFilterConfig;
CAN_FilterTypeDef sFilterConfig2;
CAN_RxHeaderTypeDef canRxHeader;
CAN_TxHeaderTypeDef canTxHeader;
CAN_TxHeaderTypeDef canTxHeader2;
uint8_t canRx0Data[8];
uint32_t TxMailBox;
uint32_t TxMailBox2;

BatteryTemp_t T_BatteryTemp;
BatteryInfo_t T_BatteryInfo;
BatteryDiagnose_t R_BatteryDiagnose;
uint32_t STM32_ID = 0x32F103A;
uint32_t STM32_ID2 = 0x32F103B;




uint32_t STM32_TempMonitorID = 0x405DC;
uint32_t BatteryInsideID = 0x405DB;
uint32_t R_BatteryFanCnt = 0x334C;

/*-------------------------Function Prototypes--------------------------------*/
void GAS_Can_txSetting(void);
void GAS_Can_rxSetting(void);
void GAS_Can_init(void);

void GAS_Can_sendMessage(CAN_HandleTypeDef hcan,CAN_TxHeaderTypeDef canHeader, uint8_t Data[]);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan);
//void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);

void GAS_Can_txSetting(void)
{

//	  canTxHeader.StdId = (0x283>>18)&0x7ff;
	  canTxHeader.ExtId = STM32_TempMonitorID;
	  canTxHeader.IDE	= CAN_ID_EXT;
	  canTxHeader.RTR	= CAN_RTR_DATA;
	  canTxHeader.DLC	=	8;

	  canTxHeader2.ExtId = BatteryInsideID;
	  canTxHeader2.IDE	= CAN_ID_EXT;
	  canTxHeader2.RTR	= CAN_RTR_DATA;
	  canTxHeader2.DLC	=	8;
}

void GAS_Can_rxSetting(void)
{
	/*
	 * CAN rx set function
	 * IdHigh, IdLow for each 16bit in 29bit ID
	 * Set different FIFO and FilterBank
	 *
	 */
	sFilterConfig.FilterIdHigh = (R_BatteryFanCnt<<3)>>16;				/*first 2byte in 29bit (shift need to IED,RTR,0)*/
	sFilterConfig.FilterIdLow = (0xffff & (R_BatteryFanCnt << 3)) | (1<<2);	/*second 2byte in 29bit + IDE (shift need to IED,RTR,0/)*/
	sFilterConfig.FilterMaskIdHigh = (0x0fffffff<<3)>>16;
	sFilterConfig.FilterMaskIdLow =(0xffff & (0x0FFFFFFF << 3)) | (1<<2);
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterBank = 1;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 1;

	 if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	  {
	    /* Filter configuration Error */
	    Error_Handler();
	  }

	 sFilterConfig2.FilterIdHigh =  (R_BatteryFanCnt<<3)>>16;
	 sFilterConfig2.FilterIdLow = (0xffff & (R_BatteryFanCnt << 3)) | (1<<2);
	 sFilterConfig2.FilterMaskIdHigh = (0x0fffffff<<3)>>16;
 	 sFilterConfig2.FilterMaskIdLow = (0xffff & (0x0FFFFFFF << 3)) | (1<<2);
	 sFilterConfig2.FilterFIFOAssignment = CAN_RX_FIFO1;
	 sFilterConfig2.FilterBank = 15;
	 sFilterConfig2.FilterMode = CAN_FILTERMODE_IDMASK;
	 sFilterConfig2.FilterScale = CAN_FILTERSCALE_32BIT;
	 sFilterConfig2.FilterActivation = ENABLE;
	 sFilterConfig2.SlaveStartFilterBank = 15;

	 if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig2) != HAL_OK)
	 {
	     /* Filter configuration Error */
	     Error_Handler();
	 }

}


void GAS_Can_init(void)
{

	GAS_Can_txSetting();
	GAS_Can_rxSetting();
//

	HAL_CAN_Start(&hcan1);
	HAL_CAN_Start(&hcan2);
//
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
	  Error_Handler();
	}
//
	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{
	  Error_Handler();
	}


}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	/*
	 * CAN recieve data interrupt function
	 * Check Instance then recieve data in stm32_2.RxData
	 */
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, R_BatteryDiagnose.RxData);

	}
}

//void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	/*
//	 * CAN recieve data interrupt function
//	 * Check Instance then recieve data in stm32_2.RxData
//	 */
//	if(hcan->Instance == CAN2)
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &canRxHeader, R_BatteryDiagnose.RxData);
//
//	}
//}

//void GAS_Can_sendMessage_1( uint8_t Data[])
//{
//
//	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
//	HAL_CAN_AddTxMessage(&hcan1, &canTxHeader, &Data[0], &TxMailBox);
//
//}

void GAS_Can_sendMessage_Temp()
{

	TxMailBox2 = HAL_CAN_GetTxMailboxesFreeLevel(&hcan2);
	HAL_CAN_AddTxMessage(&hcan2, &canTxHeader2, &T_BatteryTemp.TxData[0], &TxMailBox2);

}

//void GAS_BatteryInfoPass(){
//	T_BatteryInfo.B.HighestTemp =T_BatteryTemp.B.HighestTemp;
//	T_BatteryInfo.B.LowestTemp =T_BatteryTemp.B.LowestTemp;
//	T_BatteryInfo.B.MeanTemp =T_BatteryTemp.B.MeanTemp;
//	T_BatteryInfo.B. prechargeStateSignal1 = R_BatteryDiagnose.B.prechargeStateSignal1;
//	T_BatteryInfo.B. prechargeStateSignal2 = R_BatteryDiagnose.B.prechargeStateSignal2;
//	T_BatteryInfo.B. RelayContactSignal1 = R_BatteryDiagnose.B.RelayContactSignal1;
//	T_BatteryInfo.B. RelayContactSignal2 = R_BatteryDiagnose.B.RelayContactSignal2;
//	T_BatteryInfo.B. RelayContactSignal3 = R_BatteryDiagnose.B.RelayContactSignal3;
//	T_BatteryInfo.B. TsalSignal = R_BatteryDiagnose.B.TsalSignal;
//	T_BatteryInfo.B. IMDStatusFrequency = R_BatteryDiagnose.B.IMDStatusFrequency;
//
//	GAS_Can_sendMessage_2(T_BatteryInfo.TxData);
//}

