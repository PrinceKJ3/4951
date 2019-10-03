#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"






//*****************************************************************************
int main(void)
{
  uint32_t ui32Loop;
	bool sw1_pressed, sw1_not_pressed;
	
	/***********************
	* Setup PORTF for GPIO *
	***********************/
	//Enable PORTF
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Enable clock on GPIO PORTF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
	{}
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //Sets pins 1,2,3 as outputs
	
	//Configure PORTF switches
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;  //unlock PF0
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	sw1_not_pressed = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
	sw1_pressed = !sw1_not_pressed;
	int state = 0;
	while(1)
	{

		
	
		

		// Use loop to generate a delay
		//for(ui32Loop = 0; ui32Loop < 20000; ui32Loop++)
		//{
		//}
		


		switch (state)
		{
		case 0: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
			if (sw1_pressed == 1)
		{
			state = 1;
		}
				break;
		case 1: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
			wait(200);
			while (sw1_pressed == 1)
			{
			}
			
			if (sw1_pressed == 1)
				state++;
			break;
		case 2: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_3);
			wait(200);
			while (sw1_pressed == 1)
			{
			}
				state++;
			break;
		case 3:if (sw1_pressed == 1) {
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_2);
			wait(200);
			while (sw1_pressed == 1)
			{
			}
		}
			state++;
			
				
			break;
		case 4: if (sw1_pressed == 1) {
			GPIOPinWrite(GPIO_PORTF_BASE, 0x7, 0x7);
			wait(200);
			while (sw1_pressed == 1)
			{
			}
			wait(100);

			while (sw1_not_pressed == 1)
			{
			}
			state = 0;
		}
			break;
		}
				




		}
	}
}
