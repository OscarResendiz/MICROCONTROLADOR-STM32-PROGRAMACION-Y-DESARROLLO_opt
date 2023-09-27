#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "usart1.h"
#include "misc.h"
#include <string.h>

#include <stdio.h>

//lista de funciones
void ADC_DMA_Config(void);
void TIM_Config(void);
//variables empleadas en nuestro programa
volatile short FLAG_ECHO=0;
volatile uint16_t ADCBuffer_value[]={0XAAAA,0XAAAA,0XAAAA,0XAAAA};
volatile uint32_t status=0;
//funcion que detecta la interrupcion IRQ en timer4
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		FLAG_ECHO=1;
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}
//funcion principal
int main(void)
{
	char cadena[100];
	USART1_Config(); //iniciamos el usart1
	ADC_DMA_Config(); //iniciamos el ADC y el GPIO
	TIM_Config();//configuramos el timer y NVIC
	print("PRUEBA DE ADC MULTIPE\r\n");

    while(1)
    {
    	//habilita los canales 1 y 2 de DMA
    	DMA_Cmd(DMA1_Channel1,ENABLE);
    	DMA_Cmd(DMA1_Channel2,ENABLE);
    	//inicia la conversion de canales
    	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    	//imprime los valores por el puerto usart
    	if(FLAG_ECHO==1)
    	{
    		print("Lecturas del ADC: ");
    		//muestro el valor obtenido por cada canal
    		for(int index=0;index<4;index++)
    		{
    	    	sprintf(cadena,"[%d]= %4.2fvc ",index,(float)ADCBuffer_value[index]*(3.3/4096));
    	    	print(cadena);
    		}
    		print("\r\n");
    		//resetea la bandera
    		FLAG_ECHO=0;
    	}
    	ADC_SoftwareStartConvCmd(ADC1,DISABLE);
    }
}
//funcion que configura los canales ADC
void ADC_DMA_Config(void)
{
	//creamos las estructuras para el GPIO, ADC y el DMA
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	//habilitamos los relojes para RCC, GPIO, ADC1, AFIO del ADC y DMA
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	//crea la estructura del GPIO de los pines
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 |//ADC_CH4 en PA4
			GPIO_Pin_5 |//ADC_CH5 en PA5
			GPIO_Pin_6 |//ADC_CH6 en PA6
			GPIO_Pin_7; //ADC_CH7 en PA7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//configuracion de los parametros DMA
	DMA_InitStructure.DMA_BufferSize=4;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)ADCBuffer_value;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	//configuramos el ADC
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=4;
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,3,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,4,ADC_SampleTime_41Cycles5);

	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);

	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}
//funcion que configura el timer4
void TIM_Config(void)
{
	//establecemos la estructura TIMER y NVIC
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//habilita el relog para el TIMER4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//configuramos los parametros del TIMER
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMER_InitStructure.TIM_Prescaler=7200;
	TIMER_InitStructure.TIM_Period=5000;
	TIM_TimeBaseInit(TIM4,&TIMER_InitStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	//configuramos en NVIC para el IRQ del Timer4
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



































