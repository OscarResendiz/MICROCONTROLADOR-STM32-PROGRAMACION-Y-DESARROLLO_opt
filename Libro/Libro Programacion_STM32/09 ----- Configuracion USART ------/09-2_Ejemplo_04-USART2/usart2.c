/**********************************************************************
  * Project Name: USART2
  * File Name: usart.c
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
  * PROGRAMA DE PRUEBA DE RECEPCION Y ENVIO DE TEXTO POR EL USART2
  *
  *  Codigo que reenvia por el mismo puerto USART2 los caracteres
  *  que se reciben.
  *
  *  Pines del USART2:   Tx PA2
  *                      Rx PA3
  *
  **********************************************************************/

// Libreria principal del microcontrolador
#include "stm32f10x.h"	

#include "usart2.h"

/*          Funcion que configura el USART2    */
//-----------------------------------------------
void USART2_Config()
{
	/* Creamos la estructura para el GPIO y USART   */
	GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
		
	/* Activamos el reloj para GPIOA   */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configuramos Pin PA2 para Tx del USART2 en modo AF_PP */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* Configuramos el Pin PA3 para Rx del USART2 en modo IN_floating */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
  
    /* Configuramos el USART2 */
    /* Activamos el reloj para USART2   */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
    /* Velocidad Baudios: 115200
	   Longitud de palabra: 8 bits
		 Bit de Stop: 1
		 Paridad: Ninguna
		 Control de flujo: Ninguna  */
		 
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);
	USART_Cmd(USART2, ENABLE);
}

/*     Funcion que envia un caracter por USART2   */
//------------------------------------------------------
void USART2_PutChar(char c)
{
	// Esperamos hasta que el registro de datos transmitidos este lleno
	while (!USART_GetFlagStatus(USART2, USART_FLAG_TXE));	
  // Enviamos los caracteres por el USART2
	USART_SendData(USART2, c);
}

/*     Funcion que envia una cadena 'String' por USART2   */
//-----------------------------------------------------------
void USART2_PutString(char *s)
{
	// Enviamos una cadena por el puerto USART2
	while (*s)
	{
    // Llama a la función ‘USART2_PutChar() para ir enviado
    // carácter a carácter el contenido de la cadena
		USART2_PutChar(*s++);
	}
}

/*    Funcion que lee un caracter recibido en el puerto USART2   */
//----------------------------------------------------------------------
uint16_t USART2_GetChar()
{
	// Esperamos a que se reciba una caracter en USART2
	while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
	// Devuelve el caracter recibido
	return USART_ReceiveData(USART2);
}

//////////////////////  Fin modulo USART2   ///////////////////////////////

