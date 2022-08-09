#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_pwm.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/uart.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

//System clock : 16MHz

//Pinouts
//1. PE4 --> Bluetooth
//2. PE1 --> Left motor
//3. PE2 --> Right motor
//4. PB6 --> Servo motor

//Duty cycles
enum ServoPos
{
	EXTREME_POS1 = 2000, //1ms pulse
	MID_POS = 3000, //1.5ms pulse
	EXTREME_POS2 = 4000 //2ms pulse
};

static uint32_t GetTick(void)
{
	return MAP_TimerValueGet(TIMER0_BASE,TIMER_A);	
}

static void Sys_Init(void)
{
	//Clock configuration
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | 
								     SYSCTL_XTAL_16MHZ);
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_8);
	//Peripheral configurations
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	//Wait for modules to be ready
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_UART5)){}	
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)){}
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}	
  //Timer configuration
	MAP_TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC_UP);
	MAP_TimerLoadSet(TIMER0_BASE,TIMER_A,16000000); //16x10^6 cycles = 1 sec
	MAP_TimerEnable(TIMER0_BASE,TIMER_BOTH);
}

static void Bluetooth_Init(void)
{
	//UART Rx configuration
	MAP_GPIOPinConfigure(GPIO_PE4_U5RX);	
	MAP_GPIOPinTypeUART(GPIO_PORTE_BASE,GPIO_PIN_4);
	MAP_UARTConfigSetExpClk(UART5_BASE,16000000,9600,
										     (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
											    UART_CONFIG_PAR_NONE));	
}

static char Bluetooth_Receive(void)
{
	char appData = '\0';
	if(MAP_UARTCharsAvail(UART5_BASE))
	{
		appData = (char)MAP_UARTCharGetNonBlocking(UART5_BASE);
	}
	return appData;
}

static void Motors_Init(void)
{
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2);	
	MAP_GPIOPinWrite(GPIO_PORTE_BASE,(GPIO_PIN_1|GPIO_PIN_2),0);	
}

static void Motors_Move(void)
{
	MAP_GPIOPinWrite(GPIO_PORTE_BASE,
							    (GPIO_PIN_1|GPIO_PIN_2),
							    (GPIO_PIN_1|GPIO_PIN_2));
}

static void Motors_Stop(void)
{
	MAP_GPIOPinWrite(GPIO_PORTE_BASE,(GPIO_PIN_1|GPIO_PIN_2),0);	
}

static void Servo_Init(void)
{	//PWM pin configuration (PB6)
	MAP_GPIOPinConfigure(GPIO_PB6_M0PWM0);
	MAP_GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_6);
	//PWM signal configuration
	MAP_PWMGenConfigure(PWM0_BASE,PWM_GEN_0,
								      PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	MAP_PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,40000); //Period of PWM signal
	MAP_PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,MID_POS);
	MAP_PWMGenEnable(PWM0_BASE,PWM_GEN_0); //Start timers in generator 0
	MAP_PWMOutputState(PWM0_BASE,PWM_OUT_0_BIT,true); //Enable PWM output
}

static void Servo_SetPosition(uint16_t pos)
{
	MAP_PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,pos);
}

int main(void)
{
	//Variables
	const uint16_t servoPos[] = {EXTREME_POS1,EXTREME_POS2};
	uint8_t servoPosIndex = 0;
	uint8_t sweepShaft = 0;
	char appData = '\0';
	uint32_t currentTick = 0;
	//Initializations
	Sys_Init();
	Bluetooth_Init();
	Motors_Init();
	Servo_Init();
	while(1)
	{
		//Process data received from bluetooth app
		appData = Bluetooth_Receive();
		switch(appData)
		{
			case '0':
				Motors_Move();
				sweepShaft = 1;
				currentTick = GetTick();
				break;
			case '1':
				Motors_Stop();
				sweepShaft = 0;
				break;
		}
		//Sweep servo shaft between extremes every 2 seconds
		if(sweepShaft)
		{
			const uint32_t sweepTime = 32000000; //2 timer periods = 2 secs
			uint32_t tickDiff = GetTick() - currentTick;
			if(tickDiff >= sweepTime)
			{
				Servo_SetPosition(servoPos[servoPosIndex]);
				servoPosIndex ^= 1;
				currentTick = GetTick();
			}
		}
		else
		{
			Servo_SetPosition(MID_POS);
		}		
	}
}
