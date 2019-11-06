#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/timer.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"

	

void Initiallization(void)
{
	uint32_t ui32Period;  //used for setting up timer

	/***********************
	* Bus Clock            *
	***********************/
	//Set system clock to 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	
	
	/***********************
	* PortF                *
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

	
	
	/***********************
	* I2C0                 *
	***********************/
	//Turn on clock to GPIOB and I2C0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0))
	{}
		
	//Configure GPIOB pins to be used by I2C0
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
		
	//Set the Launcpad as the I2C master with the slow clock 100 Kbps	
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
		
	//Set the address to the MCP4725 DAC. First byte sent for each traingle wave value.
	//*Set this value correctly***************************************/
	I2CMasterSlaveAddrSet(I2C0_BASE, 0x60, false);  //This value is the top 7 MSB's	
		
		
	/***********************
	* TIMER0               *
	***********************/
	//TIMER0 Initialization code
		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  //Enable clock for TIMER0
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);  //Set Timer0 to periodic mode
	
	ui32Period = (SysCtlClockGet()/3200);
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	
	TimerEnable(TIMER0_BASE, TIMER_A);

	
}
