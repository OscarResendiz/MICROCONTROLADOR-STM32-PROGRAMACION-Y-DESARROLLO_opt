/**********************************************************************
  * Project Name: I2C_HMC5883L
  * File Name: i2c.c
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
  *  Nuestro código configura el bus I2C para que se conecte a un sensor
  *  HMC5883 del que leeremos los datos de sus registros
  *
  **********************************************************************/
#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void SetSysClockTo56(void);

#endif /* __USART1_H */
