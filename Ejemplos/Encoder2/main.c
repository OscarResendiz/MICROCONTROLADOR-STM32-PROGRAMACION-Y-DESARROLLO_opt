#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_tim.h"
#include<misc.h>
#include<stdio.h>
#include "usart1.h"

//lista de funciones empleadas
void TIM4_Config(void);
void TIM4_IRQHandler(void);
void GPIO_Config(void);
void configuraEncoder();
void leeEncoder();
void configuraLeds();
//funciones de eventos de cambio de estado del encoder
void MuestraLeds(void);
void CLK_Change(int estado);
void DT_Change(int estado);
void SW_Change(int estado);
void EstadoInicialEncoder();
void ValidaEstadoEncoder();
void IncrementaEncoder();
void DecrementaEncoder();
void Delay_ms(unsigned int nCount);
void muestraEncoder();
//creo mis constantes
//pines del encoder
#define CLK GPIO_Pin_0
#define DT GPIO_Pin_1
#define SW GPIO_Pin_3
#define PUERTO_ENCODER GPIOB
//leds que representan los pines del encoder
#define LED_CLK GPIO_Pin_13
#define LED_DT GPIO_Pin_14
#define LED_SW GPIO_Pin_15
#define PUERTO_LEDS GPIOC
//variables para controlar el estado del encoder
int estado=0; //indica el estado de su maquina de estados
int valorEncooder=50; //valor actual del encoder
int MaximoEncoder=100; //valor maximo que puede tener el encoder
int MinimoEncoder=-100; //valor minimo que puede tener el encoder
//CLK
int CLK_status;
int CLK_statusAnterior;
// DT
int DT_status;
int DT_statusAnterior;
//switch
int SW_status;
int SW_statusAnterior;
char mensaje[100];
int intentos=0;
int main(void)
{
	USART1_Config();
	GPIO_Config();
	EstadoInicialEncoder();
	print("Esto es una prueba del encoder 2\r\n");
	TIM4_Config();
    while(1)
    {
    	//print("Esto es una prueba del encoder\r\n");
    	//no se hace nada
//    	sprintf(mensaje,"conteo: %d\r\n",intentos);
  //  	print(mensaje);
	//	Delay_ms(100);
		//intentos++;
    }
}
/*funcion que genera un retardo en milisegundos*/
void Delay_ms(unsigned int nCount)
{
	unsigned int i,j;
	for(i=0;i<nCount;i++)
	{
		for(j=0;j<0x2AFF;j++)
		{
			/*no hace nada sol perder unos siclos de relog*/
			;
		}
	}
}

//funcion que configura el GPIO del led en PC13
void GPIO_Config(void)
{
	configuraLeds();
	configuraEncoder();
}

void configuraLeds()
{
	//configurcmos el GPIO pr el led PC13
	GPIO_InitTypeDef GPIO_InitStructure;
	uint32_t RCC_APB2Periph_GPIOX;
	//selecciono uno de los tres puertos a utilizar
	if(PUERTO_LEDS==GPIOA)
		RCC_APB2Periph_GPIOX=RCC_APB2Periph_GPIOA;
	if(PUERTO_LEDS==GPIOB)
		RCC_APB2Periph_GPIOX=RCC_APB2Periph_GPIOB;
	if(PUERTO_LEDS==GPIOC)
		RCC_APB2Periph_GPIOX=RCC_APB2Periph_GPIOC;
	//iniciamos el relog para el puerto C
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX,ENABLE);
	//Configuro los pines de salida que van a reflejar el estado del encoder
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=LED_CLK;
	GPIO_Init(PUERTO_LEDS,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=LED_DT;
	GPIO_Init(PUERTO_LEDS,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=LED_SW;
	GPIO_Init(PUERTO_LEDS,&GPIO_InitStructure);
}
//funcion que configura las entradas utilizadas para el encoder
void configuraEncoder()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint32_t RCC_APB2Periph_GPIOX;
	//selecciono uno de los tres puertos a utilizar
	if(PUERTO_ENCODER==GPIOA)
		RCC_APB2Periph_GPIOX=RCC_APB2Periph_GPIOA;
	if(PUERTO_ENCODER==GPIOB)
		RCC_APB2Periph_GPIOX=RCC_APB2Periph_GPIOB;
	if(PUERTO_ENCODER==GPIOC)
		RCC_APB2Periph_GPIOX=RCC_APB2Periph_GPIOC;
	//activo el relog del puerto seleccionado
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX,ENABLE);
	//configuro los pines como entrada
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	//asigno los pines a utilizar
	GPIO_InitStructure.GPIO_Pin=CLK||DT||SW;
	GPIO_Init(PUERTO_ENCODER,&GPIO_InitStructure);

}
//funcion que se ejecuta cuando se detecta el fin del periodo
void TIM4_Config(void)
{
	//inicializa el reloj para el timer 4
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	// inicializa el reloj para el timer4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//detecta un desbordamiento cada vez que cuente un periro de tiempo que sera cada 1000 por segundo
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler=0;//8000;
	TIMER_InitStructure.TIM_Period=65535;//500;
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIMER_InitStructure);

	//creamos la interrupcion del timer4
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	//configuramos la interrupcion
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//funcion que se ejecuta cuando se detecta la interrupcion
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		leeEncoder();
		//resetea la bandera de la interrupcion
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		//parpadea el led
		//GPIOC->ODR^=GPIO_Pin_13;
	}
}
void leeEncoder()
{
	//verifica i el pin0 esta en nibel bajo
	CLK_status=GPIO_ReadInputDataBit(PUERTO_ENCODER,CLK);
	DT_status=GPIO_ReadInputDataBit(PUERTO_ENCODER,DT);
	SW_status=GPIO_ReadInputDataBit(PUERTO_ENCODER,SW);
	MuestraLeds();
		//veo si hay cambios en los estados del encoder
		if(CLK_status!=CLK_statusAnterior)
		{
			CLK_statusAnterior=CLK_status;
			ValidaEstadoEncoder();
			CLK_Change(CLK_status);
		}
		if(DT_status!=DT_statusAnterior)
		{
			DT_statusAnterior=DT_status;
			ValidaEstadoEncoder();
			DT_Change(DT_status);
		}
		if(SW_status!=SW_statusAnterior)
		{
			SW_statusAnterior=SW_status;
			ValidaEstadoEncoder();
			SW_Change(SW_status);
		}
}
//funcion que muestra el estado del encoder en los leds
void MuestraLeds()
{
	if(CLK_status==0 )
		GPIO_ResetBits(PUERTO_LEDS,LED_CLK);
	else
		GPIO_SetBits(PUERTO_LEDS,LED_CLK);
	if(DT_status==0 )
		GPIO_ResetBits(PUERTO_LEDS,LED_DT);
	else
		GPIO_SetBits(PUERTO_LEDS,LED_DT);
	if(SW_status==0 )
		GPIO_ResetBits(PUERTO_LEDS,LED_SW);
	else
		GPIO_SetBits(PUERTO_LEDS,LED_SW);
}
//lee el estado del encoder y establece el estado inicial
void EstadoInicialEncoder()
{
	CLK_status=GPIO_ReadInputDataBit(PUERTO_ENCODER,CLK);
	DT_status=GPIO_ReadInputDataBit(PUERTO_ENCODER,DT);
	SW_status=GPIO_ReadInputDataBit(PUERTO_ENCODER,SW);
	CLK_statusAnterior=CLK_status;
	DT_statusAnterior=DT_status;
	SW_statusAnterior=SW_status;
	MuestraLeds();
	//establesco el estado inicial
	if(DT_status==0 && CLK_status==0 )
		estado=2;
	if(DT_status==1 &&CLK_status==1 )
		estado=1;
}
//se dispara cuando cambia CLK
void CLK_Change(int estado)
{
}
//se dispara cuando cambia DT
void DT_Change(int estado)
{

}
// se llama cuando cambia SW
void SW_Change(int estado)
{

}
void IncrementaEncoder()
{
	valorEncooder++;
	if(valorEncooder>MaximoEncoder)
		valorEncooder=MaximoEncoder;
	muestraEncoder();
}
void DecrementaEncoder()
{
	valorEncooder--;
	if(valorEncooder<MinimoEncoder)
		valorEncooder=MinimoEncoder;
	muestraEncoder();
}
//valida y lleva el control del estado del encoder
void ValidaEstadoEncoder()
{
	/*
	 la tabla de estados de deteccion del encoder es la siguiente

	 EstadoActual	DT	CLK	SIgienteEstado	Accion
		0			0	0		2
		0			1	1		1
		1			1	0		3
		1			0	1		6
		2			0	1		9
		2			1	0		12
		3			0	0		4			++
		4			0	1		5
		5			1	1		1			++
		6			0	0		7			--
		7			1	0		8
		8			1	1		1			--
		9			1	1		10			++
		10			1	0		11
		11			0	0		2			++
		12			1	1		13			--
		13			0	1		14
		14			0	0		2			--
	*/
	switch(estado)
	{
	case 1:
		if(DT_status==1 &&CLK_status==0 )
		{
			estado=3;
		}
		if(DT_status==0 &&CLK_status==1 )
		{
			estado=6;
		}
		break;
	case 2:
		if(DT_status==0 &&CLK_status==1 )
		{
			estado=9;
		}
		if(DT_status==1 &&CLK_status==0 )
		{
			estado=12;
		}
		break;
	case 3:
		if(DT_status==0 &&CLK_status==0 )
		{
			estado=4;
			IncrementaEncoder();
		}
		break;
	case 4:
		if(DT_status==0 &&CLK_status==1 )
		{
			estado=5;
		}
		break;
	case 5:
		if(DT_status==1 &&CLK_status==1 )
		{
			estado=1;
			IncrementaEncoder();
		}
		break;
	case 6:
		if(DT_status==0 &&CLK_status==0 )
		{
			estado=7;
			DecrementaEncoder();
		}
		break;
	case 7:
		if(DT_status==1 &&CLK_status==0 )
		{
			estado=8;
		}
		break;
	case 8:
		if(DT_status==1 &&CLK_status==1 )
		{
			estado=1;
			DecrementaEncoder();
		}
		break;
	case 9:
		if(DT_status==1 &&CLK_status==1 )
		{
			estado=10;
			IncrementaEncoder();
		}
		break;
	case 10:
		if(DT_status==1 &&CLK_status==0 )
		{
			estado=11;
		}
		break;
	case 11:
		if(DT_status==0 &&CLK_status==0 )
		{
			estado=2;
			IncrementaEncoder();
		}
		break;
	case 12:
		if(DT_status==1 &&CLK_status==1 )
		{
			estado=13;
			DecrementaEncoder();
		}
		break;
	case 13:
		if(DT_status==1 &&CLK_status==1 )
		{
			estado=14;
		}
		break;
	case 14:
		if(DT_status==0 &&CLK_status==0 )
		{
			estado=2;
			DecrementaEncoder();
		}
		break;
	}

}
//muestra el valor del encoder por el puerto serial
void muestraEncoder()
{
	sprintf(mensaje,"encoder: %d\r\n",valorEncooder);
	print(mensaje);

}



















