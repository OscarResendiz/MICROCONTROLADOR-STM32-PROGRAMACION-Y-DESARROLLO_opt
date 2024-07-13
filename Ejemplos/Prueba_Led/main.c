/***************************************************************************************************************
 * programa de ejemplo que hace parpadear un led
 * ************************************************************************************************************/
//libreria principal del microcontrolador
#include "stm32f10x.h"
//librerias para los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
//Definimos el puerto y el pin que contendra el led
#define LED_Pin GPIO_Pin_15
#define LED_GPIO GPIOA
#define RCC_APB2Peiph_GPIOx RCC_APB2Periph_GPIOA
//Declaracion de funciones que se utilizaran
void Delay_ms(unsigned int nCount);
GPIO_InitTypeDef GPIO_StructureInit;
//modulo principal
int main(void)
{
	//establecemos el reloj del puerto GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Peiph_GPIOx,ENABLE);
	//Asignamos el pin 13 para configuracion
	GPIO_StructureInit.GPIO_Pin=LED_Pin;
	//se establece la velocidad a 50 MHz
	GPIO_StructureInit.GPIO_Speed=GPIO_Speed_50MHz;
	//se establece el pin como salida
	GPIO_StructureInit.GPIO_Mode=GPIO_Mode_Out_PP;
	//se establece la configuracion
	GPIO_Init(LED_GPIO,&GPIO_StructureInit);
	//bucle que se ejecuta siempre
    while(1)
    {
    	// Apaga el led
    	GPIO_ResetBits(LED_GPIO,LED_Pin);
    	Delay_ms(500);
    	//Encende el led
    	GPIO_SetBits(LED_GPIO,LED_Pin);
    	Delay_ms(500);
    }
}
//implementacion de la funcion de retardo
void Delay_ms(unsigned int nCount)
{
	unsigned int i,j;
	for(i=0;i<nCount;i++)
	{
		for(j=0;j<0x2aff;j++)
		{
			; //no hace nada
		}
	}
}
