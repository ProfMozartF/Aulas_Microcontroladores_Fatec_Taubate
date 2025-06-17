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
#define Injetor_Pin GPIO_PIN_13
#define Injetor_GPIO_Port GPIOC
#define Temp_Ar_Pin GPIO_PIN_0
#define Temp_Ar_GPIO_Port GPIOA
#define Pres_Ar_Pin GPIO_PIN_1
#define Pres_Ar_GPIO_Port GPIOA
#define Temp_Agua_Pin GPIO_PIN_2
#define Temp_Agua_GPIO_Port GPIOA
#define Pedal_1_Pin GPIO_PIN_3
#define Pedal_1_GPIO_Port GPIOA
#define Pedal_2_Pin GPIO_PIN_4
#define Pedal_2_GPIO_Port GPIOA
#define Sensor_Rot_Pin GPIO_PIN_12
#define Sensor_Rot_GPIO_Port GPIOB
#define Sensor_Rot_EXTI_IRQn EXTI15_10_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
