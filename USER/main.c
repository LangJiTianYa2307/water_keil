/*
 * @Descripttion: 
 * @version: 
 * @Author: PanHan
 * @Date: 2019-12-17 08:52:58
 * @LastEditors: PanHan
 * @LastEditTime: 2020-03-13 10:49:42
 * @FilePath: \water_gcc\Src\main.c
 */
#include "APP.h"
#include "delay.h"
#include "usart.h"
#include "mpu.h"
static void hardwareInit(void);

int main(void)
{
  hardwareInit(); //Ӳ����ʼ��
  xTaskCreate((TaskFunction_t )start_task,            
	
              (uint16_t       )START_STK_SIZE,        
              (void*          )NULL,                  
              (UBaseType_t    )START_TASK_PRIO,       
              (TaskHandle_t*  )&StartTask_Handler);          
  vTaskStartScheduler();      
}

static void hardwareInit(void)
{
    Write_Through();                //Cahceǿ��͸д
    MPU_Memory_Protection();        //������ش洢����  
    Cache_Enable();                 //��L1-Cache
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    HAL_Init();				              //��ʼ��HAL��
    delay_init(216);                //��ʱ��ʼ��
	  uart_init(115200);
    __HAL_RCC_CRC_CLK_ENABLE();		  //ʹ��CRCʱ��,EMWIN��Ҫ
}