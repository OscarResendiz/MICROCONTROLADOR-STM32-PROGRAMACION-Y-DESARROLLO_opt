/**********************************************************************
  * Project Name: TIMER1
  * File Name: delay.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * ImGPIO_StructureInitant:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *  
  *  Libreria que produce retardos en ms y us  
  *
  *************************************************************/

#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"
   

/*  Lista de las funciones de la libreria Delay.c   */

void Delay_Init(void);
void Delay_Us(uint32_t us);
void Delay_Ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif

/******************************************************************************/
