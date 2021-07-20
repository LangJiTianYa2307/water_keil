/*
 * @Descripttion: 
 * @version: 
 * @Author: PanHan
 * @Date: 2020-01-14 11:23:10
 * @LastEditors: PanHan
 * @LastEditTime: 2020-04-06 15:52:52
 * @FilePath: \water_gcc\Src\HardwareProxy\key.c
 */
#include "key.h"
#include "delay.h"

/*Һλ�����ź�PI11
	��մ�����  PH8
	����ѹ��������PC1
	�������ź�  PB12 //2��
	�������ź�  PH10 //2��
	��դ�ź�    PC13
*/
#define LIQ_SWITCH_PIN 			    GPIO_PIN_11
#define LIQ_SWITCH_PORT 		    GPIOI
#define VACUUM_SENS_PIN 		    GPIO_PIN_2
#define VACUUM_SENS_PORT 				GPIOH
#define PRES_SENS_PIN 					GPIO_PIN_8
#define PRES_SENS_PORT 		  		GPIOH
#define CYL_HIGH_POS_PIN        GPIO_PIN_0
#define CYL_HIGH_POS_PORT       GPIOB
#define CYL_LOW_POS_PIN         GPIO_PIN_3
#define CYL_LOW_POS_PORT        GPIOH
#define GRAT_PIN 					      GPIO_PIN_13
#define GRAT_PORT 		  		    GPIOC

typedef struct
{
  GPIO_TypeDef * m_port;  //gpio port
  u16 m_pin;              //gpio pin

  VolTrigType m_trigType; //trig type high or low
  ExitSing_t m_sigType;   //�ź�����
  u32 m_trigCountLimit[2];  //�������߽�����������ж���

  u32 m_trigCount[2];     //�����ͽ����������
  SensorState m_state;    //������״̬
}KeyData;

static void init(SwitchSensor * me);
static u8 singDetect(SwitchSensor * me, SensorState * state); //�źż��
static void setTrigCountLimit(SwitchSensor * me, u32 trig_on, u32 trig_off);//���ô�������
static void setTrigType(SwitchSensor * me, VolTrigType type);//���ô�����ƽ


static GPIO_TypeDef * c_portArr[SING_MAX] = {
  LIQ_SWITCH_PORT, VACUUM_SENS_PORT, PRES_SENS_PORT, CYL_HIGH_POS_PORT, CYL_LOW_POS_PORT, GRAT_PORT
};

static const u16 c_pinArr[SING_MAX] = {
  LIQ_SWITCH_PIN, VACUUM_SENS_PIN, PRES_SENS_PIN, CYL_HIGH_POS_PIN, CYL_LOW_POS_PIN, GRAT_PIN
};

static SwitchSensor switchSensor[SING_MAX];
static KeyData s_privateData[SING_MAX];

/**
 * @brief	�������ش���������
 * @param	type:����������
 * @param port:������ʹ�õ�port
 * @param pin:������ʹ�õ�����
 * @retval	ָ�򴫸��������ָ��
 */
SwitchSensor * SwitchSensor_create(ExitSing_t type)
{
  SwitchSensor * me = & switchSensor[type];
  KeyData * pKeyData = 0;
  if (0 == me->init){
    me->init = init;
    me->singDetect = singDetect;
    me->setTrigCountLimit = setTrigCountLimit;
    me->setTrigType = setTrigType;

    me->private_data = & s_privateData[type];
    pKeyData = (KeyData *)me->private_data;
    pKeyData->m_sigType = type;
    pKeyData->m_port = c_portArr[type];
    pKeyData->m_pin = c_pinArr[type];

    me->init(me);
  }
  return me;
}

/**
 * @brief	��ʼ����������Ҫ�ǳ�ʼ����Ӧ��GPIO
 * @param	
 * @retval	
 */
static void init(SwitchSensor * me)
{
  KeyData * private_data = (KeyData *)me->private_data;
  GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��
	__HAL_RCC_GPIOH_CLK_ENABLE();           //����GPIOHʱ��
	__HAL_RCC_GPIOG_CLK_ENABLE();           //����GPIOGʱ��
	__HAL_RCC_GPIOI_CLK_ENABLE();           //����GPIOHʱ��
	__HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
	__HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOAʱ��
		
	GPIO_Initure.Mode = GPIO_MODE_INPUT;      //����
	GPIO_Initure.Pull = GPIO_PULLDOWN;        //����
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //����

  GPIO_Initure.Pin = private_data->m_pin;            
	HAL_GPIO_Init(private_data->m_port, &GPIO_Initure);

  private_data->m_trigCount[TRIG_ON] = 0;
  private_data->m_trigCount[TRIG_OFF] = 0;
  private_data->m_state = TRIG_OFF;
}

/**
 * @brief	�źż��
 * @param	state:�����źż����
 * @retval	0�ź��ޱ仯��1�ź��б仯
 */
static u8 singDetect(SwitchSensor * me, SensorState * state)
{
  KeyData * private_data = (KeyData *)me->private_data;
  u8 changeFlag = 0;  //״̬�ı��־λ
	if (private_data->m_trigType == HAL_GPIO_ReadPin(private_data->m_port, private_data->m_pin)){
    /*����*/
		private_data->m_trigCount[TRIG_ON] ++;
    private_data->m_trigCount[TRIG_OFF] = 0;
		if ((private_data->m_state == TRIG_OFF) && \
        (private_data->m_trigCount[TRIG_ON] >= private_data->m_trigCountLimit[TRIG_ON])){
			private_data->m_state = TRIG_ON;
			changeFlag = 1;
		}
	}else{
		private_data->m_trigCount[TRIG_OFF] ++;
    private_data->m_trigCount[TRIG_ON] = 0;
		if ((private_data->m_state == TRIG_ON) && \
        (private_data->m_trigCount[TRIG_OFF] >= private_data->m_trigCountLimit[TRIG_OFF])){
			private_data->m_state = TRIG_OFF;
			changeFlag = 1;
		} 
	}

  * state = private_data->m_state;
  return changeFlag;
}

/**
 * @brief	�����źż��������ƣ������˲�
 * @param	trig_on:�����źŵ��ж�
 * @param trig_off:����������ж�
 * @retval	
 */
static void setTrigCountLimit(SwitchSensor * me, u32 trig_on, u32 trig_off)
{
  KeyData * private_data = (KeyData *)me->private_data;
  private_data->m_trigCountLimit[TRIG_ON] = trig_on;
  private_data->m_trigCountLimit[TRIG_OFF] = trig_off;
}

/**
 * @brief	���ô������ͣ��ߵ�ƽ�������ߵ͵�ƽ������
 * @param	
 * @retval	
 */
static void setTrigType(SwitchSensor * me, VolTrigType type)
{
  KeyData * private_data = (KeyData *)me->private_data;
  private_data->m_trigType = type;
}

/*
*�������ܣ��ⲿ�ź�������ص�GPIO��ʼ��
*�����������
*�� �� ֵ����
*/
void extIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��
	__HAL_RCC_GPIOH_CLK_ENABLE();           //����GPIOHʱ��
	__HAL_RCC_GPIOG_CLK_ENABLE();           //����GPIOGʱ��
	__HAL_RCC_GPIOI_CLK_ENABLE();           //����GPIOHʱ��
	__HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
	__HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOAʱ��
		
	GPIO_Initure.Mode = GPIO_MODE_INPUT;      //����
	GPIO_Initure.Pull = GPIO_PULLDOWN;        //����
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //����

	GPIO_Initure.Pin = LIQ_SWITCH_PIN;            
	HAL_GPIO_Init(LIQ_SWITCH_PORT, &GPIO_Initure);

	GPIO_Initure.Pin = VACUUM_SENS_PIN;           
	HAL_GPIO_Init(VACUUM_SENS_PORT, &GPIO_Initure);

	GPIO_Initure.Pin = PRES_SENS_PIN;            
	HAL_GPIO_Init(PRES_SENS_PORT, &GPIO_Initure);
	
	GPIO_Initure.Pin = CYL_HIGH_POS_PIN;            
	HAL_GPIO_Init(CYL_HIGH_POS_PORT, &GPIO_Initure);

	GPIO_Initure.Pin = CYL_LOW_POS_PIN;           
	HAL_GPIO_Init(CYL_LOW_POS_PORT, &GPIO_Initure);

	GPIO_Initure.Pin = GRAT_PIN;            
	HAL_GPIO_Init(GRAT_PORT, &GPIO_Initure);
}
