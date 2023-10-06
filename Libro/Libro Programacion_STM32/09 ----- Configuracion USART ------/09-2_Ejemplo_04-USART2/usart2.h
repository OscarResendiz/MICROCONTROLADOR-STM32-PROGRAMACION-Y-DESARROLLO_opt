/**********************************************************************
  * Project Name: USART2
  * File Name: usart2.h
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
  * PROGRAMA DE PRUEBA DE RECEPCION Y ENVIO DE TEXTO POR EL USART2
  *
  *  Codigo que reenvia por el mismo puerto USART2 los caracteres
  *  que se reciben.
  *
  *  Pines del USART2:   Tx PA2
  *                      Rx PA3
  *
  **********************************************************************/

#ifndef __USART2_H
#define	__USART2_H

#include "stm32f10x.h"

void USART2_Config(void);
void USART2_PutChar(char c);
void USART2_PutString(char *s);
uint16_t USART2_GetChar(void);

#endif /* __USART2_H */
