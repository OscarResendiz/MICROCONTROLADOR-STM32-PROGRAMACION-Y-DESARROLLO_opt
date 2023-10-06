/**********************************************************************
  * Project Name: USART_1
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
  * PROGRAMA DE PRUEBA PUERTO USARTx
  * 
  * Enviamos por el puerto USART1 del microcontrolador un texto
  * de prueba.
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
#include <stdio.h>


void Delay_ms(unsigned int nCount);

/* programa principal main*/
int main(void)
{	
	USART1_Config();   //LLama a la funcion que configura el USART1
	
	for(;;)
	{
	printf("Esto es un texto de prueba por printf\r\n");
	Delay_ms(200);  	
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

