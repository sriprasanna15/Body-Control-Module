#include<LPC21XX.H>
#include"uart0_header.h"
#include"can_driver.h"

#define LED1 1<<17

void LED1_blink(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		IOCLR0=LED1;
		delay_ms(500);
		IOSET0=LED1;
		delay_ms(500);
	}
}	
int main()
{	
	CAN2_MSG m1;
	IODIR0|=LED1;
	IOSET0=LED1;
	can2_init();
	uart0_config(115200);
	uart0_tx_str("LEFT INDICATTOR\r\n");
	while(1)
	{		
		can2_rx(&m1);
		uart0_tx_str("message received");
		if(m1.rtr==0)
		{
			if(m1.id==0x01)
			{
				LED1_blink();
			}

		}	
		else
		{
			uart0_tx_str("REMOTE-FRAME is received\r\n");
		}
	}
}
