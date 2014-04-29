#include <stdio.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include <stdint.h>
#include "DHT11/DHT11.h"
#include <time.h>
//#include "PCD8544/PCD8544.h"
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
    uint8_t buff[2],i;
    int chan=1;
    int speed=1000000;
    wiringPiSPIDataRW (chan,buff,2);
    uint16_t wRaw,dot;
    float wTemp;

    wRaw = ((uint16_t)((buff[0]<<8) | (buff[1]))&0x7FF8)>>3;
    wTemp = (float)(wRaw>>2) + (float)(wRaw&0x03)*(0.25);

    return wTemp;
}





int main(void)
{
  time_t rawtime;
  struct tm * timeinfo;
  _DHT11Data DHT11Data;
  char HumiInfo[10];
  char TempInfo[10];
  char TimeInfo[10];
  char KInfo[10];
  float wTemp;
  int cnt;
  
  int chan=1;
  int speed=1000000;

  if(wiringPiSetup() == -1)
    return -1;

  wiringPiSPISetup (chan, speed) ;

  printf(" --- Start Detect ---\n" );

  //Pull High wait for Voltage Stable
  pinMode(READPIN, OUTPUT);


  digitalWrite(READPIN, HIGH);
  delay(1000);

  cnt = 1;

  LCDInit();

  // while(cnt) 
  // {
  //    LCDSample();
  //    delay(2000);
  // }

 //return 0;

  while(cnt) 
  {
    //LCDSample();
    time ( &rawtime );
    timeinfo = localtime ( &rawtime ); 

    //at least keop low 18ms
    pinMode(READPIN, OUTPUT);
    digitalWrite(READPIN, LOW);
    delay(20);
    //Keep High 25~40us
    pinMode(READPIN, OUTPUT);
    digitalWrite(READPIN, HIGH);
    delayMicroseconds(30);

    pinMode(READPIN, INPUT);


    wTemp = DetectThermalCouple();
    

    if(FetchData(READPIN,&DHT11Data))
    {
      printf("Temperature %2dC Humidity %2d%c \n",DHT11Data.Value[0x02],DHT11Data.Value[0x00],0x25);

      sprintf(HumiInfo,"Humi %2d%c",DHT11Data.Value[0x00],0x25);
      sprintf(TempInfo,"Temp %2dC",DHT11Data.Value[0x02]);
      sprintf(TimeInfo,"%2d:%2d:%2d ",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
      sprintf(KInfo,"K %2.2f%cC",wTemp,0x27);

      LCDClear();
      gotoXY(4,0);
      LcdString(TimeInfo);    
      gotoXY(4,1);
      LcdString(HumiInfo);
      gotoXY(4,2);
      LcdString(TempInfo);
      gotoXY(4,3);
      LcdString(KInfo);   

      // LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
      // LCDclear();
      // LCDdrawstring(0, 0, "Raspberry Pi:");
      // LCDdrawline(0, 10, 83, 10, BLACK);
      // LCDdrawstring(0, 12, TempInfo);
      // LCDdrawstring(0, 20, HumiInfo);
      // sprintf(TempInfo,"%2d",cnt);
      // LCDdrawstring(0, 28, TempInfo);
      // LCDdisplay();
      delay(1500);
      //cnt--;
    }
    else
    {
      printf("Detect failed\n");
    }    
  }

}