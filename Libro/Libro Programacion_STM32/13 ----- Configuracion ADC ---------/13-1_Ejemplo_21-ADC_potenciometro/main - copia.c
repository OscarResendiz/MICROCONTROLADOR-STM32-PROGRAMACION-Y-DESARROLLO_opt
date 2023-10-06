 /**********************************************************************
  * Project Name: ADC_potenciometer
  * File Name: main.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *  
  * PROGRAMA EJEMPLO DE USO DEL ADC
  *
  * En el presente ejemplo se enciende el LED en PB1 cuando
  * la lectura analogica del voltaje de salida de un potenciometro
  * conectado al pin PA6 excede de los 1.6 voltios.
  * Mostrandose dicho valor por el puerto USART en nuestro ordenador
  *
  * Conexiones:
  *    LED 		----  PB1
  *         [  ---  GND
  *   [ Pot [  ---  PA6
  *         [  ---  3.3 Vcc
  *
  *********************************************************************/
 
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"    

// Libreria de contro del puerto USART1
#include "usart1.h"
#include <stdio.h>
#include <string.h>

/* Lista de funciones empleadas    */
int getPot(void);
void GPIO_config(void);
void ADC_config(void);
void Delay_ms(unsigned int nCount);

/*  Modulo principal   */
//=====================================================
int main(void){

	// Creamos la variable para el contador
	float x;
	
	USART1_Config(); // Inicializamos el USART1
	  
	GPIO_config(); //Inicializamos el GPIO del 
    
	ADC_config();  // Inicializamos el ADC
	
    while(1)
    {
		// La formula empleada es:
		//     [Vcc_out = Val_ADC / MaxADC_bits(4096) * Vcc_in ]
		// Calculamos el valor leido del PIN PA6
		// lo multiplicamos por 3.3 Vcc / dividio por 4095 -resolucion de 12 bits-
		// el resultado 'x' es el valor en voltios
		int Value = ADC_GetConversionValue (ADC1);
		x = Value * 3.3/4095; 
 
        if(x > 1.6)		  // Si x es mayor de 1.6
				//if(x > 1.3)		// Si x es mayor de 1.36
        {
          GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);//Enciende LED
        }
        else 
        {
          GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);//Apaga LED
        }		
						
		printf("Valor = %4.2f Vcc\n\r", x);
		Delay_ms(100);
    }
}


/* Lee el valor analogico obtenido en el pin   */
//-----------------------------------------------
int getPot(void)
{
    return ADC_GetConversionValue(ADC1);
}


/* Funcion que configura e inicializa el ADC    */
//------------------------------------------------
void ADC_config(void)
{ 
	/*  Creamos la estructura del ADC   */
	ADC_InitTypeDef ADC_StructureInit;	
	
	/*  Inicializamos el Reloj de control del ADC   */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADC (max 14MHz, 72/6=12MHz)
	
	/* Configuramos los parametros del ADC        */
	ADC_StructureInit.ADC_Mode = ADC_Mode_Independent;	
	ADC_StructureInit.ADC_ScanConvMode = DISABLE;
	ADC_StructureInit.ADC_ContinuousConvMode = ENABLE;
	ADC_StructureInit.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_StructureInit.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_StructureInit.ADC_NbrOfChannel  = 1;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5); //PA6 Input
	ADC_Init(ADC1, &ADC_StructureInit);

	/*  Iniciamos el ADC          */
	ADC_Cmd(ADC1, ENABLE);

	/* Opciones de libreria para calibracion del modulo ADC   */
	ADC_ResetCalibration(ADC1);	
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	// Iniciamos el ADC1
	ADC_Cmd(ADC1, ENABLE);
}
 
/*  Funcion que configura el GPIO para LED en PB1    */
void GPIO_config(void)
{ 		
	/*  Creamos la estructura para GPIO   */
	GPIO_InitTypeDef GPIO_StructureInit;	  	
	
	/* Activamos el reloj para GPIOB       */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
	/* Configuramos el pin PB1 para el LED  */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_1;	// Asignamos el Pin PC13 para configuracion
	GPIO_StructureInit.GPIO_Speed = GPIO_Speed_50MHz;// Establecemos la velocidad a 50MHz (Speed)
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_Out_PP; // Establecemos el pin como Salida(output) y push-pull resistor
	GPIO_Init(GPIOB, &GPIO_StructureInit);	 // Iniciamos la estructura en el compilador
	
	/* Configuramos el pin PA1 entrada del potenciometro  */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_6;		// Asignamos el Pin PA1 para configuracion
	GPIO_StructureInit.GPIO_Speed = GPIO_Speed_50MHz;// Establecemos la velocidad a 50MHz (Speed)
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_AIN;	 // Establecemos el pin como Entrada (Imput) y pull-up resitor
	GPIO_Init(GPIOA, &GPIO_StructureInit);
	
}

/* Modulo que genera un retardo (ms)    */
//--------------------------------------
void Delay_ms(unsigned int nCount){ 
		unsigned int i, j;
		for(i = 0; i < nCount; i++)
		{  
				for(j = 0; j < 0x2AFF; j++){;} 
		}
} 

 
/******************* Final del programa ***********************/
