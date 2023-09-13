#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
//definimos limite maximo periodo
#define PERIOD 1000

int main(void)
{
	int TIM_Pulse=500;
	int i;
	//configuramos GPIO para el led y pulsadores
	GPIO_InitTypeDef GPIO_InitStructure;
	//iniciamos los clock para cada puerto
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//los botones se conectan en PA0 y PA1
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 |GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	// les pin de salida PB1
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//configuracion del TIM
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler=800;
	TIMER_InitStructure.TIM_Period=PERIOD;
	TIMER_InitStructure.TIM_ClockDivision=0;
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIMER_InitStructure);
	// configuracion pulso PWM CH4
	TIM_OCInitTypeDef TIM_PWM_InitStricture;
	TIM_OCStructInit(&TIM_PWM_InitStricture);
	TIM_PWM_InitStricture.TIM_Pulse=TIM_Pulse;
	TIM_PWM_InitStricture.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_PWM_InitStricture.TIM_OutputState=TIM_OutputState_Enable;
	TIM_PWM_InitStricture.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC3Init(TIM3,&TIM_PWM_InitStricture);
	TIM_Cmd(TIM3,ENABLE);
    while(1)
    {
    	//comprueba la pulsacion de los botones
    	// boton PA0
    	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)
    	{
    		//comprueba si se alcanza el limite superior
    		if(TIM_Pulse<PERIOD)
    			TIM_Pulse++;
    		TIM3->CCR3=TIM_Pulse;
    	}
    	//Boton PA1
    	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
    	{
    		//comprueba si se alcanza el limite superior
    		if(TIM_Pulse>0)
    			TIM_Pulse--;
    		TIM3->CCR3=TIM_Pulse;
    	}
    	// se genera un retardo
    	for(i=0;i<0x10000;i++);
    }
}




































