/**********************************************************************
  * Project Name: USART_int
  * File Name: usart.h
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
  * PROGRAMA DE PRUEBA PUERTO USART1 CON DETECCION DE IRQ
  * 
  * Recibimos datos desde nuestro ordenador por el puerto USART1
  * y enviamos de nuevo los datos recibidos. Pero sin que el
  * proceso normal quede desatendido.
  *
  *  Pines del USART1:   Tx PA9
  *                      Rx PA10
  *
  **********************************************************************/
 
#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(u32 baund);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void NVIC_Configuration(void);
void USART1_IRQHandler(void);

#endif /* __USART1_H */
