 /**********************************************************************
  * Project Name: PWM_1
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
  *  PROGRAMA DE EJEMPLO DE CONFIGURACION DE SEÑAL PWM
  *
  *  En el ejemplo se configura el canal 1 del TIMER 1
  *  que corresponde al pin PA8 donde saldra una señal
  *  PWM a diferentes frecuencias y ciclo de trabajo.
  *   
  ********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h" 
#include "stm32f10x_rcc.h"  
#include "stm32f10x_tim.h"  

void TIM1_PWM_Config(u16 arr,u16 psc, u16 pulse);


/*          Modulo   Principal           */
//=========================================
int main(void)
{		
  
	//TIM1_PWM_Config(7199, 999, 50);	// 10 Hz 50%
  //TIM1_PWM_Config(2399, 999, 25);	// 30 Hz 25%
  TIM1_PWM_Config(3599, 0, 15);  	  // 20 kHz 15%
  //TIM1_PWM_Config(959, 0, 25);	  // 75 kHz 25%
	 
  
   while(1)
	{		
    ;;
	} 
}

/*       Funcion que configura el TIMER y el pulso PWM   */
//----------------------------------------------------------
void TIM1_PWM_Config(u16 arr,u16 psc, u16 pulse)
{  
	/*  Creamos las estructuras para GPIO, TIM y PWM     */
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_TIM1, ENABLE);
  //  Configuracion del pin PA8 canal 1 del TIM1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  // Configuracion TIM1 --------------------------------------
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

  // Configuracion PWM --------------------------------------
	TIM_OCInitStructure.TIM_Pulse = (uint16_t)(((arr+1)*pulse)/100)-1;   
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); // Canal 1 TIM1

  // Iniciamos el modulo de salida PWM
  TIM_CtrlPWMOutputs(TIM1,ENABLE);	

  //Activa el registro de precarga de periféricos TIMx en CCR1
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); 
	
  // Iniciamos el TIM1
	TIM_Cmd(TIM1, ENABLE);    
}


//////////////////////////  Fin del programa   ////////////////////////
