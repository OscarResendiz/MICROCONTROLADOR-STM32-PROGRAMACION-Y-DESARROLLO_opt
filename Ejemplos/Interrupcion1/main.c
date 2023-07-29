#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"
//llamad la libreria que contiene el retardo
//#include "delay.h"
//listado de funciones
void Configurar_LED(void);
void Configurar_Boton(void);
void Config_Int(void);
void espera(unsigned int nCount);
unsigned char bandera=0;
//modulo principal
int main(void)
{
	//DelayInit(); //inicializa la funcion delay
	Configurar_LED(); //configra el pin del led
	Configurar_Boton(); //configura el pin del boton
	Config_Int(); //configura XTI Line0 (Pin PA0) en modo de interrupcion
    while(1)
    {
    	// blik led en PC13 para control del programa
    	GPIOC->BRR=GPIO_Pin_13;
    	espera(1000);
    	GPIOC->BSRR=GPIO_Pin_13;
    	espera(1000);
    }
}
//funcion que configura el GPIO para el LED
void Configurar_LED(void)
{
	//creamos la estructura del GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	//habilitamos el reloj para el GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	//Configuramos PB1 para salida del led
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//configuramos el PC13 para salida del led2
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
//funcion que configura el GPIO para el boton
void Configurar_Boton(void)
{
	//creamos la estructura del GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	//habilitamos el reloj para el GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//Configuramos PA0 como entrada
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

}
//funcion que configura la interrupcion por PA0
void Config_Int(void)
{
	//creamos la estructura para NVIC y EXTI
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//configura la interrupcion EXTI linea 0 en PA0
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising; //flanco de subida
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//Habilita el NVIC para EXT 1 canal IRQ
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//funcion que se ejecuta cuando se detecta la interrupcion
void EXTI0_IRQHandler(void)
{
	//conprueba si la linea 0 de EXTI se pulsa
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		GPIOB->BSRR=GPIO_Pin_1;
		espera(100);
		GPIOB->BRR=GPIO_Pin_1;
		espera(100);
	}
	//desactiva la interrupcion para que se vuelva a comprovar
	EXTI_ClearITPendingBit(EXTI_Line0);
}

//funcion espera
void espera(unsigned int nCount)
{
	unsigned int i,j;
	for(i=0;i<nCount;i++)
		for(j=0;j<0x2AFF;j++);
}
