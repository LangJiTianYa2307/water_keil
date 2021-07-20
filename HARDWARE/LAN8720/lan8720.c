/*
 * @Descripttion: 
 * @version: 
 * @Author: PanHan
 * @Date: 2020-01-14 11:23:10
 * @LastEditors: PanHan
 * @LastEditTime: 2020-03-13 11:40:55
 * @FilePath: \water_gcc\Src\HardwareProxy\lan8720.c
 */
#include "lan8720.h"
#include "lwip_comm.h"
#include "delay.h"
#include "my_malloc.h"

ETH_HandleTypeDef ETH_Handler;      //?????

ETH_DMADescTypeDef *DMARxDscrTab;	//???DMA????????????
ETH_DMADescTypeDef *DMATxDscrTab;	//???DMA???????????? 
uint8_t *Rx_Buff; 					//?????????buffers?? 
uint8_t *Tx_Buff; 					//?????????buffers??
  
//LAN8720???
//???:0,??;
//    ??,??
u8 LAN8720_Init(void)
{      
  u8 macaddress[6];
  GPIO_InitTypeDef GPIO_Initure;
  __HAL_RCC_GPIOI_CLK_ENABLE();			//??GPIOB??

  GPIO_Initure.Pin = GPIO_PIN_8; //PB0,1
  GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //????
  GPIO_Initure.Pull = GPIO_PULLUP;          //??
  GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //??
  HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //???GPIOB.0?GPIOB.1

  
  INTX_DISABLE();                         //??????,?????????!
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET);	//PB1?1
  delay_xms(100);
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET);	//PB1?1
  delay_xms(100);
  INTX_ENABLE();                          //??????  

  macaddress[0]=lwipdev.mac[0]; 
	macaddress[1]=lwipdev.mac[1]; 
	macaddress[2]=lwipdev.mac[2];
	macaddress[3]=lwipdev.mac[3];   
	macaddress[4]=lwipdev.mac[4];
	macaddress[5]=lwipdev.mac[5];
        
	ETH_Handler.Instance = ETH;
  ETH_Handler.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;  //??????? 
  ETH_Handler.Init.Speed = ETH_SPEED_100M;                        //??100M,??????????,??????
  ETH_Handler.Init.DuplexMode = ETH_MODE_FULLDUPLEX;              //?????,??????????,??????
  ETH_Handler.Init.PhyAddress = LAN8720_PHY_ADDRESS;              //LAN8720??  
  ETH_Handler.Init.MACAddr = macaddress;                          //MAC??  
  ETH_Handler.Init.RxMode = ETH_RXINTERRUPT_MODE;                 //?????? 
  ETH_Handler.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;       //?????  
  ETH_Handler.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;     //RMII??  
  if(HAL_ETH_Init(&ETH_Handler)==HAL_OK)
  {
      return 0;   //??
  }
  else return 1;  //??
}

//ETH????,????,????
//?????HAL_ETH_Init()??
//heth:?????
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_ETH_CLK_ENABLE();             //??ETH??
    __HAL_RCC_GPIOA_CLK_ENABLE();			      //??GPIOA??
	__HAL_RCC_GPIOB_CLK_ENABLE();			        //??GPIOB??
    __HAL_RCC_GPIOC_CLK_ENABLE();			      //??GPIOC??
    __HAL_RCC_GPIOG_CLK_ENABLE();			      //??GPIOG??
    
    /*?????? RMII?? 
    ETH_MDIO -------------------------> PA2
    ETH_MDC --------------------------> PC1
    ETH_RMII_REF_CLK------------------> PA1
    ETH_RMII_CRS_DV ------------------> PA7
    ETH_RMII_RXD0 --------------------> PC4
    ETH_RMII_RXD1 --------------------> PC5
    ETH_RMII_TX_EN -------------------> PB11
    ETH_RMII_TXD0 --------------------> PG13
    ETH_RMII_TXD1 --------------------> PG14
    ETH_RESET-------------------------> PCF8574??IO*/
    
    //PA1,2,7
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //????
    GPIO_Initure.Pull=GPIO_NOPULL;              //?????
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //??
    GPIO_Initure.Alternate=GPIO_AF11_ETH;       //???ETH??
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //???
    
    //PB11
    GPIO_Initure.Pin=GPIO_PIN_11;               //PB11
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //??
    
    //PC1,4,5
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5; //PC1,4,5
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //???
	
    //PG13,14
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14;   //PG13,14
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);         //???
    
    HAL_NVIC_SetPriority(ETH_IRQn,5,0);         //????????????,???FREE??????????
    HAL_NVIC_EnableIRQ(ETH_IRQn);
}
//??PHY????
u32 LAN8720_ReadPHY(u16 reg)
{
    u32 regval;
    HAL_ETH_ReadPHYRegister(&ETH_Handler,reg,&regval);
    return regval;
}
//?LAN8720????????
//reg:???????
//value:?????
void LAN8720_WritePHY(u16 reg,u16 value)
{
    u32 temp=value;
    HAL_ETH_ReadPHYRegister(&ETH_Handler,reg,&temp);
}
//??8720?????
//???:
//001:10M???
//101:10M???
//010:100M???
//110:100M???
//??:??.
u8 LAN8720_Get_Speed(void)
{
	u8 speed;
	speed=((LAN8720_ReadPHY(31)&0x1C)>>2); //?LAN8720?31????????????????
	return speed;
}

extern void lwip_pkt_handle(void);		//?lwip_comm.c????

//??????
void ETH_IRQHandler(void)
{
    //OSIntEnter(); 
    while(ETH_GetRxPktSize(ETH_Handler.RxDesc))   
    {
        lwip_pkt_handle();//???????,???????LWIP
    }
    //???????
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_R); 
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_NIS); 
    //OSIntExit();  
}

//?????????
//DMARxDesc:??DMA???
//???:???????
u32  ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDesc)
{
    u32 frameLength = 0;
    if(((DMARxDesc->Status&ETH_DMARXDESC_OWN)==(uint32_t)RESET) &&
     ((DMARxDesc->Status&ETH_DMARXDESC_ES)==(uint32_t)RESET) &&
     ((DMARxDesc->Status&ETH_DMARXDESC_LS)!=(uint32_t)RESET)) 
    {
        frameLength=((DMARxDesc->Status&ETH_DMARXDESC_FL)>>ETH_DMARXDESC_FRAME_LENGTHSHIFT);
    }
    return frameLength;
}

//?ETH????????
//???:0,??
//    ??,??
u8 ETH_Mem_Malloc(void)
{ 
	DMARxDscrTab = myMalloc(MEM_SRAM, ETH_RXBUFNB * sizeof(ETH_DMADescTypeDef));//????
	DMATxDscrTab = myMalloc(MEM_SRAM, ETH_TXBUFNB * sizeof(ETH_DMADescTypeDef));//????  
	Rx_Buff = myMalloc(MEM_SRAM, ETH_RX_BUF_SIZE * ETH_RXBUFNB);	//????
	Tx_Buff = myMalloc(MEM_SRAM, ETH_TX_BUF_SIZE * ETH_TXBUFNB);	//????
	if(!(u32)&DMARxDscrTab||!(u32)&DMATxDscrTab||!(u32)&Rx_Buff||!(u32)&Tx_Buff)
	{
		ETH_Mem_Free();
		return 1;	//????
	}	
	return 0;		//????
}

//??ETH ?????????
void ETH_Mem_Free(void)
{ 
	myFree(MEM_SRAM,DMARxDscrTab);  //????
	myFree(MEM_SRAM,DMATxDscrTab);  //????
	myFree(MEM_SRAM,Rx_Buff);		    //????
	myFree(MEM_SRAM,Tx_Buff);		    //????  
}



