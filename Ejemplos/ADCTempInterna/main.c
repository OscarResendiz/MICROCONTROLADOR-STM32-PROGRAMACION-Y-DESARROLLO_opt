#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x.h"
#include "usart1.h"
#include <stdio.h>
#include <string.h>
// lista de funciones empleadas
void IntTemp_Config(void);
void Delay_ms(unsigned int nCount);
//lita de variables empleadas
const uint16_t V25=1.41; //valor de v25=1.41 para 33vcc de REF
const uint16_t Avg_Slope=4.3; // valor de Avg_Slope=4.3mv/c para 3.Vcc de REF
//uint16_t
uint16_t TemperatureC;
uint16_t AD_Value;
int main(void)
{
	//creaos la variable para el contaor
	char cadena[100];
	USART1_Config();
	IntTemp_Config();
    while(1)
    {
    	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
    	//leemos l valor
    	float Vaor_Leido=ADC_GetConversionValue(ADC1);
    	//calculamos el valor obteniodo enl adc segun vref
    	Vaor_Leido=Vaor_Leido*(3.3/4096);
    	AD_Value=Vaor_Leido;
    	sprintf(cadena,"Valor ADC = %4.2f Vcc\r\n",(float)Vaor_Leido);
    	print(cadena);
    	//reseteamos labandera de conversion
    	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
    	//convertimos el valor obtenido en un valor de grados celsius
    	TemperatureC=((V25-AD_Value)/Avg_Slope+25);
    	//imprimimos por el puerto serial el valor obtenido
    	sprintf(cadena,"Temperatura = %d C\r\n",TemperatureC);
    	print(cadena);
    	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    	Delay_ms(1000);
    }
}
//funcion que configura e inicializa el ADC
void IntTemp_Config(void)
{
	// nombramos la estructura de configuracion de ADC
	ADC_InitTypeDef ADC_InitStructure;
	//inicia el clock del ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	//iniciaizamos el reloj de controldel ADC
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//
	//configuramos los parametros del ADC
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_Init(ADC1,&ADC_InitStructure);
	//configuramos l canal 16 delADC1
	//establecemos unos 41.5 ciclos de conversion
	//y el rango en el secuenciador=1
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime_41Cycles5);
	//inicamos el sensor de temperatura inteno
	ADC_TempSensorVrefintCmd(ENABLE);
	//iniciamos el ADC
	ADC_Cmd(ADC1,ENABLE);
	//opciones de libreria para calibacion del modulo ADC
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	//inicializacion dela conversion porsoftwre del ADC1
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}
//funcion para generar un retardo de un milisegundo
void Delay_ms(unsigned int nCount)
{
	unsigned int i,j;
	for(i=0;i<nCount;i++)
	{
		for(j=0;j<0X2AFF;j++)
		{
			;
		}
	}
}























