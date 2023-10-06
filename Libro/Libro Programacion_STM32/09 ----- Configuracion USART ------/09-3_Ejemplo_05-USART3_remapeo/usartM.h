 /**********************************************************************
  * Project Name: USART_remap
  * File Name: usartM.h
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
  * PROGRAMA DE PRUEBA DE REMAPEO PUERTO USART1
  * 
  * Transmite un mensaje de prueba, por el puerto USART1 del
  * microcontrolador pero re-mapeado de los pines por defecto
  * a traves del la configuracion siguiente.
  *
  *   USART1 Tx (PB6)
  *          Rx (PB7)
  *
  ***************************************************************/
  
#ifndef __USARTM_H_
#define __USARTM_H_

#include "stm32f10x.h"
#include <stdio.h>

void USARTremap_Config(void);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);


#endif /* __USARTM_H_ */

