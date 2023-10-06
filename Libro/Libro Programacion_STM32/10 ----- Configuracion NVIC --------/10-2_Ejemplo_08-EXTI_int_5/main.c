 /**********************************************************************
  * Project Name: EXTI5_int
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
  * PROGRAMA DE PRUEBA DE LA INTERRUPCION EXT5 en PA5
  * 
  * Enciende y apaga el LED de pruebas en PC13 de la placa
  * a la vez de que enciende y apagará un LED conectado al pin PB1 
  * cuando se recibe una señal del sensor de movimiento HC-SR501 
  * conectado al pin PB5, comprobando que se ha producido una interrupcion
  * del tipo EXTI9_5_IRQ.   
  *
  **********************************************************************/
 
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"     

// Listado de funciones
void Configurar_LED(void);
void Configurar_BOTON(void);
void EXT5_Config(void);
void espera(unsigned int nCount);

unsigned char bandera = 0;

/*   Modulo principal  */
int main(void){	
	Configurar_LED(); 		// Configura pin del LED
  
	Configurar_BOTON();		// Configura pin del BOTON
  
	EXT5_Config(); 			//Configura EXTI Line0 (Pin PA0) en modo interrupcion

	while (1){			//Bucle infinito independiente
		
		// Blink LED en PC13 para control del programa
		GPIO_SetBits(GPIOC, GPIO_Pin_13); /* Activa el pin PB1 */
		espera(100);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13); /* Activa el pin PB1 */
		espera(100);
	}		
}

/* Funcion configura puerto y pin para el LED en PB1 y PC13 */
//-----------------------------------------------------------
void Configurar_LED(void){
	/*  Creamos la estructura para GPIO     */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configuramos PB1 para salida de LED  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			// Configura el pin 13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	// El pin ser� salida push/pull	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	// Velocidad del puerto
	GPIO_Init(GPIOB, &GPIO_InitStructure);				// Pasa la config. a la estructura.
	
	// Configuramos PC13 de Salida LED en modo push-pull 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*   Funcion que configura puerto y pin para BOTON en PA5  */
//--------------------------------------------------------
void Configurar_BOTON(void){
	// Creamos la estructrua para GPIO Boton
	GPIO_InitTypeDef GPIO_InitStructure;			
	
	// Activamos el reloj para GPIO en puerto A
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	
	// Configura pin PA5 como entrada y resistor flotante 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*   Funcion que configura la interrupcion EXT9_5_IRQ  */
//------------------------------------------------------
void EXT5_Config(void){
	// Creamos la estructura para NVIC y EXTI
	NVIC_InitTypeDef NVIC_InitStructure; 	
	EXTI_InitTypeDef EXTI_InitStructure; 	

	//Configura la interrupcion EXTI5 en linea 5 de PA5
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// Condiguracion NVIC para EXT5 canal IRQ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*   Funcion que se ejecuta cuando se detecta la interrupcion    */
//-----------------------------------------------------------------
void EXTI9_5_IRQHandler(void) 
{
	// Comprueba si la linea Linea5 de EXTI ha tenido algún cambio  
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
	
	{
		int i;
		// Produce el parpadeo del LED PB1 4 veces como alarma al
    // recibir el pulso de detección del sensor
		for (i = 0; i < 6; i++)
		{			
      GPIO_SetBits(GPIOB, GPIO_Pin_1); /* Activa el pin PB1 */
			espera(80);
			GPIO_ResetBits(GPIOB, GPIO_Pin_1); /* Activa el pin PB1 */
			espera(80);
		}		
		
		// Desactiva la interrupcion para que se vuelva a comprobar
		EXTI_ClearITPendingBit(EXTI_Line5);  
	}
}

// Funcion de produce una espera
//---------------------------------------
void espera(unsigned int nCount)
{
	unsigned int i, j;
	for (i = 0; i < nCount; i++)
		for (j = 0; j < 0x2AFF; j++);
}


/******************************   Fin del programa   ******************************************/

