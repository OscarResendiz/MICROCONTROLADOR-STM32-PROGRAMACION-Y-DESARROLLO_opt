 /**********************************************************************
  * Project Name: ADC_DMA
  * File Name: usart1.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *  
  * PRUEBA DE LECTURA MULTIPLE DE CANALES ADC.
  *
  *  En el programa se muestra por el puerto USART en nuestro ordenador
  *  los valores obtenidos en diferentes canales ADC, obteniendo los
  *  valores de su lectura en el DMA del modulo ADC.
  *
  *  Conexiones:
  *                 ADC1_CH4 en PA4
  *                 ADC1_CH5 en PA5
  *                 ADC1_CH6 en PA6  
  *                 ADC1_CH7 en PA7
  *
  **************************************************************/
#include "usart1.h"

#include <stdarg.h>

 /* USART1_Config     */
void USART1_Config(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* Configuramos el clock para USART1 ------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configuramos Pin PA9 para Tx del USART1 ------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
	/* Configuramos el Pin PA10 para Rx del USART1 --------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* Configuramos los parametros de comunicacion USART1 --------*/
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);
}

// Envia un byte por el pin de salida del USART1
int fputc(int ch, FILE *f)
{	
	USART_SendData(USART1, (unsigned char) ch);

	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

/* Funcion itoa  */
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
	*ptr = 0;
	
	return string;
} 

/* Funcion USART1_printf              */
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
		case 'r':							
			USART_SendData(USARTx, 0x0d);
			Data ++;
		break;
		
		case 'n':							
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
		case 's':					
			s = va_arg(ap, const char *);
	for ( ; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
		Data++;
		break;
	
	case 'd':						
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


/******************* Fin del modulo  *************************/

