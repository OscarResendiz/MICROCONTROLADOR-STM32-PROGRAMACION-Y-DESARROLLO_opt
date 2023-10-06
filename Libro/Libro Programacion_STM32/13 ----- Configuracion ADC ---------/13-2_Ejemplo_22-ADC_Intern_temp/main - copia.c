 /**********************************************************************
  * Project Name: Inten_temp
  * File Name: main.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * =====================================================================
  *  
  *  PROGRAMA EJEMPLO DE USO DEL ADC
  *
  * Programa que lee el sensor interno de temperatura del micro y
  * nos muestra la información por el puerto serial USART1.
  * 
  ***************************************************************/	

// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

// Llamada a la librería de control del puerto USARTx
#include "usart1.h"

// Llamada a la librería especial del Keil para manejo del comando ‘printf’
#include <stdio.h>

// Lista de funciones empleadas
void IntTemp_Config(void);
void Delay_ms(unsigned int nCount);

// Lista de variables empleadas
const uint16_t V25 = 1.43;      // valor de V25=1.41V para 3.3Vcc de REF
const uint16_t Avg_Slope = 4.3; // valor de avg_slope=4.3mV/C para 3.3Vcc de REF
uint16_t AD_value, TemperatureC;

/*               Modulo principal                           */
//============================================================
int main(void)
{
  USART1_Config(115200);   // Inicializamos el USART1
  
  IntTemp_Config();        // Inicializamos el Sensor Interno

  
  while (1)
  {
    
    // Esperamos a que se termine la converision    
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    //  Leemos el valor leido     
    float AD_value=ADC_GetConversionValue(ADC1);
    /*  Calculamos el valor obtenido en el ADC según VRef
        Valor representado sera = ADC_val * (3.3 Vcc / 4096)  */
    AD_value = AD_value * (3.3/4096);
    printf(" Valor ADC  : %4.2f \r\n", (float) AD_value);
    
    // Reseteamos la bandera de fin de conversion
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);       

    // Convertimos el valor obtenido en un valor en grados Celsius
    TemperatureC = (uint16_t) ((V25-AD_value)/ Avg_Slope+25);
        
    // Imprimimos por el puerto USART el valor obtenido
    printf(" Temperatura: %d C\r\n", TemperatureC);
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);   

    Delay_ms(1000);
  }
}


/*       Funcion que configura el ADC del sensor interno   */
//----------------------------------------------------------------
void IntTemp_Config(void)
{
    /*  Creamos la estructura para ADC    */
    ADC_InitTypeDef ADC_InitStructure;
  
    // Iniciamos el reloj APB2 para ADC1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    /*  Configuramos los parámetros del ADC1  */      
    /*  
    - NbrOfChannel = 1 - ADC_Channel_16 (ADC_Channel_TempSensor)
    - Mode = Single ConversionMode (ContinuousConvMode disabled)
    - Resolution = 12Bits
    - Prescaler = /1
    - sampling time 192 */
  
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // Continuo sampler
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /* Configuramos el canal 16 del ADC1 
       Establecemos unos 41.5 cycles de conversión
       y el rango en el secuenciador =1          */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_41Cycles5);
    // También existe esta forma de entrar estos parámetros
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_239Cycles5);
    
    // Iniciamos el Sensor de Temperatura Interno
    ADC_TempSensorVrefintCmd(ENABLE);

    // Iniciamos el ADC           
    ADC_Cmd(ADC1, ENABLE);
    
    /* Opciones de libreria para calibracion del modulo ADC   */
    ADC_ResetCalibration(ADC1);
    
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    ADC_StartCalibration(ADC1);
    
    while(ADC_GetCalibrationStatus(ADC1));    
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                  
}

/*   Modulo que genera un retardo (ms)    */
//--------------------------------------
void Delay_ms(unsigned int nCount){ 
		unsigned int i, j;
		for(i = 0; i < nCount; i++)
		{  
				for(j = 0; j < 0x2AFF; j++){;} 
		}
} 

//////////////////////////  Fin del Programa  //////////////////
