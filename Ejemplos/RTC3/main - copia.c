/**********************************************************************
  * Project Name: RTC_Calendario
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
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL CALENDARIO EN EL RTC
  * 
  * Programa que muestra el empleo del RTC como calendario de
  * fecha y hora.
  * 
  **********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"        
#include "stm32f10x_pwr.h"
#include "stm32f10x_usart.h"
#include "misc.h"

#include <stdio.h> // Funcion que permite usar el 'printf'

// Definicion de variables  -----------------------------------
#define JULIANO_BASE_DATE	2440588 // Dia inicial del Calendario Juliano 1/1/1970 0:00 UTC
#define TSegundosenDia      86400 // (3600*24) Segundos en un dia

typedef struct
{
	uint8_t RTC_Horas;
	uint8_t RTC_Minutos;
	uint8_t RTC_Segundos;
	uint8_t RTC_Dia;
	uint8_t RTC_DSem;
	uint8_t RTC_Mes;
	uint16_t RTC_Anno;
} RTC_FechaHoraTypeDef;

// Creamos la estructura para la Fecha y la Hora
RTC_FechaHoraTypeDef	RTC_DateTimeStructure;

// Seleccion de la fuente del reloj RTC
//------------------------------------------
//#define RTCClockSource_LSE  // Externo 
#define RTCClockSource_LSI //(*) Interno OK

uint8_t const *DiasSemana[] = {"Lunes","Martes","Miercoles","Jueves","Viernes","Sabado","Domingo"};
uint8_t const *Meses[] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiempre","Octubre","Noviembre","Diciembre"};

uint8_t RTC_Estado = 0;   // Creamos la variable estado del RTC

// Lista de Funciones   ------------------------
void RTC_Init(void);
void RTC_IRQHandler(void);
void USART1_Config(void); 
  
/*     Funcion que lee el contador RTC y lo convierte a Fecha y Hora  */
//-----------------------------------------------------------------------------------
void Lee_RTC(uint32_t RTC_Contador, RTC_FechaHoraTypeDef* RTC_DateTimeStruct) {
	unsigned long Tiempo;
	unsigned long t1, a, b, c, d, e, m;

  uint16_t Temp_Anno;
  uint8_t Temp_Mes, Temp_DSem, Temp_Dia, Temp_Horas, Temp_Minutos, Temp_Segund;
  
	uint64_t jd = 0;;
	uint64_t TDiasJulianos = 0; 

	// Averiaguamos el Total de dias del Calendario Juliano
  //            43200=seg*12horas / 86400 Segundos en un dia
  //a = ((counter + 43200)/(SECOND_A_DAY>>1)) + (2440587<<1) + 1;  
  jd = ((RTC_Contador+43200)/(86400>>1)) + (2440587<<1) + 1;
	TDiasJulianos = jd>>1;   

	Tiempo = RTC_Contador;
	t1 = Tiempo/60;    
	Temp_Segund = Tiempo - t1*60;

	Tiempo = t1;
	t1 = Tiempo/60;
	Temp_Minutos = Tiempo - t1*60;

	Tiempo = t1;
	t1 = Tiempo/24;
	Temp_Horas = Tiempo - t1*24;

	// Extraemos el el resto [%] como num de día de la Semana	
  Temp_DSem = TDiasJulianos%7;

	// Extraemos los dias, meses y anos de los TDias del Calendario Juliano
  a = TDiasJulianos + 32044;
	b = (4*a+3)/146097;     // (400x365)+100-3=146097 = Total dias de 400 anos(CuadCentury) 
	c = a - (146097*b)/4;
	d = (4*c+3)/1461;       // 1461 => 4 anos * 365.25 = 1460,8 ~ 1461
	e = c - (1461*d)/4;
	m = (5*e+2)/153;
	Temp_Dia = e - (153*m+2)/5 + 1;
	Temp_Mes = m + 3 - 12*(m/10);
	Temp_Anno = 100*b + d - 4800 + (m/10);

  // Pasamos los datos a la estructura 
	RTC_DateTimeStruct->RTC_Anno = Temp_Anno;
	RTC_DateTimeStruct->RTC_Mes = Temp_Mes;
	RTC_DateTimeStruct->RTC_Dia = Temp_Dia;
  RTC_DateTimeStruct->RTC_DSem = Temp_DSem;
	RTC_DateTimeStruct->RTC_Horas = Temp_Horas;
	RTC_DateTimeStruct->RTC_Minutos = Temp_Minutos;
	RTC_DateTimeStruct->RTC_Segundos = Temp_Segund;	
}

/*  Funcion que pasa la Nueva fecha a Segundos    */
//-------------------------------------------------------------
uint32_t Convierte_FechaHora(RTC_FechaHoraTypeDef* RTC_DateTimeStruct) {
	uint8_t a;
	uint16_t y;
	uint8_t m;
  uint32_t TDiasJulianos, TSegDJulianos;

	// Calcular los dias del Calendario Juliano
  //-------------------------------------------------------------
  // a = (14 - Mes) /12
  // y = Ano + 4800 - a
  // m = Mes + 12 * a -3
  // JDN = Dia + (((153*m +2) /5) + (365*y) + (y/4) - (y/100) + (y/400)) - 32045;
  //-------------------------------------------------------------------------------
  a=(14-RTC_DateTimeStruct->RTC_Mes) / 12;
	y=RTC_DateTimeStruct->RTC_Anno + 4800 - a;
	m=RTC_DateTimeStruct->RTC_Mes+(12*a)-3;

	// Calculamos el total de Dias de la Fecha
    TDiasJulianos=RTC_DateTimeStruct->RTC_Dia;
	TDiasJulianos+= (((153*m+2)/5) + (365*y) + (y/4) - (y/100)	+ (y/400)) - 32045;
	TDiasJulianos = TDiasJulianos - JULIANO_BASE_DATE;
  // Pasamos los días a segundos
	TSegDJulianos*= 86400;  // x 86400 Segundos en un dia
  
  // Sumamos al total de Segundos los segundos de la hora
	TSegDJulianos+=(RTC_DateTimeStruct->RTC_Horas*3600);  // + TSegundos Horas
	TSegDJulianos+=(RTC_DateTimeStruct->RTC_Minutos*60);  // + TSegundos Minutos
	TSegDJulianos+=(RTC_DateTimeStruct->RTC_Segundos);    // + TSegundos

  // Devolvemos el total de segundos de la fecha inicial al RTC
	return TSegDJulianos;
}

/*       Modulo Principal                            */
//========================================================================================
int main(void)
{
	uint32_t RTC_Contador = 0;    // Creamos la variable contador
  
	USART1_Config();    // Inicializamos el USART1
  
  printf("\r\n =======================================================\r\n");
  printf("  PROGRAMA DEMO DE UTILIZACION DEL CALENDARIO DEL RTC  \r\n");
  printf(" =======================================================\r\n");
  
  // Comprobamos si los registros Bakcup no estan configurados
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
     printf("\r\n  RTC NO CONFIGURADO....\r\n");    
     printf("\r\n  Reiniciando RTC, espere....\r\n");         
     printf("\r\n  Configurando RTC, espere....\r\n");   
     RTC_Estado = 0;	
     RTC_Init();
  
     // Establecemos el Calendario Inicial
     // 02-03-2018
     RTC_DateTimeStructure.RTC_Dia = 04;
     RTC_DateTimeStructure.RTC_Mes = 03;
     RTC_DateTimeStructure.RTC_Anno = 2018;
     // 20:03:10
     RTC_DateTimeStructure.RTC_Horas = 06;
     RTC_DateTimeStructure.RTC_Minutos = 03;
     RTC_DateTimeStructure.RTC_Segundos = 10;		
     
     RTC_SetCounter(Convierte_FechaHora(&RTC_DateTimeStructure));   
          
     printf("\r\n  --- RTC CONFIGURADO --- !!! \\n\r\n");
   
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
      printf("  ... Iniciando el RTC, espere ...\r\n\r\n");        
      RTC_Estado = 1;	
      RTC_Init();
      }
    
  /* Reseteamos banderas RCC para volver a usarlas */
	RCC_ClearFlag(); 	
	
	while(1)
  {    	         	

		if (RTC_GetCounter() - RTC_Contador > 10U) 
    {
      RTC_Contador = RTC_GetCounter();    	
      printf("\r\n\r\n  Contador segundos RTC : %d\r\n", (int)RTC_Contador);
      
		  // Lee RTC
      Lee_RTC(RTC_Contador, &RTC_DateTimeStructure);
      
      // Muestra la fecha y hora leida		  
      printf("  Fecha: %02d/%02d/%04d Hora: %02d:%02d:%02d\r\n",      
              RTC_DateTimeStructure.RTC_Dia, 
              RTC_DateTimeStructure.RTC_Mes, 
              RTC_DateTimeStructure.RTC_Anno,
              RTC_DateTimeStructure.RTC_Horas, 
              RTC_DateTimeStructure.RTC_Minutos, 
              RTC_DateTimeStructure.RTC_Segundos);		  
      
      printf("  Dia %s, %d de %s de %04d \r\n",
              DiasSemana[RTC_DateTimeStructure.RTC_DSem],
              RTC_DateTimeStructure.RTC_Dia,
              Meses[RTC_DateTimeStructure.RTC_Mes -1],
              RTC_DateTimeStructure.RTC_Anno);
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

	    /* Configuramos PA10 para Rx USART1 -------------------*/
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

// Funcion que nos permite utilizar el comando 'printf'
int fputc(int ch, FILE *f)
{
	/* Envia dato */
	USART_SendData(USART1, (unsigned char) ch);
	
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
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
  }    
}

////////////////////////   Fin del Programa   //////////////////////////////////
