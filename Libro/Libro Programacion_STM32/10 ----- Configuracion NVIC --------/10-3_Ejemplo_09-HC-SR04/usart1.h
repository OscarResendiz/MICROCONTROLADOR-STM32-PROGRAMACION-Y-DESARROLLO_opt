/**********************************************************************
  * Project Name: HC-SR04
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
  *  
  *  PROGRAMA DE EJEMPLO DE MANEJO DEL TIMER COMO CONTADOR DE PULSOS
  *
  *  Prueba con el dispositivo HC-SR04:
  *
  *  Convierte el tiempo transcurrido entre la señal de TRIGGER
  *  y cuando se recibe la señal de recepcion ECHO y lo convertimos
  *  en logitud de distancia hacia un objeto.  
  *
  *  Conexiones:
  *      HC-SR04    STM32F103
  *         Vcc -----  5 Vcc
  *     Trigger -----  PB15
  *        Echo -----  PB0   (EXTI_Line0)
  *         Gnd -----  Gnd
  *
  **********************************************************************/
 

#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
