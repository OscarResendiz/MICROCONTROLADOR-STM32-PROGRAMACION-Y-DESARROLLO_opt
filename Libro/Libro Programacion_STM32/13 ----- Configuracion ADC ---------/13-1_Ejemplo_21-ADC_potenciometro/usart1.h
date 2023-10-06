 /**********************************************************************
  * Project Name: ADC_potenciometer
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
  * En el presente ejemplo se enciende el LED en PB1 cuando
  * la lectura analogica del voltaje de salida de un potenciometro
  * conectado al pin PA6 excede de los 1.6 voltios.
  * Mostrandose dicho valor por el puerto USART en nuestro ordenador
  *
  * Conexiones:
  *    LED 		----  PB1
  *         [  ---  GND
  *   [ Pot [  ---  PA6
  *         [  ---  3.3 Vcc
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
