 /**********************************************************************
  * Project Name: USART_remap
  * File Name: usartM.c
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
  * PROGRAMA DE PRUEBA DE REMAPEO PUERTO USART1
  * 
  * Transmite un mensaje de prueba, por el puerto USART1 del
  * microcontrolador pero re-mapeado de los pines por defecto
  * a traves del la configuracion siguiente.
  *
  *   USART1 Tx (PB6)
  *          Rx (PB7)
  *
  ***************************************************************/
  
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Libreria de perifericos
#include "stm32f10x_rcc.h"		
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"   

// Llamada al fichero con la especificacion de la funcion 'USARTremap'
#include "usartM.h"
#include <stdarg.h>		// Libreria para usar 'printf'

/* Funcion que configura los parametros del USART  */
void USARTremap_Config(void)
{
		/* Creamos la estructura para el GPIO    */
	GPIO_InitTypeDef GPIO_StructureInit;
	USART_InitTypeDef USART_StructureInit;
	
	/* Activamos el reloj para AFIO, USART1 y GPIOB   */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,   ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Iniciamos el remapeo de pines del USART1 para su
	   uso con PB6 y PB7                               */
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);	
	
	/* Configuramos el GPIO para el USART1 */
	/* Configuramos Pin PB6 para Tx del USART1 en modo push-pull */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_6;
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_StructureInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_StructureInit);    
	
	/* Configuramos el Pin PB7 para Rx del USART1 en modo floating */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_7;
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_StructureInit);
	  
	/* Configuramos el USART1 */
	/* Velocidad Baudios: 115200
	   Longitud de palabra: 8 bits
	   Bit de Stop: 1
	   Paridad: Ninguna
	   Control de flujo: Ninguna  */
	USART_StructureInit.USART_BaudRate = 115200;
	USART_StructureInit.USART_WordLength = USART_WordLength_8b;
	USART_StructureInit.USART_StopBits = USART_StopBits_1;
	USART_StructureInit.USART_Parity = USART_Parity_No ;
	USART_StructureInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_StructureInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_StructureInit); 
	
	/* Activamos el USART1   */
	USART_Cmd(USART1, ENABLE);
}

/* Funcion que configura el empleo de 'fputc' */
// Para enviar un byte por el pin de salida del USART1
int fputc(int ch, FILE *f)
{
	/* Envia dato */
	USART_SendData(USART1, (unsigned char) ch);
	
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

/* Funcion que permite utilizar el comando 'itoa ' */
// Para funcionamiento del USART1_printf() 
static char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	
	/* Esta parte del codigo es solo para numeros decimales */
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}
	
	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}
	
	/* Solo para valores negativos menos el simbolo menos */
	if (value < 0)
	{
	    *ptr++ = '-';
	
	    /* Marca el valor positivo */
	    value *= -1;
	}
	
	for (i = 10000; i > 0; i /= 10)
	{
	    d = value / i;
	
	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}
	
	/* Incluye el valor 'Null' al final del texto   */
	*ptr = 0;	
	return string;

} 

 /* Funcion USART1_printf
 *  Formatos posibles:
 *    USART1_printf( USART1, "\r\n texto demo \r\n" );  imprime un texto
 *    USART1_printf( USART1, "\r\n %d \r\n", i ); imprime valor decimal
 *    USART1_printf( USART1, "\r\n %s \r\n", j ); imprime cadena de caracteres
 */
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0)      
	{				                          
		if ( *Data == 0x5c )  
	{									  
	switch ( *++Data )
	{
		case 'r':							       // \r  envia CR
			USART_SendData(USARTx, 0x0d);
			Data ++;
		break;
		
		case 'n':							       // \n  Nueva linea
			USART_SendData(USARTx, 0x0a);	
			Data ++;
		break;
		
		default:
			Data ++;
		break;
	}			 
	}
	else if ( *Data == '%')
	{									  
	switch ( *++Data )
	{				
		case 's':							// %s Cadena de cacteres			 
			s = va_arg(ap, const char *);
	for ( ; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
		Data++;
		break;
	
	case 'd':								// %d Valor entero decimal		
	d = va_arg(ap, int);
	itoa(d, buf, 10);
	for (s = buf; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
	Data++;
	break;
		 default:
				Data++;
		    break;
	}		 
	} 
	else USART_SendData(USARTx, *Data++);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}

