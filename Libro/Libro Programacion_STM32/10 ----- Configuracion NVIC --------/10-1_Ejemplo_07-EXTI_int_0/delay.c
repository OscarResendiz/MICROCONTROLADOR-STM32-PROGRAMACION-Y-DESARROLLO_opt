 /**********************************************************************
  * Project Name: EXTI_int_1
  * File Name: delay.c
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
 
// Fichero con el litado de funciones	
#include "delay.h"

// Variable usTicks para registro contador
static __IO uint32_t usTicks;

// Funcion que detecta interrupcion Tick del sistema cada 1 us
void SysTick_Handler()
{
	if (usTicks != 0)
	{
		usTicks--;
	}
}

/*  Funcionn que configura el Delay_init   */
void DelayInit()
{
	// Actualiza el valor SystemCoreClok del sistema
	SystemCoreClockUpdate();
	// Establece el Systick timer para desborde cada 1 us
	SysTick_Config(SystemCoreClock / 1000000);
}

/*  Funcion que configura retardos en microsegundos (us)  */
void DelayUs(uint32_t us)
{
	// Recoje el valor enviado 'us'
	usTicks = us;
	// Espera hasta que usTick se recargue a cero
	while (usTicks);
}

/*  Funcion que configura retardos en milisegundos (ms)  */
void DelayMs(uint32_t ms)
{
	// Cuenta hasta que ms llega a cero
	while (ms--)
	{
		// cuenta un retardo de 1 ms
		DelayUs(1000);
	}
}

//////////////////////////////////////////////////////////////////////////////