/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include "stm8s.h"
#include "stm8s_it.h"

/**
	* PINOUTS
	* 1. Servo -> PC1 
	* 2. Bluetooth -> PD6
	* 3. Left motor -> PB4
	* 4. Right motor -> PB5
*/

//Servo positions (in cycles)
enum ServoPos
{
	EXTREME_POS1 = 1000, //1ms pulse
	MID_POS = 1500, //1.5ms pulse
	EXTREME_POS2 = 2000 //2ms pulse
};

//Bluetooth function(s)
static void Bluetooth_Init(void)
{
	//Initialize UART2 Rx [PD6]
	UART2_Init(9600,UART2_WORDLENGTH_8D,
						 UART2_STOPBITS_1,UART2_PARITY_NO,
						 UART2_SYNCMODE_CLOCK_DISABLE,
						 UART2_MODE_RX_ENABLE);
	UART2_Cmd(ENABLE);
}

static char Bluetooth_Receive(void)
{
	char rxData = '\0';
	if(UART2_GetFlagStatus(UART2_FLAG_RXNE) == SET)
	{
		rxData = (char)UART2_ReceiveData8();
	}
	return rxData;
}

//Function(s) for DC motors
static void Motors_Init(void)
{
	//Left and Right motor Init 
	GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_SLOW);
}

static void Motors_Move(void)
{
	//Drive both left and right motors
	GPIO_WriteHigh(GPIOB,GPIO_PIN_4);
	GPIO_WriteHigh(GPIOB,GPIO_PIN_5);
}

static void Motors_Stop(void)
{
	GPIO_WriteLow(GPIOC,GPIO_PIN_3);
	GPIO_WriteLow(GPIOC,GPIO_PIN_4);	
}

//Servo motor function(s)
static void Servo_Init(void)
{
	//Timer 1 Init for PWM generation [50Hz frequency]
	//Prescaler = 8, period = 20000 cycles
	const uint16_t prescaler = 8-1;
	const uint16_t period = 20000-1;
	TIM1_TimeBaseInit(prescaler,TIM1_COUNTERMODE_UP,period,0);
	//Output compare enable for PWM pin connected to servo
	TIM1_OC1Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE,
							 TIM1_OUTPUTNSTATE_ENABLE,MID_POS,
						   TIM1_OCPOLARITY_LOW,TIM1_OCNPOLARITY_HIGH,
						   TIM1_OCIDLESTATE_SET,
						   TIM1_OCNIDLESTATE_RESET);
	//TIM1 counter enable
	TIM1_Cmd(ENABLE);
	//TIM1 Main Output Enable
	TIM1_CtrlPWMOutputs(ENABLE);					 
}

static void Servo_SetPosition(uint16_t pos)
{
	TIM1_SetCompare1(pos);
}

static void BackgroundTimer_Init(void)
{
	//Periodic timer with 1ms timebase
	//Prescaler = 64, Period = 124
	const uint16_t period = 125-1;
	TIM2_TimeBaseInit(TIM2_PRESCALER_64,period);
	TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);
	TIM2_Cmd(ENABLE);	
}

int main(void)
{
	//Variables
	const uint16_t servoPos[] = {EXTREME_POS1,EXTREME_POS2};
	uint8_t servoPosIndex = 0;
	uint8_t sweepShaft = 0;
	char appData = '\0';
	uint32_t currentMillis = 0;
	//Initializations
	disableInterrupts();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2); //8MHz
	Bluetooth_Init();
	Motors_Init();
	Servo_Init();
	BackgroundTimer_Init();
	enableInterrupts();
	
	while (1)
	{
		//Process data received from bluetooth app
		appData = Bluetooth_Receive();
		switch(appData)
		{
			case '0':
				Motors_Move();
				sweepShaft = 1;
				currentMillis = Timer_GetMillis();
				break;
			case '1':
				Motors_Stop();
				sweepShaft = 0;
				break;
		}
		//Sweep servo shaft between extremes every 2 seconds
		if(sweepShaft)
		{
			if(((Timer_GetMillis() - currentMillis) > 2000))
			{
				Servo_SetPosition(servoPos[servoPosIndex]);
				servoPosIndex ^= 1;
				currentMillis = Timer_GetMillis();
			}
		}
		else
		{
			Servo_SetPosition(MID_POS);
		}
	}
}