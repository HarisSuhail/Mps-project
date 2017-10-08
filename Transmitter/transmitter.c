#include<regx51.h>
#define d_time 20
#define in P0
sbit out = P2^0;
sbit in1 = P0^0;
sbit in2 = P0^1;
sbit in3 = P0^2;
sbit in4 = P0^3;

void Delay(unsigned int);
void rf_transmit(unsigned char);
void ready(void);
unsigned char h = 0;

void main(void)
{
	in = 0xFF; //setting it as an input port

	while(1) {
	rf_transmit(in);
	Delay(60);
	}
	while(1);
	return;
}


void rf_transmit(unsigned char i)
{
	ready();
	out = i & 0x08;
	Delay(d_time);
	out = i & 0x04;
	Delay(d_time);
	out = i & 0x02;
	Delay(d_time);
	out = i & 0x01;
	Delay(d_time);
	out = 0;
	Delay(20);
}


void ready()
{
	out = 1;
	Delay(100);
	for (h = 0; h < 3;h++) {
		out = 0;
		Delay(d_time);
		out = 1;
		Delay(d_time);
	}
	out = 0;
	Delay(d_time);
}





void Delay(unsigned int k)
{
//	while(k)
//	{
//	TMOD = 0x01;
//	TH0 = 0xFC;
//	TL0 = 0x66;
//	TR0 = 1;
//	while(TF0 == 0);
//	TR0  = 0;
//	TF0 = 0;
//	k--;
//	}
 unsigned char l = 255;
while(k--)
{		while(l--);
l = 255;


}
}