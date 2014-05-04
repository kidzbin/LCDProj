#define  HIGH  1
#define  LOW   0

typedef struct
{
  uint8_t Value[5];
}_DHT11Data;


int InitDHT11(uint8_t ReadPinSetup);
int Waiting(int Signal);
int FetchData(_DHT11Data * DHT11Data);

