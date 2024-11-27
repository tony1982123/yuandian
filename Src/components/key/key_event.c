#include "key_drive.h"
#include "key_event.h"
#include "mixt.h"
#include "func.h"

extern struct KeyInfo KeyUpInfo ;
extern struct KeyInfo KeyDownInfo ;
extern struct KeyInfo KeyOkInfo ;

KeyStateEnum KeyState = KeyDefault;

extern SysStateEnum SysState;

extern uint32_t SysStateChangeCount;

/*---------------------------------------------------------------------
FuncName: KeyHandleEvent
Describe: �ֱ��¼�
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
static void KeyUpEvent(void)
{
	uint8_t key_value=0;
	key_value = key_scan(KEY_UP_PORT,KEY_UP_PIN,&KeyUpInfo);
	switch(key_value)
	{
		case KEY_CLICK_ACTION:
			KeyState = KeyUp;
			
		break;
	}
}
/*---------------------------------------------------------------------
FuncName: KeyFootEvent
Describe: ��̤�¼�
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
static void KeyDownEvent(void)
{
	uint8_t key_value=0;
	key_value = key_scan(KEY_DOWN_PORT,KEY_DOWN_PIN,&KeyDownInfo);
	switch(key_value)
	{
		case KEY_CLICK_ACTION:
			KeyState = KeyDown;
		break;
	}
}
/*---------------------------------------------------------------------
FuncName: Key_Power_Event
Describe: ��Դ�¼�
Input	:	
output	:	
Other	: 
---------------------------------------------------------------------*/ 
static void KeyOkEvent(void)
{
	uint8_t key_value=0;
	key_value = key_scan(KEY_OK_PORT,KEY_OK_PIN,&KeyOkInfo);
	switch(key_value)
	{
		case KEY_CLICK_ACTION:
			KeyState = KeyOk;
		break;
	}
}
/*---------------------------------------------------------------------
FuncName: KeyEventHand
Describe: �¼�����
Input		:	
output	:	
Other		: 
---------------------------------------------------------------------*/ 
void KeyDecetHand(void)
{
	KeyUpEvent();
	KeyDownEvent();
	KeyOkEvent();
}

