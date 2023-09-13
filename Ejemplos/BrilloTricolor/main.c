#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
void espera(unsigned int nCount);
#define PERIOD 1000

int main(void)
{
	int TIM_Pulse_R=0;
	int TIM_Pulse_G=0;
	int TIM_Pulse_B=0;
	// creamos las estructuras GPI y TIM
	GPIO_InitTypeDef GPIO_InitStructure;
	// activamos los clokcs correspondientes
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//confguramos los GPIO de los pines de salida
	//PB6_Red PB7_Green PB8_Blue
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 |GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//confgurmos el timer 4
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler=720;
	TIMER_InitStructure.TIM_Period=PERIOD;
	TIMER_InitStructure.TIM_ClockDivision=0;
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIMER_InitStructure);
	//configuacion del PWM
	TIM_OCInitTypeDef TIM_PWM_InitStricture;
	TIM_OCStructInit(&TIM_PWM_InitStricture);
	TIM_PWM_InitStricture.TIM_Pulse=0;
	TIM_PWM_InitStricture.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_PWM_InitStricture.TIM_OutputState=TIM_OutputState_Enable;
	TIM_PWM_InitStricture.TIM_OCPolarity=TIM_OCPolarity_High;
	//cargamos en cada canal del TIM4 el valor de TIM_Pulse=0 inicial
	TIM_OC1Init(TIM4,&TIM_PWM_InitStricture); //TIM4_CH1 PB6
	TIM_OC2Init(TIM4,&TIM_PWM_InitStricture); //TIM4 CH2 PB7
	TIM_OC3Init(TIM4,&TIM_PWM_InitStricture); //TIM4 CH3 PB8

	TIM_Cmd(TIM4,ENABLE);


    while(1)
    {
    	// incrementamos el TIM_Pulse de la señal Rojo
        TIM_Pulse_R++;
        // comprobamos que no se supera el umbral de la variable PERIOD
        if(TIM_Pulse_R>PERIOD)
        	TIM_Pulse_R=0;
    	// incrementamos el TIM_Pulse de la señal verde
        TIM_Pulse_G+=2;
        // comprobamos que no se supera el umbral de la variable PERIOD
        if(TIM_Pulse_G>PERIOD)
        	TIM_Pulse_G=0;
    	// incrementamos el TIM_Pulse de la señal verde
        TIM_Pulse_B+=4;
        // comprobamos que no se supera el umbral de la variable PERIOD
        if(TIM_Pulse_B>PERIOD)
        	TIM_Pulse_B=0;
        //enviamos los vcalores a los registros de cada color
        //TIM_PWM_InitStructure.Tim_Pulse=TIM_Pulse_RGB
        TIM4->CCR1=TIM_Pulse_R;
        TIM4->CCR2=TIM_Pulse_G;
        TIM4->CCR3=TIM_Pulse_B;
        //realizamos un delay
        espera(10);
    }
}
// funcion de espera
void espera(unsigned int nCount)
{
	unsigned int i,j;
	for(i=0;i<nCount;i++)
		for(j=0;j<0x2AFF;j++);
}






























