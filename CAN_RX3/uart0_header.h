#include <stdio.h>

void uart0_config(unsigned int baud)
{
	PINSEL0|=0x05;
	U0LCR=0x83;
	switch(baud)
	{
		case 115200:U0DLL=32;	break;//PCLK=60MHz
		case 921600:U0DLL=4;	break;
		default:	U0DLL=32	;
	}
	U0DLM=0;
	U0LCR=0x03;
}

void uart0_tx(char ch)
{
	U0THR=ch;
	while((U0LSR>>5&1)==0);
}
void uart0_tx_str(char *s)
{
	while(*s)
	{
		uart0_tx(*s++);
	}
}

char uart0_rx(void)
{
    while((U0LSR & 0x01) == 0);  // Wait until data is available in the receiver buffer
    return U0RBR;  // Read and return the received character from the receiver buffer register
}

void uart0_rx_str(char s[], unsigned int max_length)
{
    unsigned int i;
    unsigned char ch;

    for(i = 0; i < max_length - 1; i++)
    {
        ch = uart0_rx();  // Receive a character
        if(ch == '\r' || ch == '\n')  // Check for end of line
            break;
        s[i] = ch;  // Store received character in the string
    }
    s[i] = '\0';  // Null-terminate the string
}

void uart0_tx_integer(int num){
	char buf[10];
	sprintf(buf,"%d",num);
	uart0_tx_str(buf);
}


void uart0_tx_hex(int num){
	char buf[10];
	sprintf(buf,"%X",num);
	uart0_tx_str(buf);
}
