#include "key_drive.h"
#include "delay.h"

/*---------------------------------------------------------------------
FuncName: KeyTimeCnt
Describe: 按键计数器
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
Describe: 底层按键扫描
Input		:	
output	:	
Other		: 
---------------------------------------------------------------------*/ 
/* 只针对按键按下为低电平的情况，按下为高点平需要修改 */
/* 按键底层扫描程序 */
static uint8_t key_botton_scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,struct KeyInfo *info)
{
    uint8_t key_press=0;     																//按键IO电平
    uint8_t key_return = KEY_NO_ACTION;  										//按键类型返回

    key_press = HAL_GPIO_ReadPin(GPIOx,GPIO_Pin);//GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);  //读取key引脚电平

    switch (info->press_state)
    {
		case key_state_0:
		if(!key_press) 																//key按下
		{
			info->key_times =0;
			info->usr_timer_cnt=0; 											//定时器开始计时
			info->press_state =key_state_1;  			 			//进入按键确认状态    
		}
		break;
					
		case key_state_1:
		if(!key_press)  															//key按下
		{
			info->key_times=info->usr_timer_cnt;  			//读取定时器计数值
			if(info->key_times>=SINGLE_TIME) 						//按键消抖30ms
			{
				info->key_flag=1;   										//有有效按键按下
				info->press_state=key_state_2;  				//有效按键，进入判断单击、长按
				info->key_times=0;
				info->usr_timer_cnt=0;
			}
		}
		else
		{
			info->press_state=key_state_0; 							//无效按键，返回初态
		}
		break;
							
        case key_state_2:
		if((info->key_flag==1)&&(key_press)) 					//已经有按键按下后松开按键
		{
			key_return=KEY_CLICK_ACTION;  						//返回单击
			info->press_state=key_state_0;        		//返回初态
			info->key_flag=0;
		}
		else if((info->key_flag==1)&&(!key_press))  	//有按键按下且按着
		{
			info->key_times=info->usr_timer_cnt;
			if(info->key_times>=LONG_KEY_TIME)  			//大于2s,长按
			{
				key_return=KEY_LONG_PRESS_ACTION;			//返回长按
				info->press_state=key_state_3;
				info->key_times=0;
				info->usr_timer_cnt=0; 
			}
		}
		break;
					
        case key_state_3:
		if(key_press) 																//长按松键
		{
			info->key_times=info->usr_timer_cnt;
			if(info->key_times>=SINGLE_TIME)  
			{ 
				if(key_press)  												//消抖完成
				{
					key_return=KEY_LONG_RELEASE_ACTION;  //返回按键释放
					info->key_flag=0;
					info->press_state=key_state_0; 
				}
			}
		}
		break;
        default: 
					  
		break;
    }
    return key_return;  //返回无键、单击、长按、长按释放
}
/*---------------------------------------------------------------------
 FuncName: key_scan
 Describe: 按键扫描
 Input		:	
 output	:	
 Other		: 
 ---------------------------------------------------------------------*/  
uint8_t key_scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,struct KeyInfo *info)
{
    uint8_t  RE_return = KEY_NO_ACTION;  //默认返回无按键
    uint8_t  key_type=0;     //保存底层扫描程序返回键值
    uint16_t  key_time1=0;

    key_type = key_botton_scan(GPIOx,GPIO_Pin,info);   //获取底层扫描函数

    switch (info->repeat_state)
    {
		case key_state_0:
		if(key_type==KEY_CLICK_ACTION)  	//获取一个单击
		{
			info->click_flag=1;  					//记录一次单击
			info->repeat_state=key_state_1;  //进入确认单击、双击状态
			key_type=0;             			//清零防止对下个状态产生影响
			info->key_timer_cnt=0;        //计数值清零
		}
		else
		{
			RE_return=key_type;    //返回无键、长按、长按释放
		}
		break;
					
        case key_state_1:
		if(info->click_flag==1)     //已有一次单击
		{
			key_time1=info->key_timer_cnt;  
			if(key_time1<=DOUBLE_TIME)   //两次单击间隔时间200ms
			{
				if(key_type==KEY_CLICK_ACTION)  //再有一次单击
				{ 
					info->repeat_state=key_state_0;
					RE_return=KEY_DOUBLE_ACTION;  //双击
					info->click_flag=0;
				}
			}
			else  //没在200ms内获取第二次单击，故返回单击
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
Describe: 按键初始化
Input		:	
output	:	
Other		: 
---------------------------------------------------------------------*/ 
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT; 
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	
	/*上键*/
	GPIO_InitStruct.Pin = KEY_UP_PIN; 
	HAL_GPIO_Init(KEY_UP_PORT, &GPIO_InitStruct);

	/*下键*/
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





