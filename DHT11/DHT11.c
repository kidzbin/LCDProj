#include <stdint.h>
#include <wiringPi.h>
#include "DHT11.h"

#define  DEBUG   0

uint8_t bReadPin;


int InitDHT11(uint8_t ReadPinSetup)
{
  bReadPin = ReadPinSetup;

  //Pull High wait for Voltage Stable
  pinMode(bReadPin, OUTPUT);

  digitalWrite(bReadPin, HIGH);
  delay(50);
}

int Waiting(int Signal)
{
  int bCnt=0x00;

  while( digitalRead(bReadPin) == Signal )
  {
      bCnt++;
      delayMicroseconds(1);
      if(bCnt > 200) break;
  }

  return bCnt;
}

int FetchData(_DHT11Data * DHT11Data)
{
  uint8_t i,j;
  uint8_t rawus[40];
  uint8_t sbit[40];
  uint8_t sum;
  uint8_t Maxus=0x00,Minus=0xFF;
  uint8_t diffrange;

  //at least keop low 18ms
  pinMode(bReadPin, OUTPUT);
  digitalWrite(bReadPin, LOW);
  delay(20);
  //Keep High 25~40us
  pinMode(bReadPin, OUTPUT);
  digitalWrite(bReadPin, HIGH);
  delayMicroseconds(30);

  pinMode(bReadPin, INPUT);

  //Wait DHT11 Prepare Data
  Waiting(LOW);
  Waiting(HIGH);

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

  if( (DHT11Data->Value[4] != sum) || !sum)
    return 0;
  else
    return 1;
}