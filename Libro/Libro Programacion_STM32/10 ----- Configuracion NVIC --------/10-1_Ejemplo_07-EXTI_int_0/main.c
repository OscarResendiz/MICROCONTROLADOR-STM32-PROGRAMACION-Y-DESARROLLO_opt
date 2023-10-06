 /**********************************************************************
  * Project Name: EXTI_int_1
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
  * PROGRAMA DE PRUEBA DE LA INTERRUPCION EXTI_0.
  * 
  * Enciende y apaga un el LED conectado al pin PB1, 
  * al pulsar el boton conectado en PA0 tras 
  * detectarse una Interrupcion EXTI_
  *
  **********************************************************************/
 
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"   

// Libreria que contiene funcion de retardo
#include "delay.h"

// Listado de funciones
void Configurar_LED(void);
void Configurar_BOTON(void);
void Config_Int(void);
void espera(unsigned int nCount);
unsigned char bandera = 0;

/*   Modulo principal  */
int main(void){
	DelayInit();	// Inicializa la funcion Delay
	
	Configurar_LED(); 		// Configura pin del LED
	Configurar_BOTON();		// Configura pin del BOTON
	Config_Int(); 			//Configura EXTI Line0 (Pin PA0) en modo interrupcion

	while (1){			//Bucle de espera la interrupcion.
		
		// Blink LED en PC13 para control del programa
		GPIOC->BRR = GPIO_Pin_13;
		DelayMs(1000);
		GPIOC->BSRR = GPIO_Pin_13;
		DelayMs(1000);
	}		
}

/* Funcion configura puerto y pin para el LED                */
//-------------------------------------------------------------
void Configurar_LED(void){
	/*  Creamos la estructura para GPIO   -------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configuramos PB1 para salida de LED ------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			// Configura el pin 13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	// El pin será salida push/pull	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	// Velocidad del puerto
	GPIO_Init(GPIOB, &GPIO_InitStructure);				// Pasa la config. a la estructura.
	
	/* Configuramos PC13 de Salida LED ----------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*   Funcion que configura puerto y pin para PA0             */
//-------------------------------------------------------------
void Configurar_BOTON(void){
	/* Creamos la estructrua para GPIO Boton  ---------------*/
	GPIO_InitTypeDef GPIO_InitStructure;			
	
	/* Activamos el reloj para GPIO en puerto A -------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	
	/* Configura pin PA0 como entrada y resistor flotante ---*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*   Funcion que configura la interrupcion por PA0           */
//-------------------------------------------------------------
void Config_Int(void){
	/* Creamos la estructura para NVIC y EXTI ---------------*/
	NVIC_InitTypeDef NVIC_InitStructure; 		// Estructura para el NVIC
	EXTI_InitTypeDef EXTI_InitStructure; 		// Estructura para la interrupcion

	/* Configura la interrupcion EXTI linea 0 de PA0 ---------*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Crea el NVIC para EXT_1 canal IRQ ---------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*   Funcion que se ejecuta cuando se detecta la interrupcion  */
//--------------------------------------------------------------
void EXTI0_IRQHandler(void) 
{
	// Comprueba si la linea Linea0 de EXTI se pulsa
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) // Verifica el estado del pin.
	
	{
		int i;
		// Trogle LED PB1
		for (i = 0; i < 5; i++)
		{
			GPIOB->BSRR = GPIO_Pin_1;
			espera(100);
			GPIOB->BRR = GPIO_Pin_1;
			espera(100);
		}		
		
		// Desactiva la interrupcion para que se vuelva a comprobar
		EXTI_ClearITPendingBit(EXTI_Line0);  
	}
}

// Funcion de espera
void espera(unsigned int nCount)
{
	unsigned int i, j;
	for (i = 0; i < nCount; i++)
		for (j = 0; j < 0x2AFF; j++);
}


/******************************   Fin del programa   ******************************************/

