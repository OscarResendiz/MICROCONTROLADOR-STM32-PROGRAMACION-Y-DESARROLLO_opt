/**********************************************************************
  * Project Name: RTC_Alarma
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
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DE LA ALARMA EN EL RTC
  *
  *  En el programa se configura la Alarma del RTC para que se
  *  active a una hora determinada. 
  *  También se configura la interrupcion EXT_5 en el pin PA5
  *  donde se conecta un boton conectado a GND, que  al ser
  *  pulsado se desactiva la configuracion de la Alarma RTC.  
  * 
  **********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_bkp.h"  
#include "stm32f10x_exti.h"    
#include "stm32f10x_usart.h"
#include "misc.h"

#include <stdio.h>    // Funcion que permite usar el 'printf'

//  Lista de funciones ------------------------

void RTC_Init(void);
void NVIC_Configuration(void);
void Inicializa_Hora(uint8_t Horas,uint8_t Minutos,uint8_t Segundos);
void RTC_LeeHora(void);
void Muestra_Hora(uint32_t TimeValor);
void Establece_Alarma(int8_t Horas,uint8_t Minutos,uint8_t Segundos)   ;

void EXT5_Config(void);
void RTC_IRQHandler(void);
void EXTI9_5_IRQHandler(void) ;

void USART1_Config(void);
void USARTSend(char *pucBuffer);

/*       Permite configurar el reloj LCE o LSI       */
//-----------------------------------------------------
//  Selecciones una de las dos líneas             
//-----------------------------------------------------
//#define RTCClockSource_LSE  // Externo 
#define RTCClockSource_LSI //(*) Interno OK

uint32_t RTC_CountSecond = 0; // Variable donde se guarda seg. transcurridos

static int RTC_Estado = 0;   // Creamos la variable estado del RTC
static int ALARMA_Estado = 1;// Variable que indica Alarma activada o no

/*       Modulo Principal                            */
//=====================================================
int main(void)  
{  
  USART1_Config();    // Inicializamos el USART1
  
  NVIC_Configuration(); // Inicializamos el NVIC para IRQ del RTC
  
  EXT5_Config();      // Inicializa la interrupcion del Boton
  
  printf("\r\n====================================================\r\n");
  printf("  PROGRAMA DEMO DE UTILIZACION DE LA ALARMA DEL RTC  \r\n");
  printf("====================================================\r\n");

  // Comprobamos si los registros Bakcup no estan configurados
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
     printf("\r\n  RTC NO CONFIGURADO....\r\n");
    
     printf("\r\n  Reiniciando RTC, espere....\r\n");    
     RTC_Init();   // Inicializamos el RTC

     printf("\r\n  Configurando RTC, espere....\r\n");    
     // Inicializamos Horas, Minutos y Segundos   
     
     Inicializa_Hora(15, 00, 00);
    
     // Establece la Alarma  
     Establece_Alarma(15, 02, 00);
          
     printf("\r\n  --- RTC CONFIGURADO --- !!! \r\n");
   
     // Cargamos un valor en el registro 1 del Backup
     BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);    
  }
   else
  {
    /* Comprobamos si el reinicio es POR o PDR */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n  Un Reinicio a ocurrido....\r\n\r\n");
    }
    /* Comprobamos si el reinicio es por pulsar el pin NRST */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n  Un Reset externo se a detectado....\r\n\r\n");
    }
	
	printf("  NO es necesario configurar de nuevo el RTC....\r\n");
  printf("  ... Iniciando el RTC, espere ...\r\n");
  
  // Asignamos '1' para indicar un reinicio sin reseteo del RTC
  RTC_Estado = 1;	
  RTC_Init();     // Inicializamos el RTC
    
  /* Clear reset flags */
	RCC_ClearFlag();
	
  }
  
  while(1)
  {  
      // Muestra la hora cada 20 segundos
      if (RTC_GetCounter() - RTC_CountSecond > 10U)
      {
      printf("\r\n  Total segundos: %d", (int)RTC_CountSecond);
      RTC_LeeHora();
  
      // Restablece el valor del contador         
      RTC_CountSecond = RTC_GetCounter();
      }
  }
    
}

/*   Funcion que configura el RTC                    */
//-----------------------------------------------------
void RTC_Init(void)
{
  /* Iniciamos el reloj APB1 para PWR y BKP    */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | 
                         RCC_APB1Periph_BKP, ENABLE);
 
  /* Permitir acceso a los registros BKP */
  PWR_BackupAccessCmd(ENABLE);
  
  /* Comprobamos si es un reinicio de un nuevo RTC  */
  if (RTC_Estado != 1)
  {
    printf("  Reinicio del RTC \r\n"); 
    /* Reseteo de los registros Backup  */
    BKP_DeInit();
    
    // Reseteando registros backup del RTC
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
	printf("    - Reseteando backup de registros RTC\r\n");
  } 
   
  
  /////////////////////////////////////////////////
  // ----   Si se habilita el  reloj LSI    -------
  #ifdef RTCClockSource_LSI
  /////////////////////////////////////////////////
  /* Habilitar reloj LSI */  
  RCC_LSICmd(ENABLE);  
   printf("    - Habilitando fuente LSI \r\n");
  //	 
  /* Espere hasta que el LSI esté listo */
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET)
  {;}
  //
  /* Seleccionamos LSI como fuente de reloj del RTC */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	
  printf("    - Habilitando LSI en RTCCLK \r\n");
  /////////////////////////////////////////////////
  // ----  Si se habilita el reloj LSE     --------
  #elif defined RTCClockSource_LSE 
  /////////////////////////////////////////////////
  /* Habilitar reloj LSE */ 
  RCC_LSEConfig(RCC_LSE_ON);
  printf("    - Habilitando fuente LSE \r\n");
  //  
  /* Espere hasta que el LSE esté listo */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {;}
   
  // Deshabilitamos el reloj Lsi
  RCC_LSICmd(DISABLE);       
  printf("    - Deshabilitando LSI \r\n"); 
  //   
  /* Seleccionamos LSE como fuente de reloj del RTC */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  
  printf("    - Habilitando LSE para RTCCLK \r\n");
  #endif
  /////////////////////////////////////////////////
  /*  Habilita el reloj RTC             */  
  RCC_RTCCLKCmd(ENABLE);
  printf("    - Habilitando RTCCLK\r\n");  
  
  /* Espera hasta que finalice la última operación de escritura en registros RTC */
  RTC_WaitForLastTask();
  
  /* Espera a que se sincronice los registros RTC  */
  printf("    - Esperando sincronizacion \r\n");
  RTC_WaitForSynchro();

  /* Espera hasta que finalice la última operación de escritura en registros RTC */
  RTC_WaitForLastTask();
    
  /* Habilitar la Interrupcio RTC por Segundos */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);
  printf("    - Habilitando Interrupcion general RTC \r\n");
    
  /* Habilitar la Interrupcion ALARMA del RTC   */
  RTC_ITConfig(RTC_IT_ALR, ENABLE);
  printf("    - Habilitando Interrupcion de Alarma RTC \r\n");
  /* Espera hasta que finalice la última operación de escritura en registros RTC */
  RTC_WaitForLastTask();
  
  ////////////////////////////////////////////////////////////////////
  // Establecer el periodo del contador RTC en 1 segundo
  // Para LSE = RTCCLK/RTC period = 32768Hz/1Hz = 32767+1
  // para LSI = RTCCLK/RTC period = 40000Hz/1Hz = 39999+1
  ////////////////////////////////////////////////////////////////////
  #ifdef RTCClockSource_LSI
  ////////////////////////////////////////////////////////////////////
  /* Establecemos el Preescaler del RTC para un período de 1 segundo */
  RTC_SetPrescaler(39999); /* Peridodo RTC = RTCCLK/RTC_Prescal = (40 000 Hz)/39999+1) */
  printf("    - Configurado Prescaler LSI (40 kHz) \r\n");
  ////////////////////////////////////////////////////////////////////
  #elif defined  RTCClockSource_LSE
  ////////////////////////////////////////////////////////////////////
  /* Establecemos el Preescaler del RTC para un período de 1 segundo */
  RTC_SetPrescaler(32767); /* Periodo RTC = RTCCLK/RTC_Prescal = (32.768KHz)/(32767+1) */
  printf("    - Configurado Prescaler LSE (32,768 kHz) \r\n");
  #endif
  ////////////////////////////////////////////////////////////////////
    
  /* Espere hasta que finalice la última operación de escritura en registros RTC */
  RTC_WaitForLastTask();  
}

/*       Funcion que Lee la Hora en el RTC           */
//-----------------------------------------------------
void RTC_LeeHora(void)
{
  uint32_t TimeValor;   
 
  TimeValor = RTC_GetCounter();  // Leemos contador del RTC
 
  // Muestra la hora obtenida
  Muestra_Hora(TimeValor);  
}

/*  Funcion que extrae la hora, minutos y segundos del valor obtenido */
//----------------------------------------------------------------------
void Muestra_Hora(uint32_t TimeValor)
{  
  uint8_t THor_High, THor_Low, TMin_High, TMin_Low, TSeg_High, TSeg_Low;
  uint8_t THor, TMin, TSeg;
  
  // Extraemos los valores
  THor_High = (uint8_t) (TimeValor/3600) / 10;
  THor_Low  = (uint8_t) (TimeValor/3600) % 10;
  TMin_High  = (uint8_t) ((TimeValor%3600)/60)/10;
  TMin_Low   = (uint8_t) ((TimeValor%3600)/60)%10;
  TSeg_High  = (uint8_t) ((TimeValor%3600) %60)/10;
  TSeg_Low   = (uint8_t) ((TimeValor%3600) %60)%10;
      
  // Pasamos los valores de NibbleHIGH y NibbleLOW a su valor Decimal
  THor = (THor_High*10) + THor_Low;
  TMin = (TMin_High*10) + TMin_Low;
  TSeg = (TSeg_High*10) + TSeg_Low;
    
  // Imprimimos los valores obtenidos por USART1  
  printf("\r\n  HORA RTC:  %d:%d:%d " ,THor, TMin, TSeg);
  
  // Indica si la alarma esta activada
  if (ALARMA_Estado == 1)
  {
    printf (" (A) \r\n");
  }  else  {
    printf("\r\n");
  }
      
}

/*     Funcion que carga la hora nueva en el RTC                  */
//------------------------------------------------------------------
void Inicializa_Hora(uint8_t Horas,uint8_t Minutos,uint8_t Segundos) 
{   
  uint32_t CounterValue; 
  // Deshabilitar la Interrupcio RTC por Segundos 
  RTC_ITConfig(RTC_IT_SEC, DISABLE);
  // Habilitamos el acceso a los registros BacKup
  PWR_BackupAccessCmd(ENABLE);
  //RTC_WaitForLastTask();  
  
  // Convertimos segundos totales el horario nuevo
  CounterValue=((Horas * 3600) + (Minutos * 60) + Segundos);   
  
  // Grabamos el horario nuevo en los registros RTC  
  RTC_SetCounter(CounterValue);   

  // Esperamos a que se termine el acceso a los registros   
  RTC_WaitForLastTask();  
  
  // Rehabilitar la Interrupcio RTC por Segundos */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);  
}


/*    Funcion que configura NVIC para IRQ del RTC      */
//-------------------------------------------------------
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Configuramos la interrupcion general del RTC      */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
    
  /* Configuramos la Interrupcion IRQ de la Alarma     */
  NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
  /* Condiguracion NVIC para EXT5 canal 5 IRQ en PA5   */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}

/*   Funcion que configura la interrupcion EXT5_IRQ  */
//------------------------------------------------------
void EXT5_Config(void){
  
  // Creamos la estructrua para GPIO Boton en PA5
	GPIO_InitTypeDef GPIO_InitStructure;			
	
	// Activamos el reloj para GPIO en puerto A
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	
	// Configura pin PA5 como entrada del boton
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	// Creamos la estructura para EXTI
	EXTI_InitTypeDef EXTI_InitStructure; 	

	//Configura la interrupcion EXTI5 en linea 5 de PA5
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/*  Funcion que establece la hora de la Alarma en el RTC  */
//----------------------------------------------------------
void Establece_Alarma(int8_t Horas,uint8_t Minutos,uint8_t Segundos)   
{   
  uint32_t TimeValor;   
  
  // Convertimos el horario indicado a segundos totales  
  TimeValor=((Horas * 3600) + (Minutos * 60) + Segundos);             

  // Establecemos la Hora de la Alarma en el RTC
  RTC_SetAlarm(TimeValor);   
  RTC_WaitForLastTask();   
} 


/*     Funcion que se genera cada vez que se produce la IRQ del RTC  */
//---------------------------------------------------------------------
void RTC_IRQHandler(void)
{
  // Comprobamos si se ha producido la IRQ general del RTC
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    // Receteamos la bandera de la Interrupcio RTC por Segundos 
    RTC_ClearITPendingBit(RTC_IT_SEC);   

    // Esperamos a que se termine el acceso a los registros       
    RTC_WaitForLastTask();
    
    //  Reseteamos el contador del RTC cuando se detecte la hora 23:59:59 
    if (RTC_GetCounter() == 0x00015180)
    {
      // Reinicia el contador a '0'
      RTC_SetCounter(0x0);
      // Esperamos a que se termine el acceso a los registros       
      RTC_WaitForLastTask();
    }        
  }
  
  // Comprobamos si se ha producido la IRQ de la Alarma  
  if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {   
      RTC_ClearITPendingBit(RTC_IT_ALR);
      RTC_LeeHora();
      printf("  ALARMA ACTIVADA !!!!!!!\r\n");
      
      // Repetimos la alarma a los 3 minutos
      RTC_SetAlarm(RTC_GetCounter()+180);   
      RTC_WaitForLastTask();         
    }  
}

/*    Funcion que controla si se pulsa el boton      */
//-----------------------------------------------------
void EXTI9_5_IRQHandler(void) 
{
	// Comprueba si se ha producido la interrupcion
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
	
	{
		// Desactiva la interrupcion para que se vuelva a comprobar
		EXTI_ClearITPendingBit(EXTI_Line5);  
    
    // Reseteamos la hora de la alarma
    RTC_SetAlarm(0x0); 
    RTC_WaitForLastTask();
    
    RTC_ITConfig(RTC_IT_ALR, DISABLE);
    printf("  ALARMA DESABILITADA !!!!!! \r\n");
    ALARMA_Estado = 0;  // Alarma desactivada    
	}
}

/*           Funcion que configura el USART1           */
//-------------------------------------------------------
void USART1_Config(void)
{     
      /* Configuramos el GPIO para USART1 -----------------*/
	    GPIO_InitTypeDef GPIO_InitStructure;
  
	    /* Habilitamos los reloj para GPIO y el USART1     */
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	    /* Configuramos PA9 para Tx USART1 ------------------*/
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);

	    /* Configuramos Rx para Rx USART1 -------------------*/
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);

	    /* Configuramos el USART1  --------------------------*/
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

// Funncion que nos permite utilizar el comando 'printf'
int fputc(int ch, FILE *f)
{
	/* Envia dato */
	USART_SendData(USART1, (unsigned char) ch);
	
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

/////////////////////  Fin  del  Programa    ///////////////

