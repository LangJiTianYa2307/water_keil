/*
 * @Descripttion: 
 * @version: 
 * @Author: panhan
 * @Date: 2019-12-24 13:35:27
 * @LastEditors: PanHan
 * @LastEditTime: 2020-05-04 16:46:23
 * @FilePath: \water_gcc\Src\FunctionMoudleLayer\lwip_comm.c
 */
#include "lwip_comm.h" 
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "ethernetif.h" 
#include "lwip/timers.h"
#include "lwip/tcp_impl.h"
#include "lwip/ip_frag.h"
#include "lwip/tcpip.h" 
#include "lwip/timers.h"
#include "my_malloc.h"
#include "delay.h"
#include "usart.h" 
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
	   
   
__lwip_dev lwipdev;						
struct netif lwip_netif;			

extern u32 memp_get_memorysize(void);	
extern u8_t *memp_memory;				
extern u8_t *ram_heap;					



#define LWIP_DHCP_TASK_PRIO       		7
#define LWIP_DHCP_STK_SIZE  		    256
TaskHandle_t LWIP_DHCP_TaskHandler;	


void lwip_dhcp_task(void *pdata); 


void lwip_pkt_handle(void)
{
	ethernetif_input(&lwip_netif);
}



u8 lwip_comm_mem_malloc(void)
{
	u32 mempsize;
	u32 ramheapsize; 
	mempsize = memp_get_memorysize();			    
  printf("mes size is   %d\r\n", mempsize);
	memp_memory = mymalloc(SRAMIN, mempsize);	
	ramheapsize = LWIP_MEM_ALIGN_SIZE(MEM_SIZE) + 2*LWIP_MEM_ALIGN_SIZE(4*3) + MEM_ALIGNMENT;
  printf("ram_heap size is   %d\r\n", ramheapsize);
	ram_heap = mymalloc(SRAMIN, ramheapsize);	
	if(!memp_memory||!ram_heap)             
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}

/**
  * @brief	free lwip memory
  * @param	
  * @retval	
  */
void lwip_comm_mem_free(void)
{ 	
	myfree(SRAMIN,memp_memory);
	myfree(SRAMIN,ram_heap);
}

/**
  * @brief	set default ip
  * @param	lwipx:
  * @retval	
  */
void lwip_comm_default_ip_set(__lwip_dev *lwipx)
{
	u32 sn0;
	sn0 = *(vu32*)(0x1FF0F420);
	//default remote ip:192.168.1.100
	lwipx->remoteip[0] = 192;	
	lwipx->remoteip[1] = 168;
	lwipx->remoteip[2] = 1;
	lwipx->remoteip[3] = 100;
	//MAC
	lwipx->mac[0] = 2;
	lwipx->mac[1] = 0;
	lwipx->mac[2] = 0;
	lwipx->mac[3] = (sn0>>16)&0XFF;
	lwipx->mac[4] = (sn0>>8)&0XFFF;
	lwipx->mac[5] = sn0&0XFF; 
	//default ip:192.168.1.50
	lwipx->ip[0] = 192;	
	lwipx->ip[1] = 168;
	lwipx->ip[2] = 1;
	lwipx->ip[3] = 35;
	//default netmask:255.255.255.0
	lwipx->netmask[0] = 255;	
	lwipx->netmask[1] = 255;
	lwipx->netmask[2] = 255;
	lwipx->netmask[3] = 0;
	//default gateway:192.168.1.1
	lwipx->gateway[0] = 192;	
	lwipx->gateway[1] = 168;
	lwipx->gateway[2] = 1;
	lwipx->gateway[3] = 1;	
	lwipx->dhcpstatus = 0;// dhcp status
} 

/**
  * @brief	lwip initialization
  * @param	
  * @retval	0,OK;other,NG
  */
u8 lwip_comm_init(void)
{
  u8 retry = 0;
	struct netif *Netif_Init_Flag;		
	struct ip_addr ipaddr;  			    
	struct ip_addr netmask; 			    
	struct ip_addr gw;      			    

	if(ETH_Mem_Malloc())  return 1;		    
	if(lwip_comm_mem_malloc())return 2;	  
	lwip_comm_default_ip_set(&lwipdev);	 
  //LAN8720 initialization
	while(LAN8720_Init()){
		retry ++;
		if(retry > 5){			
			retry = 0; //
			printf("Network initialization failed\n");
			return 3;
		}
	}
	tcpip_init(NULL,NULL);				

	IP4_ADDR(&ipaddr,lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);
	IP4_ADDR(&netmask,lwipdev.netmask[0],lwipdev.netmask[1] ,lwipdev.netmask[2],lwipdev.netmask[3]);
	IP4_ADDR(&gw,lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);
	printf("MAC address is:.......%d.%d.%d.%d.%d.%d\r\n",lwipdev.mac[0],lwipdev.mac[1],lwipdev.mac[2],lwipdev.mac[3],lwipdev.mac[4],lwipdev.mac[5]);
	printf("static IP address.....%d.%d.%d.%d\r\n",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);
	printf("subnet mask...........%d.%d.%d.%d\r\n",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);
	printf("default gateway.......%d.%d.%d.%d\r\n",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);

  
	Netif_Init_Flag = netif_add(&lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&tcpip_input);
	if(Netif_Init_Flag==NULL) return 4;
	else{
		netif_set_default(&lwip_netif); 
		netif_set_up(&lwip_netif);		
	}
	return 0;//OK.
}   
