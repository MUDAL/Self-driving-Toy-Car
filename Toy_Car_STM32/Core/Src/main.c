/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum ServoPos
{
  EXTREME_POS1 = 800, //1ms pulse
  MID_POS = 1200, //1.5ms pulse
  EXTREME_POS2 = 1600 //2ms pulse
};
enum ServoDir
{
  LEFT = 0,
  MIDDLE,
  RIGHT
};
enum AppCmd
{
  START = '0',
  STOP = '1',
  REVERSE = '2'
};
enum MotorSpeed
{
  NO_SPEED = 0,
  SPEED_35PERCENT = 5600,
  SPEED_40PERCENT = 6400,
  SPEED_50PERCENT = 8000,
  SPEED_60PERCENT = 9600,
  SPEED_75PERCENT = 12000
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;
osThreadId Task_MainHandle;
osThreadId Task_BlinkHandle;
osThreadId Task_MotionHandle;
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
void StartDefaultTask(void const * argument);
void StartTaskMain(void const * argument);
void StartTaskBlink(void const * argument);
void StartTaskMotion(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char Bluetooth_Receive(void)
{
  char btData = '\0';
  if((huart1.Instance->SR & USART_SR_RXNE) == USART_SR_RXNE)
  {
     btData = huart1.Instance->DR;
  }
  return btData;
}

void Servo_SetPosition(uint16_t pos)
{
  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pos);
}

void Move_Forward(void)
{
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,SPEED_60PERCENT);
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,SPEED_60PERCENT);
  HAL_GPIO_WritePin(leftMotor_GPIO_Port,leftMotor_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(rightMotor_GPIO_Port,rightMotor_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(leftMotor2_GPIO_Port,leftMotor2_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(rightMotor2_GPIO_Port,rightMotor2_Pin,GPIO_PIN_RESET);
}

void Reverse(void)
{
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,SPEED_60PERCENT);
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,SPEED_60PERCENT);
  HAL_GPIO_WritePin(leftMotor_GPIO_Port,leftMotor_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(rightMotor_GPIO_Port,rightMotor_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(leftMotor2_GPIO_Port,leftMotor2_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(rightMotor2_GPIO_Port,rightMotor2_Pin,GPIO_PIN_SET);
}

void Turn_Right(void)
{
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,SPEED_60PERCENT);
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,SPEED_40PERCENT);
  HAL_GPIO_WritePin(leftMotor_GPIO_Port,leftMotor_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(rightMotor_GPIO_Port,rightMotor_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(leftMotor2_GPIO_Port,leftMotor2_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(rightMotor2_GPIO_Port,rightMotor2_Pin,GPIO_PIN_RESET);
}

void Turn_Left(void)
{
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,SPEED_40PERCENT);
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,SPEED_60PERCENT);
  HAL_GPIO_WritePin(leftMotor_GPIO_Port,leftMotor_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(rightMotor_GPIO_Port,rightMotor_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(leftMotor2_GPIO_Port,leftMotor2_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(rightMotor2_GPIO_Port,rightMotor2_Pin,GPIO_PIN_RESET);
}

void Stop(void)
{
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,NO_SPEED);
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,NO_SPEED);
  HAL_GPIO_WritePin(leftMotor_GPIO_Port,leftMotor_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(rightMotor_GPIO_Port,rightMotor_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(leftMotor2_GPIO_Port,leftMotor2_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(rightMotor2_GPIO_Port,rightMotor2_Pin,GPIO_PIN_RESET);
}

uint32_t Sensor_GetDistance(void)
{
  const uint32_t sysClockFreq = 8000000;
  uint32_t distanceCM;
  if((htim4.Instance->SR & TIM_SR_CC2IF) == TIM_SR_CC2IF)
  {
    uint32_t pulseWidth = htim4.Instance->CCR2;
    distanceCM = (float)pulseWidth * htim4.Init.Prescaler * 1000000 / (58 * sysClockFreq);
  }
  return distanceCM;
}

uint8_t IndexWithLargestElement(uint32_t* distArr)
{
  uint8_t largest = distArr[0];
  uint8_t largestIndex = 0;
  for(uint8_t i = 1; i < 3; i++)
  {
    if(largest < distArr[i])
    {
      largestIndex = i;
    }
  }
  return largestIndex;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Task_Main */
  osThreadDef(Task_Main, StartTaskMain, osPriorityNormal, 0, 128);
  Task_MainHandle = osThreadCreate(osThread(Task_Main), NULL);

  /* definition and creation of Task_Blink */
  osThreadDef(Task_Blink, StartTaskBlink, osPriorityNormal, 0, 128);
  Task_BlinkHandle = osThreadCreate(osThread(Task_Blink), NULL);

  /* definition and creation of Task_Motion */
  osThreadDef(Task_Motion, StartTaskMotion, osPriorityNormal, 0, 128);
  Task_MotionHandle = osThreadCreate(osThread(Task_Motion), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadSuspend(Task_MotionHandle);
  osThreadSuspend(Task_BlinkHandle);
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 10-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 16000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = MID_POS;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */
  //Channel 1 pulse = 12 (15uS pulse width for sensor)
  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 10-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 16000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 12;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = NO_SPEED;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  //PWM init for Trig pin
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  //PWM init for left and right motors
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 200-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 8000-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim4, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */
  //PWM input capture init for Echo pin
  HAL_TIM_IC_Start(&htim4,TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim4,TIM_CHANNEL_2);
  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, leftMotor2_Pin|rightMotor2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_Pin|leftMotor_Pin|rightMotor_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : leftMotor2_Pin rightMotor2_Pin */
  GPIO_InitStruct.Pin = leftMotor2_Pin|rightMotor2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Pin leftMotor_Pin rightMotor_Pin */
  GPIO_InitStruct.Pin = LED_Pin|leftMotor_Pin|rightMotor_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  while(1)
  {
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTaskMain */
/**
* @brief Function implementing the Task_Main thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskMain */
void StartTaskMain(void const * argument)
{
  /* USER CODE BEGIN StartTaskMain */
  /* Infinite loop */
  char appData = '\0';
  while(1)
  {
    appData = Bluetooth_Receive();
    switch(appData)
    {
      case START:
	osThreadResume(Task_MotionHandle);
	osThreadResume(Task_BlinkHandle);
	Move_Forward();
	break;
      case STOP:
	osThreadSuspend(Task_MotionHandle);
	osThreadSuspend(Task_BlinkHandle);
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
	Stop();
	break;
      case REVERSE:
	osThreadSuspend(Task_MotionHandle);
	Reverse();
	break;
    }
  }
  /* USER CODE END StartTaskMain */
}

/* USER CODE BEGIN Header_StartTaskBlink */
/**
* @brief Function implementing the Task_Blink thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskBlink */
void StartTaskBlink(void const * argument)
{
  /* USER CODE BEGIN StartTaskBlink */
  /* Infinite loop */
  while(1)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartTaskBlink */
}

/* USER CODE BEGIN Header_StartTaskMotion */
/**
* @brief Function implementing the Task_Motion thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskMotion */
void StartTaskMotion(void const * argument)
{
  /* USER CODE BEGIN StartTaskMotion */
  /* Infinite loop */
  uint32_t distances[] = {0,0,0}; //left,mid,right
  const uint16_t servoPos[] = {EXTREME_POS2,MID_POS,EXTREME_POS1};
  while(1)
  {
    //Handle obstacle avoidance
    //turn servo through all positions and read the distances
    for(uint8_t i = 0; i < 3; i++)
    {
      Servo_SetPosition(servoPos[i]);
      osDelay(500);
      distances[i] = Sensor_GetDistance();
    }
    Servo_SetPosition(MID_POS);
    //follow the direction of sensor with largest distance
    uint8_t index = IndexWithLargestElement(distances);
    switch(index)
    {
      case LEFT:
	Turn_Left();
	break;
      case MIDDLE:
	Move_Forward();
	break;
      case RIGHT:
	Turn_Right();
	break;
    }
  }
  /* USER CODE END StartTaskMotion */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
