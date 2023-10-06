 /**********************************************************************
  * Project Name: ADC_inyectado
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
  * PROGRAMA EJEMPLO DE USO DE CANALES ADC INYECTADO
  *
  * Se lee los valores de conversion ADC en los Pines PA0 y PA1
  * y se envian por el puerto USART1 hacia el ordenador
  *
  *************************************************************/

#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"

void USART1_Config(void);

#endif /* __USART1_H */
