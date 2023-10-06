  /**********************************************************************
  * Project Name: ADC_potenciometer
  * File Name: usart1.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2017.09.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * ===================================================================
  *  
  * PROGRAMA EJEMPLO DE USO DEL ADC
  *
  * En el presente ejemplo se enciende el LED en PB1 cuando
  * la lectura analogica del voltaje de salida de un potenciometro
  * conectado al pin PA6 excede de los 1.6 voltios.
  * Mostrandose dicho valor por el puerto USART en nuestro ordenador
  *
  * Conexiones:
  *    LED 		----  PB1
  *         [  ---  GND
  *   [ Pot [  ---  PA6
  *         [  ---  3.3 Vcc
  *
  **********************************************************************/
 
#include "usart1.h"
#include <stdarg.h>

/* Funcion que confifura el USART1   */
void USART1_Config(void)
{
	/* Creamos la estructura para el GPIO    */
	GPIO_InitTypeDef GPIO_StructureInit;
	USART_InitTypeDef USART_StructureInit;
	
	/* Activamos el reloj para USART1 y GPIOA   */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configuramos el GPIO para el USART1 */
	/* Configuramos Pin PA9 para Tx del USART1 en modo push-pull */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_9;
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_StructureInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_StructureInit);    
	
	/* Configuramos el Pin PA10 para Rx del USART1 en modo floating */
	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_10;
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_StructureInit);
	  
	/* Configuramos el USART1 */
	/* Velocidad Baudios: 9600
	   Longitud de palabra: 8 bits
		 Bit de Stop: 1
		 Paridad: Ninguna
		 Control de flujo: Ninguna  */
	USART_StructureInit.USART_BaudRate = 9600;
	USART_StructureInit.USART_WordLength = USART_WordLength_8b;
	USART_StructureInit.USART_StopBits = USART_StopBits_1;
	USART_StructureInit.USART_Parity = USART_Parity_No ;
	USART_StructureInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_StructureInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_StructureInit); 
	USART_Cmd(USART1, ENABLE);
}

//=== Incluimos ========
void SetSysClockTo72(void)
{
	ErrorStatus HSEStartUpStatus;
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
    	//FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);

        /* Flash 2 wait state */
        //FLASH_SetLatency( FLASH_Latency_2);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);

        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config( RCC_HCLK_Div2);

        /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLConfig(0x00010000, RCC_PLLMul_9);

        /* Enable PLL */
        RCC_PLLCmd( ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
    else
    {       
        while (1)
        {
        }
    }
}
//====================

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
 * Formatos posibles:
 *         USART1_printf( USART1, "\r\n texto demo \r\n" );  imprime un texto
 *         USART1_printf( USART1, "\r\n %d \r\n", i ); imprime valor decimal
 *         USART1_printf( USART1, "\r\n %s \r\n", j ); imprime valor 
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
