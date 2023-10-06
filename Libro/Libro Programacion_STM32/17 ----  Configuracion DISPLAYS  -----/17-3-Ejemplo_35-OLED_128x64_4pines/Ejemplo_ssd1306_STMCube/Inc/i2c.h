/*****************************************************************************
  * File Name          : I2C.h
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  *****************************************************************************/

#ifndef __i2c_H
#define __i2c_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

void MX_I2C1_Init(void);
   
void MX_GPIO_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
