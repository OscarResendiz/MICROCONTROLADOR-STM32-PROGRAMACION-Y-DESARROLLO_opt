/**********************************************************************
  * Project Name: TIMER2
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
  *  PROGRAMA DE EJEMPLO DE MANEJO DEL TIMER
  *
  *  Enciende y apaga el LED de prueba en PC13	
  *
  *********************************************************************/
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

// Lista de funciones empleadas
void TIM4_Config(void);
void TIM4_IRQHandler(void);
void GPIO_Config(void);

/*             Modulo principal                    */
//==========================================================
int main(void)
{  
    TIM4_Config();  // Inicializamos el Timer4 y la interrupcion
  
    GPIO_Config();  // Configuramos el GPIO del LED
    
    while(1)
    {
        // No es necesario lineas de codigo adicionales
        // ya que la funcion que se ejecuta esta configurada
        // dentro de la funcion TIM4_IRQHandler
    }
}


/*  Funcion que configura el GPIO del LED en PC13    */
//-------------------------------------------------------------
void GPIO_Config(void)
{
    /* Inicializamos pin conecctado LED PC13 */
    GPIO_InitTypeDef GPIO_InitStructure;

    // Iniciamos reloj para PORTC 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // Configura el pin PC13 como salida en push-pull para LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
 
    GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Establecemos pin LED apagado
  
}

/*   Función que se ejecuta cuando se detecta el fin del periodo  */
//---------------------------------------------------------------
void TIM4_Config(void)
{    
    // Crea la estructura del TIMER_4
    TIM_TimeBaseInitTypeDef TIM_InitStructure;    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
   
    /* Configuramos los parametros del TIMER4 para
     detectar un desbordamiento cada vez que cuente
     un periodo de tiempo que será cada 1000 veces por segundo. */
    TIM_TimeBaseStructInit(&TIM_InitStructure);
    TIM_InitStructure.TIM_Prescaler = 8000; 
    TIM_InitStructure.TIM_Period = 500;   
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM4, &TIM_InitStructure);
  
    // Creamos la interrupcion para TIM2
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
    
    // Iniciamos el TIM2 programado
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
 
void TIM4_IRQHandler(void)
{
        if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
        {
            TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
            GPIOC->ODR ^= GPIO_Pin_13;
        }
}
 

////////////////////     Fin del  Programa  ////////////////////
