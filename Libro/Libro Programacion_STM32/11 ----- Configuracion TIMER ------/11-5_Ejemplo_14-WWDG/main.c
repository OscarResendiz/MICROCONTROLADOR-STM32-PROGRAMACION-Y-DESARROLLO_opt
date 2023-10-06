/**********************************************************************
  * Project Name: WWDG
  * File Name: main.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroL
  * =====================================================================
  * 
  * PROGRAMA DE PRUEBA INTERRUPCION WWDG WATCHDOG
  * 
  *  Programa de ejemplo de manejo del Watchdog WWDG.
  *  En el que configuramos el contador del WWDG para que
  *  se reinicie cada .... pero que establecemo un delay
  *  en el modulo principal para que se restablezca el 
  *  contador antes de que se produzca el restablecimiento
  *  del contador.
  *
  **********************************************************************/

// Libreria principal del microcontrolador
#include "stm32f10x.h"

// Las librerias para los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_wwdg.h"
#include "misc.h"

#include "delay.h"
 
void LED_Init(void);

/*   Funcion de servicio que se ejecuta cuando se produce 
     el restablecimiento                                   */
//-----------------------------------------------------------
void WWDG_IRQHandler(void) {    
   
  WWDG_ClearFlag();       // Reseteamos la bandera del WWDG
  WWDG_SetCounter(100);  // (101) Valor entre 64 <--> 127
  /* Toggle LED en PC13*/
  GPIOC->ODR ^= GPIO_Pin_13;
}
 

/*                  Modulo Principal                  */
//-----------------------------------------------------
int main(void)
{
  LED_Init();     // Inicializamos el GPIO del LED de pruebas
  
  DelayInit();    // Inicializamos la libreria 'delay'
  
  /*  Inicializamos el relon del Watchdog WDG    */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
  
  WWDG_DeInit();	// Inicializamos el Watchdog
  
  /* Donde configuramos el WWDG con los siguientes valores:
    WWDG_clock_counter = (PCLK1 (36MHz)) /4096)/8 = 1098,6 Hz (~910 us)  
    ----------------------------------------------------------------*/
  WWDG_SetPrescaler(WWDG_Prescaler_8); // Valores de Prescaler 1, 2, 4, 8
  
  /* Ventana con valor 0x50 (80 decimal), que significa que el
     contador WWDG debe actualizarse solo cuando el contador 
     esté por debajo de 0x50 (80) y por encima de 0x40 (64),
     de lo contrario se generara un restablecimiento y un
     reseteo del sistema.                                                  
   --------------------------------------------------------------*/     
  WWDG_SetWindowValue(0x50); // (80) valor debe ser < 127
  
  /* Establecemos el tiempo de restablecimiento:
     El contador del WWDG  se establece en 0x7F (127) los
     que se genera un timeout de = ~910 us * 64 = 58.2 ms
     ----------------------------------------------------*/    
  WWDG_Enable(0x7F);   // (127) Valor entre 64 <--> 127
  
  WWDG_ClearFlag();	  // Borramos y reiniciamos la bandera del WWDG
  
  WWDG_EnableIT();    // Habilitamos la interrupcion WWDB
 
  /*  Iniciamos el NVIC para la interrupcion   */
  NVIC_InitTypeDef NVIC_InitStructure;  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    /*Interrupcion WWDG */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
    
while (1)
  {               
    /*  Mientras se restablezca el contador en las siguientes
        lineas, antes de que se produzca el restablecimiento
        no se producirá el reseteo del sistema.
    
        Si desabilitamos las dos líneas, o bajamos el valor 
	    del retraso generado con el delay a un valor que deje 
	    que se restablezca el contador WWDG, se producira
        el restablecimiento del WWDG automáticamente y veremos
        encenderse el LED en PC13 indicando que se esta produciendo 
        el restablecimiento.   
        ----------------------------------------------------------*/    
    DelayMs(91);           // < 91 Se produce el reseteo
    WWDG_SetCounter(100);  // Valor debe estar entre 0x40(64) y 0x7F(127)
  }   
}

/*     Funcion que configura el GPIO del LED de pruebas         */
//----------------------------------------------------------------
void LED_Init(void)
{
   /* Creamos la estructura GPIO */
   GPIO_InitTypeDef  GPIO_InitStructure;
 	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);      
   
   /* Configuramos el pin PC13 para el LED  */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
   GPIO_Init(GPIOC, &GPIO_InitStructure);			
  
   // Apagamos el LED al inicio
   GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

//////////////   Fin del Programa   /////////////////////
