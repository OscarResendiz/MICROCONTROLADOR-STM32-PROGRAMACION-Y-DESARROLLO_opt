 /**********************************************************************
  * Project Name: TIMER_1
  * File Name: delay.h
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
  *  PROGRAMA DE EJEMPLO DE MANEJO DEL TIMER
	*
	*  En el programa se enciende y apaga dos LED conectados
	*  a los pines PC13 y a PB1.
	*  El primero se encendera y apagar¨¢ cada 2.5 segundos y
	*  el segundo LED conectado al TIM2 cada 500 milisegundos.
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
