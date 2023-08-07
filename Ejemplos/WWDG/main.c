#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_wwdg.h"
#include "misc.h"
//#include "delay.h"
//Declaracion de funciones que se utilizaran
void Delay_ms(unsigned int nCount);

void LED_Init(void);
//funcion de servicio que se ejecuta cuando se produce el restablecimiento
void WWDG_IRQHandler(void)
{
	WWDG_ClearFlag(); //restablecemos la bandera del WWDG
	WWDG_SetCounter(100); //(101) valor entre 64<-->127
	//invierte el led del pin C13
	GPIOC->ODR^=GPIO_Pin_13;
}
//modulo principal
int main(void)
{
	LED_Init(); //iniciamos el GIO del LED del pruebas
	//DelayInit();// inicializamos la libreria delay
	//Inicializamos el relog del watchdog WCG
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	WWDG_DeInit(); //iniciamos el watchdog
	//donde configuramos el WWDG con los siguientes valores
	//WWDG_Clock_counter=(PCLK1 (36Mhz))/9046)/8=1098.6hz (~910 us)
	WWDG_SetPrescaler(WWDG_Prescaler_8); //valores de prescaler 1,2,4,8
	//valor de ventana en 0x50 (80 decimal), que significa que el contador WWDG debe actualizarse solo cuando el contador
	//este por debajo de 0x50 (80) y por encima de 0x40 (64), de lo contrario se generara un restablecimiento y un
	//reseteio del sistema
	WWDG_SetWindowValue(0x50);
	// establecemos el tiempo de restablecimiento: el contador del WWDG se establece 0x7F (127) lo que se genera un
	//timeout de =~910us*64=58.2 ms
	WWDG_Enable(0x7F); //(127) valor entre 64<-->127
	WWDG_ClearFlag(); //borramos y reiniciamos la bandera del WWDG
	WWDG_EnableIT(); //habilitamos la interrupcion WWDB
	//iniciamos el NVIC para la interrupcion
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn; //interrupcion WWDG
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    while(1)
    {
    	//mientras se restablesca el contador en las iguientes lineas, antes de que se podusca el restablecimiento no se pducira el reseteo del sistema
    	//si habilitamos las dos lineas, o bajamos el valor del retraso generado con el delay a un valor que deje que se restablesca con contador WWDG,
    	//se producira el restablecimiento del WWDG automaticamete y veremos encenderse el led en PC13 indicando que se esta produciendo el restablecimiento
    	Delay_ms(24); //> 23 se produce el reseteo
    	WWDG_SetCounter(100); //valor debe estar entre 0x40 y 0x7F
    }
}
void LED_Init(void)
{
	//creamos la estructura GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//configurmos el pin PC13 paa el led
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//apagamos el led
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
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





















