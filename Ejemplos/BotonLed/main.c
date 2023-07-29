#include "stm32f10x.h" 		//libreria principal del micro
#include "stm32f10x_rcc.h"	//libreria de control de RCC
#include "stm32f10x_gpio.h"	//libreria para control de GPIO


int main(void)
{
	GPIO_InitTypeDef GPIO_StructureInit; //estructupa para configuracion del GPIO
	//Activamos el relog para GPIOA y GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//configuracmos el Pin13 para el led
	GPIO_StructureInit.GPIO_Pin=GPIO_Pin_13;
	GPIO_StructureInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_StructureInit.GPIO_Mode=GPIO_Mode_Out_PP; //PP significa push pull
	GPIO_Init(GPIOC,&GPIO_StructureInit);
	//configuramos el PB0 para el boton
	GPIO_StructureInit.GPIO_Pin=GPIO_Pin_0;
	GPIO_StructureInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_StructureInit.GPIO_Mode=GPIO_Mode_IPU; //I= input PU= pull up conectando una resistencia interna a vcc
	GPIO_Init(GPIOB,&GPIO_StructureInit);
	//bucle infinito
    while(1)
    {
    	//verifica i el pin0 esta en nibel bajo
    	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
    	{
    		//enciende el led
    		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
    	}
    	else
    	{
    		//apaga el led
    		GPIO_SetBits(GPIOC,GPIO_Pin_13);
    	}
    }
}
