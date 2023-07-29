#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
//funcion que configura los gpios
void GPIO_Config(void)
{
	//configurcmos el GPIO pr el led PC13
	GPIO_InitTypeDef GPIO_InitStructure;
	//inicimos el reloj pr el GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//configuramos el GPIO del pin 13 para el led
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	//configuramos el gpio para pb0 y pb1
	//configuramos lo pines pr los pulsadores PB0 y PB1
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
 //funcion que configura inicialmente el IWDG para que se reinicie el sistema cada medio segundo
void IWDG_setup()
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32); //preescaler a 32 (32khz/31=1 khz)
	IWDG_SetReload(500); //recarga cada 500 ms
	IWDG_ReloadCounter();
	IWDG_Enable();
}
//funcion que permite reconfigurar el IWDG en un tiempo determinado. wn wl wjwmplo pre=32 rlr=400
//-----------------------------------------------------------------------------------------------
// T_out=tiermpo(ms)/(frc_lst * IWDG_preescaler)
// pejemplo 4000ms (4 segundos) presc=32
// T_out=4000ms /(32*32)=4000
//-----------------------------------------------------------------------------------------------
void IWDG_ReInit(u8 prer, u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prer); //preescaler a 32 (32khz/31=1 khz)
	IWDG_SetReload(rlr); //recarga cada 500 ms
	IWDG_ReloadCounter();
	IWDG_Enable();
}
int main(void)
{
	GPIO_Config();
	GPIO_ResetBits(GPIOC,GPIO_Pin_13); //encendemos el led con 0
	//introducimos un retardo al inicial el sistema
//	for(int i=0;i<0x100000;i++);
	//GPIO_SetBits(GPIOC,GPIO_Pin_13); //se apaga el led
	//iniciamos el relog LSI
//	SysTick_Config(SystemCoreClock/10);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
//	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();
	RCC_LSICmd(ENABLE);
	for(int i=0;i<0x100000;i++);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13); //encendemos el led con 0
	for(int i=0;i<0x100000;i++);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	//esperamos a que la bandera del reloj LSI este operativo
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET)
	{
		for(int i=0;i<0x100000;i++);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13); //encendemos el led con 0
		for(int i=0;i<0x100000;i++);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);

	}
	GPIO_ResetBits(GPIOC,GPIO_Pin_13); //encendemos el led con 0
	//configuramos el watchDog para provocar un reinicio del sistea cada segundo
	IWDG_setup();
	//apagamos el led para mostrar cuando se ha reiniciado el sistema poniendo el pin a valor 1
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
    while(1)
    {
    	//comprueba cuando el pin PB0 esta a nivel bajo
    	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
    	{
    		//encendemos el led
    		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
    		//reconfiguramos el IWDG para un retardo de unos 4 segundos
    		IWDG_ReInit(IWDG_Prescaler_32,4000);
    	}
    	else
    	{
    		//mientras el boton no se pulsa
    		//apagamos el led si no se ha pulsado
    		GPIO_SetBits(GPIOC,GPIO_Pin_13);
    	}
    }
}


































