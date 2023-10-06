/**********************************************************************
  * Project Name: AWD
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
  * PROGRAMA DE PRUEBA INERRUPCION WWDG WATCHDOG  
  *
  *
  **********************************************************************/
// Libreria principal del microcontrolador
#include "stm32f10x.h"

// Las librerias para los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_usart.h"         


// Llamada a la libreria externa que configura el USART1
#include "usart1.h"
 
// Creamos las variables 
volatile char buffer[50] = {'\0'};
volatile char ADC_IT_AWD_FLAG = 0;
 
/*   Funcion que se ejecutará cuando se detecte la interrupcion  */
//-----------------------------------------------------------------
void ADC1_2_IRQHandler(void)
{
    if(ADC_GetITStatus(ADC1,ADC_IT_AWD))
    {
        ADC_IT_AWD_FLAG = 1;
        ADC_ClearITPendingBit(ADC1,ADC_IT_AWD);
    }
} 

/*              Modulo  Principal                                */
//-----------------------------------------------------------------
int main(void)
{
    int adc_value;   // Creamos la variable que contendra el valor convertido
 
    USART1_Config(); // Inicializamos el USART1

    /* Activamos el RCC de los canales ADC  --------------------------*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    /* Activamos el reloj ‘ADCCLK’ del ADC con divisor 6 (72/6=12MHz)-*/
    RCC_ADCCLKConfig (RCC_PCLK2_Div6);
      
    /* Creamos la estructura GPIO para los canales ADC  --------------*/
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    /* Configuracion del NVIC ----------------------------------------*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;   //IRQn ADC 1/2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
    /* Configuración del ADC   ---------------------------------------*/
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init (ADC1, &ADC_InitStructure);    
 
    /* Configuramos el canal 0 del ADC1  -----------------------------*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28Cycles5); 
 
    /* Configuramos los umbrales mayor y menor del watchdog ----------*/
    ADC_AnalogWatchdogThresholdsConfig(ADC1, 3000, 1000);
    
    /* Configuamos el canal 0 como canal de vigilancia watchdog ----- */
    ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_0);
    
    /* Habilitamos el canal del watchdog ADC1 como simple canal regular*/
    ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
 
    /* Habilita la bandera de la interrupcion AWD --------------------*/
    ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
 
    /* Inicializa el ADC1  -------------------------------------------*/
    ADC_Cmd(ADC1, ENABLE);
  
    /* Restablecemos el ADC para reiniciar los parámetros configurados */
    ADC_ResetCalibration(ADC1); 
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1); 
    while(ADC_GetCalibrationStatus(ADC1));
 
    /* Iniciamos el ADC1 ---------------------------------------------*/
    ADC_Cmd (ADC1,ENABLE);  
    
    /* Iniciamos el procesos de conversion en ADC1 en modo continuo  -*/
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
 
    while (1)
    {
        // Comprobamos el estado de la bandera del AWD
        // Si el watchdog se ha activado, se lee el valor convertido
        if (ADC_IT_AWD_FLAG == 1) {
            adc_value = ADC_GetConversionValue(ADC1);
            printf(" Valor ADC = %d\r\n", adc_value);
            ADC_IT_AWD_FLAG = 0;
        }
    }
}

////////////////////   Fin  del  Programa    ///////////////////////////
