#include<lpc21xx.h>
#define LCD_D 0x0f<<20
#define SW 14
#define rs 1<<17
#define rw 1<<18
#define e 1<<19
void delay_ms(unsigned int ms)
{
	T0PR=60000-1;
	T0TCR=0x01;
	while(T0TC<ms);
	T0TCR=0x03;
	T0TCR=0x00;
}
void LCD_command(unsigned char cmd)
{
	IOCLR1=LCD_D;
	IOSET1=((cmd&0xf0)<<16);
	IOCLR1=rs;
	IOSET1=e;
	delay_ms(5);
	IOCLR1=e;
	
	IOCLR1=LCD_D;
	IOSET1=((cmd&0x0f)<<20);
	IOCLR1=rs;
	IOSET1=e;
	delay_ms(5);
	IOCLR1=e;
}
void LCD_data(unsigned char d)
{
	IOCLR1=LCD_D;
	IOSET1=((d&0xf0)<<16);
	IOSET1=rs;
	IOSET1=e;
	delay_ms(5);
	IOCLR1=e;
	
	IOCLR1=LCD_D;
	IOSET1=((d&0x0f)<<20);
	IOSET1=rs;
	IOSET1=e;
	delay_ms(5);
	IOCLR1=e;
}
void LCD_init(void)
{
	IODIR1|=LCD_D|rs|e|rw;
	IOCLR1=rw;
	LCD_command(0x01);
	LCD_command(0x02);
	LCD_command(0x0c);
	LCD_command(0x28);
	LCD_command(0x80);
}
void LCD_str(char* msg)
{
	int i;
	for(i=0;msg[i];i++)
	LCD_data(msg[i]);
}

void LCD_int(int n)
{
	unsigned char arr[5];
	signed char i=0;
	
	if(n=='0')
		LCD_data('0');
	else
	{
		if(n<0)
		{
			LCD_data('-');
			n=-n;
		}
		while(n>0)
		{
			arr[i++]=n%10;
			n=n/10;
		}
		for(i=i-1;i>=0;i--)
		{
			LCD_data(arr[i]+48);
		}
	}
}

void LCD_float(float f)
{
	int temp;
	temp=f;
	LCD_int(temp);
	LCD_data('.');
	temp=(f-temp)*100;
	LCD_int(temp);
}

void LCD_scroll(char *s)
{
	int i=0x80;
	while(1)
	{
		LCD_command(i);
		LCD_str(s);
		i++;
		delay_ms(200);
		LCD_command(0x01);
		if(i==0x92)
			break;
	}
}
	

