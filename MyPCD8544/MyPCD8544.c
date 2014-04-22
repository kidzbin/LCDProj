
#include <wiringPi.h>
#include "MyPCD8544.h"
#include <stdio.h>

// LCD pins      Raspberry Pi
// LCD1 - GND    P06  - GND
// LCD2 - VCC    P01 - 3.3V
// LCD3 - CLK    P11 - GPIO0
// LCD4 - Din    P12 - GPIO1
// LCD5 - D/C    P13 - GPIO2
// LCD6 - CS     P15 - GPIO3
// LCD7 - RST    P16 - GPIO4
// LCD8 - LED    P01 - 3.3V 

//delayMicroseconds(30);

typedef struct 
{
	unsigned char CLK;
	unsigned char DIN;
	unsigned char DC;
	unsigned char CE;
	unsigned char RST;
}_LCDCtrl;


_LCDCtrl LCDCtrl;

void LCDReset(void)
{
	digitalWrite(LCDCtrl.CE , LOW);
	digitalWrite(LCDCtrl.RST, LOW);
	delay(500);
	digitalWrite(LCDCtrl.RST, HIGH);
	digitalWrite(LCDCtrl.CE , HIGH);
}


int LCDInit(void)
{

  if(wiringPiSetup() == -1)
    return -1;

	LCDCtrl.CLK = 0;
	LCDCtrl.DIN = 1;
	LCDCtrl.DC  = 2;
	LCDCtrl.CE  = 3;
	LCDCtrl.RST = 4;

	pinMode(LCDCtrl.CLK , OUTPUT);
	pinMode(LCDCtrl.DIN , OUTPUT);
	pinMode(LCDCtrl.DC  , OUTPUT);
	pinMode(LCDCtrl.CE  , OUTPUT);
	pinMode(LCDCtrl.RST , OUTPUT);

	LCDReset();

	delayMicroseconds(100);

	digitalWrite(LCDCtrl.CE , LOW);
	digitalWrite(LCDCtrl.CLK, LOW);

	return 0;
}

void Sned(unsigned char bMode,unsigned char bData)
{
	unsigned char i;

	digitalWrite(LCDCtrl.DC , bMode);
	delayMicroseconds(1);
	
	i=0x08;
	do
	{
		i--;
		digitalWrite(LCDCtrl.CLK , LOW     );
		delayMicroseconds(1);
		digitalWrite(LCDCtrl.DIN , (bData>>i)&0x01 );
		delayMicroseconds(1);
		digitalWrite(LCDCtrl.CLK , HIGH    );
		delayMicroseconds(1);
	}while(i);

	printf("%2x ",bData);

	i=0x08;
	do
	{
		i--;
		printf("%d ",(bData>>i)&0x01);
	}while(i);

	printf("\n");
}

void LCDSample(void)
{
	//Function Set
	Sned(CMD,0x21);
	Sned(CMD,0x90);
	Sned(CMD,0x20);
	Sned(CMD,0x0C);

	Sned(DATA,0x1F);
	Sned(DATA,0x05);
	Sned(DATA,0x07);

	Sned(DATA,0x05);
	Sned(DATA,0x07);
	Sned(DATA,0x00);
	Sned(DATA,0x0F);

	Sned(DATA,0x04);
	Sned(DATA,0x1F);

	Sned(CMD,0x0D);



}