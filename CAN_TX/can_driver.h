typedef unsigned int u32;
typedef unsigned int u8;

typedef struct CAN2
{
	u32 id;
	u32 rtr;
	u32 dlc;
	u32 byteA;
	u32 byteB;
}CAN2_MSG;

void can2_init(void)
{
	PINSEL1|=0x00014000;// p0.24 td2  p0.23 rd2
	VPBDIV=0x01;
	C2MOD=0x01;// reset mode
	AFMR=0x02;// for accepting all messages
	C2BTR=0x001C001D;// 125kbps 60mhz
	C2MOD=0x00;// normal mode
}

void can2_tx(CAN2_MSG m1)
{
	C2TID1=m1.id; // msg id
	C2TFI1=(m1.dlc<<16);
	uart0_tx_str("message transmitted"); 
	if(m1.rtr==0)
	{
		C2TFI1&=~(1<<30); // data frame
		C2TDA1=m1.byteA; //lower 4 bytes
		C2TDB1=m1.byteB; // upper 4 bytes
	}
	else
	{
		C2TFI1|=1<<30; // remote frame
	}
	uart0_tx_str("data frame");
	C2CMR=(1<<0)|(1<<5); //start transmission and select tx buffer 1
	while((C2GSR&(1<<3))==0);// wait for data transmission
}

void can2_rx(CAN2_MSG *m1)
{
	while((C2GSR&0x1)==0);// wait for receive buffer status
	m1->id=C2RID; // received msg id
	m1->dlc=(C2RFS>>16)&0xF;
	m1->rtr=(C2RFS>>30)&0x1;
	if(m1->rtr==0)
	{
		m1->byteA=C2RDA;
		m1->byteB=C2RDB;
	}
	C2CMR=(1<<2);
}
