#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
//definimos limite maximo periodo
//#define PERIOD 1000

int main(void)
{
	int pasos=10;
	int TIM_Pulse=1000;
	int i;
	//configuramos GPIO para el led y pulsadores
	GPIO_InitTypeDef GPIO_InitStructure;
	//configuramos GPIO del pin de salida PB6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	// les pin de salida PB1
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//los botones se conectan en PA0 y PA1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 |GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//creamos la estructura para el timer4
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//configuracion del TIM
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler=72;
	TIMER_InitStructure.TIM_Period=20000;
	TIMER_InitStructure.TIM_ClockDivision=0;
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIMER_InitStructure);
	// configuracion pulso PWM CH4
	TIM_OCInitTypeDef TIM_PWM_InitStricture;
	TIM_OCStructInit(&TIM_PWM_InitStricture);
	TIM_PWM_InitStricture.TIM_Pulse=TIM_Pulse;
	TIM_PWM_InitStricture.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_PWM_InitStricture.TIM_OutputState=TIM_OutputState_Enable;
	TIM_PWM_InitStricture.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM4,&TIM_PWM_InitStricture);
	TIM_Cmd(TIM4,ENABLE);
	TIM_Pulse=TIM_PWM_InitStricture.TIM_Pulse;
    while(1)
    {
    	//comprueba la pulsacion de los botones
    	// boton PA0
    	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)
    	{
    		//comprueba si se alcanza el limite superior
    		if(TIM_Pulse<3000)
    			TIM_Pulse+=pasos;
    	}
    	//Boton PA1
    	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
    	{
    		//comprueba si se alcanza el limite superior
    		if(TIM_Pulse>100)
    			TIM_Pulse-=pasos;
    	}
		TIM4->CCR1=TIM_Pulse;
    	// se genera un retardo
    	for(i=0;i<0x10000;i++);
    }
}




































