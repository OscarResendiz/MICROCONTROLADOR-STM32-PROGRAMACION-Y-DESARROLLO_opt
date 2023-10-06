/**********************************************************************
  * Project Name: BKP
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
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL BKP
  * 
  * Programa ejemplo de utilizacion de los registros BKP
  *
  **********************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_usart.h"
#include "misc.h"

#include <stdio.h>


//  Lista de funciones ----------------------------------------
void USART1_Config(void);

/*                  Funcion          Principal                */
//**************************************************************
int main(void)
{
	uint16_t Contador = 0;    // Creamos la variables 'Contador'
  
	USART1_Config();      // Inicializamos el USART1

	// Habilitamos los relojes para modulos BKP y PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
  // Deshabilitamos la protección de escritura en los registros BKP
	PWR_BackupAccessCmd(ENABLE);

	// Leemos el valor del 'Contador' guardado en el registro 1 del BKP
	Contador = BKP_ReadBackupRegister(BKP_DR1);
	
  // Aumentamos en 1 el 'Contador' tras el reinicio
	Contador++;
	
  // Volvemos a grabar el valor actual del 'Contador'
	BKP_WriteBackupRegister(BKP_DR1, Contador);

	// Mostramos por el puerto USART1 el valor actual del 'Contador'
	printf("\r\n Valor del BKP_DR1: %d\r\n", Contador);
	
  while(1)   
    {
    }
}


//  Funcion que configura el USART1                         */
//------------------------------------------------------------
void USART1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

// Funcion que nos permite utilizar el comando 'printf'
int fputc(int ch, FILE *f)
{
	/* Envia dato */
	USART_SendData(USART1, (unsigned char) ch);
	
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

///////////////////////////////   Fin del programa   ////////////////////////////
