#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
/*llama al modulo externo de configuracion del puerto USART*/
#include "usart1.h"
#include <stdio.h>
void Delay_ms(unsigned int nCount);

/*modulo principal*/
int main(void)
{
	//USART1_Config();
	//hacemos el redireccionamiento de pines
	USARTremap_Config();
	for(;;)
	{
		print("Esto es un texto de prueba por printf\r\n");
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
