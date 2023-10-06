/**********************************************************************
  * Project Name: PWM_2
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
  *  PROGRAMA DE EJEMPLO DE CONFIGURACION DEL PWM
  *
  *  El programa configura como salida el canal 4 del TIM3
  *  en el pin PB1, en el que tambien hemos conectado un LED
  *  al que le ajustamos el brillo con la señal PWM que es
  *  controlada mediante dos pulsadores conectados a los
  *  pines PA0 y PA1.
  *
  *
  *********************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos              
#include "stm32f10x_gpio.h"           
#include "stm32f10x_rcc.h"            
#include "stm32f10x_tim.h"            

// Definimos limite max. para el valor del PERIODO
#define PERIOD 1000  

/*  Modulo principal incluidas las configuraciones GPIO y PWM  */
//===============================================================
int main(void)
{
  int TIM_Pulse = 10;
  
  /* Configuramos GPIO para el LED y Pulsadores  */
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // Iniciamos los clock para cada puerto
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   // TIM3 Clock	

  /*     LED     Pin de salida en PB1         */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // PB1 -> TIM3_CH4  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*    BOTONES  Pines de entrada PA0 & PA1   */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*     Configuracion del TIM                */
  TIM_TimeBaseInitTypeDef TIMER_InitStructure;
  TIM_TimeBaseStructInit(&TIMER_InitStructure);
  TIMER_InitStructure.TIM_Period = PERIOD;
  TIMER_InitStructure.TIM_Prescaler = 10;  
  TIMER_InitStructure.TIM_ClockDivision = 0;
  TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIMER_InitStructure);

  /*      Configuracion pulso PWM  CH4      */
  TIM_OCInitTypeDef TIM_PWM_InitStructure;
  TIM_OCStructInit(&TIM_PWM_InitStructure);  
  TIM_PWM_InitStructure.TIM_Pulse = TIM_Pulse;
  TIM_PWM_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_PWM_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_PWM_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC4Init(TIM3, &TIM_PWM_InitStructure); //TIM_OC4 = TIMx_CH4 (PB1)

  TIM_Cmd(TIM3, ENABLE); // Timer 3
  

  while(1)
  {
	    /* Comprueba pulsacion de los botones */
      // Boton PA0
    	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) {
    		// Comprueba si se alcanza el limite superior
		// PERIOD=1000 si no aumenta uno
		if (TIM_Pulse < PERIOD)
    		   TIM_Pulse++;    		  
           TIM3->CCR4 = TIM_Pulse;   //  TIMx->CCR4 = TIMx_CH4 (PB1)
    	}
      // Boton PA1
    	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0) {
		// Comprueba si se alcanza el 0
		// Si es mayor > 0 disminuye uno
    		if (TIM_Pulse > 0)
    		   TIM_Pulse--;    		
           TIM3->CCR4 = TIM_Pulse;   //  TIMx->CCR4 = TIMx_CH4 (PB1)
    	}

    	/* delay */
    	for(int i=0;i<0x10000;i++);
    }
}

//////////////////////  Fin del Programa   ////////////////////

