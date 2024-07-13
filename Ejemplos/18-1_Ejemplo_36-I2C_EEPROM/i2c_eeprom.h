/**********************************************************************
  * Project Name: I2c_EEPROM
  * File Name: i2c_eeprom.h
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * =====================================================================
  * 
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL PUERTO I2C
  * 
  * Programa ejemplo de utilizacion del puerto I2C que
  * lee y escribe un byte en una memoria EEPROM y muestra
  * el valor leido por el puerto serie USART1
  *
  **********************************************************************/

#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

#include "stm32f10x.h"  

void I2C1_EE_Init(void);
void I2C2_Memory_Init(void);

int I2C_eeprom_Read(I2C_TypeDef*, uint8_t, uint8_t);

int I2C_eeprom_Write(I2C_TypeDef*, uint8_t, uint8_t, uint8_t);

#endif
