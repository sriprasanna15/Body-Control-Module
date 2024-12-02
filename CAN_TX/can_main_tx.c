#include<LPC21XX.H>
#include"uart0_header.h"
#include"can_driver.h"
#include"lcd.h"
#define SW1 14
#define SW2 15
#define SW3 16
int wiper_on=0;
int main()
{
	CAN2_MSG m1;
	uart0_config(115200);
	LCD_init();
	can2_init();
	m1.id=0x123;
	m1.rtr=0;
	m1.dlc=1;
	m1.byteA=0x01;
	m1.byteB=0;
	LCD_str("BODY CNTL MODULE");
	
	while(1)
	{
		if(((IOPIN0>>SW1)&1)==0)
		{
			LCD_command(0x01);
			LCD_command(0x80);
			LCD_str("DASHBOARD:");
			m1.id=0x01;
			can2_tx(m1);
			LCD_command(0xc0);
			LCD_str("Left LED on");
		}
		if(((IOPIN0>>SW2)&1)==0)
		{
			LCD_command(0x01);
			LCD_command(0x80);
			LCD_str("DASHBOARD:");
			m1.id=0x02;
			can2_tx(m1);
			LCD_command(0xc0);
			LCD_str("Right LED on");
		}
		
		if (((IOPIN0 >> SW3) & 1) == 0) {
            LCD_command(0x01);
            LCD_command(0x80);
            LCD_str("DASHBOARD:");
            m1.id = 0x03;
            can2_tx(m1);
            
            if (wiper_on) 
			{
                wiper_on = 0;
                LCD_command(0xc0);
                LCD_str("WIPER off");
            } 
			else
			 {
                wiper_on = 1;
                LCD_command(0xc0);
                LCD_str("WIPER on");
            }
		}
	}
}
