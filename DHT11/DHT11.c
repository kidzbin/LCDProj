#include "DHT11.h"

#define  DEBUG   0

int Waiting(int ReadPin , int Signal)
{
  int bCnt=0x00;

  while( digitalRead(ReadPin) == Signal )
  {
      bCnt++;
      delayMicroseconds(1);
      if(bCnt > 200) break;
  }

  return bCnt;
}

int FetchData(int ReadPin , _DHT11Data * DHT11Data)
{
  int i,j;
  int rawus[40];
  int sbit[40];
  int sum;
  int Maxus=0x00,Minus=0xFFFF;
  int diffrange;

  //Wait DHT11 Prepare Data
  Waiting(ReadPin,LOW);
  Waiting(ReadPin,HIGH);

  for( i=0 ; i< 40; i++ )
  {
    Waiting( ReadPin, LOW);
    rawus[i]=Waiting( ReadPin, HIGH);
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