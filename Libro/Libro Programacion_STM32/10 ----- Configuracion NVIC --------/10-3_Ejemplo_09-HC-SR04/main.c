/**********************************************************************
  * Project Name: HC-SR04
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
  *  PROGRAMA DE EJEMPLO DE MANEJO DEL TIMER COMO CONTADOR DE PULSOS
  *
  *  Prueba con el dispositivo HC-SR04:
  *
  *  Convierte el tiempo transcurrido entre la señal de TRIGGER
  *  y cuando se recibe la señal de recepcion ECHO y lo convertimos
  *  en logitud de distancia hacia un objeto.  
  *
  *  Conexiones:
  *      HC-SR04    STM32F103
  *         Vcc -----  5 Vcc
  *     Trigger -----  PB15
  *        Echo -----  PB0   (EXTI_Line0)
  *         Gnd -----  Gnd
  *
  *********************************************************************/
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"

// Libreria para control de Interrupciones Externas
#include "stm32f10x_exti.h"

// Librerria para manejo del USART1
#include "usart1.h"

// Libreria para manejo de variables
#include "stdio.h"

// Lista de funciones utilizadas
void GPIO_Config(void);
void TIM_Config(void);
void EXTI0_IRQHandler(void);

// Lista de varianles empleadas
volatile uint8_t Senal_ECHO = 0;
volatile uint16_t Valor_Escanner;

/*  Funcion que genera la señal de Trigger   */
//-----------------------------------------------
void HCSR04_start() {
	int i;
	GPIO_SetBits(GPIOB, GPIO_Pin_15);   // Activamos PB15	
	for(i=0;i<0x7200;i++);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15); // Apagamos PB15
}


/*  Funcion que cuenta el tiempo transcurrido  */
unsigned int HCSR04_get() {
	unsigned long Escanner;  
	Escanner = (unsigned long)Valor_Escanner /75;      
	return (unsigned int)Escanner;
}

/*                  Modulo principal                      */
//=============================================================
int main(void)
{
    GPIO_Config();    // Configuramos el GPIO de señales Trigger y Echo
    TIM_Config();     // Configuramos e inicializamos el TIMER
    USART1_Config();  // Configurtamos la comunicacion por USART1
  
    while(1)
    {
      // Comprobamos si la señal ECHO se ha recibido
      if (Senal_ECHO == 1) {
        // Imprimimos por el puerto USART1 el valor obtenido 
        USART1_printf(USART1, "  Distancia %d cm\r\n", HCSR04_get());
        // Desactivamos la bandera de deteccion de la señal ECHO
    		Senal_ECHO = 0;
    	}
    }
}


/*    Funcion que configura el GPIO  */
//---------------------------------------------
void GPIO_Config(void) 
{
    /* Crea la estructura GPIO ----------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;
  
    /* Habilita el reloj para PORTC -----------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	
	/* Creamos señal Trigger en Pin PB15 como salida ------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Creamos señal Echo en pin PB0 como entrada ---------*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    /* Crea la interrupcion EXTI en PB0 -------------------*/
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Habilita el reloj para AFIO ------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Creamos el NVIC para la IRQ del EXTI ---------------*/
	/* Configuramos el pin PB0 conectado como EXTI_Line0 --*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);

	/* Inicializa el pin PB0 para la interrupcion EXTI_Line0 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);

	/* Configura el pin PD0 para EXTI_Line0 --------------*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	
	EXTI_Init(&EXTI_InitStructure);
}

/*     Funcion que configura el TIMER_3                  */
//---------------------------------------------------------
void TIM_Config(void)  
{
    /* Creamos la configuracion para el Timer TIM3 ----------*/  
	TIM_TimeBaseInitTypeDef timer_base;
    TIM_TimeBaseStructInit(&timer_base);
    timer_base.TIM_CounterMode = TIM_CounterMode_Up;
    timer_base.TIM_Prescaler = 72;
    TIM_TimeBaseInit(TIM3, &timer_base);
    TIM_Cmd(TIM3, ENABLE);
      
    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
    
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  	TIM_TimeBaseStructInit(&TIMER_InitStructure);
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Prescaler = 7200;
    TIMER_InitStructure.TIM_Period = 5000;
    TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);
  
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  
    TIM_Cmd(TIM4, ENABLE);

    /* Creamos la interrupcion para la interrupcion del Timer */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*  Funcion que se ejecuta cuando la interrupcion es detectada */
//-----------------------------------------------------------------
void TIM4_IRQHandler(void)
{
        if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
        {
        	HCSR04_start();
			Senal_ECHO = 1;
        	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        }
}

/*  Funcion que se ejecuta cuando se detecta la Interrupcion EXTI_0  */
//-------------------------------------------------------------------
void EXTI0_IRQHandler(void) 
{	
	// Comprueba si la bandera de estado esta activa	
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) 
	{
        // Comprueba si PB0 esta activo
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) != 0) {
			// Inicia el conteo del tiempo 
			TIM_SetCounter(TIM3, 0);
		}
        
		// COmprueba si el PB0 se ha desactivado
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
			// Pasa el valor del conteo obtenido a la variable 'Escanner'
			Valor_Escanner = TIM_GetCounter(TIM3);
		}
		
	// Reinicia la Interrupcion EXT_0 
	EXTI_ClearITPendingBit(EXTI_Line0);
	}
}


////////////////   Fin del Programa ///////////////////////////////
