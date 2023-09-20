#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x.h"
#include "usart1.h"
#include <stdio.h>
#include <string.h>
// lista de funciones empleadas
int getPot(void);
void GPIO_config(void);
void adc_config(void);
void Delay_ms(unsigned int nCount);
int main(void)
{
	//creaos la variable para el contaor
	char cadena[100];
	float x;
	USART1_Config();
	GPIO_config();
	adc_config();
    while(1)
    {
    	/*
    	 * la formula empleada es
    	 * (Vcc_out=Val_Adc/MaxADC_bits(4096)*Vcc_in)
    	 * calculamos e valor leido del Pin PA6 de la siguinte forma
    	 * lo multiplicamos por 3.3 Vcc/dividido por 4095 que es la resolucion de 12 bits
    	 * el resultado x es el valor en voltios
    	 */
    	x=getPot()*3./4095;
    	//comparams el valor obtenido con un limite
    	if(x>1.6)
    	{
    		GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET); //enciende el led
    	}
    	else
    	{
    		GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET); //apaga el led
		}
    	//enviamos el valor obtenido por el puerto serial
    	sprintf(cadena,"Valor= %4.2f Vcc\r\n",x);
    	print(cadena);
    }
}
//funcion que le el valor analogico obtenido en el pin
int getPot(void)
{
	return ADC_GetConversionValue(ADC1);
}
//funcion que configura e inicializa el ADC
void adc_config(void)
{
	// nombramos la estructura de configuracion de ADC
	ADC_InitTypeDef ADC_InitStructure;
	//iniciaizamos el reloj de controldel ADC
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//
	//inicia el clock del ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	//configuramos los parametros del ADC
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_55Cycles5); //PA6 entrada
	ADC_Init(ADC1,&ADC_InitStructure);
	//iniciamos el ADC
	ADC_Cmd(ADC1,ENABLE);
	//opciones de libreria para calibacion del modulo ADC
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	//iniciamos el ADC
	ADC_Cmd(ADC1,ENABLE);
}
void GPIO_config(void)
{
	//creamos la estructura para el GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	//activamos el relog para el GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//configuramos el pin PB1 para el led
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//configuramos el pin PA1 para el boton
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

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























