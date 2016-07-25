#include <stdio.h>
#include <wiringPi.h>
#include <stdint.h>
#include <time.h>
#include "MyPCD8544/MyPCD8544.h"
#include "lcdtest.h"

#define _DIN 1
#define _CLK 0
#define _DC  2
#define _RST 4
#define _CE  3
#define _CONTRAST  30

int main(void)
{
  time_t rawtime;
  struct tm * timeinfo;
  char LCDStr[12];
  int cnt;

  if(wiringPiSetup() == -1)
    return -1;

  LCDInit(_CLK, _DIN , _DC , _CE , _RST, _CONTRAST);

  LCDClear();

  cnt = 1;
  while(cnt) 
  {
    //LCDSample();
    time ( &rawtime );
    timeinfo = localtime ( &rawtime ); 

    //wTemp = DetectThermalCouple();

    //if(FetchData(&DHT11Data))
    {
      //printf("Temperature %2dC Humidity %2d%c \n",DHT11Data.Value[0x02],DHT11Data.Value[0x00],0x25);
      //printf("%2d %2d %3.2f\n",DHT11Data.Value[0x02],DHT11Data.Value[0x00],wTemp);

      //LCDClear();

      //LCDSample();

      if(timeinfo->tm_hour<12)
        sprintf(LCDStr,"%02d:%02d:%02d ",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
      else
        sprintf(LCDStr,"%02d:%02d:%02d ",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

       gotoXY(0,0);
       LcdString(LCDStr);      

      //sprintf(LCDStr,"Humi %2d%c",DHT11Data.Value[0x00],0x25);
     // gotoXY(4,1);
      //LcdString(LCDStr);

      //sprintf(LCDStr,"Temp %2dC",DHT11Data.Value[0x02]);
     // gotoXY(4,2);
      //LcdString(LCDStr);

      //sprintf(LCDStr,"Roast");
      //gotoXY(4,3);
      //LcdString(LCDStr);      

      //sprintf(LCDStr,"%2.2fC",wTemp);
      //gotoXY(30,4);
     // LcdString(LCDStr);

      delay(1000);
      //cnt--;
    }
  }
}