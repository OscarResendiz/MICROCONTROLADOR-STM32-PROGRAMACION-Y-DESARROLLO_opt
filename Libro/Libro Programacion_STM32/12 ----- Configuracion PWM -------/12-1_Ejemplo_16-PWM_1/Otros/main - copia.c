 /**********************************************************************
  * Project Name: PWM
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
  *  PROGRAMA DE EJEMPLO DE GENERAR UNA SEÑAL PWM
	*
  *  Se enciende un apaga un LED conectado a un canal de un
  *  pin de salida Analogica, controlandose mediente una señal
  *  PWM el brillo del LEE.
  *
  *  Conexiones:
  *
  *   LED conectado en PA8 (TIM1_CH1)	
	*
  *********************************************************************/
 
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

// Libreria para generar retardos
#include "delay.h"

void PWM_Config(int arr, int psc, int Pulse);


/*     Modulo principal                    */
//============================================
int main(void)
 {	
	u16 LED_valor=0;   // Varible que contendrá el valor  
	u8 LED_direc=1;	
	 
	Delay_Init(); 
       
  //PWM_Config(1000, 959, 242); // Configuracion para PWM de 72 Hz 
  //PWM_Config(999, 0, 0); // Configuracion para PWM = 72000/ 72 Hz = (999+1) 
  //PWM_Config(899, 0, 0); // Configuracion para PWM = 72000/ 80 kHz = (899+1) 
  PWM_Config(2116, 0, 0);  // Configuracion para PWM = 72000/ 34 kHz = (2116+1) 
  
   
  while(1)
	{
 		Delay_Ms(10);	 
		
		if(LED_direc)LED_valor++;
		
		else LED_valor--;	 
		
 		// Cuando la variable 'LED_Valor' llegue a '300' cambia el sentido del LED
    if(LED_valor>300)
				LED_direc=0;	
    // Cuando la variable 'LED_Valor' llegue a '0' cambia el sentido del LED
		if(LED_valor==0)
				LED_direc=1;	   					 
		
    // Compara el valor resultante en el contador del Timer_1
		TIM_SetCompare1(TIM1,LED_valor);	   
	} 
}

/*  Funcion que confgura los parametros para GPIO, TIMER y TIM_CH  */
void PWM_Config(int arr, int psc, int Pulse)
{  
	/* Creamos y configuramos el GPIO del Pin PA8   */
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* Configuracion del TIMER_1 utilizado  */
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
  TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

  /* Configuramos el tipo de salida para el TIM1_CH1   */	
  TIM_OCInitTypeDef  CHANNEL_InitStructure;  
  CHANNEL_InitStructure.TIM_Pulse = Pulse; 
	CHANNEL_InitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	CHANNEL_InitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	CHANNEL_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM1, &CHANNEL_InitStructure);  

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); 
	
	TIM_Cmd(TIM1, ENABLE);  
    
}

//////////////////   Fin del programa  /////////////////////////

