/**********************************************************************
  * Project Name: TIMER3
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
  *  Cuenta el tiempo transcurrido entre que se pulsa el boton en
  *  el pin PB0 y pulsar el boton ern PB1, enviando el valor del
  *  tiempo transcurrido en ms por el puerto USART1
  *
  *********************************************************************/
 
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"

// Libreria para manejo del USART1
#include "usart1.h"

// Librería complementaria para manejo del comando Sprintf
#include <stdarg.h>

// Lista de funciones utilizadas
void GPIO_Config(void);
void TIM_Config(void);
void TIM4_IRQHandler(void);

// Lista de variables empleadas
volatile int TimeElapsed;     // Variable que guarda el tiempo final transcurrido
volatile int TimeSec;
volatile uint8_t TimeState = 0;
float TimeSEGUNDOS = 0;

 
/*            Modulo principal      */
//-----------------------------------------
int main(void)
{
    char buffer[80] = {'\0'};
    
    GPIO_Config();    //   Configuramos el GPIO de Pulsadores y LED testigo
    
    TIM_Config();     // Configuramos e inicializamos el TIMER

    USART1_Config();  // Configurtamos la comunicacion por USART1
    
    while(1)
    {
        if (TimeState == 0) {
            // Comprueba si se ha pulsado el boton PB0
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {			
                TIM_Cmd(TIM4, ENABLE);
                TIM_SetCounter(TIM4, 0);
                TimeSec = 0;
                // Establecemos Bandera de Tiempo inicial = 1 (On)
                TimeState = 1; 
                // Apagamos el LED testigo en PC13
                GPIO_ResetBits(GPIOC, GPIO_Pin_13);
                // Imprimimos un mensaje de inicio por el USART1
                USART1_printf(USART1, " Inicio...");
            }
        }
 
        if (TimeState == 1) {
            // Comprueba si se ha pulsado el boton PB1
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
                // Sumamos en la variable 'TimeElapsed' el tiempo
                // transcurrido mediante la formula                
                TimeElapsed = TIM_GetCounter(TIM4)/10 + TimeSec * 1000; // Time im msec
                TIM_Cmd(TIM4, DISABLE);
                // Bandera de tiempo transcurrido finalizado = 0
                TimeState = 0; 
                // Encendemos el LED testigo en PC13
                GPIO_SetBits(GPIOC, GPIO_Pin_13);
                // Imprimimos el tiempo transcurrido por USART1
                TimeSEGUNDOS = TimeElapsed * 0.001;
                USART1_printf(USART1, "Tiempo transcurrido: %d ms ", TimeElapsed);
                printf( "-> %4.2f Seg. \r\n", TimeSEGUNDOS);
                                
            }
        }
    }
}


/*  Funcion donde configuramos los GPIO   */
//---------------------------------------------
void GPIO_Config(void)
{
    /* Configuramos GPIO para el LED en PC13  */
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    // Iniciamos el reloj para PORTC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    /* Configuramos GPIO para LED en PC13 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
  
    GPIO_SetBits(GPIOC, GPIO_Pin_13); 
 
    /* Configuramos el GPIO para PB0 y PB1 */
    // Iniciamos el reloj para PORTB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
    /* Configuramos los pines para los pulsadores en PB0 y PB1  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
}


/*     Funcion que configura el TIMER4     */
//---------------------------------------------------
void TIM_Config(void)
{
    // Crea la estructura del TIMER_4 TIMER4
    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
    // Inicializa el reloj para el TIMER4 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    /* Configuramos los parametros del TIMER4    */
    TIM_TimeBaseStructInit(&TIMER_InitStructure);    
    TIMER_InitStructure.TIM_Prescaler = 7200; 
    TIMER_InitStructure.TIM_Period = 10000; 
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);
  
    // Creamos la interrupcion para TIM4
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
    // Iniciamos el TIM4 programado
    TIM_Cmd(TIM4, ENABLE);
 
    /* Configuracion del NVIC  */
    /* Configurar e iniciar la interrupcion TIM4_IRQn */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
} 

/*   Funcion que se ejecuta cuando se detecta la primera interrupcion  */
//----------------------------------------------------------------------
void TIM4_IRQHandler(void)
{
        if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
        {
          TIM_ClearITPendingBit(TIM4, TIM_IT_Update);            
          // Se va incrementando la variable de tiempo
          TimeSec++; 
        }
}

