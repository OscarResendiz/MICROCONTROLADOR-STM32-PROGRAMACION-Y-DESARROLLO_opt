 /**********************************************************************
  * Project Name: LCD_16x2
  * File Name: delay.c
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
  * PROGRAMA DE EJEMPLO DE LIBRERIA DEL LCD 16x2
  *   
  * Ejemplo del empleo de la libreria para el manejo de una
  * pantalla LCD 16x2.
  *
  **********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

#include "delay.h"

static u8 fac_us=0;
static u16 fac_ms=0;

/*  Funcion que inicializa el reloj del sistema */
void delay_init(u8 SYSCLK)
{
  SysTick->CTRL&=0xfffffffb;
  fac_us=SYSCLK/8; 
  fac_ms=(u16)fac_us*1000;
} 

/*  Funcion que genera un retraso de ms   */
void delay_ms(u16 nms)
{ 
  u32 temp; 
  SysTick->LOAD=(u32)nms*fac_ms;
  SysTick->VAL =0x00; 
  SysTick->CTRL=0x01 ; 
  do
  {
     temp=SysTick->CTRL;
  }
  while(temp&0x01&&!(temp&(1<<16))); 
  SysTick->CTRL=0x00; 
  SysTick->VAL =0X00; 
} 

/*  Funcion que genera un retraso de us   */
void delay_us(u32 nus)
{ 
  u32 temp; 
  SysTick->LOAD=nus*fac_us; 
  SysTick->VAL=0x00; 
  SysTick->CTRL=0x01 ; 
  do
  {
    temp=SysTick->CTRL;
  }
  while(temp&0x01&&!(temp&(1<<16))); 
  SysTick->CTRL=0x00; 
  SysTick->VAL =0X00; 
}


/////////////////////////////////////////////////////////////////////
