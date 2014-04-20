#define  HIGH  1
#define  LOW   0

typedef struct
{
  int Value[5];
}_DHT11Data;

int Waiting(int ReadPin ,int Signal);
int FetchData(int ReadPin , _DHT11Data * DHT11Data);