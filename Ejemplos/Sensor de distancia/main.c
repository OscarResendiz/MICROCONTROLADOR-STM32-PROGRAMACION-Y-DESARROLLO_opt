#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
//libreria para el manejo del pruerto serrial
#include "usart1.h"
//libreria para las interrupciones externas
#include "stm32f10x_exti.h"

//libreria complementaria para el maneo del comando sprintf
#include<stdio.h>
#include<stdarg.h>
#include<misc.h>
//lista de funciones utilizadas
void GPIO_Config(void);
void TIM_Config(void);
void EXTI0_IRQHandler(void);
//lista de variables empleadas
volatile uint8_t senal_ECHO=0;
volatile int_fast16_t Valor_Scanner;

//funcion que genera la señal de disparo
void HCSR04_start()
{
	int i;
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	for(i=0;i<0x7200;i++);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}
//funcion que cuenta el tiempo transcurrido
unsigned int HCSR04_get()
{
	unsigned long Escanner;
	Escanner=(unsigned long) Valor_Scanner/75;
	return (unsigned int)Escanner;
}
//modulo principal
int main(void)
{
	USART1_Config();
	GPIO_Config();
	TIM_Config();
	print("Hola\r\n");
	char buffer[80]={'\0'};

    while(1)
    {
    	//comprobamos si la señal echo a legado
    	if(senal_ECHO==1)
    	{
    		//imprimimos por el puerto usart el valor obtenido
			sprintf(buffer,"Distancia %d cm\r\n",HCSR04_get());
			print(buffer);
			//desactivamos la bandera de deteccion de la señal echo
			senal_ECHO=0;
    	}
    }
}
//funcion que configura el GPIO
void GPIO_Config(void)
{
	//configurcmos el GPIO pr el led PC13
	GPIO_InitTypeDef GPIO_InitStructure;
	//inicimos el reloj pr el GPIOC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	//creamos señal trigger en pin PB15 coo salida
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//creamos la señal echo en pin PB0 como estrada
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//EXTI en PB0
	EXTI_InitTypeDef EXTI_InitStruccture;
	NVIC_InitTypeDef NVIC_InitStructure;
	// iniciamos el relog para AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//creamos la IRQ del NVIC
	//configuraos el pin PB0 conectado como EXTI_Line0
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//llamamos el pin PB0 para la interrupcion EXTI_line0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	//configuramos el PD0 para EXTI_Line0
	EXTI_InitStruccture.EXTI_Line=EXTI_Line0;
	//iniciamos la interrupcion
	EXTI_InitStruccture.EXTI_LineCmd=ENABLE;
	//configuramos el modo de la interrupcion
	EXTI_InitStruccture.EXTI_Mode=EXTI_Mode_Interrupt;
	//configuramos para detectar flanco desendente
	EXTI_InitStruccture.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	//iniciamos la interrupcion
	EXTI_Init(&EXTI_InitStruccture);
}
//funcion ue configur el timer 3
void TIM_Config(void)
{
	//crea la estructur deltimer 3
	TIM_TimeBaseInitTypeDef timer_base;
	//configuramos los parametros del timer3
	TIM_TimeBaseStructInit(&timer_base);
	timer_base.TIM_CounterMode=TIM_CounterMode_Up;
	timer_base.TIM_Prescaler=72;
	TIM_TimeBaseInit(TIM3,&timer_base);
	TIM_Cmd(TIM3,ENABLE);
	//inicializa el reloj para el timer 4
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMER_InitStructure.TIM_Prescaler=7200;
	TIMER_InitStructure.TIM_Period=5000;
	TIM_TimeBaseInit(TIM4,&TIMER_InitStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	//creamos la interrupcion del timer
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//funcion que se ejecuta cuando la interrupcion es detectada
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		HCSR04_start();
		senal_ECHO=1;
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}
void EXTI0_IRQHandler(void)
{
	//COMPRUEBA SI LA BANDERA DE ESTADO ESTA ACTIVA
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		//comprueba si el PB0 esta activo
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)!=0)
		{
			//inicia el conteo del tiempo
			TIM_SetCounter(TIM3,0);
		}
		//comprueba si el PB0 se ha desactivado
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
		{
			//inicia el conteo del tiempo
			Valor_Scanner=TIM_GetCounter(TIM3);
		}
		//reinicia la interrupcion
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}













