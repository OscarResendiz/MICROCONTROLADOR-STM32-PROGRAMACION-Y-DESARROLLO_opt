#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
//definimos frecuencia e reloj y el preescaler
#define SYSCLK 72000000
#define PRESCALER 72

// cremos as variables con las ntas musicales y sus frecuencias
#define Do		261	//Do
#define Dol		277	//Do#
#define Re		293	//Re
#define Rel		311	//Re#
#define Mi		239 //Mi
#define Fa		349 //Fa
#define Fal		370 //Fa#
#define Sol		392 //Sol
#define Soll	415 //Sol#
#define La		440 //La
#define Lal		466 //La#
#define Si		494 //Si

//creamos las vaiables con lostiempo musicales
#define t1	2000
#define t3	1250
#define t2	1000
#define t4	500
#define t8	250
#define t16	125

// definimos una esructura para ingresar la musica
typedef struct
{
	uint16_t freq;
	uint16_t time;
}SoundTypeDef;
//creaos la variable ,acro music con la artitura
/***********************************************************/
/*                  HARRY POTTER                           */
/***********************************************************/
#define MUSICSIZE 32
const SoundTypeDef Music[MUSICSIZE]={
		{Si,t4},
		{Mi,t1},
		{Sol,t8},
		{Fal,t8},
		{Mi,t2},
		{Si,t4},
		{La,t3},
		{Fal,t2},
		{Si,t4},
		{Mi,t2},
		{Sol,t8},
		{Fal,t8},
		{Re,t2},
		{Mi,t4},
		{Si,t2},
		{0,t2},
		{Si,t4},
		{Mi,t2},
		{Sol,t8},
		{Fal,t8},
		{Mi,t2},
		{Si,t4},
		{Re,t2},
		{Dol,t4},
		{Do*2,t2},
		{La,t4},
		{Do*2,t2},
		{Si,t8},
		{Lal,t8},
		{Si,t2},
		{Sol,t4},
		{Mi,t2}
};

//deinimolas variables a emplear
int Paso_Sonido=0;
char Play_Musica=0;
int Timer_Sonido;
int Sound_Counte;

//funcion que confgura los arametros GPIO Timer4 y NVIC
void Sonido_Confi(void)
{
	//creamos la estructura GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	//establecemos el reloj para el GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//configuramos la salida del altavoz en pi PB6
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_StructInit(&GPIO_InitStructure);
	//configuramos el TIM
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler=72;
	TIMER_InitStructure.TIM_Period=0xFFFF;
	TIMER_InitStructure.TIM_ClockDivision=0;
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIMER_InitStructure);
	//configuamos el PWM
	// configuracion pulso PWM CH4
	TIM_OCInitTypeDef TIM_PWM_InitStricture;
	TIM_OCStructInit(&TIM_PWM_InitStricture);
	TIM_PWM_InitStricture.TIM_Pulse=0;
	TIM_PWM_InitStricture.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_PWM_InitStricture.TIM_OutputState=TIM_OutputState_Enable;
	TIM_PWM_InitStricture.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM4,&TIM_PWM_InitStricture);
	//iniciamos la interrupcion para el tim4
	TIM_ITConfig(TIM4,TIM_IT_CC4,ENABLE);
	//iniciamos la interrupcion para tim2
	NVIC_EnableIRQ(TIM4_IRQn);
}
//funcion que gnera la frecuencia en el altavoz
void Sound(int frec, int time_ms)
{
	if(frec>0)
	{
		TIM4->ARR=SYSCLK/PRESCALER/frec;
		TIM4->CCR1=TIM4->ARR/2;
	}
	else
	{
		TIM4->ARR=1000;
		TIM4->CCR1=0;
	}
	TIM_SetCounter(TIM4,0);
	Timer_Sonido=((SYSCLK/PRESCALER/TIM4->ARR)*time_ms)/1000;
	Sound_Counte=0;
	TIM_Cmd(TIM4,ENABLE);
}

//funcion que inicia a musica en el altavoz
void Sonido_Init(void)
{
	Paso_Sonido=0;
	Play_Musica=1;
	Sound(Music[Paso_Sonido].freq,Music[Paso_Sonido].time);
}
//funcion que detecta la interrupcion y procide el sonido
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_CC4)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC4);
		Sound_Counte++;
		if(Sound_Counte>Timer_Sonido)
		{
			if(Play_Musica==0)
			{
				TIM_Cmd(TIM4,DISABLE);
			}
			else
			{
				//mientras el indice pao_sonido sea menor del total musicsize de nota de a partitura
				if(Paso_Sonido<MUSICSIZE-1)
				{
					if(TIM4->CCR1==0)
					{
						//pasamos a la siguiente nota a reproducir
						Paso_Sonido++;
						//rproducimos la frecuencia de la nota d la partitura
						Sound(Music[Paso_Sonido].freq,Music[Paso_Sonido].time);
					}
					else
					{
						Sound(0,30); //se hace un silencio
					}
				}
				else
				{
					//Play_Musica=0;
					Paso_Sonido=0;
					Sound(0,30); //se hace un silencio
					//TIM_Cmd(TIM4,DISABLE);
				}
			}
		}
		if(TIM_GetFlagStatus(TIM4,TIM_FLAG_CC4OF)!=RESET)
		{
			TIM_ClearFlag(TIM4,TIM_FLAG_CC4OF);
		}
	}
}
int main(void)
{
	//iniciamos la coniguracion del GPIO yTIMER
	Sonido_Confi();
	//Iniciamos los parametros que inician la melodia
	Sonido_Init();
    while(1)
    {
    }
}



































