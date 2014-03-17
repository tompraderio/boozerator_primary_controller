#ifndef DS18X20_H_
#define DS18X20_H_


/* pin mapping
 *
 * expansion pin - msp430 pin
 * 2.0 - 2.7
 * 2.1 - 4.2
 * 2.2 - 4.1
 * 2.3 - 8.1
 * 2.4 - 2.3
 * 2.5 - 2.6
 * 2.6 -
 * 2.7 -
 *
 */

#define TEMP0_OUT	P2OUT
#define TEMP0_DIR	P2DIR
#define TEMP0_SEL	P2SEL
#define TEMP0_REN	P2REN
#define TEMP0_IN	P2IN
#define TEMP0_PIN	BIT7

#define TEMP1_OUT	P4OUT
#define TEMP1_DIR	P4DIR
#define TEMP1_SEL	P4SEL
#define TEMP1_REN	P4REN
#define TEMP1_IN	P4IN
#define TEMP1_PIN	BIT2

#define TEMP2_OUT	P4OUT
#define TEMP2_DIR	P4DIR
#define TEMP2_SEL	P4SEL
#define TEMP2_REN	P4REN
#define TEMP2_IN	P4IN
#define TEMP2_PIN	BIT1

#define TEMP3_OUT	P8OUT
#define TEMP3_DIR	P8DIR
#define TEMP3_SEL	P8SEL
#define TEMP3_REN	P8REN
#define TEMP3_IN	P8IN
#define TEMP3_PIN	BIT1

#define TEMP4_OUT	P2OUT
#define TEMP4_DIR	P2DIR
#define TEMP4_SEL	P2SEL
#define TEMP4_REN	P2REN
#define TEMP4_IN	P2IN
#define TEMP4_PIN	BIT3

#define TEMP5_OUT	P2OUT
#define TEMP5_DIR	P2DIR
#define TEMP5_SEL	P2SEL
#define TEMP5_REN	P2REN
#define TEMP5_IN	P2IN
#define TEMP5_PIN	BIT6

#define DS1820_SKIP_ROM             0xCC
#define DS1820_READ_SCRATCHPAD      0xBE
#define DS1820_CONVERT_T            0x44

unsigned int Reset_TEMP0 ( void );
void TEMP0_HI(void);
void TEMP0_LO(void);
void TEMP0_RELEASE(void);
void WriteZero_TEMP0(void);
void WriteOne_TEMP0(void);
unsigned int ReadBit_TEMP0(void);
void Write_TEMP0 (unsigned char,int );
uint16_t Read_TEMP0 ( void );
uint16_t GetData_TEMP0(void);

unsigned int Reset_TEMP1 ( void );
void TEMP1_HI(void);
void TEMP1_LO(void);
void TEMP1_RELEASE(void);
void WriteZero_TEMP1(void);
void WriteOne_TEMP1(void);
unsigned int ReadBit_TEMP1(void);
void Write_TEMP1 (unsigned char,int );
uint16_t Read_TEMP1 ( void );
uint16_t GetData_TEMP1(void);

unsigned int Reset_TEMP2 ( void );
void TEMP2_HI(void);
void TEMP2_LO(void);
void TEMP2_RELEASE(void);
void WriteZero_TEMP2(void);
void WriteOne_TEMP2(void);
unsigned int ReadBit_TEMP2(void);
void Write_TEMP2 (unsigned char,int );
uint16_t Read_TEMP2 ( void );
uint16_t GetData_TEMP2(void);
#endif /*DS18X20_H_*/
