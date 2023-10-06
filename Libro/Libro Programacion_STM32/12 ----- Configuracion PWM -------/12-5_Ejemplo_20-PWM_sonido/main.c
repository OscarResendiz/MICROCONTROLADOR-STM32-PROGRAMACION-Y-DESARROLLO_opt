 /**********************************************************************
  * Project Name: PWM_sonido
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
  *  PROGRAMA DE EJEMPLO DE MANEJO GENERANDO SENAL DE PWM
  *
  *  En el programa generamos una melodia en un altavoz 
  *  conectado al pin PB6
  *
  *********************************************************************/
  
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Librerias de los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

// Definimos la frecuencia del reloj y el prescaler
#define SYSCLK 72000000
#define PRESCALER 72

// Creamos las variables con las notas musicales y sus frecuencias
#define Do	 261 //Do
#define Do1	 277 //Do#
#define Re	 293 //Re
#define Re1	 311 //Re#
#define Mi   239 //Mi
#define Fa	 349 //Fa
#define Fa1	 370 //Fa#
#define Sol	 392 //Sol
#define Sol1 415 //Sol#
#define La	 440 //La
#define La1	 466 //La#
#define Si	 494 //Si

// Creamos las variables con los tiempos musicales
#define t1	2000
#define t3  1250
#define t2	1000
#define t4	500
#define t8	250
#define t16	125

// Definimos una estructura para entrar la musica
typedef struct
{
	uint16_t freq;
	uint16_t time;
}SoundTypeDef;


// Creamos la variable macro Music con la partitura  
/**************************************/
/*       HARRY POTTER                 */
/**************************************/
#define MUSICSIZE 32
const SoundTypeDef Music[MUSICSIZE] ={
	{Si, t4},
	{Mi, t1},
  {Sol, t8},
  {Fa1, t8},
  {Mi, t2},
  {Si, t4},
  {La, t3},
  {Fa1, t2},
  {Si, t4},
  {Mi, t2},
  {Sol, t8},
  {Fa1, t8},
  {Re, t2},
  {Mi, t4},
  {Si, t2},
  {0, t2},
  {Si, t4},
  {Mi, t2},
  {Sol, t8},
  {Fa1, t8},
  {Mi, t2},
  {Si, t4},
  {Re, t2},
  {Do1, t4},
  {Do*2, t2},
  {La, t4},
  {Do*2, t2},
  {Si, t8},
  {La1, t8},
  {Si, t2},
  {Sol, t4},
  {Mi, t2},	
};

// Definimos las variables 
int Paso_sonido = 0;
char Play_Musica = 0;

int TIMER_Sounido;
int sound_counter;


void SONIDO_Config(void) 
{
	/*  Creamos la estructura para GPIO  */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Establecemos el reloj pra GPIO  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/*  Configuamos la salida del altavoz en pin PB6 */		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Iniciamos GPIO 
	GPIO_StructInit(&GPIO_InitStructure);

	/*   Configuramos el TIM  */
  // Creamos la estructura para TIM 
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	
	// Establecemos el reloj para TIM4  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	

	// Configuramos el TIM
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler = PRESCALER;
	TIMER_InitStructure.TIM_Period = 0xFFFF;
	TIMER_InitStructure.TIM_ClockDivision = 0;
	TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);

	/*  Configuramos el PWM  */
	// Creamos la estructura para PWM
	TIM_OCInitTypeDef TIM_PWM_InitStructure;
	// Configuramos el PWM
	TIM_OCStructInit(&TIM_PWM_InitStructure);
	TIM_PWM_InitStructure.TIM_Pulse = 0;
	TIM_PWM_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_PWM_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_PWM_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_PWM_InitStructure);

  /* Iniciamos la interrupcion para TIM4   */
  TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);

  /* Iniciamos la interrupcion para TIM2   */
  NVIC_EnableIRQ(TIM4_IRQn);
}

/*  Funcion que genera la frecuencia en el Altavoz   */
void sound (int freq, int time_ms) {
	if (freq > 0) {		
		TIM4->ARR = SYSCLK / PRESCALER / freq;
		TIM4->CCR1 = TIM4->ARR / 2;
	}
	else {
		TIM4->ARR = 1000;
		TIM4->CCR1 = 0;
	}
	TIM_SetCounter(TIM4, 0);

	TIMER_Sounido = ((SYSCLK / PRESCALER / TIM4->ARR) * time_ms ) / 1000;
	sound_counter = 0;
	TIM_Cmd(TIM4, ENABLE);
}

/*  Funcion que  inicia la musica1  en el altavoz */
void Sonido1_Init(void) {
	Paso_sonido = 0;
	Play_Musica = 1;
	sound(Music[Paso_sonido].freq, Music[Paso_sonido].time);
}

/*  Funcion que detecta la interrupcion y produce el sonido ^*/
void TIM4_IRQHandler(void){

	if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
	  {
	    
	    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
	    sound_counter++;
			
	    if (sound_counter > TIMER_Sounido) {
	    	if (Play_Musica == 0) {
	    		TIM_Cmd(TIM4, DISABLE);
	    	}
	    	else {
	    		if (Paso_sonido < MUSICSIZE-1) {
	    			if (TIM4->CCR1 == 0){
	    				Paso_sonido++;
	    				sound(Music[Paso_sonido].freq, Music[Paso_sonido].time);
	    			}
	    			else{
	    				sound(0, 30);
	    			}
	    		}
	    		else {
		    		Play_Musica = 0;
		    		TIM_Cmd(TIM4, DISABLE);
	    		}
	    	}
	    }
	    
	    if (TIM_GetFlagStatus(TIM4, TIM_FLAG_CC4OF) != RESET)
	    {
	      TIM_ClearFlag(TIM4, TIM_FLAG_CC4OF);
	      
	    }
	  }
}



/*   Modulo principal   */
//================================================================
int main(void)
{
	
	SONIDO_Config();    // Inicializamos la configuracion del GPIO y TIMER

	Sonido1_Init();      // Iniciamos los parametros que inician la meolodia

	while(1)
    {

    }

}


/////////////////////////   Fin del Programa   ///////////////////////
