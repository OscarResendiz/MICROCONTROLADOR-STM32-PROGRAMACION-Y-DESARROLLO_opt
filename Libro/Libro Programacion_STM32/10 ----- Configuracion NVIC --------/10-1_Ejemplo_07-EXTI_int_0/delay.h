 /**********************************************************************
  * Project Name: EXTI_int_1
  * File Name: delay.h
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
  * PROGRAMA DE PRUEBA DE LA INTERRUPCION EXTI_0.
  * 
  * Enciende y apaga un el LED conectado al pin PB1, 
  * al pulsar el boton conectado en PA0 tras 
  * detectarse una Interrupcion EXTI_
  *
  *********************************************************************/
 
#ifndef __DELAY_H
#define __DELAY_H

// Libreria principal del microcontrolador
#include "stm32f10x.h"

// Listado de funciones
void DelayInit(void);
void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);

#endif

/***********************   Fin del fichero   ****************************/
