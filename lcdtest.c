#include <stdio.h>
#include <stdint.h>
#include "MyPCD8544/MyPCD8544.h"
#include "lcdtest.h"

#define READPIN 7 

#define KDIN    6
#define KSCK    5

//uint8_t shiftIn (uint8_t dPin, uint8_t cPin, uint8_t order) ;

// pin setup
// int _din = 1;
// int _sclk = 0;
// int _dc = 2;
// int _rst = 4;
// int _cs = 3;

// lcd contrast
int contrast = 50;


// uint16_t GetTemperature(uint8_t bDataIn , uint8_t bClk);

// uint16_t GetTemperature(uint8_t bDataIn , uint8_t bClk)
// {
//   uint16_t wTemp;
//   uint8_t  i;

//   wTemp = 0x00;

//   i = 16;
//   do
//   {
//     i--;
//     wTemp |= (shiftIn(bDataIn, bClk, MSBFIRST)&0x01) << i ;
//   }while(i);

//   return wTemp;
// }


int main(void)
{
  //_DHT11Data DHT11Data;
  //char HumiInfo[10];
  //char TempInfo[10];
  uint16_t wTemp;
  int cnt;

  // if(wiringPiSetup() == -1)
  //   return -1;

  printf(" --- Start Detect ---\n" );

  // //Pull High wait for Voltage Stable
  // pinMode(READPIN, OUTPUT);
  // digitalWrite(READPIN, HIGH);
  // delay(1000);

  cnt = 1;

  LCDInit();

  //while(cnt) 
  {
     LCDSample();
     
  }

 //return 0;

  // while(cnt) 
  // {
  //   //LCDSample();

  //   //at least keop low 18ms
  //   pinMode(READPIN, OUTPUT);
  //   digitalWrite(READPIN, LOW);
  //   delay(20);
  //   //Keep High 25~40us
  //   pinMode(READPIN, OUTPUT);
  //   digitalWrite(READPIN, HIGH);
  //   delayMicroseconds(30);

  //   pinMode(READPIN, INPUT);

  //   if(FetchData(READPIN,&DHT11Data))
  //   {
  //     printf("Temperature %2dC Humidity %2d%c \n",DHT11Data.Value[0x02],DHT11Data.Value[0x00],0x25);

  //     // sprintf(HumiInfo,"Humi %2d%c",DHT11Data.Value[0x00],0x25);
  //     // sprintf(TempInfo,"Temp %2dC",DHT11Data.Value[0x02]);
      
      
  //     // LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
  //     // LCDclear();
  //     // LCDdrawstring(0, 0, "Raspberry Pi:");
  //     // LCDdrawline(0, 10, 83, 10, BLACK);
  //     // LCDdrawstring(0, 12, TempInfo);
  //     // LCDdrawstring(0, 20, HumiInfo);
  //     // sprintf(TempInfo,"%2d",cnt);
  //     // LCDdrawstring(0, 28, TempInfo);
  //     // LCDdisplay();
  //     delay(5000);
  //     //cnt--;
  //   }
  //   else
  //   {
  //     printf("Detect failed\n");
  //   }    
  // }

}