#include <stdio.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include <stdint.h>
#include "DHT11/DHT11.h"
#include <time.h>
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

//pinMode(11  , OUTPUT);  CE
//pinMode(3  , OUTPUT);  CLK
//pinMode(6  , OUTPUT);  DIN


float DetectThermalCouple(void)
{
  uint8_t  buff[2],i;
  int      chan=1;
  int      speed=1000000;
  uint16_t wRaw,dot;
  float    wTemp;

  wiringPiSPIDataRW(chan,buff,2);

  wRaw = ((uint16_t)((buff[0]<<8) | (buff[1]))&0x7FF8)>>3;
  wTemp = (float)(wRaw>>2) + (float)(wRaw&0x03)*(0.25);

  return wTemp;
}


int main(void)
{
  time_t rawtime;
  struct tm * timeinfo;
  _DHT11Data DHT11Data;
  char LCDStr[12];
  float wTemp;
  int cnt;

  int chan=1;
  int speed=1000000;

  if(wiringPiSetup() == -1)
    return -1;

  wiringPiSPISetup (chan, speed) ;

  InitDHT11(READPIN);
  LCDInit();

  cnt = 1;
  while(cnt) 
  {
    //LCDSample();
    time ( &rawtime );
    timeinfo = localtime ( &rawtime ); 

    wTemp = DetectThermalCouple();

    if(FetchData(&DHT11Data))
    {
      //printf("Temperature %2dC Humidity %2d%c \n",DHT11Data.Value[0x02],DHT11Data.Value[0x00],0x25);
      printf("%2d %2d %3.2f\n",DHT11Data.Value[0x02],DHT11Data.Value[0x00],wTemp);

      LCDClear();

      if(timeinfo->tm_hour<12)
        sprintf(LCDStr,"%02d:%02d:%02d ",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
      else
        sprintf(LCDStr,"%02d:%02d:%02d ",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

      gotoXY(0,0);
      LcdString(LCDStr);      

      sprintf(LCDStr,"Humi %2d%c",DHT11Data.Value[0x00],0x25);
      gotoXY(4,1);
      LcdString(LCDStr);

      sprintf(LCDStr,"Temp %2dC",DHT11Data.Value[0x02]);
      gotoXY(4,2);
      LcdString(LCDStr);

      sprintf(LCDStr,"Roast");
      gotoXY(4,3);
      LcdString(LCDStr);      

      sprintf(LCDStr,"%2.2fC",wTemp);
      gotoXY(30,4);
      LcdString(LCDStr);

      //delay(1000);
      cnt--;
    }
  }
}