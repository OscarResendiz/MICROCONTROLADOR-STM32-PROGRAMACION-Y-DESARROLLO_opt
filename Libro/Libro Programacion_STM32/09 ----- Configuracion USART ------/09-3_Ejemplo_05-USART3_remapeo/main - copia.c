  /**********************************************************************
  * Project Name: USART_rempa
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
  * PROGRAMA DE PRUEBA DE REMAPEO PUERTO USART1
  * 
  * Transmite un mensaje de prueba, por el puerto USART1 del
  * microcontrolador pero re-mapeado de los pines por defecto
  * a traves del la configuracion siguiente.
  *
  *   USART1 Tx (PB6)
  *          Rx (PB7)
  *
  **********************************************************************/
 
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"		
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"     

// LLama al modulo externo de configuracion del puerto USART1
#include "usartM.h"		

// Funcion que genera un retardo
void Delay_ms(unsigned int nCount);

/*     Modulo principal          */
//===================================================
int main(void)
{	
	
	USARTremap_Config();   //LLama a la funcion que configura el USART1
	
	for(;;)   // Produce un bucle infinito
	{
	
  // Imprime un mensaje por el puerto USART1 medienta el comando printf  
  printf("Esto es un texto de prueba por printf\r\n");
	Delay_ms(200);
	
  // Imprime un mensaje por el puerto USART1 mediante la función USART1_printf
	USART1_printf(USART1, "Esto es un texto de prueba por USART1_printf demo \r\n");
	Delay_ms(200);	
		
	// Imprime un número pro el puerto USART1 mediante el comando printf
  printf("Esto es el numero decimal %d \r\n", 12345);
	Delay_ms(1000);	
		
	}
}


// Modulo que genera un retardo (ms)
//--------------------------------------
void Delay_ms(unsigned int nCount){ 
		unsigned int i, j;
		for(i = 0; i < nCount; i++)
		{  
				for(j = 0; j < 0x2AFF; j++){;} 
		}
} 



////////////////////////  Fin del Programa   ///////////////////

