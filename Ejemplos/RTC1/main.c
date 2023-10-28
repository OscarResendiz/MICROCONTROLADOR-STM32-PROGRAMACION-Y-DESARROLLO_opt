/* =====================================================================
 *
 * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL RTC
 *
 *  Programa que configura los registros del RTC en una hora
 *  determinada; mostrando cada 20 segundos el tiempo del RTC.
 *
 *  Cuando se reinicia el sistema, detecta el tipo de reinicio
 *  y si se puede continuar con el tiempo guardado, si se ha
 *  mantenido la alimentacion.
 *
 **********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "usart1.h"

#include <stdio.h>    // Funcion que permite usar el 'printf'
//  Lista de funciones ------------------------

void RTC_Init(void);
void NVIC_Configuration(void);
void Inicializa_Hora(uint8_t Horas, uint8_t Minutos, uint8_t Segundos);
void RTC_LeeHora(void);
void Muestra_Hora(uint32_t TimeValor);
/*       Permite configurar el reloj LCE o LSI       */
//-----------------------------------------------------
#define RTCClockSource_LSE  // Externo

uint32_t RTC_CountSecond = 0; // Variable donde se guarda seg. transcurridos

static int RTC_Estado = 0; // Creamos la variable estado del RTC
char cadena[100];
/*       Modulo Principal                           */
//====================================================
int main(void)
{
	USART1_Config(); // Inicializamos el USART1

	NVIC_Configuration(); // Inicializamos el NVIC para IRQ del RTC

	sprintf(cadena, "\r\n==============================================\r\n");
	print(cadena);
	sprintf(cadena, "      PROGRAMA DEMO DE UTILIZACION DEL RTC  \r\n");
	print(cadena);
	sprintf(cadena, "==============================================\r\n");
	print(cadena);

	// Comprobamos si los registros Bakcup no estan configurados
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) {
		sprintf(cadena, "\r\n  RTC NO CONFIGURADO....\r\n");
		print(cadena);

		sprintf(cadena, "\r\n  Reiniciando RTC, espere....\r\n");
		print(cadena);
		RTC_Init(); // Inicializamos el RTC

		sprintf(cadena, "\r\n  Configurando RTC, espere....\r\n");
		print(cadena);
		// Inicializamos Horas, Minutos y Segundos
		Inicializa_Hora(16, 45, 00);

		sprintf(cadena, "\r\n  --- RTC CONFIGURADO --- !!! \r\n");
		print(cadena);

		// Cargamos un valor en el registro 1 del Backup
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	} else {
		/* Comprobamos si el reinicio es POR o PDR */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) {
			sprintf(cadena, "\r\n  Un Reinicio a ocurrido....\r\n\r\n");
			print(cadena);
		}
		/* Comprobamos si el reinicio es por pulsar el pin NRST */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) {
			sprintf(cadena,
					"\r\n  Un Reset externo se a detectado....\r\n\r\n");
			print(cadena);
		}

		sprintf(cadena, "  NO es necesario configurar de nuevo el RTC....\r\n");
		print(cadena);
		sprintf(cadena, "  ... Iniciando el RTC, espere ...\r\n");
		print(cadena);

		// Asignamos '1' para indicar un reinicio sin reseteo del RTC
		RTC_Estado = 1;
		RTC_Init(); // Inicializamos el RTC

		/* Clear reset flags */
		RCC_ClearFlag();
	}

	while (1) {
		// Muestra la hora cada 20 segundos
		if (RTC_GetCounter() - RTC_CountSecond > 20U) {
			sprintf(cadena, "\r\n  Total segundos: %d", (int) RTC_CountSecond);
			print(cadena);
			RTC_LeeHora();

			// Restablece el valor del contador
			RTC_CountSecond = RTC_GetCounter();
		}
	}

}
/*   Funcion que configura el RTC              */
//-------------------------------------------------
void RTC_Init(void) {
	/* Iniciamos el reloj APB1 para PWR y BKP    */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Permitir acceso a los registros BKP */
	PWR_BackupAccessCmd(ENABLE);

	/* Comprobamos si es un reinicio de un nuevo RTC  */
	if (RTC_Estado != 1) {
		sprintf(cadena, "    - Reiniciando RTC  \r\n");
		print(cadena);
		/* Reseteo de los registros Backup  */
		BKP_DeInit();

		// Reseteando registros backup del RTC
		RCC_BackupResetCmd(ENABLE);
		RCC_BackupResetCmd(DISABLE);
		sprintf(cadena, "    - Reseteando backup de registros RTC\r\n");
		print(cadena);
	}

	// ----  Si se habilita el reloj LSE     --------
	/////////////////////////////////////////////////
	/* Habilitar reloj LSE */
	sprintf(cadena, "    - Habilitando fuente LSE \r\n");
	print(cadena);
	RCC_LSEConfig(RCC_LSE_ON);
	//
	/* Espere hasta que el LSE este listo */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
		;
	}
	print("fuente LSE Habilitada :)\r\n");
	/* Seleccionamos LSE como fuente de reloj del RTC */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	sprintf(cadena, "    - Habilitando LSE para RCC_RCT \r\n");
	print(cadena);
	/////////////////////////////////////////////////

	/*  Habilita el reloj RTC             */
	RCC_RTCCLKCmd(ENABLE);
	sprintf(cadena, "    - Habilitando RTCCLK\r\n");
	print(cadena);

	/* Espera hasta que finalice la ultima operacion de escritura en registros RTC */
	//RTC_WaitForLastTask();
	/* Espera a que se sincronice los registros RTC  */
	sprintf(cadena, "    - Esperando sincronizacion \r\n");
	print(cadena);
	RTC_WaitForSynchro();

	/* Espera hasta que finalice la ultima operacion de escritura en registros RTC */
	RTC_WaitForLastTask();

	/* Habilitar la Interrupcio RTC por Segundos */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	sprintf(cadena, "    - Habilitando Interrupcion general RTC \r\n");
	print(cadena);

	////////////////////////////////////////////////////////////////////
	// Establecer el periodo del contador RTC en 1 segundo
	// Para LSE = RTCCLK/RTC period = 32768Hz/1Hz = 32767+1
	// para LSI = RTCCLK/RTC period = 40000Hz/1Hz = 39999+1
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	/* Establecemos el Preescaler del RTC para un periodo de 1 segundo */
	RTC_SetPrescaler(32767); /* Periodo RTC = RTCCLK/RTC_Prescal = (32.768KHz)/(32767+1) */
	sprintf(cadena, "    - Configurado Prescaler LSE (32,768 kHz) \r\n");
	print(cadena);
	////////////////////////////////////////////////////////////////////

	/* Espere hasta que finalice la ultima operacion de escritura en registros RTC */
	RTC_WaitForLastTask();
}
/*       Funcion que Lee la Hora en el RTC   */
//---------------------------------------------
void RTC_LeeHora(void) {
	uint32_t TimeValor;

	TimeValor = RTC_GetCounter(); // Leemos contador del RTC

	// Muestra la hora obtenida
	Muestra_Hora(TimeValor);
}
/*  Funcion que extrae la hora, minutos y segundos del valor obtenido */
//----------------------------------------------------------------------
void Muestra_Hora(uint32_t TimeValor) {
	uint8_t THor_High, THor_Low, TMin_High, TMin_Low, TSeg_High, TSeg_Low;
	uint8_t THor, TMin, TSeg;

	// Extraemos los valores
	THor_High = (uint8_t)(TimeValor / 3600) / 10;
	THor_Low = (uint8_t)(TimeValor / 3600) % 10;
	TMin_High = (uint8_t)((TimeValor % 3600) / 60) / 10;
	TMin_Low = (uint8_t)((TimeValor % 3600) / 60) % 10;
	TSeg_High = (uint8_t)((TimeValor % 3600) % 60) / 10;
	TSeg_Low = (uint8_t)((TimeValor % 3600) % 60) % 10;

	// Pasamos los valores de NibbleHIGH y NibbleLOW a su valor Decimal
	THor = (THor_High * 10) + THor_Low;
	TMin = (TMin_High * 10) + TMin_Low;
	TSeg = (TSeg_High * 10) + TSeg_Low;

	// Imprimimos los valores obtenidos por USART1
	sprintf(cadena, "\r\n  HORA RTC:  %d:%d:%d\r\n", THor, TMin, TSeg);
	print(cadena);
}
/*     Funcion que carga la hora nueva en el RTC    */
//----------------------------------------------------------------
void Inicializa_Hora(uint8_t Horas, uint8_t Minutos, uint8_t Segundos) {
	uint32_t CounterValue;
	// Deshabilitar la Interrupcio RTC por Segundos
	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	// Habilitamos el acceso a los registros BacKup
	PWR_BackupAccessCmd(ENABLE);
	//RTC_WaitForLastTask();

	// Convertimos segundos totales el horario nuevo
	CounterValue = ((Horas * 3600) + (Minutos * 60) + Segundos);

	// Grabamos el horario nuevo en los registros RTC
	RTC_SetCounter(CounterValue);

	// Esperamos a que se termine el acceso a los registros
	RTC_WaitForLastTask();

	// Rehabilitar la Interrupcio RTC por Segundos */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
}
/*    Funcion que configura el NVIC       */
//--------------------------------------------
void NVIC_Configuration(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// Configuramos y habilitamos la interrupcion del RTC
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*     Funcion que se genera cada vez que se produce la IRQ del RTC  */
//---------------------------------------------------------------------
void RTC_IRQHandler(void) {
	// Comprobamos el estado de la Interrupcio RTC por Segundos
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {
		// Receteamos la bandera de la Interrupcio RTC por Segundos
		RTC_ClearITPendingBit(RTC_IT_SEC);

		// Esperamos a que se termine el acceso a los registros
		RTC_WaitForLastTask();

		//  Reseteamos el contador del RTC cuando se detecte la hora 23:59:59
		if (RTC_GetCounter() == 0x00015180) {
			// Reinicia el contador a '0'
			RTC_SetCounter(0x0);
			// Esperamos a que se termine el acceso a los registros
			RTC_WaitForLastTask();
		}

	}

}

/////////////////////  Fin  del  Programa    ///////////////

