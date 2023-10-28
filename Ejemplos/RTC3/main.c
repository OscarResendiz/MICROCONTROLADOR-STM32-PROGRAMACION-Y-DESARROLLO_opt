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
#define TSegundosenDia     86400 // (3600*24) Segundos en un dia
#define BAKUPR 0xA5A3
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
uint8_t const *DiasSemana[] = {"Lunes","Martes","Miercoles","Jueves","Viernes","Sabado","Domingo"};
uint8_t const *Meses[] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiempre","Octubre","Noviembre","Diciembre"};
double fJuliano;
uint8_t RTC_Estado = 0;   // Creamos la variable estado del RTC

// Lista de Funciones   ------------------------
void RTC_Init(void);
void RTC_IRQHandler(void);
void USART1_Config(void); 
double fechaJuliana(int anio, int mes, int dia, int hora, int minuto, int segundo) ;  
void julianaAGregoriana(double juliana, int *anio, int *mes, int *dia, int *hora, int *minuto, int *segundo) ;
void valida(void);
/*     Funcion que lee el contador RTC y lo convierte a Fecha y Hora  */
//-----------------------------------------------------------------------------------
void Lee_RTC(double RTC_Contador, RTC_FechaHoraTypeDef* RTC_DateTimeStruct)
 {
  int Temp_Anno;
  int Temp_Mes, Temp_DSem, Temp_Dia, Temp_Horas, Temp_Minutos, Temp_Segund;
  julianaAGregoriana(RTC_Contador, &Temp_Anno, &Temp_Mes, &Temp_Dia, &Temp_Horas, &Temp_Minutos, &Temp_Segund);
  Temp_DSem=Temp_Dia%7;
  printf("\r\njuliano=%f año=%d mes=%d dia=%d hora=%d minutos=%d segundos=%d\r\n",RTC_Contador,Temp_Anno,Temp_Mes,Temp_Dia,Temp_Horas,Temp_Minutos,Temp_Segund);
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
double Convierte_FechaHora(RTC_FechaHoraTypeDef* RTC_DateTimeStruct)
 {
  return fechaJuliana(RTC_DateTimeStruct->RTC_Anno, RTC_DateTimeStruct->RTC_Mes, RTC_DateTimeStruct->RTC_Dia, RTC_DateTimeStruct->RTC_Horas, RTC_DateTimeStruct->RTC_Minutos, RTC_DateTimeStruct->RTC_Segundos);
}


/*       Modulo Principal                            */
//========================================================================================
int main(void)
{
	int RTC_Contador = 0;    // Creamos la variable contador
  
	USART1_Config();    // Inicializamos el USART1
  valida();
  
  printf("\r\n =======================================================\r\n");
  printf("  PROGRAMA DEMO DE UTILIZACION DEL CALENDARIO DEL RTC  \r\n");
  printf(" =======================================================\r\n");
  
  // Comprobamos si los registros Bakcup no estan configurados
  if (BKP_ReadBackupRegister(BKP_DR1) != BAKUPR+1)
  {
     printf("\r\n  RTC NO CONFIGURADO....\r\n");    
     printf("\r\n  Reiniciando RTC, espere....\r\n");         
     printf("\r\n  Configurando RTC, espere....\r\n");   
     RTC_Estado = 0;	
     RTC_Init();
  
     // Establecemos el Calendario Inicial
     // 02-03-2018
     RTC_DateTimeStructure.RTC_Anno = 2023;
     RTC_DateTimeStructure.RTC_Mes = 10;
     RTC_DateTimeStructure.RTC_Dia = 27;
     // 20:03:10
     RTC_DateTimeStructure.RTC_Horas = 23;
     RTC_DateTimeStructure.RTC_Minutos = 47;
     RTC_DateTimeStructure.RTC_Segundos = 00;		
     fJuliano=Convierte_FechaHora(&RTC_DateTimeStructure);
     RTC_SetCounter(RTC_Contador);   
     printf("\r\n  --- RTC CONFIGURADO --- !!! \\n\r\n");  
     // Cargamos un valor en el registro 1 del Backup
     BKP_WriteBackupRegister(BKP_DR1, BAKUPR);    
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
      double tmp=fJuliano+(double)RTC_Contador/1000000;
      printf("\r\n\r\n  Contador segundos RTC : %d  calculo= %f  tmp=%f  base=%f\r\n", (int)RTC_Contador,(double)RTC_Contador/1000000,tmp,fJuliano);
		  // Lee RTC
      Lee_RTC(tmp , &RTC_DateTimeStructure);
      
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
  /* Habilitar reloj LSE */ 
  RCC_LSEConfig(RCC_LSE_ON);
  printf("    - Habilitando fuente LSE \r\n");
  /* Espere hasta que el LSE est� listo */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {;}
   
  printf("    - Deshabilitando LSI \r\n"); 
  //   
  /* Seleccionamos LSE como fuente de reloj del RTC */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  
  printf("    - Habilitando LSE para RTCCLK \r\n");
  /////////////////////////////////////////////////
  /*  Habilita el reloj RTC             */  
  RCC_RTCCLKCmd(ENABLE);
  printf("    - Habilitando RTCCLK\r\n");  
  
  /* Espera hasta que finalice la �ltima operaci�n de escritura en registros RTC */
  RTC_WaitForLastTask();
  
  /* Espera a que se sincronice los registros RTC  */
  printf("    - Esperando sincronizacion \r\n");
  RTC_WaitForSynchro();

  /* Espera hasta que finalice la �ltima operaci�n de escritura en registros RTC */
  RTC_WaitForLastTask();
    
  /* Habilitar la Interrupcio RTC por Segundos */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);
  printf("    - Habilitando Interrupcion general RTC \r\n");
    
  /* Habilitar la Interrupcion ALARMA del RTC   */
  RTC_ITConfig(RTC_IT_ALR, ENABLE);
  printf("    - Habilitando Interrupcion de Alarma RTC \r\n");
  /* Espera hasta que finalice la �ltima operaci�n de escritura en registros RTC */
  RTC_WaitForLastTask();
  /* Establecemos el Preescaler del RTC para un per�odo de 1 segundo */
  RTC_SetPrescaler(32767); /* Periodo RTC = RTCCLK/RTC_Prescal = (32.768KHz)/(32767+1) */
  printf("    - Configurado Prescaler LSE (32,768 kHz) \r\n");
   
  /* Espere hasta que finalice la �ltima operaci�n de escritura en registros RTC */
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
double fechaJuliana(int anio, int mes, int dia, int hora, int minuto, int segundo) 
{
    int a, y, m;
    
    a = (14 - mes) / 12;
    y = anio + 4800 - a;
    m = mes + 12 * a - 3;

    int juliana = dia + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    double fraccion = (double)(hora * 3600 + minuto * 60 + segundo)/ 247762;
    return (double)juliana+fraccion;
}
void julianaAGregoriana(double juliana, int *anio, int *mes, int *dia, int *hora, int *minuto, int *segundo) 
{
	int j,g,dg,c,dc,b,db,a,da,y,m,d,yrom,mrom,drom;
	j=juliana+32044;
	g=j/146097;
	dg=j%146097;
	c=(dg/36524+1)*3/4;
	dc=dg-c*36524;
	b=dc/1461;
	db=dc%1461;
	a=(db/365+1)*3/4;
	da=db-a*365;
	y=g*400+c*100+b*4+a;
	m=(da*5+308)/153-2;
	d=da-(m+4)*153/5+122;
	yrom=y-48;
	mrom=m+1;
	drom=d+1;
	*anio=y-4800+(m+2)/12;
	*mes=((m+2)%12)+1;
	*dia=d+1;
	float fraccion=(juliana-((int)juliana))*247762;
	*hora=(int)fraccion/3600;
	fraccion=fraccion-(*hora*3600);
	*minuto=(int)fraccion/60;	
	*segundo=fraccion-(*minuto*60);
//	printf("\r\nhora=%d minutos=%d segundos=%d",*hora,*minuto,*segundo);
}
void valida(void)
{
  printf("--------------------------------------------Validando----------------------------------\r\n");
        int anio=2023, mes=10, dia=27, hora=18, minuto=25, segundo=31;

        //printf("Ingrese la fecha gregoriana (anio, mes, dia, hora, minuto, segundo): ");
        //scanf("%d %d %d %d %d %d", &anio, &mes, &dia, &hora, &minuto, &segundo);

        double juliana = fechaJuliana(anio, mes, dia, hora, minuto, segundo);

        printf("La fecha juliana correspondiente es: %f\n", juliana);

        julianaAGregoriana(juliana, &anio, &mes, &dia, &hora, &minuto, &segundo);

        printf("La fecha gregoriana correspondiente es: %d-%02d-%02d %02d:%02d:%02d\n", anio, mes, dia, hora, minuto, segundo);

}

////////////////////////   Fin del Programa   //////////////////////////////////
