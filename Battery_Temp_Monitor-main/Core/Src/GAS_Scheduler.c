/*
 * GAS_Scheduler.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Suprhimp
 *      		InitusNovus
 */


#include "GAS_Scheduler.h"
#include "stdio.h"
#include "tim.h"
//#include "GAS_Can.h"


extern uint8_t canRx0Data[8];
uint8_t pin_state;

#define False 0
#define True 1

volatile int gTimerCnt;

Scheduler_task gTask;

void GAS_Scheduler_init(void);
static void GAS_Scheduler_taskCounter_1ms(void);
static void GAS_Scheduler_taskCounter_10ms(void);
static void GAS_Scheduler_taskCounter_100ms(void);
static void GAS_Scheduler_taskCounter_1000ms(void);
static void GAS_Scheduler_taskBackground(void);
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_SYSTICK_Callback(void);
void GAS_Schduler(void);

void GAS_Scheduler_init(void)
{
//	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
//	  {
//		  Error_Handler();
//	  }
	SysTick_Config(SystemCoreClock/1000);

//********ADC initialization********=

//********CAN initialization********
	GAS_Can_init();

//********PWM initialization********



}



static inline __attribute__((always_inline)) void GAS_Scheduler_taskCounter_1ms(void)
{



//	GAS_Can_sendMessage();


}

static inline __attribute__((always_inline)) void GAS_Scheduler_taskCounter_10ms(void)
{
//	GAS_Can_sendMessage();
	GAS_Can_sendMessage_Temp();
}

uint8_t Count300ms = 0;
uint8_t CScount = 0;
static inline __attribute__((always_inline)) void GAS_Scheduler_taskCounter_100ms(void)
{


//	if (Count300ms%2){
//		CScount+=1;
//		GAS_BulkADC_select(CScount);
//	}else




}

static inline __attribute__((always_inline)) void GAS_Scheduler_taskCounter_300ms(void)
{
	Count300ms += 1;
	if (Count300ms%2){
		CScount+=1;
		GAS_BulkADC_select(CScount);
	}else{
		GAS_BulkADC_run_100ms(CScount);
		Count300ms = 0;
	}

	if(CScount>9)CScount=0;


}
static inline __attribute__((always_inline)) void GAS_Scheduler_taskCounter_1000ms(void)
{

	HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
	HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);


#ifdef INVCON_TEST
	InverterControl_test_1000ms();
#endif

}
static inline __attribute__((always_inline)) void GAS_Scheduler_taskBackground(void)
{

}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
void HAL_SYSTICK_Callback(void)
{
	gTimerCnt++;

	gTask.flag_1ms=True;

	if(gTimerCnt % 10 == 0)
	{
		gTask.flag_10ms=True;
	}

	if(gTimerCnt % 100 == 0)
	{
		gTask.flag_100ms=True;
	}

	if(gTimerCnt % 300 == 0)
	{
		gTask.flag_300ms=True;
	}

	if(gTimerCnt == 1000)
	{
		gTask.flag_1000ms=True;
		gTimerCnt = 0;
	}
}

void GAS_Scheduler(void)
{
	if(gTask.flag_1ms == True)
	{
		gTask.flag_1ms = False;
		GAS_Scheduler_taskCounter_1ms();
	}

	if(gTask.flag_10ms == True)
	{
		gTask.flag_10ms = False;
		GAS_Scheduler_taskCounter_10ms();
	}

	if(gTask.flag_100ms == True)
	{
		gTask.flag_100ms = False;
		GAS_Scheduler_taskCounter_100ms();
	}
	if(gTask.flag_300ms == True)
	{
		gTask.flag_300ms = False;
		GAS_Scheduler_taskCounter_300ms();
	}
	if(gTask.flag_1000ms == True)
	{
		gTask.flag_1000ms = False;
		GAS_Scheduler_taskCounter_1000ms();
	}

	GAS_Scheduler_taskBackground();

}
