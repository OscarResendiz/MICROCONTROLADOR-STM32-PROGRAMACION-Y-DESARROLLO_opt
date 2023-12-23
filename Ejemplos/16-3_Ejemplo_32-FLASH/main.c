/**********************************************************************
  * Project Name: FLASH
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
  * PROGRAMA DE EJEMPLO DE USO DE LA MEMORIA FLASH
  * 
  * Programa que graba datos simples en direcciones de memoria
  * Flash de nuestro microcontrolador; que a pesar de desconectar
  * la alimentacion del microcontrolador se mantendrán en dicha
  * memoria.
  *
  * Utilizando la lectura de los bancos de memoria al iniciar
  * y aumentando el valor leido en un contador que se incrementara 
  * cada vez que se reinicie el microcontrolador.
  *
  **********************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"      
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h" 
#include "stm32f10x_exti.h"  
#include "stm32f10x_flash.h"   //(*)

#include <stdio.h>

// Definimos una variable que contendrá el inicio de nuestra pagina
#define DIRECC_MI_PAGINA 0x0800FC00

// Definimos una estructura para manejar los datos en bloques de 32 bits 
typedef	struct
{
  uint8_t Dato1;	// Dato1 +1 byte  (8 bits)
	uint8_t Dato2;  // Dato2 +1 byte  (8 bits)
	uint16_t Dato3;	// Dato3 +2 byte  (16 bits)
	uint32_t Dato4; // Dato4 +4 bytes (32 bits)
                  // = 8 bytes totales = 32 bits words_leng
} WordByte;

// Nombramos la estructura de nuestra variable de array
WordByte WordByteStructure;

// CREAMOS UNA VARIABLE QUE GUARDA EL TAMAÑO DE CADA CAMPO DE NUESTRA ESTRUCTURA
#define SIZE_DATO_WORDBYTE sizeof(WordByteStructure)/4

// Lista de funciones -------------------------------
void USART1_Config(void);
void GPIO_Config(void);
void NVIC_Configuration(void);
void EXT5_Config(void);
void EXTI9_5_IRQHandler(void);

/*   Funcion que inicializa el modulo FLASH       */
//----------------------------------------------------
void FLASH_Init(void){    
	/* Habilitar buffer de precarga */
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	/* Establecemos el Estado de latencia a 2 */
	FLASH_SetLatency( FLASH_Latency_2);
}


/*     Funcion que extrae lo dotas guardados en la memoria Flash   */
//-------------------------------------------------------------------
void Lee_Flash(void) {
	// Establece las variables de 
  // Direccion Origen utilizando el puntero de los datos en la Flash
	uint32_t *direcc_origen = (uint32_t *)DIRECC_MI_PAGINA;
  
  // y de Direccion Destino de nuestros datos para el array
	uint32_t *direcc_destino = (void *)&WordByteStructure;
  
  // Crea un bucle que va extrallendo cada dato de las
  // direcciones origenes del puntero de nuestro array
	for (uint16_t i=0; i<SIZE_DATO_WORDBYTE; i++) {
    // pasa cada dato a su posicion de direccion del array
		*direcc_destino = *(__IO uint32_t*)direcc_origen;
    
    // incrementamos los valores de las direcciones
		direcc_origen++; direcc_destino++;
	}  
}

/*   Funcion que graba los datos del array en direcciones de memoria Flash  */
//----------------------------------------------------------------------------
void Graba_Flash(void) {
	// Desbloquea memoria flash para poder manipularla
  FLASH_Unlock();
  
  // Borrar toda la pagina de nuestros datos
	FLASH_ErasePage(DIRECC_MI_PAGINA);

	// Establece las variables de 
  // Direccion Origen utilizando el puntero de los datos en la Flash
	uint32_t *direcc_origen = (void*) (&WordByteStructure);    
  
  // y de Direccion Destino la Primera pagina de nuestro ejemplo
  uint32_t *direcc_destino = (uint32_t *) DIRECC_MI_PAGINA;
  
  // Crea un bucle que va grabando en la direccion de la primera pagina
  // de nuestro proyecto los datos extraidos del puntero de nuestro array  
	for (uint16_t i=0; i<SIZE_DATO_WORDBYTE; i++) {
    
	  // Graba en la memoria Flash en direccion origen los datos del array
    FLASH_ProgramWord((uint32_t)direcc_destino, *direcc_origen);
    
		// incrementamos los valores de las direcciones
    direcc_origen++; direcc_destino++;
	}

	// Bloquea de nuevo la memoria FLASH
  FLASH_Lock();
}

/*              Modulo Principal                       */
//*******************************************************
int main(void)
{    
    USART1_Config();    // Inicializamos el USART1
    GPIO_Config();      // Inicializamos el GPIO para el LED
    NVIC_Configuration(); // Inicializamos el NVIC para IRQ del RTC
    EXT5_Config();      // Inicializa la interrupcion del Boton
  
    printf("\r\n =====================================================\r\n");
    printf("  PROGRAMA EJEMPLO DE UTILIZACION DE LA MEMORIA FLASH  \r\n");
    printf(" =====================================================\r\n");
  
    FLASH_Init();       // Inicializamos el modulo FLASH
  
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
          
    // Reseamos las banderas RCC para reutilizarlas
    RCC_ClearFlag(); 
    
	// Lee si existen datos almacenados y los pasa al array
    Lee_Flash();   // Lee los datos almacenados

    // Incrementa los valores los valores '1' del array
    WordByteStructure.Dato1 = WordByteStructure.Dato1+1;
    WordByteStructure.Dato2 = WordByteStructure.Dato2+1;
    WordByteStructure.Dato3 = WordByteStructure.Dato3+1;
    WordByteStructure.Dato4 = WordByteStructure.Dato4+1;

	// Graba de nuevo los datos en la memoria FLASH
	Graba_Flash();  
    
    // Enviamos la informacion por el puerto USART1
    printf("  Datos leidos [1] %d [2] %d [3] %d [4] %d \n\r", 
        WordByteStructure.Dato1,
        WordByteStructure.Dato2,
        WordByteStructure.Dato3,
        WordByteStructure.Dato4);

    while (1)
    {
      // Enciende y apaga (Toggle) LED en PC13
		  GPIOC->ODR ^= GPIO_Pin_13;
      // delay 
    	for(int i=0;i<0x100000;i++);     
    }
}

//       Funcion que configura el USART1                    */
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

/*     Funcion que configura el NVIC para EXT_5_IRQ    */
//-------------------------------------------------------
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Condiguracion NVIC para EXT5 canal 5 IRQ en PA5   */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}

/*       Funcion que configura el GPIO para LED y PULSADOR  */
//------------------------------------------------------------
void GPIO_Config(void)
{
		/* Creamos la estructura para el GPIO del LED */
		GPIO_InitTypeDef  GPIO_InitStructure;
	
		// Activamos el reloj para el GPIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |
							             RCC_APB2Periph_GPIOA, ENABLE);
	
		/* Configuramos los parametros para el pin PC13 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Apagamos el PC13
  
  	/* Configuracion para el pin PA5 entrada pulsador */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*  Funcion que configura la Interrupcion IRQ_5  en PA5   */
//----------------------------------------------------------
void EXT5_Config(void){

	// Activamos el reloj para GPIO en puerto A
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	
	// Creamos la estructura para EXTI
	EXTI_InitTypeDef EXTI_InitStructure; 	

	//Configura la interrupcion EXTI5 en linea 5 de PA5
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/*  Funcion que controla si se pulsa el boton en PA5  */
//-----------------------------------------------------
void EXTI9_5_IRQHandler(void) 
{
	// Comprueba si se ha producido la interrupcion
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)   
	{
		// Desactiva la interrupcion para que se vuelva a comprobar
		EXTI_ClearITPendingBit(EXTI_Line5);  
    
   // Desbloquea memoria flash para poder manipularla
    FLASH_Unlock();
  
    // Procedemos a borrar toda la pagina de nuestros datos
	  FLASH_ErasePage(DIRECC_MI_PAGINA);  
    
    // Mostramos el mensaje de Borrado
    printf("\n\r  MEMORIA FLASH BORRADA !!!! \r\n");
    
    // delay 
    for(int i=0;i<0x2000000;i++);
    
    // Realizamos un reset del sistema (software reset)
    SCB->AIRCR = 0x05fa0004;
 	}
}

//////////////////////////  Fin del Programa   //////////////////////////////

