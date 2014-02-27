#include "ishan.h"

void delay_ms(int ms)
{
	while (ms--)
	{
		__delay_cycles(8000); // set for 16Mhz change it to 1000 for 1 Mhz
	}	
	
}


void delay_us(int us)
{
	while (us--)
	{
		__delay_cycles(6); // set for 16Mhz change it to 1000 for 1 Mhz
	}	
	
}
