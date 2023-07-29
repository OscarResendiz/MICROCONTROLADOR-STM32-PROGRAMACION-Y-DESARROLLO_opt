#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "misc.h"

/*llama al modulo externo de configuracion del puerto USART*/
#include "usart1.h"
#include <stdio.h>
void Delay_ms(unsigned int nCount);

/*modulo principal*/
int main(void)
{
	USART1_Config();
	Config_Int();
	for(;;)
	{
		//print("Esto es un texto de prueba por printf\r\n");
		Delay_ms(200);

	}
}
/*funcion que genera un retardo en milisegundos*/
void Delay_ms(unsigned int nCount)
{
	unsigned int i,j;
	for(i=0;i<nCount;i++)
	{
		for(j=0;j<0x2AFF;j++)
		{
			/*no hace nada sol perder unos siclos de relog*/
			;
		}
	}
}
void Config_Int(void)
{
	//creamos la estructura para NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	//iniciamos el grupo4 para la prioridad del IRQ USART
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	//establecemos el System Timer IRQ como alta prioridad
	NVIC_SetPriority(SysTick_IRQn,0);
	//Establecemos el IRQ para el USART1
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//funcion de interrupcion del puerto USART1
void USART1_IRQHandler(void)
{
	u8 caracter;
	//comprobamos si existe dato recibido comprobando el estado de la bandera USART_IT_RXNE
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		caracter=USART1->DR;
		USART_SendData(USART1,caracter);
	}
}
