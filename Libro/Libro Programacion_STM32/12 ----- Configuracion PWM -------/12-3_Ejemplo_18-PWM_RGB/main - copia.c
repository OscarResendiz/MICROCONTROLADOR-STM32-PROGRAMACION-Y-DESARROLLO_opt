/**********************************************************************
  * Project Name: PWM_RGB
  * File Name: main.c
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
  * PROGRAMA DE PRUEBA DE SENAL PWM
  *
  *  Prueba el funcionamiento de un LED Tricolor RGB conectado
  *  a los pines PB6, PB7 y PB8 y mediante señales PWM se va
  *  modificando el color resultante 
  *
  * Conexiones:
  *
  *   STM32      LED
  *   PB6  ----  Rojo
  *   PB7  ----  Verde
  *   PB8  ----  Azul
  *   GND  ----  GND
  *
  *************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

void espera(unsigned int nCount);

#define PERIOD 1000

int main(void)
{
	int TIM_Pulse_R = 0;
	int TIM_Pulse_G = 0;
	int TIM_Pulse_B = 0;

	/*  Creamos las estructuras GPIO y TIM  */
	GPIO_InitTypeDef GPIO_InitStructure;
  
	/* Activamos los clock correspondientes */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* Configuramos los GPIO de los piones de salida
     PB6_Red	PB7_Green  PB8_Blue  */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*   Configuracion del TIM   */
  TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler = 720;
	TIMER_InitStructure.TIM_Period = PERIOD;// Periodo(1000) Max
	TIMER_InitStructure.TIM_ClockDivision = 0;
	TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);

	/*  Configuracion del PWM   */
  TIM_OCInitTypeDef TIM_PWM_InitStructure;
	TIM_OCStructInit(&TIM_PWM_InitStructure);
	TIM_PWM_InitStructure.TIM_Pulse = 0;
	TIM_PWM_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_PWM_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_PWM_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_PWM_InitStructure);   // TIM4_CH1 PB6
	TIM_OC2Init(TIM4, &TIM_PWM_InitStructure);   // TIM4_CH2 PB7
	TIM_OC3Init(TIM4, &TIM_PWM_InitStructure);   // TIM4_CH3 PB8

    TIM_Cmd(TIM4, ENABLE);

    while(1)
    {
    	
			TIM_Pulse_R++;
    	if (TIM_Pulse_R > PERIOD)
    		TIM_Pulse_R = 0;

    	TIM_Pulse_G +=2;
    	if (TIM_Pulse_G > PERIOD)
    		TIM_Pulse_G = 0;

    	TIM_Pulse_B +=4;
    	if (TIM_Pulse_B > PERIOD)
    		TIM_Pulse_B = 0;

    	TIM4->CCR1 = TIM_Pulse_R; // TIM4_CH1 PB6      
    	TIM4->CCR2 = TIM_Pulse_G; // TIM4_CH2 PB7
        TIM4->CCR3 = TIM_Pulse_B; // TIM4_CH3 PB8
    	espera(200);
    }
}


// Funcion de espera
void espera(unsigned int nCount)
{
	unsigned int i, j;
	for (i = 0; i < nCount; i++)
		for (j = 0; j < 0x2AFF; j++);
}

/////////////////  Fin del Programa   /////////////////////////////

