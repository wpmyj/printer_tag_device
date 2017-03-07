#include"HeadType.h"	


#define	START_KEY_IO					GPIO_Pin_6
#define	START_KEY_PORT				GPIOA
#define	START_KEY_RCC				  RCC_APB2Periph_GPIOA
#define	STOP_KEY_IO					  GPIO_Pin_5
#define	STOP_KEY_PORT				  GPIOA
#define	STOP_KEY_RCC				  RCC_APB2Periph_GPIOA
 
#define READ_START_KEY    		GPIO_ReadInputDataBit(START_KEY_PORT,START_KEY_IO) //���ص���һ���ֽڣ�������һ��λ
#define READ_STOP_KEY    		  GPIO_ReadInputDataBit(STOP_KEY_PORT,STOP_KEY_IO) //���ص���һ���ֽڣ�������һ��λ

#define	START_LIGHT_IO				GPIO_Pin_4
#define	START_LIGHT_PORT			GPIOA
#define	START_LIGHT_RCC				RCC_APB2Periph_GPIOA
#define	STOP_LIGHT_IO					GPIO_Pin_3
#define	STOP_LIGHT_PORT				GPIOA
#define	STOP_LIGHT_RCC				RCC_APB2Periph_GPIOA
 
#define START_LIGHT_ON  		 	GPIO_SetBits(START_LIGHT_PORT, START_LIGHT_IO)
#define START_LIGHT_OFF  			GPIO_ResetBits(START_LIGHT_PORT, START_LIGHT_IO)
#define STOP_LIGHT_ON   			GPIO_SetBits(STOP_LIGHT_PORT, STOP_LIGHT_IO)
#define STOP_LIGHT_OFF  			GPIO_ResetBits(STOP_LIGHT_PORT,STOP_LIGHT_IO)

#define KEY_SHORT_TIME 		    15
#define KEY_LONG_TIME			    200
#define KEY_LONGLONG_TIME			400
u8 Key_ScanNum;
u8 Device_State;

//=============================================================================
//��������: KEY_GPIO_Config
//���ܸ�Ҫ:��ť��������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
static void KEY_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ֹͣ����IO����
	RCC_APB2PeriphClockCmd(STOP_KEY_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = STOP_KEY_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(STOP_KEY_PORT, &GPIO_InitStructure);
	//��������IO����
	RCC_APB2PeriphClockCmd(START_KEY_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = START_KEY_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(START_KEY_PORT, &GPIO_InitStructure);
}
//=============================================================================
//��������: Light_GPIO_Config
//���ܸ�Ҫ:����������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
static void Light_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ֹͣ����IO����
	RCC_APB2PeriphClockCmd(STOP_LIGHT_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = STOP_LIGHT_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(STOP_LIGHT_PORT, &GPIO_InitStructure);
	//��������IO����
	RCC_APB2PeriphClockCmd(START_LIGHT_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = START_LIGHT_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(START_LIGHT_PORT, &GPIO_InitStructure);
}
static u8 Key_Scan(void)
{
	 u8 key_num;
	 static u8  start_key_triggerstate;
	 static u16 start_key_timercount;
	 static u8 stop_key_triggerstate;
	 static u16 stop_key_timercount;
	 key_num = 0;
	 //start_key
		if(READ_START_KEY == READLOW){
				if(start_key_triggerstate == 0){
					start_key_triggerstate = 1;
				}else if(start_key_triggerstate == 1){
					start_key_timercount++;
					if(start_key_timercount > KEY_LONG_TIME){
						key_num |=0x11;
						start_key_triggerstate = 2;
						start_key_timercount = 0;
						return key_num;
					}
				}
		}else{
				if(start_key_triggerstate >= 1){
					if(start_key_timercount <KEY_SHORT_TIME){
							key_num &=0xfe;
					}else if((start_key_timercount >=KEY_SHORT_TIME)&&(start_key_timercount <KEY_LONG_TIME)){
							key_num |=0x01;
					}
					start_key_triggerstate =0;				
				}
				start_key_timercount = 0;
			}
	 //stop_key
		if(READ_STOP_KEY == READLOW){
				if(stop_key_triggerstate == 0){
					stop_key_triggerstate = 1;
				}else{
					stop_key_timercount++;
					if(stop_key_timercount > KEY_LONG_TIME){
						key_num |=0x12;
						stop_key_triggerstate = 2;
						stop_key_timercount = 0;
						return key_num;
					}
				}
		}else{
				if(stop_key_triggerstate >= 1){
					if(stop_key_timercount <KEY_SHORT_TIME){
							key_num &=0xfd;
					}else if((stop_key_timercount >=KEY_SHORT_TIME)&&(stop_key_timercount <KEY_LONG_TIME)){
							key_num |=0x02;
					}
					stop_key_triggerstate =0;				
				}
				stop_key_timercount = 0;
			}
	 return key_num;
}

static void Light_Scan(void)
{
  if(Device_State == 1){//Device_State=0������״̬��Device_State=1������״̬��Device_State=2��ֹͣ״̬
		START_LIGHT_ON;
		STOP_LIGHT_OFF;
	}else if(Device_State == 2){
    STOP_LIGHT_ON;
		START_LIGHT_OFF;
	}else{
		STOP_LIGHT_OFF;
		START_LIGHT_OFF;
	}
}


void Key_Light_Init(void)
{
		KEY_GPIO_Config();
		Light_GPIO_Config();
}

void Key_Light_Dispose(void)
{
  if(Key_ScanNum ==0){
		Key_ScanNum = Key_Scan();
	}
	if(Key_ScanNum !=0){  
		if(Key_ScanNum == 0x01){
			Device_State = 1; 
			belt.state = READY;			//����Ƥ������
		}else if((Key_ScanNum == 0x02)||(Key_ScanNum == 0x12)){
			Device_State = 2;  
			belt.state = END ;     //����Ƥ��ֹͣ
		}else{
			Device_State = 0;   //�����������������֣����Իص�����״̬
			belt.state = END ;     //����Ƥ��ֹͣ
		}
		Key_ScanNum = 0;
	}
	Light_Scan();
}


