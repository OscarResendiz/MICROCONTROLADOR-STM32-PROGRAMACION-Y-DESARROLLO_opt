 /**********************************************************************
  * Project Name: TIMER1
  * File Name: usart1.h
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *  
  * PRUEBA DE LECTURA MULTIPLE DE CANALES ADC.
  *
  *  En el programa se muestra por el puerto USART en nuestro ordenador
  *  los valores obtenidos en diferentes canales ADC, obteniendo los
  *  valores de su lectura en el DMA del modulo ADC.
  *
  *  Conexiones:
  *                 ADC1_CH4 en PA4
  *                 ADC1_CH5 en PA5
  *                 ADC1_CH6 en PA6  
  *                 ADC1_CH7 en PA7
  *
  **************************************************************/
  
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
