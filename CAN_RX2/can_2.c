#include<LPC21XX.H>
#include"can_driver.h"
#include"uart0_header.h"
#define LED2 1<<18

void LED2_blink(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		IOCLR0=LED2;
		delay_ms(500);
		IOSET0=LED2;
		delay_ms(500);
	}
}	
int main()
{	
	CAN2_MSG m1;
	IODIR0|=LED2;
	IOSET0=LED2;
	can2_init();
	uart0_config(115200);
	uart0_tx_str("nodeB TESTING CAN\r\n");
	while(1)
	{		
		can2_rx(&m1);
		if(m1.rtr==0)
		{
			if(m1.id==0x02)
			{
				LED2_blink();
			}

		}	
		else
		{
			uart0_tx_str("nodeB: REMOTE-FRAME is received\r\n");
		}
	}
}
