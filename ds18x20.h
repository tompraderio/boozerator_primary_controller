#ifndef DS18X20_H_
#define DS18X20_H_

#define DS1820_OUT 					P4OUT
#define DS1820_DIR 					P4DIR
#define DS1820_SEL					P4SEL
#define DS1820_REN					P4REN
#define DS1820_IN					P4IN
#define DS1820_GND          		BIT0
#define DS1820_DATA_IN_PIN          BIT1
#define DS1820_VCC			        BIT2




#define DS1820_SKIP_ROM             0xCC
#define DS1820_READ_SCRATCHPAD      0xBE
#define DS1820_CONVERT_T            0x44

void InitDS18B20(void);
unsigned int ResetDS1820 ( void );
void DS1820_HI(void);
void DS1820_LO(void);
void DS1820_RELEASE(void);
void WriteZero(void);
void WriteOne(void);
unsigned int ReadBit(void);
void WriteDS1820 (unsigned char,int );
uint16_t ReadDS1820 ( void );
uint16_t GetData(void);
#endif /*DS18X20_H_*/
