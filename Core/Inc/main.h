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
#include "stm32l5xx_hal.h"

#include "stm32l5xx_ll_ucpd.h"
#include "stm32l5xx_ll_bus.h"
#include "stm32l5xx_ll_cortex.h"
#include "stm32l5xx_ll_rcc.h"
#include "stm32l5xx_ll_system.h"
#include "stm32l5xx_ll_utils.h"
#include "stm32l5xx_ll_pwr.h"
#include "stm32l5xx_ll_gpio.h"
#include "stm32l5xx_ll_dma.h"

#include "stm32l5xx_ll_exti.h"

#include "hci_tl_interface.h"
#include "stm32l5xx_nucleo.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "custom_hybrid_sensors.h"
#include "custom_hybrid_sensors_ex.h"
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
#define BSP_BUTTON_EXTI_IRQn EXTI13_IRQn
#define VBUS_SENSE_Pin GPIO_PIN_2
#define VBUS_SENSE_GPIO_Port GPIOC
#define CS_BLE_Pin GPIO_PIN_2
#define CS_BLE_GPIO_Port GPIOA
#define IRQ_BLE_Pin GPIO_PIN_3
#define IRQ_BLE_GPIO_Port GPIOA
#define IRQ_BLE_EXTI_IRQn EXTI3_IRQn
#define RST_BLE_Pin GPIO_PIN_12
#define RST_BLE_GPIO_Port GPIOF
#define EN_R_Pin GPIO_PIN_13
#define EN_R_GPIO_Port GPIOE
#define UCPD_FLT_Pin GPIO_PIN_14
#define UCPD_FLT_GPIO_Port GPIOB
#define UCPD_FLT_EXTI_IRQn EXTI14_IRQn
#define CS_T_Pin GPIO_PIN_14
#define CS_T_GPIO_Port GPIOD
#define CS_R_Pin GPIO_PIN_15
#define CS_R_GPIO_Port GPIOD
#define LED_GREEN_Pin GPIO_PIN_7
#define LED_GREEN_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_9
#define LED_RED_GPIO_Port GPIOA
#define UCPD_DBN_Pin GPIO_PIN_5
#define UCPD_DBN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
