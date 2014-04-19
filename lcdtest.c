#include <stdio.h>
#include <wiringPi.h>
#include "lcdtest.h"

#define  DEBUG   0

#define  HIGH  1
#define  LOW   0

#define READPIN 7 


int main(void)
{
  int cnt;
  int res;
  _DHT11Data DHT11Data;

  if(wiringPiSetup() == -1)
    return -1;

  printf(" --- Start Detect ---\n" );

  //Pull High wait for Voltage Stable
  pinMode(READPIN, OUTPUT);
  digitalWrite(READPIN, HIGH);
  delay(1000);

  cnt = 10;

  while(cnt--) 
  {
    //at least keop low 18ms
    pinMode(READPIN, OUTPUT);
    digitalWrite(READPIN, LOW);
    delay(20);
    //Keep High 25~40us
    pinMode(READPIN, OUTPUT);
    digitalWrite(READPIN, HIGH);
    delayMicroseconds(30);

    pinMode(READPIN, INPUT);

    //Wait DHT11 Prepare Data
    Waiting(LOW);
    Waiting(HIGH);

    if(FetchData(&DHT11Data))
    {
      printf("Temperature %2dC Humidity %2d%c \n",DHT11Data.Value[0x02],DHT11Data.Value[0x00],0x25);
    }
    else
    {
      printf("Detect failed\n");
    }

    delay(500);
  }
}


int Waiting(int Signal)
{
  int bCnt=0x00;

  while( digitalRead(READPIN) == Signal )
  {
      bCnt++;
      delayMicroseconds(1);
      if(bCnt > 200) break;
  }

  return bCnt;
}

int FetchData(_DHT11Data * DHT11Data)
{
  int i,j;
  int rawus[40];
  int sbit[40];
  int sum;
  int Maxus=0x00,Minus=0xFFFF;
  int diffrange;

  for( i=0 ; i< 40; i++ )
  {
    Waiting(LOW);
    rawus[i]=Waiting(HIGH);
  }

  for( i=0 ; i< 40; i++ )
  {
    if( rawus[i] > Maxus ) Maxus = rawus[i];
    if( rawus[i] < Minus ) Minus = rawus[i];
  }  
  diffrange = Minus + (Maxus-Minus)/2;

  for( i=0 ; i< 40; i++ )
  {
    sbit[i] = (rawus[i]>diffrange)?1:0;
  }

  for(i=0;i<5;i++)
  {
    DHT11Data->Value[i] = 0x00;
    for(j=0;j<8;j++)
    {
        DHT11Data->Value[i] |= sbit[i*8 + j] <<( 7 - (j&0x07) ) ;
    } 
  }

#if DEBUG
  printf("--- Debug ----\n");
  printf("Maxus = %d , Minus = %d , diff = %d \n",Maxus,Minus, diffrange);
  for( i=0 ; i< 40; i++ )
  {
    printf("%2d ",rawus[i]);
  }
  printf("\n");
  for( i=0 ; i< 40; i++ )
  {
    printf("%2d ",sbit[i]);
  }
  printf("\n");
  for(i=0;i<5;i++)
    printf("%d ",DHT11Data->Value[i]);
  printf("\n------------\n");
#endif

  sum = 0x00;
  for(i=0;i<4;i++)
    sum += DHT11Data->Value[i];

  if( DHT11Data->Value[4] == sum)
    return 1;
  else
    return 0;
}