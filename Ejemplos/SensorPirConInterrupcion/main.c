#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
//libreria para el control de las interrupciones externas
#include "stm32f10x_exti.h"
//luberia para el manejo del USART4
#include "usart1.h"
//libreria para manejo de variables
#include <stdio.h>
#include "misc.h"
//lista de funciones utilizadas
void GPIO_Config(void);
void TIM_Config(void);
//void EXTI0_IRQHandler(void);
//lista de variables empleadas
volatile uint8_t senal_ECHO=0;
volatile uint16_t Valor_Escanner;
//funcion que genera la señal del trigger

 void HCSR04_start()
{
	int i;
	//activamos PB15
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	//contador para producir retardo
	for(i=0;i<0x7200;i++);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}

//funcion que cuenta el tiempo transcurrido
unsigned int HCSR04_get()
{
	unsigned long Escanner;
	Escanner=(unsigned long ) Valor_Escanner/75;
	return (unsigned int)Escanner;
}

//modulo principal
int main(void)
{
	char texto[100];
	//configuramos el GPIO de señales trigger y echo
	GPIO_Config();
	//configuramos la comunicacion por USART1
	USART1_Config();
	//configuramos e iniciamos el timer
	TIM_Config();

    while(1)
    {
		//print("\r\nHola");
    	//comprobamos si la señal echo se ha recibido
    	if(senal_ECHO==1)
    	{
    		//imprimimos por el puerto USART1 el valor obtenido
    		sprintf(texto,"Distancia %d cm\r\n",HCSR04_get());
    		print(texto);
    		//desactivamos la bandera de deteccion de la señan echo
    		senal_ECHO=0;
    	}
    }
}

//funcion que configura el GPIO
void GPIO_Config(void)
{
	//configuramos GPIO para el led PC13
	GPIO_InitTypeDef GPIO_InitStructure;
	//iniciamos el relog para el GPIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	//creamos señal trigger en pin PB13 como salida
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//creamos señal echo en pin PB0 como entrada
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//habilitamos el reloj para el remapeo de PB0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//creamos el GPIO para el pin PB0 dela EXTI_Line0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	//configuramos los parametros de la EXTI_0
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	//iniciamos la interrupcion
	EXTI_Init(&EXTI_InitStructure);

	//creamos la estructura para el EXTI en PB0 y el NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	//creamos en NVIC para la IRQ del EXTI 0
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//Funcion que configura los timers 3 y 4
void TIM_Config(void)
{
	//creamos la configuracion para el timer 4
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;

	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMER_InitStructure.TIM_Prescaler=72;
	TIM_TimeBaseInit(TIM3,&TIMER_InitStructure);
	TIM_Cmd(TIM3,ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMER_InitStructure.TIM_Prescaler=7200;
	TIMER_InitStructure.TIM_Period=5000;
	TIM_TimeBaseInit(TIM4,&TIMER_InitStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);

	//creamos la interrupcion del timer 4
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

//funcion que se ejecuta cuando se detecta la interrupcion EXTI_0
void EXTI0_IRQHandler(void)
{
	//comprueba si la bandera de estado esta activa
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		//comprueba si PB0 esta activo
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)!=0)
		{
			//inicia el conteo del tiempo
			TIM_SetCounter(TIM3,0);
		}
		//comprueba si el PB0 se ha desactivado
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
		{
			//pasa el valor del conteo obtenido ala variable escanner
			Valor_Escanner=TIM_GetCounter(TIM3);
		}
		//reinicia la interrupcion
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

