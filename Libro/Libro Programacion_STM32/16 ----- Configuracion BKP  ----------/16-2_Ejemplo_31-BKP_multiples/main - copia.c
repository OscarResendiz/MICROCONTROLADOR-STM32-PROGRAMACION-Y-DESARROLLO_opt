/**********************************************************************
  * Project Name: BKP_multi
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
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DE VARIOS REGISTROS BKP
  * 
  * Programa ejemplo de utilizacion de varios registros del BKP
  *
  **********************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_usart.h"
#include "misc.h"

#include <stdio.h>

// Definimos el maximo de registros de nuestro microcntrolador
#define NUM_Max_RBKP 10  

// Creamos una variable indexada para su manejo
uint16_t RegistroBKP[NUM_Max_RBKP] = { BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4,
        BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8, BKP_DR9, BKP_DR10};


/* Funcion que inicializa el modulo BKP                  */
//---------------------------------------------------------
void BKP_Init()
{
  // Habilitamos los relojes para modulos BKP y PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
  // Deshabilitamos la protección de escritura en los registros BKP
	PWR_BackupAccessCmd(ENABLE);
}

/*        Funcion que graba datos en los registros BKP     */
//-----------------------------------------------------------
void Graba_Registro(uint16_t index, uint16_t valor)
{
  // Grabamos en el registro BKP_DR(index) un dato
	BKP_WriteBackupRegister(RegistroBKP[index], valor);
}

/*        Funcion que lee datos de los registros BKP       */
//-----------------------------------------------------------
uint16_t Lee_Registro(uint16_t index)
{
  // Leemos el valor del 'Contador' guardado en el registro 1 del BKP
	uint16_t valor = BKP_ReadBackupRegister(RegistroBKP[index]);
  
  return valor;
}

//  Lista de funciones ----------------------------------------
void USART1_Config(void);

/*                  Funcion          Principal                */
//**************************************************************
int main(void)
{
	uint16_t Contador = 0;// Creamos la variables 'Contador'
  
	USART1_Config();      // Inicializamos el USART1

  BKP_Init();           // Inicializamos el modulo BKP
  
  printf("\r\n =====================================\r\n");
  printf("  PROGRAMA DEMO DE UTILIZACION DE BKP  \r\n");
  printf(" =====================================\r\n");

  // Leemos el valor del 'Contador' guardado en el registro 1 del BKP
	Contador = Lee_Registro(0);

  // Aumentamos en 1 el 'Contador' tras el reinicio
	Contador++;
	
  // Volvemos a grabar el valor actual del 'Contador'
	Graba_Registro(0, Contador);  
 
  printf(" Registro BKP_DR1 : %d", Lee_Registro(0));
  
  
  // Comprobamos si los registros contienen informacion    
  if (Lee_Registro(2) != 102)
  {
     printf("\r\n  Registro BKP vacios, espere...\r\n");   
     
     // Si estan vacios los llenamos con datos      
     for (uint32_t Indice = 1; Indice < NUM_Max_RBKP; Indice++)
     {
      Graba_Registro(Indice, 100 + (Indice));
     }
     
     printf("\r\n  ... Registros BKP rellenados\n\r\n"); 
  }
  // Si los registros contienen datos
  else {
	  
	  // Comprobamos si el reinicio es POR o PDR o pin reset     
    if (RCC_GetFlagStatus(RCC_FLAG_SFTRST))
    {
       printf("\r\n  Un Reinicio por software ocurrido....\r\n\r\n");
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_PORRST))
    {
       printf("\r\n  Reinicio por Power On/Off detectado....\r\n\r\n");
    }
     
    // Comprobamos si el reinicio es por pulsar el pin NRST 
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST))
    {
       printf("\r\n  Boton Reset pulsado detectado....\r\n\r\n");
    }     
          
     for (uint32_t Indice = 1; Indice < NUM_Max_RBKP; Indice++)
     {                     
       // Leemos el valor del 'Contador' guardado en el registro 1 del BKP
       printf(" Valor Registro BKP_DR%2d : %d \n\r", Indice+1, Lee_Registro(Indice));                     
     }     
     
  }
  // Reseamos las banderas RCC para reutilizarlas
  RCC_ClearFlag(); 

  // Mostramos por el puerto USART1 el valor actual del 'Contador'
	printf("\r\n Valor Contador del BKP_DR1: %d\r\n", Contador);
	
  while(1)   
    {
    }
}

/*          Funcion que configura el USART1                 */
//------------------------------------------------------------
void USART1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

// Funcion que nos permite utilizar el comando 'printf'
int fputc(int ch, FILE *f)
{
	/* Envia dato */
	USART_SendData(USART1, (unsigned char) ch);
	
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

///////////////////////////////   Fin del programa   ////////////////////////////

