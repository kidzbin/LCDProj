#define  HIGH  1
#define  LOW   0

#define CMD    0
#define DATA   1

void LCDReset(void);
int LCDInit(void);

void LCDSample(void);
void Sned(unsigned char bMode,unsigned char bData);
