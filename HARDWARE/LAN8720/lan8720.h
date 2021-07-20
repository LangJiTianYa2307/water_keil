/*
 * @Descripttion: 
 * @version: 
 * @Author: PanHan
 * @Date: 2020-01-14 11:23:10
 * @LastEditors: PanHan
 * @LastEditTime: 2020-01-14 13:59:19
 * @FilePath: \water_keil\Src\HardwareProxy\lan8720.h
 */
#ifndef __LAN8720_H
#define __LAN8720_H
#include "sys.h"
#include "stm32f7xx_hal_conf.h"

#ifdef __cplusplus
       extern "C" {
#endif
         
extern ETH_HandleTypeDef ETH_Handler;               //?????
extern ETH_DMADescTypeDef *DMARxDscrTab;			//???DMA????????????
extern ETH_DMADescTypeDef *DMATxDscrTab;			//???DMA???????????? 
extern uint8_t *Rx_Buff; 							//?????????buffers?? 
extern uint8_t *Tx_Buff; 							//?????????buffers??
extern ETH_DMADescTypeDef  *DMATxDescToSet;			//DMA?????????
extern ETH_DMADescTypeDef  *DMARxDescToGet; 		//DMA????????? 
 

u8 LAN8720_Init(void);
u32 LAN8720_ReadPHY(u16 reg);
void LAN8720_WritePHY(u16 reg,u16 value);
u8 LAN8720_Get_Speed(void);
u8 ETH_MACDMA_Config(void);
u8 ETH_Mem_Malloc(void);
void ETH_Mem_Free(void);
u32  ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDesc);
         
                
#ifdef __cplusplus
        }
#endif
        
        
#endif
