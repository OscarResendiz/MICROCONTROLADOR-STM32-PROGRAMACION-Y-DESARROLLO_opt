 /**********************************************************************
  * Project Name: GPIO_LED_KEY_1
  * File Name: main.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4-5
  * Author:   Jesus Pestano
  * E-mail:   jmpestano@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * =====================================================================
  * 
  * PROGRAMA DE PRUEBA SI SE HA PULSADO UN BOTON Y ENTONCES
  * ENCIENDE UN LED
  *
  * Enciende y apaga el LED de prueba de la placa conectado 
  * al pin PC13 al detectar que se ha pulsado un boton 
  * conectado en PA1 y a (Gnd).
  *
  **********************************************************************/
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"		
#include "stm32f10x_gpio.h"

/*             Modulo Principal               */
//******************************************************
int main(void){	

	/*  Nombramos la estructura que contendra los GPIOx  */
	GPIO_InitTypeDef GPIO_StructureInit;	  
	
	/* Activamos el clock para GPIOA y GPIOC   */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	/* Establecemos el reloj del Puerto GPIOC (clock enable) */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* Establecemos el reloj del Puerto GPIOA (clock enable) */
	
	/* Configuramos el pin PC13 para el LED  */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_13;				// Asignamos el Pin PC13 para configuracion
	GPIO_StructureInit.GPIO_Speed = GPIO_Speed_50MHz;	// Establecemos la velocidad a 50MHz (Speed)
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_Out_PP;	// Establecemos el pin como Salida(output) y push-pull resistor
	GPIO_Init(GPIOC, &GPIO_StructureInit);					  // Iniciamos la estructura en el compilador
	
	/* Configuramos el pin PB0 para el pulsador  */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_0;					// Asignamos el Pin PB0 para configuracion
	GPIO_StructureInit.GPIO_Speed = GPIO_Speed_50MHz;	// Establecemos la velocidad a 50MHz (Speed)
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_IPU;	    // Establecemos el pin como Entrada (Imput) y pull-up resitor
	GPIO_Init(GPIOB, &GPIO_StructureInit);					  // Iniciamos la estructura en el compilador
	
	
	while(1){
		
		// Comprueba cuando el pin PB0 esta a nivel bajo '0'
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0 )  
		{
			//  Encendemos el led
      GPIO_ResetBits(GPIOC, GPIO_Pin_13); /* Desactiva el pin PC13 */
			
		}
		else
		{
      // Apagamos el led
			GPIO_SetBits(GPIOC, GPIO_Pin_13);	 /* Activamos el pin PC13 */
			
		}
	}
}

/////////////////////////  Fin del Programa ////////////////////////

