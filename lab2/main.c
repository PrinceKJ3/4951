#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"

#define PF2  (*((volatile uint32_t *)0x40025010))

void Initiallization(void);
uint16_t CalcSineWave(void);
uint16_t CalcRecSineWave(void);
uint16_t FuncGenOutput;


int main(void)
{
    Initiallization();
	  
    while(1)
    {
    }
}

void Timer0A_Handler(void)
{
	
	bool sw1_pressed, sw1_not_pressed;
	
	uint8_t temp;	
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	
	//Toggle PF2 for profiling
	PF2 ^= 0x04;

	//Replace with your function generator code
	

		sw1_not_pressed = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
		sw1_pressed = !sw1_not_pressed;
		if (sw1_not_pressed)
		{
			FuncGenOutput = CalcSineWave();
			
		}
		else if (sw1_pressed)
		{
			FuncGenOutput = CalcRecSineWave();
		}
			

	/***********************
	* I2C0 Output to DAC   *
	***********************/
	temp = ( FuncGenOutput & (0x0F00) ) >> 8;   //4 MSB's of the sine/sawtooth wave value
	//remember to set the top 4 control MSB's correctly
	I2CMasterDataPut(I2C0_BASE, temp); //second byte sent from the I2C module. Top 4 MSB's and control bits
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); //Outputs first byte(address) and second byte
	while(I2CMasterBusy(I2C0_BASE))  //Wait for I2CO master to finish transmission
	{}
	
	//set temp to correct value
	temp = FuncGenOutput & (0x00FF);   //8 LSB's sine/sawtooth wave value		
	I2CMasterDataPut(I2C0_BASE, temp);  //third data byte sent 
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);   //Outputs bottom 8 MSB's
	while(I2CMasterBusy(I2C0_BASE))  //Wait for I2CO master to finish transmission
	{}

	}

//Sine table
const uint16_t SineWaveTable[32] = {
	2048, 2447, 2831, 3185, 3496, 3750, 3940, 4056,
	4095, 4056, 3940, 3750, 3496, 3185, 2831, 2447,
	2048, 1648, 1264, 910, 599, 345, 155, 39,
	0, 39, 155, 345, 599, 910, 1264, 1648};

uint16_t CalcSineWave(void)
{
	static uint32_t loopCnt=0;
	uint16_t SineOut;
	SineOut = SineWaveTable[loopCnt];
	if (loopCnt == 31)
		loopCnt = 0;
	else
		loopCnt++;
	return(SineOut);
}

//Your rectified sine wave code

const uint16_t RecSineTable[32] = {
	2048, 2447, 2831, 3185, 3496, 3750, 3940, 4056,
	4095, 4056, 3940, 3750, 3496, 3185, 2831, 2447,
	2048, 2447, 2831, 3185, 3496, 3750, 3940, 4056,
	4095, 4056, 3940, 3750, 3496, 3185, 2831, 2447};  //Put your rectified sine wave values in table

uint16_t CalcRecSineWave(void)
{
	static uint8_t loopCnt = 0;
	uint16_t RecSineOut;
	RecSineOut = RecSineTable[loopCnt];
	if (loopCnt == 31)
		loopCnt = 0;
	else
		loopCnt++;
	return RecSineOut;

}



