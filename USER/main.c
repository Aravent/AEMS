#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "can.h" 
#include "string.h"

 int main(void)
 {	 
	u8 i=0;
	u8 s=0;
	u8 h=0;
	u8 r=0;
	u8 canbuf[8];
	u8 Can_Send_Flag;
	u8 len;

	delay_init();	    	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);	 	//串口初始化为115200
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_3tq,CAN_BS1_8tq,3,CAN_Mode_Normal);//CAN普通模式初始化, 波特率500Kbps 
	
	printf("欢迎使用大翼虎嗅空气质量传感器！\n详情请访问：http://www.airdwing.com/");
 	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			s = (int)len/8;
			if((len%8) != 0)s++;
			printf("\r\n您发送的消息为:\r\n");
			for(h=0;h<s;h++)
			{
				for(i=0;i<8;i++)
				{
					canbuf[i]=USART_RX_BUF[r];//填充发送缓冲区
					USART_SendData(USART1,(int)USART_RX_BUF[r]);//向串口1发送数据
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
					r++;
				}
				Can_Send_Flag=Can_Send_Msg(canbuf,8);//发送8个字节 
				delay_ms(1);
			}
			if(Can_Send_Flag)   printf("\ndata Send failed\n");
			else  printf("\ndata Send ok %d\n",s);
      //while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束			
			r=0;
			USART_RX_STA=0;
		}
	}
}
