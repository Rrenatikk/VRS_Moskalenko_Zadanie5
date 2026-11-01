/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C1_Init(void);

/* USER CODE BEGIN Prototypes */
/* Types of function pointers (callbacks) for I2C abstraction.
 * dev_addr - I2C sensor address (7-bit, no offset)
 * reg_addr - the register address inside the sensor
 * data     - pointer to the data buffer
 * len      - number of bytes
 */
typedef uint8_t (*I2C_Write_Callback)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
typedef uint8_t (*I2C_Read_Callback)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

/* Prototypes of our hardware functions for STM32 */
uint8_t STM32_I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
uint8_t STM32_I2C_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

