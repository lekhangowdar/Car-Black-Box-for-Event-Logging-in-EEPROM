#include <xc.h>
#include "main.h"

extern unsigned char sec;
void __interrupt() isr(void)
{
	static unsigned short count;

	if (TMR0IF)
	{
		TMR0 = TMR0 + 8;

		if (count++ == 20000)
		{
			count = 0;
            if(sec>0)
			sec--;
		}
	}
    		TMR0IF = 0;

}