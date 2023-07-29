#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
//libreria para el manejo del pruerto serrial
#include "usart1.h"
//libreria complementaria para el maneo del comando sprintf
#include<stdarg.h>
#include<misc.h>
//lista de funciones utilizadas
void GPIO_Config(void);
void TIM_Config(void);
void TIM4_IRQHandler(void);
//lista de variables empleadas
volatile int Timelapsed; //variable que guarda el tiempo final transcurrido
volatile int TimeSec;
volatile uint8_t TimeState=0;
float TimeSegundos=0;
int minutos=0;
int horas=0;
//modulo principal
int main(void)
{
	GPIO_Config();
	TIM_Config();
	USART1_Config();
	char buffer[80]={'\0'};

    while(1)
    {
    	if(TimeState==0)
    	{
    		//comprueba si se ha pulsado el boton PB0
    		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
    		{
    			TIM_Cmd(TIM4,ENABLE);
    			TIM_SetCounter(TIM4,0);
    			TimeSec=0;
    			//establecemos bandera de tiempo inicial =1 (on)
    			TimeState=1;
    			//apagamos led testigo en el PC13
    			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
    			//imprimimos un mensaje de inicio por USART1
    			print("Inicio...");
    		}
    	}
    	if(TimeState==1)
    	{
    		//comprueba si se ha pulsado el boton PB0
    		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
    		{
    			//sumamos en la variable TimeLapse el tiempo transcurrido
    			Timelapsed=TIM_GetCounter(TIM4)/10+TimeSec*1000;
    			TIM_Cmd(TIM4,DISABLE);
    			//badera de tiempo transcurrido finalizado
    			TimeState=0;
    			//encendemos led testigo en el PC13
    			GPIO_SetBits(GPIOC,GPIO_Pin_13);
    			//imprimimos el tiempo transcurriso por USART1
    			TimeSegundos=Timelapsed*0.001;
//    			sprintf(buffer,"Tiempo transcurrido: %d ms",Timelapsed);
  //  			print(buffer);
    //			sprintf(buffer,"-> %4.2f Seg. \r\n",TimeSegundos);
    	//		print(buffer);
    			minutos=0;
    			if(TimeSegundos>60)
    			{
    				minutos=(int)TimeSegundos/60;
    				TimeSegundos=TimeSegundos-(minutos*60);
    			}
    			horas=0;
    			if(minutos>60)
    			{
    				horas=minutos/60;
    				minutos=minutos-(horas*60);
    			}
    			sprintf(buffer,"Tiempo transcurrido %d Horas %d Minutos  %4.2f Seg. \r\n",horas,minutos,TimeSegundos);
    			print(buffer);

    		}
    	}
    }
}

void GPIO_Config(void)
{
	//configurcmos el GPIO pr el led PC13
	GPIO_InitTypeDef GPIO_InitStructure;
	//inicimos el reloj pr el GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//configuramos el GPIO del pin 13 para el led
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	//configuramos el gpio para pb0 y pb1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//configuramos lo pines pr los pulsadores PB0 y PB1
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
//funcion ue configur el timer 4
void TIM_Config(void)
{
	//crea la estructur deltimer 4
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	//inicializa el reloj para el timer 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//configuramos los parametros del timer4
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler=7200;
	TIMER_InitStructure.TIM_Period=10000;
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIMER_InitStructure);
	//cremos la interrupcion para timer 4
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	//configuramos el NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//funcion ue se ejecut cundo se detect la primer interrupcion
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		//se va incrementndo l variable del tiempo
		TimeSec++;
	}
}


































