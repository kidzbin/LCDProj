
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bcm2835.h>
#include "MyPCD8544.h"

// COnfiguration for the LCD
#define LCD_C     LOW
#define LCD_D     HIGH
#define LCD_CMD   0


#define  DEBUG 1

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

#define LCDWIDTH  48
#define LCDHEIGHT 84

void SPI_Shift(uint8_t bDIN,uint8_t bCLK,uint8_t bData);  //MSB First

typedef struct 
{
	uint8_t CLK;
	uint8_t DIN;
	uint8_t DC;
	uint8_t CE;
	uint8_t RST;
}_LCDCtrl;


_LCDCtrl LCDCtrl;


void gotoXY(uint8_t x, uint8_t y)
{
  SendLCD( 0, 0x80 | x);  // Column.
  SendLCD( 0, 0x40 | y);  // Row.
}


int LCDInit(void)
{

    if (!bcm2835_init())
        return 1;

	LCDCtrl.CLK = 0;
	LCDCtrl.DIN = 1;
	LCDCtrl.DC  = 2;
	LCDCtrl.CE  = 3;
	LCDCtrl.RST = 4;

	// pinMode(LCDCtrl.CLK , OUTPUT);
	// pinMode(LCDCtrl.DIN , OUTPUT);
	// pinMode(LCDCtrl.DC  , OUTPUT);
	// pinMode(LCDCtrl.CE  , OUTPUT);
	// pinMode(LCDCtrl.RST , OUTPUT);

    // Set the pin to be an output
    bcm2835_gpio_fsel(LCDCtrl.CLK , BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LCDCtrl.DIN , BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LCDCtrl.DC  , BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LCDCtrl.CE  , BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LCDCtrl.RST , BCM2835_GPIO_FSEL_OUTP);


	// digitalWrite(LCDCtrl.CE , LOW);
	// digitalWrite(LCDCtrl.RST, LOW);
	// digitalWrite(LCDCtrl.RST, HIGH);
	// digitalWrite(LCDCtrl.CE , HIGH);

	bcm2835_gpio_write(LCDCtrl.CE , LOW);
	bcm2835_gpio_write(LCDCtrl.RST, LOW);
	bcm2835_gpio_write(LCDCtrl.RST, HIGH);
	bcm2835_gpio_write(LCDCtrl.CE , HIGH);

	SendLCD(CMD,0x21);
	SendLCD(CMD,0xBF);
	SendLCD(CMD,0x04);
	SendLCD(CMD,0x14);
	SendLCD(CMD,0x0C);

  	SendLCD(LOW, 0x20);
  	SendLCD(LOW, 0x0C);

  	LCDClear();

	return 0;
}

int LCDClear(void)
{
	uint16_t i;
	for ( i = 0; i < LCDWIDTH*LCDHEIGHT/8 ; i++)
		SendLCD(DATA,0x00);

}

void SendLCD(uint8_t bMode,uint8_t bData)
{
	uint8_t i;

	// digitalWrite(LCDCtrl.CE , LOW);
	// digitalWrite(LCDCtrl.DC , bMode);
	bcm2835_gpio_write(LCDCtrl.CE, LOW);
	bcm2835_gpio_write(LCDCtrl.DC, bMode);

	SPI_Shift(LCDCtrl.DIN,LCDCtrl.CLK, bData) ;
	// digitalWrite(LCDCtrl.CE , HIGH);
	bcm2835_gpio_write(LCDCtrl.CE, HIGH);

#if DEBUG
	printf("%2x ",bData);

	i=0x08;
	do
	{
		i--;
		printf("%d ",(bData>>i)&0x01);
	}while(i);

	printf("\n");
#endif
}

void LcdCharacter(char character)
{
	uint8_t index;
  	SendLCD(LCD_D, 0x00);
  	for (index = 0; index < 5; index++)
  	{
    	SendLCD(LCD_D, ASCII[character - 0x20][index]);
  	}
  	SendLCD(LCD_D, 0x00);
}

void LcdString(char *characters)
{
  while (*characters)
  {
    LcdCharacter(*characters++);
  }
}

void LCDSample(void)
{
	LCDClear();
 	gotoXY(7,1);
  	LcdString("Nokia 5110");
  	gotoXY(4,2);
  	LcdString("Scroll Demo");

}

void SPI_Shift(uint8_t bDIN,uint8_t bCLK,uint8_t bData)
{
	uint8_t i;

  	for (i = 7 ; i >= 0 ; --i)
    {
      bcm2835_gpio_write (bDIN, bData & (1 << i)) ;
      bcm2835_gpio_write (bCLK, HIGH) ;
      bcm2835_gpio_write (bCLK, LOW) ;
    }

}