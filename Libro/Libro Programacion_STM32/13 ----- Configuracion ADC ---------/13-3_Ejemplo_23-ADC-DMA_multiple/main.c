 /**********************************************************************
  * Project Name: TIMER1
  * File Name: main.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *  
  * PRUEBA DE LECTURA MULTIPLE DE CANALES ADC.
  *
  *  En el programa se muestra por el puerto USART en nuestro ordenador
  *  los valores obtenidos en diferentes canales ADC, obteniendo los
  *  valores de su lectura en el DMA del modulo ADC.
  *
  *  Conexiones:
  *                 ADC1_CH4 en PA4
  *                 ADC1_CH5 en PA5
  *                 ADC1_CH6 en PA6  
  *                 ADC1_CH7 en PA7
  *
  **************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"

// Llama a la libreria para el puerto USART1
#include "usart1.h"

#include <stdio.h>

// Lista de funciones
void ADC_DMA_Config(void);
void TIM_Config(void);

// Variables empleadas en nuestro programa
volatile short FLAG_ECHO = 0;
volatile uint16_t ADCBuffer_value[] = {0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA};
//---------
volatile uint32_t status = 0;

/*   FUncion que detecta la interrupcion IRQ en TIMER4 */
//--------------------------------------------------------
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		FLAG_ECHO = 1;
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);   
    
	}
}


/*             Modulo principal                        */
//=======================================================
int main(void)
{
    //char buffer[80] = {'\0'};

    USART1_Config(115200);  // Inicializamos USART1
  
    ADC_DMA_Config();       // Inicializamos ADC y GPIO 
    
    TIM_Config();           // Configuramos TIMER y NVIC
    
    printf("  PRUEBA DE ADC Multiple  \r\n");
          
    while (1)
    {
        // habilita los canales 1 y 2 de DMA
        DMA_Cmd(DMA1_Channel1, ENABLE);
        DMA_Cmd(DMA1_Channel2, ENABLE);
        // Inicia la conversion de canales
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);          
  
        // Imprime los valores por el puerto USART1
        if (FLAG_ECHO == 1) {    		
            printf (" Lecturas de ADC: ");        
        
            for (int index = 0; index <5; index++){
                printf("[%d] %4.2fVc ", index,  (float) ADCBuffer_value[index+16]*(3.3/4096) );          
            }
       
            printf ("\r\n");    		
            FLAG_ECHO = 0;
        }
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    }
}

/*            Funcion que configura los canales ADC    */
//-------------------------------------------------------
void ADC_DMA_Config(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  
  /*  Habilitamos los relejos para RCC, GPIO, ADC1, AFIO del ADC y DMA1  */
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|
                         RCC_APB2Periph_ADC1 | 
                         RCC_APB2Periph_AFIO , ENABLE);  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE );
  
  /*  Crea la estructura del GPIO de los pines --------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | // ADC1_CH4 en PA4
                                GPIO_Pin_5 | // ADC1_CH5 en PA5
                                GPIO_Pin_6 | // ADC1_CH6 en PA6  
                                GPIO_Pin_7;  // ADC1_CH7 en PA7  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

  /*   Configuracion de los parametros DMA ------------------------*/
  DMA_InitStructure.DMA_BufferSize = 4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCBuffer_value;
  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE) ;        
  
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 4;	
	ADC_Init(ADC1, &ADC_InitStructure);
  
	
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_41Cycles5);
	
  ADC_Cmd(ADC1, ENABLE) ;
	ADC_DMACmd(ADC1, ENABLE) ;
	ADC_ResetCalibration(ADC1);

	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd ( ADC1 , ENABLE ) ;
}

/*   Funcion que configura el TIMER 4                     */
//----------------------------------------------------------
void TIM_Config(void)
{
    /*  Establece la estructrua TIMER y NVIC -------------*/
    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /*  Habilita el reloj para el TIMER 4  ---------------*/
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  	/*   Configura los parametros del TIMER --------------*/
    TIM_TimeBaseStructInit(&TIMER_InitStructure);
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Prescaler = 7200;
    TIMER_InitStructure.TIM_Period = 5000;
    TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);

    /* Configura el NVIC  para el IRQ del TIMER 4 --------*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/////////////////////   FIn del Programa   ///////////////////

