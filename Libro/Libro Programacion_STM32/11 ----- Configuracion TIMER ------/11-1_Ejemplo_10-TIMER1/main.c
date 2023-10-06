 /**********************************************************************
  * Project Name: TIMER1
  * File Name: main.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *  
  * PROGRAMA EJEMPLO DE CONFIGURACION DE TIMER
	*
  * En el progrma se configuran dos temporizadores con diferentes
  * frecuencias de trabajo que encienden y apagan dos led a diferentes
  * tiempos.
  *  
  * Conexiones:
  *
  *   LED1 en PB1 se enciende cada 0.5 Seg a 2 Hz
  *   LED2 en PC13 se eciende cada 0.1 seg a 10 Hz
  *
  *************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

// Lista de funciones empleadas
void TIM2_Config(void);
void TIM2_IRQHandler(void);

void TIM3_Config(void);
void TIM3_IRQHandler(void);

void GPIO_Config(void);


/*   Modulo principal    */
//==========================================================
int main(void){
  GPIO_Config();  // Inicializamos los GPIO
	
	TIM2_Config();	// Inicializa el Timer 2
  TIM3_Config();  // Inicializa el Timer 3
	  
 	while (1)
	{
		;;
	}
}

/*       Funcion que configura el GPIO para los LED      */
//---------------------------------------------------------
void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC, ENABLE);
  
  // Configura el pin PB1 como salida para LED1
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// Configura el pin PC13 como salida para LED2	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
  
}


/*  Funcion que configura el TIM2                  */
//----------------------------------------------------
void TIM2_Config(void)
{
	// TIM2   -------------------------------------------
  // Crea la estructura del TIMER 2
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* Configuramos los parametros del TIMER_2 para
     detectar un desbordamiento cada vez que cuente
     un periodo de tiempo de 2 HZ (0,5 seg = 500 ms)  
   -----------------------------------------------------
    Formula para calcular el periodo de tiempo:  
     Update_event (Hz) = 72 000 000  / 2 = 36 000 000 Hz
       TIM_Prescaler(ARR) = (3600 - 1)
       TIM_Period   (PSC) = (10 000 - 1)                    */
  
	TIM_TimeBaseInitStruct.TIM_Prescaler = 3599;
	TIM_TimeBaseInitStruct.TIM_Period = 9999;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// Creamos la interrupcion para TIM2
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  
	// Iniciamos el TIM2 programado
	TIM_Cmd(TIM2, ENABLE);
	
	// NVIC  ---------------------------------------------------
  // Nombramos la estructura para configurar la interrupcion
	NVIC_InitTypeDef NVIC_InitStruct;
	// Configuramos los parametros de la interrupcion
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}


/*  Funcion que configura el TIM3   */
//-------------------------------------------------
void TIM3_Config(void)
{
	// TIM3  ------------------------------------------------
  // Crea la estructura del TIMER 3
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* Configuramos los parametros del TIMER_3 para
     detectar un desbordamiento cada vez que cuente
     un periodo de tiempo de 10 HZ  (0,1 seg) 100ms
   -----------------------------------------------------
    Formula para calcular el periodo de tiempo:
     Update Event(Hz) = Timer_CLK (Hz) / Frec_event (Hz)
                       =    72 000 000  /  10   = 7 200 000 Hz
      TIM_Prescaler(ARR) = (7 200 - 1)
      TIM_Period   (PSC) = (1 000 - 1)                     */
  
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	// Creamos la interrupcion para TIM3
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  
	// Iniciamos el TIM3 programado
	TIM_Cmd(TIM3, ENABLE);
	
	// NVIC --------------------------------------------------
  // Nombramos la estructura para configurar la interrupcion
	NVIC_InitTypeDef NVIC_InitStruct;
	// Configuramos los parametros de la interrupcion
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}


/*  Funcion que se ejecutara cuando se detecta el desbordamiento 
    en TIMER 2                                                   */
//-----------------------------------------------------------------
void TIM2_IRQHandler()
{
	// Comprueba si se produce la interrupcion del TIM2 
	if (TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		// Enciende y apaga (Toggle) LED en PB1
		GPIOB->ODR ^= GPIO_Pin_1;    

		// Limpia la bandera de la interrupcion de TIM2 para reiniciarla    
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


/*  Funcion que se ejecutara cuando se detecta el desbordamiento
    en TIMER 3                                                    */
//------------------------------------------------------------------
void TIM3_IRQHandler()
{
	// Comprueba si se produce la interrupcion del TIM3 
	if (TIM_GetITStatus(TIM3, TIM_IT_Update))
	{
		// Enciende y apaga (Toggle) LED en PC13
		GPIOC->ODR ^= GPIO_Pin_13;

		// Limpia la bandera de la interrupcion de TIM3 para reiniciarla    
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


/////////////   Fin del programa  /////////////////////////////////
