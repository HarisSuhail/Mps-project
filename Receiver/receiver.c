#include<reg51.h>
#define d_time 20 //time delay
#define lcd_dat P1		 //lcd data port
sbit rs = P3^5;		   //lcd cmd port
sbit rw = P3^6;			//lcd cmd port
sbit en = P3^7;			//lcd enable pin
sbit motorControl = P3^4;			  //lcd cmd port

unsigned char number[14] ;
unsigned char count = 0;
unsigned char togglesms = 1;
unsigned char menucontrol = 0;
unsigned char onoff = 0;
unsigned char key = 0;
//Keypad Connections
sbit R1 = P2^0;
sbit R2 = P2^1;
sbit R3 = P2^2;
sbit R4 = P2^3;
sbit C1 = P2^4;
sbit C2 = P2^5;
sbit C3 = P2^6;
sbit C4 = P2^7;
//Hey
//End Keypad Connections

sbit in = P3^2; // the pin from which the data comes in
unsigned char previousstate;

#define control ACC 
sbit three = control^3;
sbit two = control^2;
sbit one = control^1;
sbit zero = control^0;
unsigned char controlword; // for lcd
void Delay(unsigned int);
void rf_receive(void);
void ready(void);
//-----prototypes for lcd-----
void write_lcd(char);
void cmd_lcd(char);
void disp_lcd(char);
void init_lcd();
void str_lcd(char *s);
void int_lcd(unsigned int);
void float_lcd(float);
void welcome(void);
void display_ready(void);
void check_motor_status(void);

//--------------------------
//-----prototype for keypad---
char Read_Keypad(void);
void test_keypad(void);
//----------------------------
//global variables
unsigned char latest = 100;
unsigned char h = 0;
unsigned char i = 0;
unsigned char str; //for gsm
//-----------------------lcd code start----------------
void write_lcd(char dat)
{
lcd_dat = dat;
rw = 0;
en = 1; //for latching one byte info
en = 0; //clear for latching next info
Delay(2);
}

void cmd_lcd(char cmd)
{
rs=0; //set cmd register
write_lcd(cmd);
}

void disp_lcd(char c)
{
rs=1; //set data register
write_lcd(c);
}											 
void init_lcd()
{
cmd_lcd(0x02); //return cursor home
cmd_lcd(0x38); //8 bit mode of operation
cmd_lcd(0x0E); //display on cur off
cmd_lcd(0x01); //clr DDRAM (display data ram)
cmd_lcd(0x06); //shift cursor right after each character displayed
cmd_lcd(0x80); //begin disp from line 1 pos 0
}

void str_lcd(char *s)
{
while(*s)
 disp_lcd(*s++);
}
 unsigned char i;
//--------------------------Lcd code end-----------------

// -------------------- GSM code ------------------------

void init_serial()	  // Initialize serial port
{
	TMOD = 0x20;         // Mode=2
	TH1 = 0xFD;	        // 9600 baud
	//TL1 = 0xFD;		   //As done in another code :p
	SCON = 0x50;      // Serial mode=1 ,8-Bit data,1 Stop bit ,1 Start bit, (TOTAL 10 bits) Receiving on
	TR1 = 1;	     // Start timer
}




void transmit_data(unsigned char value)		// Function to transmit data through serial port
{
			 
	SBUF = value;                // Store data in sbuf
			
	while(TI == 0);	            // Wait till data transmit
	TI = 0;			           //after data is transmitted, turn TI to zero.



	
}

 void sendstring(unsigned char *s)
{
while(*s)
 transmit_data(*s++);
}

void receive_data() interrupt 4	 // Function to recieve data serialy from RS232 into microcontroller
 {
	 str = SBUF;                       // Read sbuf
	 RI = 0;                          // 
	 //transmit_data(str);             // Transmit to HyperTerminal
	 disp_lcd(str);
	 
			
 }


void sendmessage(unsigned char *d)
{

unsigned char tl1 = 255;
unsigned char tl2 = 255;
init_serial();
EA = 0;
sendstring("AT+CMGS=\""); // send the SMS number
sendstring(number);
sendstring("\"");

transmit_data(13);
while(tl1--);
transmit_data(10);
tl1 = 255;		
//Delay(50);

while(tl1--)
while(tl2--);
sendstring(d); // SMS body
//Delay(500);
tl1 = 255;
tl2 = 255;
while(tl1--)
while(tl2--);
transmit_data(0x1A);
transmit_data(0x0D);
transmit_data(0x0A);

  
//EA = 1;
}

void activatesms()
{
init_serial();
EA = 0;
sendstring("AT+CMGF=1"); // send the SMS number
transmit_data(0x1A);
transmit_data(0x0D);
transmit_data(0x0A);
Delay(1000);

EA = 1;}

void main(void)
{
				
	motorControl = 0;
	number[0] =  '+';
	number[1] = '9';
	number[2] =  '2';
	number[3] = '3';
	number[4] =  '3';
	number[5] = '2';
	number[6] =  '8';
	number[7] = '1';
	number[8] =  '8';
	number[9] = '5';
	number[10] = '1';
	number[11] =  '2';
	number[12] = '1';
	number[13] = 0;
	

	welcome();	 //display welcome on the screen

		init_serial();
    activatesms();
	Delay(1000);
	init_serial();
		sendmessage("Project Turned on.");
	//test_keypad(); //test the keypad (display whatever pressed)

	init_lcd(); //initialize the lcd

	welcome();	 //display welcome on the screen

	display_ready();	//display 'READY' on the lcd screen
	 
	check_motor_status();	   // check the status and display the required message


	
	
}


void ready(void) 
{
	IE = 0x81; //enable external interrupt 0
	IT0 = 1; //set interrupt to be edge triggered
}



void rf_receive(void) interrupt 0
{

	// once the interrupts have been called, 
	// we disable the EA signal
	EA = 0;
	
	//recognize a pattern. 
	h = 3; // 3 times the loop runs
	Delay(10);
	for (h = 0 ; h < 3 ; h++) {
		if (in == 0) {
			Delay(20);
			if (in == 1) {
				Delay(20);
			}
			else {
			EA = 1;
			return;
			}
		}
		else {
		EA =1 ;
		return;
		}
	}

	if(in == 1) {
	   EA = 1;
		return;
	}
	if(latest == 100)
	{latest = 0;}
	Delay(d_time);
	three = in;
	controlword = ACC;
	Delay(d_time);
	ACC = controlword;
	two = in;
	controlword = ACC;
	Delay(d_time);
	ACC = controlword;
	one = in;
	controlword = ACC;
	Delay(d_time);
	ACC = controlword;
	zero = in;
	controlword = ACC;
	
	switch(controlword)
	{
	case 0:
	
	motorControl = 0;
	break;


	case 15:
	if(onoff == 0)
	motorControl = 1;
	break;
	}




	EA = 1;
	return;
	
}

void Delay(unsigned int k)
{ 
//	while(k)
//	{
//		TMOD = 0x01;
//		TH0 = 0xFC;
//		TL0 = 0x66;
//		TR0 = 1;
//		while(TF0 == 0);
//		TR0  = 0;
//		TF0 = 0;
//		k--;
//	}

 unsigned char l = 255;
while(k--)
{		while(l--);
l = 255;


}
}
//--------------keypad code start-------------------------
char Read_Keypad()
{

  C1=1;
  C2=1;
  C3=1;
  C4=1;
  R1=0;
  R2=1;
  R3=1;
  R4=1;
  if(C1==0){Delay(100);while(C1==0);return 'D';}
  if(C2==0){Delay(100);while(C2==0);return 'C';}
  if(C3==0){Delay(100);while(C3==0);return 'B';}
  if(C4==0){Delay(100);while(C4==0);return 'A';}
  R1=1;
  R2=0;
  R3=1;
  R4=1;
  if(C1==0){Delay(100);while(C1==0);return '#';}
  if(C2==0){Delay(100);while(C2==0);return '9';}
  if(C3==0){Delay(100);while(C3==0);return '6';}
  if(C4==0){Delay(100);while(C4==0);return '3';}
  R1=1;
  R2=1;
  R3=0;
  R4=1;
  if(C1==0){Delay(100);while(C1==0);return '0';}
  if(C2==0){Delay(100);while(C2==0);return '8';}
  if(C3==0){Delay(100);while(C3==0);return '5';}
  if(C4==0){Delay(100);while(C4==0);return '2';}
  R1=1;
  R2=1;
  R3=1;
  R4=0;
  if(C1==0){Delay(100);while(C1==0);return '+';}
  if(C2==0){Delay(100);while(C2==0);return '7';}
  if(C3==0){Delay(100);while(C3==0);return '4';}
  if(C4==0){Delay(100);while(C4==0);return '1';}

  
  return 0;
}

//----------keypad testing-------
void test_keypad(void)
{
	char z;
	init_lcd();
	cmd_lcd(0x80);
	str_lcd("WELCOME");
	Delay(500);
	cmd_lcd(0x01);
	cmd_lcd(0x80);

	while(1) {
 		while(!(z = Read_Keypad()));
  		disp_lcd(z);
  	}
  return;
}
//-------------------keypad code end---------------------------

void welcome(void)
{

	for(i=0;i<16;i++) {
		cmd_lcd(0x80+i);
		str_lcd("WELCOME");
		Delay (50);
		cmd_lcd(0x01);
		cmd_lcd(0x80);
	}
}


void display_ready(void)
{

	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("Ready!");
	Delay(1000);
	return;
}

void check_motor_status(void)
{
	
					  
				 
				 
				 
//----------------------------------------
	motorControl = 0;
  in = 1;
  			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("Connecting to");
			cmd_lcd(0xC0);
			
			str_lcd("Transmitter");
			Delay(500);
	ready();  // the receiver is ready
	
	while(latest==100)
	{
	

	}
	EA = 0;
	cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("Connected");
			cmd_lcd(0xC0);
			
			str_lcd("........");
			Delay(1000);
			EA =1;
	while(1) {
	
	if(onoff == 1 && latest != 1 )
	motorControl = 1;
	if(onoff == 2)
	motorControl = 0;
//Start of if(!menucontrol)
	if(!menucontrol)
	{
	//Start of switch(controlword)	
	  switch(controlword)
		{	
			case 0 :  //If output is 0000 0000b
								//water level high	, 4/4
								//send message to LCD
			if (latest == 1){break;}
			EA = 0;
			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("Full");
			cmd_lcd(0xC0);
			str_lcd("4/4 ");
			motorControl = 0;
				if(motorControl)
		str_lcd("Motor On");
		else
		str_lcd("Motor Off");
			Delay(1000);
			latest = 1;
			ready();
			
		
			
				//SEND SMS
				if(togglesms)
				{
		 	//activatesms();
			//Delay(1000);
			//init_serial();
			sendmessage("Motor Turned Off. Tank is full.");
			togglesms = 0;
			

				}
				//END OF SEND SMS
			
			break;

			case 1:   // If output is 0000 0001b
								//water level 	medium, 3/4
			if(latest == 2) {break;}
								//send message to LCD	
			EA  = 0;
			togglesms = 1;
			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("High");
			cmd_lcd(0xC0);
			str_lcd("3/4 ");
			
			
		if(onoff == 1)
		{
		str_lcd("Motor On");
		}

			else if(motorControl)
		str_lcd("Motor On");
		else
		str_lcd("Motor Off");

			
			latest = 2;
		    ready();
			break;

			case 3:	  //If output is 0000 0011b
								//water level medium, 2/4
              	//send message to LCD
				togglesms = 1;
			if(latest == 3)	{break;}
			EA = 0 ;
			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("Medium");
			cmd_lcd(0xC0);
			str_lcd("2/4 ");
				
				if(motorControl)
		str_lcd("Motor On");
		else
		str_lcd("Motor Off");
		
			latest = 3;
			ready();


			break;

			case 7:	 //If output is 0000 0111b

			if(latest == 4)	{break;}
								// Water level low, 1/4
			EA = 0;   //send message to LCD
			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("Low");
			cmd_lcd(0xC0);
			str_lcd("1/4 ");
			if(onoff == 0)
				motorControl = 1;
				if(motorControl)
		str_lcd("Motor On");
		else
		str_lcd("Motor Off");
			Delay(1000);
					
			latest = 4;
			ready();
			break;

			case 15: // If output is 0000 1111b
							 //send message to LCD 
			if(latest == 5)	{break;}
			EA = 0;
		cmd_lcd(0x01);
		cmd_lcd(0x80);
		str_lcd("Empty");
		cmd_lcd(0xC0);
		str_lcd("0/4 ");
		if(motorControl)
		str_lcd("Motor On");
		else
		str_lcd("Motor Off");

		Delay(1000);
 	
	if(onoff == 0)
{
		motorControl = 1;
			if(togglesms)
				{
			//activatesms();
			//Delay(1000);
			//init_serial();
			sendmessage("Motor Turned On. Tank is Empty.");
			

				}
				togglesms = 0;
		}
		latest = 5;
		ready();
		break;

	default:
		if(latest == 6) {break;} 	//Send error message.
		EA = 0;
		cmd_lcd(0x01);
		cmd_lcd(0x80);
		str_lcd("Error: Check");
			cmd_lcd(0xC0);
		str_lcd("transmitter.");
		Delay(1000);
		motorControl = 0;
		latest = 6;
		ready();
		break;	
	}
	//end of switch controlword

	 }
	 //end of if menucontrol
	 EA = 0;
	 key = 0 ;	 
	 key = Read_Keypad();
	 EA = 1;
	 

	 //Start of if key != 0
	 if(key != 0)
	 {
	 
	 //Start of if menucontrol == 0
	 if(menucontrol == 0)
	 {
	 menucontrol = 1;
	 key = 0;
	 EA = 0;
	 	cmd_lcd(0x01);
		cmd_lcd(0x80);
		if(onoff == 0)
		str_lcd("Menu:(1)ON(2)OFF");
		else
		str_lcd ("(1,2)AutoEnable")	;
		cmd_lcd(0xC0);
		str_lcd("(3)Change Number");
		latest = 0;
		EA = 1;
	 
	 }
	  //End of if menucontrol == 0

	 else if(menucontrol == 1)
	 {
	 menucontrol = 0;
	 
	 switch(key)
	 {
	 case '1':

	  if(onoff == 0)
	  {
	 EA = 0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("Turned on");
	cmd_lcd(0xC0);
	str_lcd("Auto-disabled");		
	Delay(1500);
	latest = 0;
	EA = 1;
	 onoff = 1;
	 }
	 else
	 {
	  EA = 0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("Autoenabled");
	cmd_lcd(0xC0);
	str_lcd("-----------");		
	Delay(1500);
	latest = 0;
	EA = 1;
	 onoff = 0;
	 }
	 break;
	 case  '2':	   
	 if(onoff == 0)
	  {
	 EA = 0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("Turned off");
	cmd_lcd(0xC0);
	str_lcd("Auto-disabled");		
	Delay(1500);
	latest = 0;
	EA = 1;
	 onoff = 2;
	 }
	 else
	 {
	  EA = 0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("Autoenabled");
	cmd_lcd(0xC0);
	str_lcd("-.-.-.-.-.-.-.-.-.-.-");		
	Delay(1500);
	latest = 0;
	EA = 1;
	 onoff = 0;
	 }
	 break;
	case '3':
	EA = 0;
	menucontrol = 2;
	
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("Enter new number:");
	cmd_lcd(0xC0);
	count = 0;
	
	latest = 0;
		
	 
	 EA =1;
	break;
	 
	 }//End of switch key
	 }//End of menucontrol == 1

	 else if(menucontrol == 2)
	 {
	 if(count == 13)
	 {
	 number[13] = 0;
	 }
	 if(count < 13)
	 {
	 *(number+count) = key;
	 disp_lcd(number[count]);
	  count++;
	 }
	 else
	 {
	 latest = 0;
	 menucontrol = 0;
	 EA  = 0;
	 	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("New number:");
	cmd_lcd(0xC0);
	str_lcd(number);
	Delay(1000);
	EA = 1;
	


	 }
	 }

	 }
	 //End if key != 0
	  //EA = 1;
}//End of while 1



}//End of main