/*
 * @Descripttion: 
 * @version: 
 * @Author: PanHan
 * @Date: 2020-01-14 11:23:10
 * @LastEditors: PanHan
 * @LastEditTime: 2020-04-14 09:14:07
 * @FilePath: \water_gcc\Src\HardwareProxy\key.h
 */
#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

#ifdef __cplusplus
       extern "C" {
#endif


/*�ⲿ�������ź�������*/
typedef enum
{
	WATER_DECT_SING = 0,  //Һλ����
	VACUUM_SING,          //��մ�����
	AIR_PRES_SING,         //ѹ��������
	H_CLY_POS_SING,           //����λ��
	L_CLY_POS_SING,           //����λ��
	GRAT_SING,             //��դ�ź�
  SING_MAX              //�������ź�ͳ��
}ExitSing_t;

//������������ʽ
typedef enum
{
  LOW_LEVEL_TRIG,    //�͵�ƽ����
  HIGH_LEVEL_TRIG  //�ߵ�ƽ����
}VolTrigType;

//������״̬
typedef enum
{
  TRIG_OFF,   //δ����
  TRIG_ON,    //����
}SensorState;

//���ش�����
typedef struct _SwitchSensor SwitchSensor;
struct _SwitchSensor
{
  void (* init)(SwitchSensor * me);
  u8 (* singDetect)(SwitchSensor * me, SensorState * state); //�źż��
  void (* setTrigCountLimit)(SwitchSensor * me, u32 trig_on, u32 trig_off);//���ô�������
  void (* setTrigType)(SwitchSensor * me, VolTrigType type);//���ô�����ƽ
  
  void * private_data;  //˽������
};
SwitchSensor * SwitchSensor_create(ExitSing_t type);

void extIO_Init(void);         
#ifdef __cplusplus
        }
#endif
        
#endif
