#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

//funcion que configura el timer y el pulso PWM
void TIM1_PWM_Config(u16 arr,u16 psc, u16 pulse)
{
	//creamos la estructura para el GPIO, TIM y PWM
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef Tim_TimBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_TIM1,ENABLE);
	//configuracion del pin PA8 canal 1 del tim1
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//configuracion del TIM1
	Tim_TimBaseInitStructure.TIM_Period=arr;
	Tim_TimBaseInitStructure.TIM_Prescaler=psc;
	Tim_TimBaseInitStructure.TIM_ClockDivision=0;
	Tim_TimBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&Tim_TimBaseInitStructure);

	//configuracion del PWM
	TIM_OCInitStructure.TIM_Pulse=(uint16_t)(((arr+1)*pulse)/100)-1;
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM1,&TIM_OCInitStructure); //canal1 tim1

	//iniciamos el modulo de salida PWM
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	//iniciamos el TIM1
	TIM_Cmd(TIM1,ENABLE);
}
int main(void)
{
	//iferentes configuraciones para probar diferentes frecuencias
	TIM1_PWM_Config(7199,999,50); //10 Hz 50%
	//TIM1_PWM_Config(2399,999,25); //30 Hz 25%
	//TIM1_PWM_Config(3599,0,15); //20 KHz 15%
	//TIM1_PWM_Config(959,0,25); // 75 kHz 25%

    while(1)
    {
    }
}
