#include <msp430.h>
#include <string.h>
#include <stdlib.h>
#include "tshell.h"
#include "fifo.h"
#include "boozerator_primary_controller.h"
#include "ishan.h"
#include "ds18x20.h"



// TEMP0 ================================================================================================================
void TEMP0_HI()
{
	TEMP0_DIR|=TEMP0_PIN; //set port as output
	TEMP0_OUT|=TEMP0_PIN;	//set port high
}
void TEMP0_LO()
{
	TEMP0_DIR|=TEMP0_PIN; //set port as output
	TEMP0_OUT&=~TEMP0_PIN;//set port low
}
void TEMP0_RELEASE()
{
	TEMP0_REN |= TEMP0_PIN;  // enable pull-up/pull-down resistor
	TEMP0_DIR &= ~TEMP0_PIN; // release bus. set port in input mode
	TEMP0_OUT |= TEMP0_PIN;  // set pull-up (1)
}
unsigned int Reset_TEMP0 ( void )
{
  	/* Steps to reset one wire bus
  	 * Pull bus low 
  	 * hold condition for 480us
  	 * release bus
  	 * wait for 60us
  	 * read bus
  	 * if bus low then device present set / return var accordingly
  	 * wait for balance period (480-60)
  	 */
  	int device_present=0;
    TEMP0_LO();         						// Drive bus low
    delay_us (480);                             // hold for 480us
    TEMP0_RELEASE();						//release bus. set port in input mode
    if(TEMP0_IN & TEMP0_PIN)
	{
		device_present=0;
	}
    delay_us (480);								//wait for 480us
  	return device_present;
}
void WriteZero_TEMP0(void)
{
	/*Steps for master to transmit logical zero to slave device on bus
	 * pull bus low
	 * hold for 60us
	 * release bus
	 * wait for 1us for recovery 
	 */ 
	
	TEMP0_LO();         						// Drive bus low
	delay_us (60);								//sample time slot for the slave
	TEMP0_RELEASE();						//release bus. set port in input mode
    delay_us (1);								//recovery time slot
	
	
}
void WriteOne_TEMP0(void)
{
	/*Steps for master to transmit logical one to slave device on bus
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 1us for recovery 
	 */ 
	TEMP0_LO();         						// Drive bus low
	delay_us (5);  
	TEMP0_RELEASE();						//release bus. set port in input mode
    delay_us (55);								//sample time slot for the slave
    delay_us (1);								//recovery time slot
    
}
void Write_TEMP0 (unsigned char data,int power )
{
   	unsigned char i;
	for(i=8;i>0;i--)
    {
    	
        if(data & 0x01)
        {
            WriteOne_TEMP0();
        }
        else
        {
        	WriteZero_TEMP0();
        }
          	
		data >>=1;
	
    }
}
unsigned int ReadBit_TEMP0 (void)
{
	
	/*Steps for master to issue a read request to slave device on bus aka milk slave device
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 45us for recovery 
	 */ 
	int bit=0;
	TEMP0_LO();         						// Drive bus low
	delay_us (5);  								//hold for 5us
	TEMP0_RELEASE();						//release bus. set port in input mode
    delay_us (10);								//wait for slave to drive port either high or low
    if(TEMP0_IN & TEMP0_PIN)			//read bus
	{
		bit=1;									//if read high set bit high
	}
    delay_us (45);								//recovery time slot
	return bit;
}
uint16_t Read_TEMP0 ( void )
{
		  
 	unsigned char i;
 	uint16_t data=0;
 	TEMP0_RELEASE();						//release bus. set port in input mode
 	
 	 for(i=16;i>0;i--)
 	{
		data>>=1;
		if(ReadBit_TEMP0())
		{
			data |=0x8000;
		}
		
		
	}
	
	return(data);
}

uint16_t GetData_TEMP0(void)
{
	uint16_t temp;
  	Reset_TEMP0();
    Write_TEMP0(DS1820_SKIP_ROM,0);
	Write_TEMP0(DS1820_CONVERT_T,1);
    delay_ms(750);
    Reset_TEMP0();
    Write_TEMP0(DS1820_SKIP_ROM,0);
    Write_TEMP0(DS1820_READ_SCRATCHPAD,0);
    temp = Read_TEMP0();

    return(temp);
	
}





// TEMP1 ================================================================================================================
void TEMP1_HI()
{
	TEMP1_DIR|=TEMP1_PIN; //set port as output
	TEMP1_OUT|=TEMP1_PIN;	//set port high
}
void TEMP1_LO()
{
	TEMP1_DIR|=TEMP1_PIN; //set port as output
	TEMP1_OUT&=~TEMP1_PIN;//set port low
}
void TEMP1_RELEASE()
{
	TEMP1_REN |= TEMP1_PIN;  // enable pull-up/pull-down resistor
	TEMP1_DIR &= ~TEMP1_PIN; // release bus. set port in input mode
	TEMP1_OUT |= TEMP1_PIN;  // set pull-up (1)
}
unsigned int Reset_TEMP1 ( void )
{
  	/* Steps to reset one wire bus
  	 * Pull bus low
  	 * hold condition for 480us
  	 * release bus
  	 * wait for 60us
  	 * read bus
  	 * if bus low then device present set / return var accordingly
  	 * wait for balance period (480-60)
  	 */
  	int device_present=0;
    TEMP1_LO();         						// Drive bus low
    delay_us (480);                             // hold for 480us
    TEMP1_RELEASE();						//release bus. set port in input mode
    if(TEMP1_IN & TEMP1_PIN)
	{
		device_present=0;
	}
    delay_us (480);								//wait for 480us
  	return device_present;
}
void WriteZero_TEMP1(void)
{
	/*Steps for master to transmit logical zero to slave device on bus
	 * pull bus low
	 * hold for 60us
	 * release bus
	 * wait for 1us for recovery
	 */

	TEMP1_LO();         						// Drive bus low
	delay_us (60);								//sample time slot for the slave
	TEMP1_RELEASE();						//release bus. set port in input mode
    delay_us (1);								//recovery time slot


}
void WriteOne_TEMP1(void)
{
	/*Steps for master to transmit logical one to slave device on bus
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 1us for recovery
	 */
	TEMP1_LO();         						// Drive bus low
	delay_us (5);
	TEMP1_RELEASE();						//release bus. set port in input mode
    delay_us (55);								//sample time slot for the slave
    delay_us (1);								//recovery time slot

}
void Write_TEMP1 (unsigned char data,int power )
{
   	unsigned char i;
	for(i=8;i>0;i--)
    {

        if(data & 0x01)
        {
            WriteOne_TEMP1();
        }
        else
        {
        	WriteZero_TEMP1();
        }

		data >>=1;

    }
}
unsigned int ReadBit_TEMP1 (void)
{

	/*Steps for master to issue a read request to slave device on bus aka milk slave device
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 45us for recovery
	 */
	int bit=0;
	TEMP1_LO();         						// Drive bus low
	delay_us (5);  								//hold for 5us
	TEMP1_RELEASE();						//release bus. set port in input mode
    delay_us (10);								//wait for slave to drive port either high or low
    if(TEMP1_IN & TEMP1_PIN)			//read bus
	{
		bit=1;									//if read high set bit high
	}
    delay_us (45);								//recovery time slot
	return bit;
}
uint16_t Read_TEMP1 ( void )
{

 	unsigned char i;
 	uint16_t data=0;
 	TEMP1_RELEASE();						//release bus. set port in input mode

 	 for(i=16;i>0;i--)
 	{
		data>>=1;
		if(ReadBit_TEMP1())
		{
			data |=0x8000;
		}


	}

	return(data);
}

uint16_t GetData_TEMP1(void)
{
	uint16_t temp;
  	Reset_TEMP1();
    Write_TEMP1(DS1820_SKIP_ROM,0);
	Write_TEMP1(DS1820_CONVERT_T,1);
    delay_ms(750);
    Reset_TEMP1();
    Write_TEMP1(DS1820_SKIP_ROM,0);
    Write_TEMP1(DS1820_READ_SCRATCHPAD,0);
    temp = Read_TEMP1();

    return(temp);

}






// TEMP2 ================================================================================================================

void TEMP2_HI()
{
	TEMP2_DIR|=TEMP2_PIN; //set port as output
	TEMP2_OUT|=TEMP2_PIN;	//set port high
}
void TEMP2_LO()
{
	TEMP2_DIR|=TEMP2_PIN; //set port as output
	TEMP2_OUT&=~TEMP2_PIN;//set port low
}
void TEMP2_RELEASE()
{
	TEMP2_REN |= TEMP2_PIN;  // enable pull-up/pull-down resistor
	TEMP2_DIR &= ~TEMP2_PIN; // release bus. set port in input mode
	TEMP2_OUT |= TEMP2_PIN;  // set pull-up (1)
}
unsigned int Reset_TEMP2 ( void )
{
  	/* Steps to reset one wire bus
  	 * Pull bus low
  	 * hold condition for 480us
  	 * release bus
  	 * wait for 60us
  	 * read bus
  	 * if bus low then device present set / return var accordingly
  	 * wait for balance period (480-60)
  	 */
  	int device_present=0;
    TEMP2_LO();         						// Drive bus low
    delay_us (480);                             // hold for 480us
    TEMP2_RELEASE();						//release bus. set port in input mode
    if(TEMP2_IN & TEMP2_PIN)
	{
		device_present=0;
	}
    delay_us (480);								//wait for 480us
  	return device_present;
}
void WriteZero_TEMP2(void)
{
	/*Steps for master to transmit logical zero to slave device on bus
	 * pull bus low
	 * hold for 60us
	 * release bus
	 * wait for 1us for recovery
	 */

	TEMP2_LO();         						// Drive bus low
	delay_us (60);								//sample time slot for the slave
	TEMP2_RELEASE();						//release bus. set port in input mode
    delay_us (1);								//recovery time slot


}
void WriteOne_TEMP2(void)
{
	/*Steps for master to transmit logical one to slave device on bus
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 1us for recovery
	 */
	TEMP2_LO();         						// Drive bus low
	delay_us (5);
	TEMP2_RELEASE();						//release bus. set port in input mode
    delay_us (55);								//sample time slot for the slave
    delay_us (1);								//recovery time slot

}
void Write_TEMP2 (unsigned char data,int power )
{
   	unsigned char i;
	for(i=8;i>0;i--)
    {

        if(data & 0x01)
        {
            WriteOne_TEMP2();
        }
        else
        {
        	WriteZero_TEMP2();
        }

		data >>=1;

    }
}
unsigned int ReadBit_TEMP2 (void)
{

	/*Steps for master to issue a read request to slave device on bus aka milk slave device
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 45us for recovery
	 */
	int bit=0;
	TEMP2_LO();         						// Drive bus low
	delay_us (5);  								//hold for 5us
	TEMP2_RELEASE();						//release bus. set port in input mode
    delay_us (10);								//wait for slave to drive port either high or low
    if(TEMP2_IN & TEMP2_PIN)			//read bus
	{
		bit=1;									//if read high set bit high
	}
    delay_us (45);								//recovery time slot
	return bit;
}
uint16_t Read_TEMP2 ( void )
{

 	unsigned char i;
 	uint16_t data=0;
 	TEMP2_RELEASE();						//release bus. set port in input mode

 	 for(i=16;i>0;i--)
 	{
		data>>=1;
		if(ReadBit_TEMP2())
		{
			data |=0x8000;
		}


	}

	return(data);
}

uint16_t GetData_TEMP2(void)
{
	uint16_t temp;
  	Reset_TEMP2();
    Write_TEMP2(DS1820_SKIP_ROM,0);
	Write_TEMP2(DS1820_CONVERT_T,1);
    delay_ms(750);
    Reset_TEMP2();
    Write_TEMP2(DS1820_SKIP_ROM,0);
    Write_TEMP2(DS1820_READ_SCRATCHPAD,0);
    temp = Read_TEMP2();

    return(temp);

}



// TEMP3 ================================================================================================================

void TEMP3_HI()
{
	TEMP3_DIR|=TEMP3_PIN; //set port as output
	TEMP3_OUT|=TEMP3_PIN;	//set port high
}
void TEMP3_LO()
{
	TEMP3_DIR|=TEMP3_PIN; //set port as output
	TEMP3_OUT&=~TEMP3_PIN;//set port low
}
void TEMP3_RELEASE()
{
	TEMP3_REN |= TEMP3_PIN;  // enable pull-up/pull-down resistor
	TEMP3_DIR &= ~TEMP3_PIN; // release bus. set port in input mode
	TEMP3_OUT |= TEMP3_PIN;  // set pull-up (1)
}
unsigned int Reset_TEMP3 ( void )
{
  	/* Steps to reset one wire bus
  	 * Pull bus low
  	 * hold condition for 480us
  	 * release bus
  	 * wait for 60us
  	 * read bus
  	 * if bus low then device present set / return var accordingly
  	 * wait for balance period (480-60)
  	 */
  	int device_present=0;
    TEMP3_LO();         						// Drive bus low
    delay_us (480);                             // hold for 480us
    TEMP3_RELEASE();						//release bus. set port in input mode
    if(TEMP3_IN & TEMP3_PIN)
	{
		device_present=0;
	}
    delay_us (480);								//wait for 480us
  	return device_present;
}
void WriteZero_TEMP3(void)
{
	/*Steps for master to transmit logical zero to slave device on bus
	 * pull bus low
	 * hold for 60us
	 * release bus
	 * wait for 1us for recovery
	 */

	TEMP3_LO();         						// Drive bus low
	delay_us (60);								//sample time slot for the slave
	TEMP3_RELEASE();						//release bus. set port in input mode
    delay_us (1);								//recovery time slot


}
void WriteOne_TEMP3(void)
{
	/*Steps for master to transmit logical one to slave device on bus
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 1us for recovery
	 */
	TEMP3_LO();         						// Drive bus low
	delay_us (5);
	TEMP3_RELEASE();						//release bus. set port in input mode
    delay_us (55);								//sample time slot for the slave
    delay_us (1);								//recovery time slot

}
void Write_TEMP3 (unsigned char data,int power )
{
   	unsigned char i;
	for(i=8;i>0;i--)
    {

        if(data & 0x01)
        {
            WriteOne_TEMP3();
        }
        else
        {
        	WriteZero_TEMP3();
        }

		data >>=1;

    }
}
unsigned int ReadBit_TEMP3 (void)
{

	/*Steps for master to issue a read request to slave device on bus aka milk slave device
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 45us for recovery
	 */
	int bit=0;
	TEMP3_LO();         						// Drive bus low
	delay_us (5);  								//hold for 5us
	TEMP3_RELEASE();						//release bus. set port in input mode
    delay_us (10);								//wait for slave to drive port either high or low
    if(TEMP3_IN & TEMP3_PIN)			//read bus
	{
		bit=1;									//if read high set bit high
	}
    delay_us (45);								//recovery time slot
	return bit;
}
uint16_t Read_TEMP3 ( void )
{

 	unsigned char i;
 	uint16_t data=0;
 	TEMP3_RELEASE();						//release bus. set port in input mode

 	 for(i=16;i>0;i--)
 	{
		data>>=1;
		if(ReadBit_TEMP3())
		{
			data |=0x8000;
		}


	}

	return(data);
}

uint16_t GetData_TEMP3(void)
{
	uint16_t temp;
  	Reset_TEMP3();
    Write_TEMP3(DS1820_SKIP_ROM,0);
	Write_TEMP3(DS1820_CONVERT_T,1);
    delay_ms(750);
    Reset_TEMP3();
    Write_TEMP3(DS1820_SKIP_ROM,0);
    Write_TEMP3(DS1820_READ_SCRATCHPAD,0);
    temp = Read_TEMP3();

    return(temp);

}


// TEMP4 ================================================================================================================

void TEMP4_HI()
{
	TEMP4_DIR|=TEMP4_PIN; //set port as output
	TEMP4_OUT|=TEMP4_PIN;	//set port high
}
void TEMP4_LO()
{
	TEMP4_DIR|=TEMP4_PIN; //set port as output
	TEMP4_OUT&=~TEMP4_PIN;//set port low
}
void TEMP4_RELEASE()
{
	TEMP4_REN |= TEMP4_PIN;  // enable pull-up/pull-down resistor
	TEMP4_DIR &= ~TEMP4_PIN; // release bus. set port in input mode
	TEMP4_OUT |= TEMP4_PIN;  // set pull-up (1)
}
unsigned int Reset_TEMP4 ( void )
{
  	/* Steps to reset one wire bus
  	 * Pull bus low
  	 * hold condition for 480us
  	 * release bus
  	 * wait for 60us
  	 * read bus
  	 * if bus low then device present set / return var accordingly
  	 * wait for balance period (480-60)
  	 */
  	int device_present=0;
    TEMP4_LO();         						// Drive bus low
    delay_us (480);                             // hold for 480us
    TEMP4_RELEASE();						//release bus. set port in input mode
    if(TEMP4_IN & TEMP4_PIN)
	{
		device_present=0;
	}
    delay_us (480);								//wait for 480us
  	return device_present;
}
void WriteZero_TEMP4(void)
{
	/*Steps for master to transmit logical zero to slave device on bus
	 * pull bus low
	 * hold for 60us
	 * release bus
	 * wait for 1us for recovery
	 */

	TEMP4_LO();         						// Drive bus low
	delay_us (60);								//sample time slot for the slave
	TEMP4_RELEASE();						//release bus. set port in input mode
    delay_us (1);								//recovery time slot


}
void WriteOne_TEMP4(void)
{
	/*Steps for master to transmit logical one to slave device on bus
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 1us for recovery
	 */
	TEMP4_LO();         						// Drive bus low
	delay_us (5);
	TEMP4_RELEASE();						//release bus. set port in input mode
    delay_us (55);								//sample time slot for the slave
    delay_us (1);								//recovery time slot

}
void Write_TEMP4 (unsigned char data,int power )
{
   	unsigned char i;
	for(i=8;i>0;i--)
    {

        if(data & 0x01)
        {
            WriteOne_TEMP4();
        }
        else
        {
        	WriteZero_TEMP4();
        }

		data >>=1;

    }
}
unsigned int ReadBit_TEMP4 (void)
{

	/*Steps for master to issue a read request to slave device on bus aka milk slave device
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 45us for recovery
	 */
	int bit=0;
	TEMP4_LO();         						// Drive bus low
	delay_us (5);  								//hold for 5us
	TEMP4_RELEASE();						//release bus. set port in input mode
    delay_us (10);								//wait for slave to drive port either high or low
    if(TEMP4_IN & TEMP4_PIN)			//read bus
	{
		bit=1;									//if read high set bit high
	}
    delay_us (45);								//recovery time slot
	return bit;
}
uint16_t Read_TEMP4 ( void )
{

 	unsigned char i;
 	uint16_t data=0;
 	TEMP4_RELEASE();						//release bus. set port in input mode

 	 for(i=16;i>0;i--)
 	{
		data>>=1;
		if(ReadBit_TEMP4())
		{
			data |=0x8000;
		}


	}

	return(data);
}

uint16_t GetData_TEMP4(void)
{
	uint16_t temp;
  	Reset_TEMP4();
    Write_TEMP4(DS1820_SKIP_ROM,0);
	Write_TEMP4(DS1820_CONVERT_T,1);
    delay_ms(750);
    Reset_TEMP4();
    Write_TEMP4(DS1820_SKIP_ROM,0);
    Write_TEMP4(DS1820_READ_SCRATCHPAD,0);
    temp = Read_TEMP4();

    return(temp);

}


// TEMP5 ================================================================================================================

void TEMP5_HI()
{
	TEMP5_DIR|=TEMP5_PIN; //set port as output
	TEMP5_OUT|=TEMP5_PIN;	//set port high
}
void TEMP5_LO()
{
	TEMP5_DIR|=TEMP5_PIN; //set port as output
	TEMP5_OUT&=~TEMP5_PIN;//set port low
}
void TEMP5_RELEASE()
{
	TEMP5_REN |= TEMP5_PIN;  // enable pull-up/pull-down resistor
	TEMP5_DIR &= ~TEMP5_PIN; // release bus. set port in input mode
	TEMP5_OUT |= TEMP5_PIN;  // set pull-up (1)
}
unsigned int Reset_TEMP5 ( void )
{
  	/* Steps to reset one wire bus
  	 * Pull bus low
  	 * hold condition for 480us
  	 * release bus
  	 * wait for 60us
  	 * read bus
  	 * if bus low then device present set / return var accordingly
  	 * wait for balance period (480-60)
  	 */
  	int device_present=0;
    TEMP5_LO();         						// Drive bus low
    delay_us (480);                             // hold for 480us
    TEMP5_RELEASE();						//release bus. set port in input mode
    if(TEMP5_IN & TEMP5_PIN)
	{
		device_present=0;
	}
    delay_us (480);								//wait for 480us
  	return device_present;
}
void WriteZero_TEMP5(void)
{
	/*Steps for master to transmit logical zero to slave device on bus
	 * pull bus low
	 * hold for 60us
	 * release bus
	 * wait for 1us for recovery
	 */

	TEMP5_LO();         						// Drive bus low
	delay_us (60);								//sample time slot for the slave
	TEMP5_RELEASE();						//release bus. set port in input mode
    delay_us (1);								//recovery time slot


}
void WriteOne_TEMP5(void)
{
	/*Steps for master to transmit logical one to slave device on bus
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 1us for recovery
	 */
	TEMP5_LO();         						// Drive bus low
	delay_us (5);
	TEMP5_RELEASE();						//release bus. set port in input mode
    delay_us (55);								//sample time slot for the slave
    delay_us (1);								//recovery time slot

}
void Write_TEMP5 (unsigned char data,int power )
{
   	unsigned char i;
	for(i=8;i>0;i--)
    {

        if(data & 0x01)
        {
            WriteOne_TEMP5();
        }
        else
        {
        	WriteZero_TEMP5();
        }

		data >>=1;

    }
}
unsigned int ReadBit_TEMP5 (void)
{

	/*Steps for master to issue a read request to slave device on bus aka milk slave device
	 * pull bus low
	 * hold for 5us
	 * release bus
	 * wait for 45us for recovery
	 */
	int bit=0;
	TEMP5_LO();         						// Drive bus low
	delay_us (5);  								//hold for 5us
	TEMP5_RELEASE();						//release bus. set port in input mode
    delay_us (10);								//wait for slave to drive port either high or low
    if(TEMP5_IN & TEMP5_PIN)			//read bus
	{
		bit=1;									//if read high set bit high
	}
    delay_us (45);								//recovery time slot
	return bit;
}
uint16_t Read_TEMP5 ( void )
{

 	unsigned char i;
 	uint16_t data=0;
 	TEMP5_RELEASE();						//release bus. set port in input mode

 	 for(i=16;i>0;i--)
 	{
		data>>=1;
		if(ReadBit_TEMP5())
		{
			data |=0x8000;
		}


	}

	return(data);
}

uint16_t GetData_TEMP5(void)
{
	uint16_t temp;
  	Reset_TEMP5();
    Write_TEMP5(DS1820_SKIP_ROM,0);
	Write_TEMP5(DS1820_CONVERT_T,1);
    delay_ms(750);
    Reset_TEMP5();
    Write_TEMP5(DS1820_SKIP_ROM,0);
    Write_TEMP5(DS1820_READ_SCRATCHPAD,0);
    temp = Read_TEMP5();

    return(temp);

}

