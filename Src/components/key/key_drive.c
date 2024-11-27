#include "key_drive.h"
#include "delay.h"

/*---------------------------------------------------------------------
FuncName: KeyTimeCnt
Describe: ����������
Input		:	
output	:	
Other		: 
---------------------------------------------------------------------*/ 
static void KeyCnt(struct KeyInfo *info)
{
	info->usr_timer_cnt++; 
	info->key_timer_cnt++;
}
/*---------------------------------------------------------------------
FuncName: key_botton_scan
Describe: �ײ㰴��ɨ��
Input		:	
output	:	
Other		: 
---------------------------------------------------------------------*/ 
/* ֻ��԰�������Ϊ�͵�ƽ�����������Ϊ�ߵ�ƽ��Ҫ�޸� */
/* �����ײ�ɨ����� */
static uint8_t key_botton_scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,struct KeyInfo *info)
{
    uint8_t key_press=0;     																//����IO��ƽ
    uint8_t key_return = KEY_NO_ACTION;  										//�������ͷ���

    key_press = HAL_GPIO_ReadPin(GPIOx,GPIO_Pin);//GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);  //��ȡkey���ŵ�ƽ

    switch (info->press_state)
    {
		case key_state_0:
		if(!key_press) 																//key����
		{
			info->key_times =0;
			info->usr_timer_cnt=0; 											//��ʱ����ʼ��ʱ
			info->press_state =key_state_1;  			 			//���밴��ȷ��״̬    
		}
		break;
					
		case key_state_1:
		if(!key_press)  															//key����
		{
			info->key_times=info->usr_timer_cnt;  			//��ȡ��ʱ������ֵ
			if(info->key_times>=SINGLE_TIME) 						//��������30ms
			{
				info->key_flag=1;   										//����Ч��������
				info->press_state=key_state_2;  				//��Ч�����������жϵ���������
				info->key_times=0;
				info->usr_timer_cnt=0;
			}
		}
		else
		{
			info->press_state=key_state_0; 							//��Ч���������س�̬
		}
		break;
							
        case key_state_2:
		if((info->key_flag==1)&&(key_press)) 					//�Ѿ��а������º��ɿ�����
		{
			key_return=KEY_CLICK_ACTION;  						//���ص���
			info->press_state=key_state_0;        		//���س�̬
			info->key_flag=0;
		}
		else if((info->key_flag==1)&&(!key_press))  	//�а��������Ұ���
		{
			info->key_times=info->usr_timer_cnt;
			if(info->key_times>=LONG_KEY_TIME)  			//����2s,����
			{
				key_return=KEY_LONG_PRESS_ACTION;			//���س���
				info->press_state=key_state_3;
				info->key_times=0;
				info->usr_timer_cnt=0; 
			}
		}
		break;
					
        case key_state_3:
		if(key_press) 																//�����ɼ�
		{
			info->key_times=info->usr_timer_cnt;
			if(info->key_times>=SINGLE_TIME)  
			{ 
				if(key_press)  												//�������
				{
					key_return=KEY_LONG_RELEASE_ACTION;  //���ذ����ͷ�
					info->key_flag=0;
					info->press_state=key_state_0; 
				}
			}
		}
		break;
        default: 
					  
		break;
    }
    return key_return;  //�����޼��������������������ͷ�
}
/*---------------------------------------------------------------------
 FuncName: key_scan
 Describe: ����ɨ��
 Input		:	
 output	:	
 Other		: 
 ---------------------------------------------------------------------*/  
uint8_t key_scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,struct KeyInfo *info)
{
    uint8_t  RE_return = KEY_NO_ACTION;  //Ĭ�Ϸ����ް���
    uint8_t  key_type=0;     //����ײ�ɨ����򷵻ؼ�ֵ
    uint16_t  key_time1=0;

    key_type = key_botton_scan(GPIOx,GPIO_Pin,info);   //��ȡ�ײ�ɨ�躯��

    switch (info->repeat_state)
    {
		case key_state_0:
		if(key_type==KEY_CLICK_ACTION)  	//��ȡһ������
		{
			info->click_flag=1;  					//��¼һ�ε���
			info->repeat_state=key_state_1;  //����ȷ�ϵ�����˫��״̬
			key_type=0;             			//�����ֹ���¸�״̬����Ӱ��
			info->key_timer_cnt=0;        //����ֵ����
		}
		else
		{
			RE_return=key_type;    //�����޼��������������ͷ�
		}
		break;
					
        case key_state_1:
		if(info->click_flag==1)     //����һ�ε���
		{
			key_time1=info->key_timer_cnt;  
			if(key_time1<=DOUBLE_TIME)   //���ε������ʱ��200ms
			{
				if(key_type==KEY_CLICK_ACTION)  //����һ�ε���
				{ 
					info->repeat_state=key_state_0;
					RE_return=KEY_DOUBLE_ACTION;  //˫��
					info->click_flag=0;
				}
			}
			else  //û��200ms�ڻ�ȡ�ڶ��ε������ʷ��ص���
			{
				info->repeat_state=key_state_0;
				RE_return=KEY_CLICK_ACTION;
				info->click_flag=0;
			}
		}
		break;
    }
    return RE_return;
}

/*******************************************************************************************************/
struct KeyInfo KeyUpInfo = {0};
struct KeyInfo KeyDownInfo = {0};
struct KeyInfo KeyOkInfo = {0};
/*---------------------------------------------------------------------
FuncName: KEY_GPIO_Init
Describe: ������ʼ��
Input		:	
output	:	
Other		: 
---------------------------------------------------------------------*/ 
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT; 
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	
	/*�ϼ�*/
	GPIO_InitStruct.Pin = KEY_UP_PIN; 
	HAL_GPIO_Init(KEY_UP_PORT, &GPIO_InitStruct);

	/*�¼�*/
	GPIO_InitStruct.Pin = KEY_DOWN_PIN; 
	HAL_GPIO_Init(KEY_DOWN_PORT, &GPIO_InitStruct);
	
	/*OK*/
	GPIO_InitStruct.Pin = KEY_OK_PIN; 
	HAL_GPIO_Init(KEY_OK_PORT, &GPIO_InitStruct);
}
/*---------------------------------------------------------------------
FuncName: KeyTimeGroup
Describe: 
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
void KeyTimeGroup(void)
{
	KeyCnt(&KeyUpInfo);
	KeyCnt(&KeyDownInfo);
	KeyCnt(&KeyOkInfo);
}





