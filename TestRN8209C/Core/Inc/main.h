/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RN8209C_CH_SW_B_Pin GPIO_PIN_6
#define RN8209C_CH_SW_B_GPIO_Port GPIOA
#define RN8209C_CH_SW_A_Pin GPIO_PIN_7
#define RN8209C_CH_SW_A_GPIO_Port GPIOA
#define LED_ERROR_Pin GPIO_PIN_0
#define LED_ERROR_GPIO_Port GPIOB
#define RN8209C_TX_Pin GPIO_PIN_10
#define RN8209C_TX_GPIO_Port GPIOB
#define RN8209C_RX_Pin GPIO_PIN_11
#define RN8209C_RX_GPIO_Port GPIOB
#define W25Q_CS_Pin GPIO_PIN_12
#define W25Q_CS_GPIO_Port GPIOB
#define W25Q_SCK_Pin GPIO_PIN_13
#define W25Q_SCK_GPIO_Port GPIOB
#define W25Q_MISO_Pin GPIO_PIN_14
#define W25Q_MISO_GPIO_Port GPIOB
#define W25Q_MOSI_Pin GPIO_PIN_15
#define W25Q_MOSI_GPIO_Port GPIOB
#define LED_RUN_Pin GPIO_PIN_11
#define LED_RUN_GPIO_Port GPIOA
#define LED_WARN_Pin GPIO_PIN_12
#define LED_WARN_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
