/**********************************************************************
  * Project Name: PWM_ServoMotor
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
  * PROGRAMA DE PREAB DE SENAL PWM CONTROLANDO UN SEVOMOTOR
  * 
  * Se controla el movimiento de un SERVO MOTOR conectado al
  * pin PB6 mediante unos PULSADORES conectados a los pines
  * PA0 y PA1
  *
  *********************************************************************/
 // Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

/*            Modulo principal               */
//=============================================
int main(void)
{
	int TIM_Pulse = 1000;	
	
	 /*  Creamos las estructuras para GPIO   */
  GPIO_InitTypeDef GPIO_InitStructure;

	/*  Configuramos GPIO del pin de Salida PB6  */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /*  Configuramos GPIO de los pines PA0 y PA1 de los botones  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*   Creamos la estructura para TIMER4   */
  TIM_TimeBaseInitTypeDef TIMER_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler = 72;	
  TIMER_InitStructure.TIM_Period = 20000;
	TIMER_InitStructure.TIM_ClockDivision = 0;
	TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);
 
  /*  Configuracion del PWM   */
  TIM_OCInitTypeDef TIM_PWM_InitStructure;
	TIM_OCStructInit(&TIM_PWM_InitStructure);
	TIM_PWM_InitStructure.TIM_Pulse = TIM_Pulse;
	TIM_PWM_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_PWM_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_PWM_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_PWM_InitStructure);

  TIM_Cmd(TIM4, ENABLE);
  
	TIM_Pulse = TIM_PWM_InitStructure.TIM_Pulse;

    while(1)
    {
      /*  Comprobamos la pulsacion de los botones    */
      
      // Pulsador PA0
    	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) {
        // Se comprueba si se sobrepasa un tope de 3000    		
        if (TIM_Pulse < 3000)
          TIM_Pulse++;    // Incrementamos la variable de TIM_Pulse
    	}
      
      // Pulsador PA1
    	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0) {
        // Se comprueba si se sobrepasa un tope de 100    		
        if (TIM_Pulse > 100)
    			TIM_Pulse--;   // Disminuimos la variable TIM_Pulse
    	}
      
      // Pasamos el valor de la variable 'TIM_Pulse'      
      TIM4->CCR1 = TIM_Pulse;
   	  
      // Se reproduce un delay
   	  for(int i=0;i<0x1000;i++);
    }
}


////////////////////////  Fin del Programa   ////////////////////

