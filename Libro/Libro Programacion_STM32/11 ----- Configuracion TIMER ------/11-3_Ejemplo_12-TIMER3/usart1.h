/**********************************************************************
  * Project Name: TIMER3
  * File Name: usart1.h
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestano@gmail.com
  * Date :    2017.09.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * =====================================================================
  * 
  * PROGRAMA DE PRUEBA DEL TIMER
  * 
  * El programa enciende y apaga un LED conectado PC13, generando
  * una frecuencia con el TIMER de 72 MHz
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
