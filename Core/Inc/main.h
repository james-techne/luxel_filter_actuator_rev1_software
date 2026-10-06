/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "cmd.h"
#include "actuator.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"



/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CP_nRST_Pin GPIO_PIN_1
#define CP_nRST_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_4
#define SPI_CS_GPIO_Port GPIOA
#define DIR_Pin GPIO_PIN_1
#define DIR_GPIO_Port GPIOB
#define POS1_BUT_Pin GPIO_PIN_2
#define POS1_BUT_GPIO_Port GPIOB
#define POS1_BUT_EXTI_IRQn EXTI2_3_IRQn
#define MP6602_ENBL_Pin GPIO_PIN_10
#define MP6602_ENBL_GPIO_Port GPIOB
#define MP6602_nFAULT_Pin GPIO_PIN_11
#define MP6602_nFAULT_GPIO_Port GPIOB
#define MP6602_nRST_Pin GPIO_PIN_12
#define MP6602_nRST_GPIO_Port GPIOB
#define MP6602_SLEEP_Pin GPIO_PIN_13
#define MP6602_SLEEP_GPIO_Port GPIOB
#define POS1_LED_Pin GPIO_PIN_14
#define POS1_LED_GPIO_Port GPIOB
#define POS2_BUT_Pin GPIO_PIN_15
#define POS2_BUT_GPIO_Port GPIOB
#define POS2_BUT_EXTI_IRQn EXTI4_15_IRQn
#define POS2_LED_Pin GPIO_PIN_8
#define POS2_LED_GPIO_Port GPIOA
#define POS3_BUT_Pin GPIO_PIN_9
#define POS3_BUT_GPIO_Port GPIOA
#define POS3_BUT_EXTI_IRQn EXTI4_15_IRQn
#define POS3_LED_Pin GPIO_PIN_10
#define POS3_LED_GPIO_Port GPIOA
#define POS4_BUT_Pin GPIO_PIN_15
#define POS4_BUT_GPIO_Port GPIOA
#define POS4_LED_Pin GPIO_PIN_3
#define POS4_LED_GPIO_Port GPIOB
#define LMT_SW_Pin GPIO_PIN_4
#define LMT_SW_GPIO_Port GPIOB
#define LMT_SW_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
