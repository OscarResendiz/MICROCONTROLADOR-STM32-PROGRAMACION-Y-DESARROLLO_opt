#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "usart1.h"
#include "misc.h"
//#include <string.h>
#include <stdio.h>
#include "stm32f10x_tim.h"
/*
void ADC_Inyected_Config(void)
{
	//creamos las estructuras para el GPIO, ADC y el DMA
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	//habilitamos los relojes para el GPIOA y ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);

	// habilitamos el reloj del ADC y esblecemos su divisor}
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 |GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//configuramos los parametros del ADC
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel=2;
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;
	ADC_Init(ADC1,&ADC_InitStructure);

	// establecemos los canales del grupo de inyectado a utilizar
	ADC_InjectedSequencerLengthConfig(ADC1,2);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_7Cycles5);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_7Cycles5);

	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_None);

	ADC_Cmd(ADC1,ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_AutoInjectedConvCmd(ADC1,ENABLE);
	ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);
}
*/
void ADC_Injected_Config(void)
{
    /*  Creamos las estructuras para GPIO, ADC y NVIC   */
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    /* Habilitamos los reloj para GPIOA y ADC1    */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);

    /* Habilitamos el reloj del ADC y establecemos su divisor */
    RCC_ADCCLKConfig (RCC_PCLK2_Div6);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*  Configuramos los parametros del ADC    */
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_NbrOfChannel = 2;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* Establecemos los canales del grupo de inyectado a utilizar  */
    ADC_InjectedSequencerLengthConfig(ADC1, 2);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_0, 1,ADC_SampleTime_7Cycles5);
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_1, 2,ADC_SampleTime_7Cycles5);

    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);

    ADC_Cmd(ADC1, ENABLE ) ;

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    ADC_AutoInjectedConvCmd(ADC1, ENABLE );
    ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);
}

int main(void)
{
	char cadena[100];
	uint16_t ADC_Valor0,ADC_Valor1;
	USART1_Config(); //iniciamos el usart1
	ADC_Injected_Config(); //inicializamos el ADC injectado

    while(1)
    {
    	ADC_Valor0=ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1);
    	ADC_Valor1=ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2);
    	sprintf(cadena,"Valores ADC [1]=%d [2]=%d \r\n ",ADC_Valor0,ADC_Valor1);
    	print(cadena);

    }
}
