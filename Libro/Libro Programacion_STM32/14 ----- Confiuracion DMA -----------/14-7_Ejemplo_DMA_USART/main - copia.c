 /**********************************************************************
  * Project Name: DMA_USART
  * File Name: main.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *
  * PROGRAMA DE PRUEBA DE UTILIZACION DEL DMA CON EL USART1
  *
  * En el programa se recogen en el DMA hasta 40 caracteres recibidos por 
  * el puerto USART1 y se retransmiten de nuevo.
  *
  *******************************************************************/
  
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"   
#include "stm32f10x_rcc.h"    
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"  

// Creamos la variable estructura 'TestSatus"
// en la que configuramos los valores 'FAILED' y 'PASSED'
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define TxBufferSize1 (cuentaFinal(TxBuffer1) - 1)   // Variable que guarda el tamano del Buffer1
#define TxBufferSize2 (cuentaFinal(TxBuffer2) - 1)   // Variable que guarda el tamano del Buffer2
#define cuentaFinal(a) (sizeof(a) / sizeof(*(a)))    // variable que guarda el valor de comparacion de tamano Buffer

uint8_t TxBuffer1[] = "Prueba de DMA en USART1 ---------------";
uint8_t RxBuffer1[TxBufferSize1];

//  Lista de funciones
void USART1_Config(void);
void DMA_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void uartStartTxDMA(DMA_Channel_TypeDef *txDMAChannel);
	

/*         Modulo Principal                  */
//================================================
int main()
{
  USART1_Config();      // Inicializamos el USART1
  
  DMA_Configuration();	// Inicializamos el DMA    	
	
	while(1)
	{
		int i=0;

		// Esperamos hasta que  la transferencia se complete en canal 4 DMA
    while (DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET)
		{
		}
		
    // Esperamos hasta que la recepción se complete en canal 5 DMA
		while (DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET)
		{			
		}
		
    // Se va añadiendo los datos recibidos a la variable de datos 
    // para transmitirlos de nuevo
		for (i=0; i < TxBufferSize1; ++i)
		{
			TxBuffer1[ i] = RxBuffer1[ i];
		}
		
    // Reiniciamos la bandera de datos pendiente en los dos canales
    DMA_ClearITPendingBit(DMA1_IT_TC4 | DMA1_IT_TC5);
    
    // Desabilitamos el canal 4 del DMA1
    DMA_Cmd(DMA1_Channel4, DISABLE);
		
    // Llamada a la funcion que transmite los datos recibidos
		uartStartTxDMA(DMA1_Channel4);
	}
	
}

/*           Funcion que configura el USART1             */
//---------------------------------------------------------
void USART1_Config(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Habilitamos los relojes para GPIO, USART y AFIO ----*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_USART1|
                         RCC_APB2Periph_AFIO, ENABLE);
		
  /* Configuramos el pin 10 para Rx USART1 --------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  /* Configuramos el pin 9 para Tx USART1  --------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configuracion del USART1 ---------------------------*/
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		
  USART_Init(USART1, &USART_InitStructure);
	
  /* Habilitamos las baderas Tx y Rx USART para el DMA ---------*/
  USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);

  /* Habilitamos el USART1 -------------------------------------*/
  USART_Cmd(USART1, ENABLE);
}

/*    Funcion que transmite los datos recibidos por USART1 */
//------------------------------------------------------------
void uartStartTxDMA(DMA_Channel_TypeDef *txDMAChannel)
{
    // Escribimos los datos en los registros CMAR y CNDTR del modulo DMA
    txDMAChannel->CMAR = (uint32_t)TxBuffer1; // Registro Memory address de DMA
    txDMAChannel->CNDTR = TxBufferSize1;      // Registro Numero de datos a transferir por DMA
    DMA_Cmd(txDMAChannel, ENABLE);
}

/*     Funcion que configura el DMA y canales 4 y 5        */
//------------------------------------------------------------
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  /* Habilitamos reloj para DMA1 --------------------------------*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  /* Configuracion del canal 4 del DMA1 para Tx USART1 ----------*/
  DMA_DeInit(DMA1_Channel4);	
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  
  /* Habilitamos el canal 4 del DMA1 para Tx ---------------------*/
  DMA_Cmd(DMA1_Channel4, ENABLE);
  
  /* Configuracion del canal 5 del DMA1 para Rx USART1 -----------*/
	DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);
  
  /* Habilitamos el canal 5 del DMA1 para Rx  --------------------*/
  DMA_Cmd(DMA1_Channel5, ENABLE);
}

/*   Funcion quee realiza la comparación de los buffers1 y 2                  */
//-------------------------------------------------------------------------------
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  // Espera a que el tamano del 'BufferLength' llegue a su limite
  while(BufferLength--)
  {
    
    // Compara el tamano de cada 'pBufferN' sea distinto
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    // Incrementamos cada 'pBufferN'
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}


///////////////////////  Fin de Programa  /////////////////////////////////////////
