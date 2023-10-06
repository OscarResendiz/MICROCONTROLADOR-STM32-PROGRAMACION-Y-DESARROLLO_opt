 /**********************************************************************
  * Project Name: TIMER_frec
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
  * PROGRAMA DE PRUEBA DEL TIMER
  * 
  * El programa enciende y apaga un LED conectado PC13, generando
	* una frecuencia con el TIMER de 72 MHz
	*
  **********************************************************************/

// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

// Listado de las funciones suplementarias
void SetSysClockToHSE(void);
void GPIO_Config(void);

/*  Funcion que se produce cuando se detecta la interrupcion  */
void TIM4_IRQHandler(void)
{
        if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
        {
        	// Reseteaamos la badera de la interrupcion para reiniciar la interrupcion
        	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
					GPIOC->ODR ^= GPIO_Pin_13;
        }
}

/*  Modulo principal    */
int main(void)
{
		SetSysClockToHSE();		// Inicializamos los relojes del sistema

		GPIO_Config();				// Inicializamos el GPIO del LED
	
    // Inicializamos el TIMER
	  /*  Creamos las estructuras para TIM y NVIC   */
    TIM_TimeBaseInitTypeDef TIMER_StructureInit;
    NVIC_InitTypeDef NVIC_StructureInit;

    /* Activamos el reloj para el TIM4   */	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

  	TIM_TimeBaseStructInit(&TIMER_StructureInit);
    TIMER_StructureInit.TIM_CounterMode = TIM_CounterMode_Up; // Режим рахунку
       
    TIMER_StructureInit.TIM_Period = 500; 
	TIMER_StructureInit.TIM_Prescaler = 8000; 
    TIM_TimeBaseInit(TIM4, &TIMER_StructureInit);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
  	TIM_Cmd(TIM4, ENABLE);

    /* Configuramos el NVIC      */
    /* Establecemos el TIM4_IRQn interrupcion */
    NVIC_StructureInit.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_StructureInit.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_StructureInit.NVIC_IRQChannelSubPriority = 0;
    NVIC_StructureInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_StructureInit);

    while(1)
    {
    	
    }
}

/* Funcion que configura las frecuencias del roloj SysClock */
void SetSysClockToHSE(void)
{
		ErrorStatus HSEStartUpStatus;
		
		RCC_DeInit();			// Inicializamos el RCC

    RCC_HSEConfig( RCC_HSE_ON);   // Inicializamos el HSE

    // Esperamos a que se active el HSE
	  HSEStartUpStatus = RCC_WaitForHSEStartUp();

    // Comprueba si se produce el evento 
	  if (HSEStartUpStatus == SUCCESS)
    {
        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);

        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div1);

        /* Seleccionamos el HSE como reloj del sistema */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_HSE);

        /* Esperamos hasta que se activa el PLL  */
        while (RCC_GetSYSCLKSource() != 0x04)
        {
        }
    }
    else
    { 
        /* Bucle infinito para que siga funcionando solo el programa   */
        while (1)
        {
        }
    }
}

void GPIO_Config(void)
{
		/* Creamos la estructura para el GPIO del LED */
		GPIO_InitTypeDef  GPIO_StructureInit;
	
		// Activamos el reloj para el GPIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
		/* Configuramos los parametros para el pin PC13 */
		GPIO_StructureInit.GPIO_Pin = GPIO_Pin_13;
		GPIO_StructureInit.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_StructureInit.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_StructureInit);

		GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Apagamos el PC13
}

/*********************   Fin del programa   **************************/

