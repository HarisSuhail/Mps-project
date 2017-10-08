#include <AT89X51.H>
 
void delay(unsigned int d)
{
unsigned int i,j;
for(i=0;i<=d;i++)
for(j=0;j<=1000;j++);
}
void main(void)
{
unsigned char z;
TMOD=0x20;
TH1=0xF3;
SCON=0x50;
TR1=1;
while(1)
 
{ 
for(z=65;z<=92;z++)   // 65 is equivalent code for A.
{
SBUF=z;
if(z==92)
{
SBUF=13; /// carriage return
}
 
while(TI==0);
 
 
delay(50);
TI=0;
 
}
 
}
} 
