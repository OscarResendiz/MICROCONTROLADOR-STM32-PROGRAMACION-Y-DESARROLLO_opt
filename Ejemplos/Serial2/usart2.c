#include "stm32f10x.h"
#include "usart2.h"
#include <stdarg.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

//funcion que configura el USART2
void USART2_Config()
{
	//creamos la estructura para el GPIO y el USART
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//activamos el relog para el GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//configuramos el pin2 PA2 para TX del UART2
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//configuramos el PA3 para RX del USART2
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//Configuramos el USART2
	//activaos el reloj para el USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	/*
	 * velocidad 15200
	 * palabra de 8 bits
	 * bits de parada 1
	 * paridad ninguna
	 * control de flijo ninguno
	 */
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	USART_Cmd(USART2,ENABLE);
}
//funcion que envia un caracter por USART2
void USART2_PutChar(char c)
{
	//esperamos hasta que el registro de datos trasmitidos este lleno
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
	//enviamos el caracter por el puerto
	USART_SendData(USART2,c);
}
//funcion que lle un caracter recibido en el puerto USART2
uint16_t USART2_GetChar()
{
	//esperamos hasta que se reciba un caracter en el pruerto
	while(!USART_GetFlagStatus(USART2,USART_FLAG_RXNE));
	//regreso el caracter cecibido
	return USART_ReceiveData(USART2);
}
//funcion que envia una cadena de caracteres por USART2
void USART2_PutString(char *s)
{
	// enviamos la cadena por el puerto usar2
	while(*s)
	{
		//envia el caracter por el puerto e incrementa el apuntador para obtener el siguiente caracter
		USART2_PutChar(*s++);
	}
}
