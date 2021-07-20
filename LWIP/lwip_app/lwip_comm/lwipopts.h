#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

//?????
#ifndef TCPIP_THREAD_PRIO
#define TCPIP_THREAD_PRIO		( configMAX_PRIORITIES - 1 )	//?????????????
#endif
#undef  DEFAULT_THREAD_PRIO
#define DEFAULT_THREAD_PRIO		8


#define SYS_LIGHTWEIGHT_PROT    1		//?1???????????????,????????????
#define NO_SYS                  0  		//??????
#define MEM_ALIGNMENT           4  		//??4??????
#define MEM_SIZE                16000 	//???heap??
#define MEMP_NUM_PBUF           20 		//MEMP_NUM_PBUF:memp???pbuf??,?????ROM??????????????,??????????
#define MEMP_NUM_UDP_PCB        6		//MEMP_NUM_UDP_PCB:UDP?????(PCB)??.?????UDP"??"????PCB.
#define MEMP_NUM_TCP_PCB        1		//MEMP_NUM_TCP_PCB:???????TCP??
#define MEMP_NUM_TCP_PCB_LISTEN 1		//MEMP_NUM_TCP_PCB_LISTEN:?????TCP????
#define MEMP_NUM_TCP_SEG        15		//MEMP_NUM_TCP_SEG:?????????TCP???
#define MEMP_NUM_SYS_TIMEOUT    8		//MEMP_NUM_SYS_TIMEOUT:???????timeout??

//pbuf??
#define PBUF_POOL_SIZE          20		//PBUF_POOL_SIZE:pbuf?????
#define PBUF_POOL_BUFSIZE       512		//PBUF_POOL_BUFSIZE:??pbuf?????

#define LWIP_TCP                1  		//??TCP
#define TCP_TTL                 255		//????

#undef TCP_QUEUE_OOSEQ
#define TCP_QUEUE_OOSEQ         0 		//?TCP?????????????,???????????????0

#undef TCPIP_MBOX_SIZE
#define TCPIP_MBOX_SIZE         MAX_QUEUE_ENTRIES   //tcpip?????????????

#undef DEFAULT_TCP_RECVMBOX_SIZE
#define DEFAULT_TCP_RECVMBOX_SIZE       MAX_QUEUE_ENTRIES  

#undef DEFAULT_ACCEPTMBOX_SIZE
#define DEFAULT_ACCEPTMBOX_SIZE         MAX_QUEUE_ENTRIES  


#define TCP_MSS                 (1500 - 40)	  	//??TCP??,TCP_MSS = (MTU - IP???? - TCP????
#define TCP_SND_BUF             (4*TCP_MSS)		//TCP???????(bytes).
#define TCP_SND_QUEUELEN        (2* TCP_SND_BUF/TCP_MSS)	//TCP_SND_QUEUELEN: TCP???????(pbuf).??????(2 * TCP_SND_BUF/TCP_MSS)
#define TCP_WND                 (2*TCP_MSS)		//TCP????
#define LWIP_ICMP               1 	//??ICMP??
#define LWIP_DHCP               0	  //???DHCP
#define LWIP_UDP                1 	//??UDP??
#define UDP_TTL                 255 //UDP???????
#define LWIP_STATS 0
#define LWIP_PROVIDE_ERRNO 1


//??????,STM32F4x7???????????IP,UDP?ICMP?????
#define CHECKSUM_BY_HARDWARE //??CHECKSUM_BY_HARDWARE,???????
#ifdef CHECKSUM_BY_HARDWARE
  //CHECKSUM_GEN_IP==0: ????IP????????
  #define CHECKSUM_GEN_IP                 0
  //CHECKSUM_GEN_UDP==0: ????UDP????????
  #define CHECKSUM_GEN_UDP                0
  //CHECKSUM_GEN_TCP==0: ????TCP????????
  #define CHECKSUM_GEN_TCP                0 
  //CHECKSUM_CHECK_IP==0: ???????IP???????
  #define CHECKSUM_CHECK_IP               0
  //CHECKSUM_CHECK_UDP==0: ???????UDP???????
  #define CHECKSUM_CHECK_UDP              0
  //CHECKSUM_CHECK_TCP==0: ???????TCP???????
  #define CHECKSUM_CHECK_TCP              0
  //CHECKSUM_CHECK_ICMP==1:???????ICMP???????
  #define CHECKSUM_GEN_ICMP               0
#else
  //CHECKSUM_GEN_IP==1: ????IP???????
  #define CHECKSUM_GEN_IP                 1
  // CHECKSUM_GEN_UDP==1: ????UDOP???????
  #define CHECKSUM_GEN_UDP                1
  //CHECKSUM_GEN_TCP==1: ????TCP???????
  #define CHECKSUM_GEN_TCP                1
  // CHECKSUM_CHECK_IP==1: ???????IP???????
  #define CHECKSUM_CHECK_IP               1
  // CHECKSUM_CHECK_UDP==1: ???????UDP???????
  #define CHECKSUM_CHECK_UDP              1
  //CHECKSUM_CHECK_TCP==1: ???????TCP???????
  #define CHECKSUM_CHECK_TCP              1
  //CHECKSUM_CHECK_ICMP==1:???????ICMP???????
  #define CHECKSUM_GEN_ICMP               1
#endif



#define LWIP_NETCONN                    1 	//LWIP_NETCONN==1:??NETCON??(????api_lib.c)
#define LWIP_SOCKET                     1	//LWIP_SOCKET==1:??Sicket API(????sockets.c)
#define LWIP_COMPAT_MUTEX               1
#define LWIP_SO_RCVTIMEO                1 	//????LWIP_SO_RCVTIMEO??netconn????recv_timeout,??recv_timeout????????

//???????
#define TCPIP_THREAD_STACKSIZE          1000	//????????
#define DEFAULT_UDP_RECVMBOX_SIZE       2000
#define DEFAULT_THREAD_STACKSIZE        512

//LWIP????
#define LWIP_DEBUG                    	 0	 //??DEBUG??
#define ICMP_DEBUG                      LWIP_DBG_OFF //??/??ICMPdebug

#endif /* __LWIPOPTS_H__ */

