/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void rec_button_callback();
void ch_button_callback(int ch);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DISP_RST_Pin GPIO_PIN_3
#define DISP_RST_GPIO_Port GPIOA
#define BTN_CH2_Pin GPIO_PIN_4
#define BTN_CH2_GPIO_Port GPIOA
#define BTN_CH2_EXTI_IRQn EXTI4_IRQn
#define SPI_CS_Pin GPIO_PIN_0
#define SPI_CS_GPIO_Port GPIOB
#define DISP_A0_Pin GPIO_PIN_1
#define DISP_A0_GPIO_Port GPIOB
#define BTN_CH1_Pin GPIO_PIN_8
#define BTN_CH1_GPIO_Port GPIOA
#define BTN_CH1_EXTI_IRQn EXTI9_5_IRQn
#define BTN_REC_Pin GPIO_PIN_3
#define BTN_REC_GPIO_Port GPIOB
#define BTN_REC_EXTI_IRQn EXTI3_IRQn
#define LED_REC_Pin GPIO_PIN_4
#define LED_REC_GPIO_Port GPIOB
#define LED_CH1_Pin GPIO_PIN_5
#define LED_CH1_GPIO_Port GPIOB
#define LED_CH2_Pin GPIO_PIN_6
#define LED_CH2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
