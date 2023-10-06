/**********************************************************************
  * Project Name: ENCODER
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
  * PROGRAMA DE EJEMPLO DE USO DEL TIMER COMO CONTADOR
  *
  * El programa detecta el sentido de giro de un encoder
  * conectado a los pines PA6 y PA7 y mostrando el valor
  * leido por el puerto USART1 en nuestro ordenador
  *
  ***********************************************************/

// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "misc.h"

#include <stdio.h>    // Libreria para el uso del 'printf'

// Lista de funciones
void ENCODER_Init(void);
void USART_Config(void);

/*            Modulo Principal                  */
//================================================
int main(void)
{
	int ValorPrevio=0;
  USART_Config();   // Inicializamos el USART1
  
	ENCODER_Init();   // Inicializamos el ENCODER

	while (1)
	{
		// Imprimimos el valor del contador en puerto USART1
    int ENCODERvalor = TIM_GetCounter(TIM3);
    printf(" Valor %d ", ENCODERvalor);
    
    // Comprobamos si se ha rotado a Derecha o Izquierda
    if (ENCODERvalor > ValorPrevio)
    {
      printf( "Derecha \r\n");      
    }
    else if (ENCODERvalor < ValorPrevio)
    {
      printf( "Izquierda \r\n");
    }
    else
    {
      printf ("\r\n");
    }
    ValorPrevio = ENCODERvalor;
       
		
		for(int i=0;i<0x100000;i++);    // Simple contador como delay
	}
}

/*  Funcion que configura los pines de entrada del ENCODER  */
//------------------------------------------------------------
void ENCODER_Init(void)
{  
  /* Configuracion GPIO para entradas del ENCODER   */
  GPIO_InitTypeDef GPIO_ENCODER_InitStructure;
  // Iniciamos el reloj para GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
  // Configuramos la estructura GPIO para los pines PA6 y PA7
	GPIO_ENCODER_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_ENCODER_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_ENCODER_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_ENCODER_InitStructure);

	// Iniciamos el reloj para TIMER 3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// Configuracion del TIMER 
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	// Inicializamos el contador TIMER a 300
	TIMER_InitStructure.TIM_Period = 300;
  TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up | TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM3, &TIMER_InitStructure);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_Cmd(TIM3, ENABLE);  
}

//   Funcion que configura el puerto USART1   */
//----------------------------------------------
void USART_Config(void)
{
    /* Configuramos GPIO para USART1 */
    GPIO_InitTypeDef GPIO_InitStructure;
	  // Iniciamos reloj para GPIOA y USART1
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    /* Configuramos pin PA0 como Tx USART1 Tx */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* Configuramos pin PA10 como Rx USART1  */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* Configuramos el USART1 */
	  USART_InitTypeDef USART_InitStructure;	 
	  USART_InitStructure.USART_BaudRate = 115200;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	  USART_Init(USART1, &USART_InitStructure);

	  /* Habilitamos el USART1 */
	  USART_Cmd(USART1, ENABLE);
}

/*  Funcion que envia caracteres por puerto USART1   
    a traves de la instruccion 'printf'             */
//-----------------------------------------------------
int fputc(int ch, FILE *f)
{	
	USART_SendData(USART1, (unsigned char) ch);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET)
  {;}
  return (ch);
}


////////////////////////   Fin del Programa   /////////////////////////////
