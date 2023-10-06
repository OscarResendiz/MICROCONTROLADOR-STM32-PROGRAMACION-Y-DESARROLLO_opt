 /**********************************************************************
  * Project Name: USART1_Irq
  * File Name: main.c
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
  * PROGRAMA DE PRUEBA PUERTO USART1 CON DETECCION DE IRQ
  * 
  * Recibimos datos desde nuestro ordenador por el puerto USART1
  * y enviamos de nuevo los datos recibidos. Pero sin que el
  * proceso normal quede desatendido.
  *
  *  Pines del USART1:   Tx PA9
  *                      Rx PA10
  *
  **********************************************************************/
 
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"		
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"     

// LLama al modulo externo de configuracion del puerto USART1
#include "usart1.h"

/*  Programa principal  */
int main(void)
{	
	/* USART1 configurado para 115200 8-N-1 */
	USART1_Config(115200);	//Inicializa el USART1
  
	NVIC_Configuration();	//Inicializa el NVIC

	printf("\r\n Este es una prueba interrupcion del USART \r\n");

	for(;;)
	{
		
	}
}


/**************** Fin del programa  ********************/
