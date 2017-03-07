#include"HeadType.h"	

#define PRINTER_START_DELAY_TIME  10;
//=============================================================================
//��������: Printer_GPIO_Config
//���ܸ�Ҫ:��ӡ����������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
 void Printer_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//��ӡ����������IO����
	RCC_APB2PeriphClockCmd(PRINTER_ERR_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_ERR_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_ERR_PORT, &GPIO_InitStructure);
	//��ӡ����ӡ��������IO����
	RCC_APB2PeriphClockCmd(PRINTER_END_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_END_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_END_PORT, &GPIO_InitStructure);
	//��ӡ����������IO����
	RCC_APB2PeriphClockCmd(PRINTER_INLINE_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_INLINE_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_INLINE_PORT, &GPIO_InitStructure);
	//��ӡ����ǩ��������IO����
	RCC_APB2PeriphClockCmd(PRINTER_TAGEND_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_TAGEND_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_TAGEND_PORT, &GPIO_InitStructure);
	//��ӡ��ɫ����������IO����
	RCC_APB2PeriphClockCmd(PRINTER_COEND_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_COEND_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_COEND_PORT, &GPIO_InitStructure);
	//��ӡ��ɫ��������IO����
	RCC_APB2PeriphClockCmd(PRINTER_COLESS_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_COLESS_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_COLESS_PORT, &GPIO_InitStructure);	
	
	//��ӡ���������IO����
	RCC_APB2PeriphClockCmd(PRINTER_START_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_START_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_START_PORT, &GPIO_InitStructure);
	//��ӡ���ظ��������IO����
	RCC_APB2PeriphClockCmd(PRINTER_RESTART_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_RESTART_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_RESTART_PORT, &GPIO_InitStructure);
	

}
static void Printer_Input_Scan(void)
{
   if((Printer.err.state == 0)&&(Printer.tag_end.state == 0)&&(Printer.color_end.state == 0)){
			Printer.input_state = 1;
		}else{ //�д������ʹ�豸�ָ�������״̬��������״̬�ſ���ȡ������״̬
			Printer.input_state = 0;
		}

}
static u8 Printer_Process_Input(void)
{
	 u8 res;
	 Printer_Input_Scan();
	 if(Device_State == 1){  //�豸����
			if(Printer.input_state == 1){  //��ӡ������״̬OK
//				if(Printer.process == 1){      //��ӡ��������ӡ
//					PRINTER_START_ON;
//				}
				res = 0;
				return res;
			}
		 
		}else if(Device_State == 2){ //�豸ֹͣ
			 PRINTER_START_OFF;
			 Printer.err.state = 0;
			 Printer.tag_end.state = 0;
			 Printer.color_end.state = 0;
			 Printer.pinline.state = 0;
		}else{
			 PRINTER_START_OFF;
			 Printer.err.state = 0;
			 Printer.tag_end.state = 0;
			 Printer.color_end.state = 0;
			 Printer.pinline.state = 0;
		}
		res = 1;
		return res;

}
void Printer_Control(void)
{
	if(1 == Printer_Process_Input()){		//ÿ�����̶���Ҫ��ѯһ�´�ӡ��״̬���豸״̬�Ƿ�OK
			Printer.process = PRINTER_RESERVE;		
	}else{
			Printer.process = Printer.process;	
	}
	switch(Printer.process){
		case PRINTER_RESERVE:		if(0 == Printer_Process_Input()){		//��ӡ��OK�
																Printer.process = PRINTER_READY;		
														}
													break ;
		case PRINTER_READY:     if((Air_Control.complete == 1)&&(Control.fluid_bag.state == 1)){  //��ʼ��ӡ��ʱ��Ͱ�Һ�������ź���λ�����Խ�����һ���ź�����
																Printer.process = PRINTER_WORKING;
																Printer.start_delay_time = PRINTER_START_DELAY_TIME;																
																PRINTER_START_ON;
																AIR_BLOW_ON;
																VACUUM_ON;
																Air_Control.complete = 0;
																Control.fluid_bag.state = 0;
														}		                  
					break ;
		case PRINTER_WORKING:   if(Printer.end.state == 1){
															Printer.process = PRINTER_END;
															Printer.complete = 1;
															AIR_BLOW_OFF;
															Printer.end.state = 0;
													  }
					break ;
		case PRINTER_END:       Printer.process = PRINTER_READY; 
					break ;
		default :break ;
	}
}



















