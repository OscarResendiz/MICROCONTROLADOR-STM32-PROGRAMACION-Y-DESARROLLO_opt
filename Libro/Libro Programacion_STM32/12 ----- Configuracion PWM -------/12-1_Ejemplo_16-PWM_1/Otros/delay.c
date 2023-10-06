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

	
#include "delay.h"

// Configuramos la variable que contendra la cuenta 
static __IO uint32_t usTicks;

// Función que se produce cada vez que el reloj del sistema llega a 1 us
void SysTick_Handler()
{
	if (usTicks != 0)
	{
		usTicks--;
	}
}

/*   Funcion que inicializa el Delay  */
void Delay_Init()
{
	// Actualiza el valor de SystemCoreClock
	SystemCoreClockUpdate();
	// Configuramos el SysTick timer para que se desborde cada 1 us
	SysTick_Config(SystemCoreClock / 1000000);
}


/*   Función que produce un Retardo de (us)   */
void Delay_Us(uint32_t us)
{
	// Recarga el valor recibido 
	usTicks = us;
	// Espera hasta que se llegue a cero
	while (usTicks);
}

/*   Funcion que produce un retardo de (ms)   */
void Delay_Ms(uint32_t ms)
{
	// Espera hasta que se llegue a cero
	while (ms--)
	{
		// Retardo de 1ms
		Delay_Us(1000);
	}
}


/******************************************************************************/
