#ifndef _KEY_DRIVE_H_
#define _KEY_DRIVE_H_

#include "main.h"

//�ϼ�
#define KEY_UP_PIN  		GPIO_PIN_4
#define KEY_UP_PORT 		GPIOA

//�¼�
#define KEY_DOWN_PIN 		GPIO_PIN_5
#define KEY_DOWN_PORT  		GPIOA

//OK
#define KEY_OK_PIN 			GPIO_PIN_6
#define KEY_OK_PORT  		GPIOA



//��ʱ��10msɨ��һ��  ��λҪ*10
#define SINGLE_TIME  1000     //����30ms���ٰ��¼�Ϊһ�ε�����30ms�ھ�Ϊ��Ч
#define DOUBLE_TIME  3000    //300ms�ڽ��յ����ε�����Ϊ˫����˫����300ms�ٵ���Ϊ����
#define LONG_KEY_TIME  100000 //��������ʱ��2000ms



/* ����״̬�� */
#define KEY_NO_ACTION            0      //û�а�������
#define KEY_CLICK_ACTION         1      //����
#define KEY_DOUBLE_ACTION        2      //˫��
#define KEY_LONG_PRESS_ACTION    3      //����
#define KEY_LONG_RELEASE_ACTION  4      //�����ͷ�



#define key_state_0   0  //״̬0
#define key_state_1   1  //״̬1
#define key_state_2   2  //״̬2
#define key_state_3   3  //״̬3

struct KeyInfo
{
	uint16_t usr_timer_cnt;   //�������¼�����
	uint16_t key_timer_cnt;   //˫��������
	uint8_t  press_state;     //�����������ͷ�״̬
	uint16_t key_times;  	   //������סʱ�����ֵ
	uint8_t  key_flag;   	   //�������±�־λ
	uint8_t  repeat_state;    //�����ظ�״̬��˫���жϣ�
	uint8_t  click_flag; 	   //�����ʶ
};






uint8_t key_scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,struct KeyInfo *info);
void KeyTimeGroup(void); 
void KEY_Init(void);

#endif
