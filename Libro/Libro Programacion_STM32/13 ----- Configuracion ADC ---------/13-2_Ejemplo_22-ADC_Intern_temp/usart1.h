  /**********************************************************************
  * Project Name: Inten_temp
  * File Name: usart1.h
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *  
  * PROGRAMA EJEMPLO DE USO DEL ADC
  *
  * Programa que lee el sensor interno de temperatura del micro y
  * nos muestra la información por el puerto serial USART1.
  * 
  ***************************************************************/	

#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(uint32_t baud);
void USART2_Config(uint32_t baud);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
	
void SetSysClockTo56(void);

#endif /* __USART1_H */
