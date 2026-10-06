/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "../Inc/mp6602.h"
#include "../Inc/circbuff.h"
#include <stdbool.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
static const uint8_t test_str[] = "Hello World!!\r\n";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void calc_pos_counts(void);
void actuator_home(void);
void actuator_move_in(void);
void actuator_move_out(void);
void actuator_move_pos(uint8_t pos);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t gl_pulse_cnt = 0;
// With no microstepping, 10310 counts is 32 mm
//uint32_t gl_pulseperposition = 39951;
uint32_t gl_pulseperposition = 79902;
uint8_t gl_filter_position;
uint32_t gl_actuator_position;
uint32_t gl_actuator_offset = 5154;
uint32_t gl_positiondelta = 0;
uint32_t gl_pos_counts[4];
bool gl_inmotion = false;
bool gl_homed = false;
bool gl_limit = false;

CB_t *UART_Rx = NULL;
CB_t *UART_Tx = NULL;
#define USB_BUFF_SIZE	128

uint8_t rxByte;

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
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  // Toggle the Reset to clear any faults
  uint32_t ii;
  HAL_GPIO_WritePin(GPIOB, MP6602_nRST_Pin, RESET);
  for(ii = 0; ii<0xFF; ii++){}
  HAL_GPIO_WritePin(GPIOB, MP6602_nRST_Pin, SET);

  //Enable the USB/UART Bridge and the Motor Driver
  HAL_GPIO_WritePin(GPIOA, CP_nRST_Pin, SET);
  HAL_GPIO_WritePin(GPIOB, MP6602_ENBL_Pin, SET);
  //HAL_GPIO_WritePin(GPIOB, MP6602_ENBL_Pin, RESET);


  mp6602_init();

  CB_init(&UART_Rx, USB_BUFF_SIZE);
  CB_init(&UART_Tx, USB_BUFF_SIZE);

  // SPI NOTES
    // The HAL seems to transmit and receive bytes on SPI
    // in reverse order. If trying to transmit 0xAA 0xBB,
    // it will grab 0xBB and put it on the bus first, then
    // 0xAA. Similar with the receiver buffer.
    // That's why none of the settings seemed to work.
    //
/*
    uint8_t txData[2] = {0x00, 0x00};
    uint8_t rxData[2] = {0x00, 0x00};
    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
    HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, 100);
    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

    txData[1] = 0x20;
    txData[0] = 0x00;
    rxData[0] = 0x00;
    rxData[1] = 0x00;
    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
    HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, 100);
    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

    txData[1] = 0x40;
    txData[0] = 0x00;
    rxData[0] = 0x00;
    rxData[1] = 0x00;
    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
    HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, 100);
    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

    txData[1] = 0x60;
    txData[0] = 0x00;
    rxData[0] = 0x00;
    rxData[1] = 0x00;
    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
    HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, 100);
    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);
*/

  	// Set motor direction
  	HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);

  	  // Set duty cycle for PWM, 50%
  	  // Half of ARR
  	  // OG NEMA 17 motor settings
  	  // Counter Period: 5000
  	  // CCR3: 2500
  	  TIM3->CCR3 = 1200;
  	  calc_pos_counts();

  	  GPIO_PinState limit_state;
  	  GPIO_PinState POS4_State;
  	  limit_state = HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin);

  	  //actuator_home();
  	  HAL_StatusTypeDef status;

  	  // Reset CP2102 chip
  	//HAL_GPIO_WritePin(GPIOA, CP_nRST_Pin, RESET);
  	//HAL_Delay(1000);
  	//HAL_GPIO_WritePin(GPIOA, CP_nRST_Pin, SET);
  	//status = HAL_UART_Receive(&huart2, &rxByte, 1, 5000);

  	//HAL_UART_Transmit_IT(&huart2, test_str, sizeof(test_str)-1);

  	  limit_state = HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin);
  	  if(limit_state == GPIO_PIN_RESET)
  	  {
  		  gl_limit = true;
  	  }
  	  else
  	  {
  		  gl_limit = false;
  	  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  POS4_State = HAL_GPIO_ReadPin(GPIOA, POS4_BUT_Pin);
	  if(POS4_State == GPIO_PIN_RESET)
	  {
		  actuator_move_pos(4);
	  }
	  //HAL_GPIO_TogglePin(GPIOA, POS2_LED_Pin|POS3_LED_Pin);
	  //HAL_GPIO_TogglePin(GPIOB, POS1_LED_Pin|POS4_LED_Pin);
	  //HAL_UART_Transmit(&huart2, (uint8_t *)test_str, sizeof(test_str) - 1U, HAL_MAX_DELAY);
	  //HAL_UART_Transmit_IT(&huart2, test_str, sizeof(test_str)-1);
	  //HAL_Delay(1000);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2400;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
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
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  //HAL_NVIC_SetPriority(TIM3_IRQn,     3, 0);  /* Motor PWM: lower priority */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CP_nRST_Pin|SPI_CS_Pin|POS2_LED_Pin|POS3_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DIR_Pin|MP6602_ENBL_Pin|MP6602_nRST_Pin|MP6602_SLEEP_Pin
                          |POS1_LED_Pin|POS4_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CP_nRST_Pin SPI_CS_Pin POS2_LED_Pin POS3_LED_Pin */
  GPIO_InitStruct.Pin = CP_nRST_Pin|SPI_CS_Pin|POS2_LED_Pin|POS3_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DIR_Pin MP6602_ENBL_Pin MP6602_nRST_Pin MP6602_SLEEP_Pin
                           POS1_LED_Pin POS4_LED_Pin */
  GPIO_InitStruct.Pin = DIR_Pin|MP6602_ENBL_Pin|MP6602_nRST_Pin|MP6602_SLEEP_Pin
                          |POS1_LED_Pin|POS4_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : POS1_BUT_Pin POS2_BUT_Pin */
  GPIO_InitStruct.Pin = POS1_BUT_Pin|POS2_BUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MP6602_nFAULT_Pin */
  GPIO_InitStruct.Pin = MP6602_nFAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MP6602_nFAULT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : POS3_BUT_Pin */
  GPIO_InitStruct.Pin = POS3_BUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(POS3_BUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : POS4_BUT_Pin */
  GPIO_InitStruct.Pin = POS4_BUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(POS4_BUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LMT_SW_Pin */
  GPIO_InitStruct.Pin = LMT_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LMT_SW_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void calc_pos_counts(void)
{
	gl_pos_counts[0] = gl_actuator_offset;
	gl_pos_counts[1] = gl_pos_counts[0] + gl_pulseperposition;
	gl_pos_counts[2] = gl_pos_counts[1] + gl_pulseperposition;
	gl_pos_counts[3] = gl_pos_counts[2] + gl_pulseperposition;

} //calc_pos_counts

void actuator_home(void)
{
	uint32_t ii;
	// If Limit switch is already depressed, move off limit
//	if(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_RESET)
//	{
//		gl_positiondelta = 8000;
//		gl_pulse_cnt = 0;
//		gl_inmotion = true;
//		HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);
//		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
//		HAL_TIM_Base_Start_IT(&htim3);
//		while(gl_inmotion){};
//		for(ii=0; ii<0x7FFFFF; ii++){}
//		HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
//		HAL_TIM_Base_Stop_IT(&htim3);
//
//	}

	// IF limit switch is not depressed, move to limit
	if(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_SET)
	{
		HAL_GPIO_WritePin(GPIOB, DIR_Pin, SET);
		gl_inmotion = true;
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
		HAL_TIM_Base_Start(&htim3);

		while(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_SET){};
		//__HAL_GPIO_EXTI_CLEAR_IT(LMT_SW_Pin);
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
		HAL_TIM_Base_Stop(&htim3);
		gl_inmotion = false;
	}

	for(ii=0; ii<0x7FFFFF; ii++){}
	__HAL_GPIO_EXTI_CLEAR_IT(LMT_SW_Pin);

	gl_positiondelta = gl_actuator_offset;
	gl_pulse_cnt = 0;
	HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);
	gl_inmotion = true;
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	HAL_TIM_Base_Start_IT(&htim3);

	gl_homed = true;



//	if(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_RESET)
//	{
//		gl_positiondelta = gl_actuator_offset;
//		HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);
//		gl_inmotion = true;
//		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
//		HAL_TIM_Base_Start_IT(&htim3);
//	}

	gl_filter_position = 1;
	gl_pulse_cnt = 0;
	gl_actuator_position = gl_actuator_offset;
	set_led_pos(gl_filter_position);


} // actuator_home

void actuator_move_in(void)
{
	if(gl_filter_position < 4)
	{
		gl_pulse_cnt = 0;

		HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
		HAL_TIM_Base_Start_IT(&htim3);

		gl_filter_position++;

		set_led_pos(gl_filter_position);

	}
} // actuator_move_in

void actuator_move_out(void)
{
	if(gl_filter_position > 1)
	{
		gl_pulse_cnt = 0;

		if(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_SET)
		{
			HAL_GPIO_WritePin(GPIOB, DIR_Pin, SET);
			HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
			HAL_TIM_Base_Start_IT(&htim3);
		}

		gl_filter_position--;

		set_led_pos(gl_filter_position);
	}
} // actuator_move_out

void actuator_move_pos(uint8_t pos)
{
	if(gl_filter_position != pos)
	{
		//Determine direction
		if(pos > gl_filter_position)
		{
			HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);
			//Determine pulse count
			gl_positiondelta = gl_pos_counts[pos-1] - gl_pos_counts[gl_filter_position -1];
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, DIR_Pin, SET);
			//Determine pulse count
			gl_positiondelta = gl_pos_counts[gl_filter_position -1] - gl_pos_counts[pos-1];
		}

		gl_inmotion = true;
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
		HAL_TIM_Base_Start_IT(&htim3);

		gl_filter_position = pos;
		set_led_pos(gl_filter_position);



	}
} //actuator_move_pos

/*
void actuator_move_out(void)
{

	gl_pulse_cnt = 0;

	if(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_SET)
	{
		HAL_GPIO_WritePin(GPIOB, DIR_Pin, SET);
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
		HAL_TIM_Base_Start_IT(&htim3);

		gl_filter_position--;
		HAL_Delay(10000);
		HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
		HAL_TIM_Base_Stop_IT(&htim3);

	}


} // actuator_move_out
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin){
	case(POS1_BUT_Pin)	:	if(gl_homed){actuator_move_pos(1);}else{actuator_home();} break;
	case(POS2_BUT_Pin)	: 	actuator_move_pos(2); break;
	case(POS3_BUT_Pin)	:	actuator_move_pos(3); break;
	case(LMT_SW_Pin)	:	if(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_RESET){gl_limit = true; HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);HAL_TIM_Base_Stop(&htim3);}else{gl_limit = false;} break;
	default				: 	break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		if(gl_pulse_cnt == gl_positiondelta)
		{
			gl_pulse_cnt = 0;
			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
			HAL_TIM_Base_Stop_IT(&htim3);
			gl_inmotion = false;
		}
		else
		{
			gl_pulse_cnt++;
		}
	}
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if (huart->Instance == USART2)
//    {
//        /* rxByte now contains one received byte. */
//
//        /* Example: echo that byte back asynchronously. */
//        HAL_UART_Transmit_IT(&huart2, &rxByte, 1);
//
//        /* Rearm receive, otherwise it stops after this one byte. */
//        HAL_UART_Receive_IT(&huart2, &rxByte, 1);
//    }
//}

/* USER CODE END 4 */

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
#ifdef USE_FULL_ASSERT
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
