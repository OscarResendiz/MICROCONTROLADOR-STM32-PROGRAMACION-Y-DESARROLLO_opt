 /**********************************************************************
  * Project Name: ADC_inyectado
  * File Name: usart1.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *  
  * PROGRAMA EJEMPLO DE USO DE CANALES ADC INYECTADO
  *
  * Se lee los valores de conversion ADC en los Pines PA0 y PA1
  * y se envian por el puerto USART1 hacia el ordenador
  *
  *************************************************************/
 
#include "usart1.h"
#include <stdarg.h>

/* Funcion que confifura el USART1   */
void USART1_Config(void)
{
	/* Creamos la estructura para el GPIO    */
	GPIO_InitTypeDef GPIO_StructureInit;
	USART_InitTypeDef USART_StructureInit;
	
	/* Activamos el reloj para USART1 y GPIOA   */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configuramos el GPIO para el USART1 */
	/* Configuramos Pin PA9 para Tx del USART1 en modo push-pull */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_9;
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_StructureInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_StructureInit);    
	
	/* Configuramos el Pin PA10 para Rx del USART1 en modo floating */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_10;
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_StructureInit);
	  
	/* Configuramos el USART1 */
	/* Velocidad Baudios: 115200
	   Longitud de palabra: 8 bits
		 Bit de Stop: 1
		 Paridad: Ninguna
		 Control de flujo: Ninguna  */
	USART_StructureInit.USART_BaudRate = 115200;
	USART_StructureInit.USART_WordLength = USART_WordLength_8b;
	USART_StructureInit.USART_StopBits = USART_StopBits_1;
	USART_StructureInit.USART_Parity = USART_Parity_No ;
	USART_StructureInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_StructureInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_StructureInit); 
	USART_Cmd(USART1, ENABLE);
}

/******************* Fin del modulo  *************************/
