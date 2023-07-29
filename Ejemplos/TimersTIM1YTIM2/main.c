#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"
//lista de funciones empleadas
void GPIO_Config(void);
void TIM2_Config(void);
void TIM3_Config(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
//modulo principall
int main(void)
{
	GPIO_Config(); //inicializamos los GPIOs
	TIM2_Config(); // inicializa el timer 2
	TIM3_Config(); // inicializa el timer 3
    while(1)
    {
    	;
    }
}
void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	//configura el pin PB1 como salida para el LED1
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	 // configura el PC12 como salida para el led 2
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
// funcion que configura el TIM2

void TIM2_Config(void)
{
	//crea la estructura del TIMER 2
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//configuramos los parametros del TIMER 2 para detectar un desbordamiento cada que cuente
	//un periodo de tiempo de 2 HZ
	//formula para calcular el periodo del tiempo
	// update_event (HZ)=72222222/2=36000000
	//TIM_Prescaler (ARR)=3600-1
	//TIM_Period(PSC)=(10000-1)
	TIM_TimeBaseInitStruct.TIM_Prescaler=3599;
	TIM_TimeBaseInitStruct.TIM_Period=9999;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	//creamos la interrupcion para TIM2
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//iniciamo el TIM2 programado
	TIM_Cmd(TIM2,ENABLE);
	//NVIC
	//Nombramos la estructura para configurar la interrupcion
	NVIC_InitTypeDef NVIC_InitStructure;
	//configuramos los parametros de la interrupcion
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

// funcion que configura el TIM3

void TIM3_Config(void)
{
	//crea la estructura del TIMER 3
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//configuramos los parametros del TIMER 3 para detectar un desbordamiento cada que cuente
	//un periodo de tiempo de 10 HZ
	//formula para calcular el periodo del tiempo
	// update_event (HZ)=72222222/10=7200000
	//TIM_Prescaler (ARR)=7200-1
	//TIM_Period(PSC)=(10000-1)
	TIM_TimeBaseInitStruct.TIM_Prescaler=7199;
	TIM_TimeBaseInitStruct.TIM_Period=9999;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	//creamos la interrupcion para TIM2
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//iniciamo el TIM2 programado
	TIM_Cmd(TIM3,ENABLE);
	//NVIC
	//Nombramos la estructura para configurar la interrupcion
	NVIC_InitTypeDef NVIC_InitStructure;
	//configuramos los parametros de la interrupcion
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//funcion que se ejecutara cuando se detecte el desbordamiento del TIMER 2
void TIM2_IRQHandler(void)
{
	//comprueba si se produce la interrupcion del TIM2
	if(TIM_GetITStatus(TIM2,TIM_IT_Update))
	{
		//enciende y apaga (toggle) el led en PB1
		GPIOB->ODR^=GPIO_Pin_1;
		//limpia la bandera de la interrupcion del TIM2 para reiniciarla
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

//funcion que se ejecutara cuando se detecte el desbordamiento del TIMER 3
void TIM3_IRQHandler(void)
{
	//comprueba si se produce la interrupcion del TIM2
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))
	{
		//enciende y apaga (toggle) el led en PB1
		GPIOC->ODR^=GPIO_Pin_13;
		//limpia la bandera de la interrupcion del TIM2 para reiniciarla
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}






























