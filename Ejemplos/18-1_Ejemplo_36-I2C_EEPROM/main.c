/**********************************************************************
  * Project Name: I2c_EEPROM
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
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL PUERTO I2C
  * 
  * Programa ejemplo de utilizacion del puerto I2C que
  * lee y escribe un byte en una memoria EEPROM y muestra
  * el valor leido por el puerto serie USART1
  *
  **********************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_gpio.h" 
#include "stm32f10x_rcc.h"  
#include "stm32f10x_i2c.h"  
#include "stm32f10x_usart.h"

#include "i2c_eeprom.h"

#include <stdio.h>

#define I2C_ADDR_EEPROM  0x50

void USART1_Config(void);
void Delay(int nTime);

int main(void)
{
	uint8_t data=0xA6, temp;
  
	USART1_Config();
	printf ("  Hola oscar\r\n");    
	
	I2C1_EE_Init();
	
	printf ("  leyendo la memoria\r\n");    
	temp = I2C_eeprom_Read(I2C1, I2C_ADDR_EEPROM, 0xA0);
	printf ("  Valor leido :0x%.2X Dec:%d\r\n", temp, temp);    
 temp++;
	data=temp;
	printf ("  Escribiendo en la memoria\r\n");      
	I2C_eeprom_Write(I2C1, I2C_ADDR_EEPROM, data, 0xA0);

	printf ("  leyendo la memoria de nuevo\r\n");    
	temp = I2C_eeprom_Read(I2C1, I2C_ADDR_EEPROM, 0xA0);
	printf ("  Valor leido :0x%.2X Dec:%d\r\n", temp, temp);    
 
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


/*   Funcion que configura un retraso        */
void Delay(int nTime)
{
     unsigned int i;
     unsigned long j;
     for(i = nTime;i > 0;i--)
          for(j = 1000;j > 0;j--);     
}

