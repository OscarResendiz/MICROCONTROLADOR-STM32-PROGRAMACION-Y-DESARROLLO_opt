#include "stm32f10x.h";
#include "stm32f10x_rcc.h";
#include "stm32f10x_gpio.h";
#include "stm32f10x_tim.h";
#include "misc.h";
//lista de funciones empleadas
void TIM4_Config(void);
void TIM4_IRQHandler(void);
void GPIO_Config(void);
//programa principal
int main(void)
{
	//inicializamos el timer 4 y la interrupcion
	TIM4_Config();
	//configuramos el GPIO del LED
	GPIO_Config();
    while(1)
    {
    	/*
    	 no es necesario lineas de codigo adicionales ya que la funcion que se ejecuya
    	 esta configurada dentro de la funcion TIM4_IRQHandler
    	 */
    }
}
// Funcion que configura el GPIO del led en el Pc13
void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_StructureInit;
	//establecemos el reloj del puerto GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//configuramos el pin13 como salida
	//Asignamos el pin 13 para configuracion
	GPIO_StructureInit.GPIO_Pin=GPIO_Pin_13;
	//se establece la velocidad a 50 MHz
	GPIO_StructureInit.GPIO_Speed=GPIO_Speed_50MHz;
	//se establece el pin como salida
	GPIO_StructureInit.GPIO_Mode=GPIO_Mode_Out_PP;
	//se establece la configuracion
	GPIO_Init(GPIOC,&GPIO_StructureInit);

}
// funcion que se ejecuta cuando se detecta el fin del periodo
void TIM4_Config(void)
{
	//se crea la estructura del Timer4
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	//se habilita el relog del timer 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	// configuramos los parametros del timer 4 para detectar un desbordamiento cada vez que cuente
	//un periodo de tiempo que sera cada 1000 veces por segundo
	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_Prescaler=8000;
	TIM_InitStructure.TIM_Period=500;
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_InitStructure);
	//creamos la interrupcion para el TIM4
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	//se inicializa el timer2
	TIM_Cmd(TIM4,ENABLE);
	//configuracion del NVIC
	//Configurar e inicial la interrupcion TIM4_IRQn
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//funcion que se ejecuta cuando se detecta la interrupcion
void TIM4_IRQHandler(void)
{
	int static estado=0;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		if(estado==0)
		{
			estado=1;
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
		}
		else
		{
			estado=0;
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		}
		//GPIOC->ODR ^=GPIO_Pin_13;
	}
}
