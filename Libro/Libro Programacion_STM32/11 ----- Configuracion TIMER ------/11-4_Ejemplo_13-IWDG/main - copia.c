/**********************************************************************
  * Project Name: IWDG
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
  * PROGRAMA DE PRUEBA INERRUPCION IWDG WATCHDOG
  * 
  * Inmediatamente después de iniciarse el sistema, se enciende
  * y se apaga el LED de prueba en PC13 y el IWDG comienza 
  * a restar del contador un 1 al valor indicado por el comando 
  * 'IWDG_SetReload'.
  *
  * Al llegar a '0', se reiniciara el sistema y volvera a repetirse
  * el proceso reiniciandose automatica cada 0.5 seg. constantemente.
  *
  * Si se pulsa el botón conectado al pin PB0, la cuenta del IWDG
  * se cambia y se establece ahora a 4 segundos. 
  *
  * Tiempo que será el que tardara ahora en reiniciarse y con ello
  * se cargará la configuracion inicial de reciniciarse cada 0.5 segundos.
  *
  *
  **********************************************************************/

// Libreria principal del microcontrolador
#include "stm32f10x.h"

// Las librerias para los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_iwdg.h"

/*         FUncion que configura los GPIO    */
//----------------------------------------------
void GPIO_Config(void)
{
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
	GPIO_Init(GPIOB, &GPIO_StructureInit);		
}


/*  Funcion que configura inicialmente el IWDG para que
    se reinicie el sistema cada 0,5 segundos            
    ----------------------------------------------------
      T_out = 500ms / (32/32) = 500                     */
//---------------------------------------------------------
void IWDG_setup(){
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
  IWDG_SetPrescaler(IWDG_Prescaler_32); // set clock to 32 KHz/32 = 1 KHz
  IWDG_SetReload(500);                  // reload every 500 ms -> 0,5 seg
  IWDG_ReloadCounter();
  IWDG_Enable();
}

/*  Funcion que permite reconfigurar el IWDG en un tiempo
    determinado -en el ejemplo pre = 32, rlr = 4000    
    -----------------------------------------------------
     T_out = Tiempo(ms) / (Frc_LSI * IWDG_Prescaler)
     P.ej. 4000ms (4seg) Presc = 32
     T_out = 4000ms / (32 * 32) = 4000                 */
//--------------------------------------------------------
void IWDG_ReInit(u8 prer, u16 rlr)
{                
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
    IWDG_SetPrescaler(prer);  
    IWDG_SetReload(rlr);  
    IWDG_ReloadCounter(); 
    IWDG_Enable();  
}

/*             Modulo Principal               */
//******************************************************
int main(void){	

  GPIO_Config();
	
  GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // Encendemos el LED con '0'
  
  // Introducimos un retardo al inciar el sistema */
  for(int i=0;i<0x100000;i++);

  /* Iniciamos el reloj LSI */
  RCC_LSICmd(ENABLE);
  
  /* Esperamos a que la bandera del reloj LSI esste operativo  */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /*  Configuramos el WatchDog para provocar un reinicio 
     del sistema cada 1 Segundo                         */
  IWDG_setup();
  
  /* Apagamos el LED para mostrar cuando se ha reiniciado
     el sistema poniendo el pin a valor '1'             */
  GPIO_SetBits(GPIOC, GPIO_Pin_13);
	
	while(1){
		
		// Comprueba cuando el pin PB1 esta a nivel bajo '0'
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0 )  
		{
			//  Encendemos el led
      GPIO_ResetBits(GPIOC, GPIO_Pin_13); /* Desactiva el pin PC13 */			      
      
      // Reconfiguramos el IWDG para un retardo de unos 4 segundos
      IWDG_ReInit(IWDG_Prescaler_32,4000);                  
      
		}
		else
		{
      // Apagamos el led si no se ha pulsado el boton
			GPIO_SetBits(GPIOC, GPIO_Pin_13);	 /* Activamos el pin PC13 */			
		}
	}
}


//////////////////////////  Fin del Programas ////////////////////

