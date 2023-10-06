 /**********************************************************************
  * Project Name: WWDG
  * File Name: delay.c
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
  * PROGRAMA DE PRUEBA INTERRUPCION WWDG WATCHDOG
  * 
  *  Programa de ejemplo de manejo del Watchdog WWDG.
  *  En el que configuramos el contador del WWDG para que
  *  se reinicie cada .... pero que establecemo un delay
  *  en el modulo principal para que se restablezca el 
  *  contador antes de que se produzca el restablecimiento
  *  del contador.
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
		DelayUs(100);
	}
}

/////////////////   Fin del Programa   ///////////////////////
