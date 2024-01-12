 /**********************************************************************
  * Project Name: LCD_16x2
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
  * PROGRAMA DE EJEMPLO DE LIBRERIA DEL LCD 16x2
  *   
  * Ejemplo del empleo de la libreria para el manejo de una
  * pantalla LCD.
  *
  **********************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"        
#include "stm32f10x_rcc.h"         
#include "misc.h"                  

#include "delay.h"
#include "lcd4bit.h"

/*            Funcion principal            */
//===========================================
int main(void)
{  
	LCD_Init();   // Inicializamos la libreria LCD
    
	lcd_cursor_on();    // Cursor visible activo
	lcd_cursor_blink(); // Cursor intermitente
  lcd_clear();        // Borra la pantalla 
  
  LCD_Set_Cursor(1,0);
  delay_ms(100);
	LCD_Put_Str("PRUEBA DE LCD");
  
	LCD_Set_Cursor(2,0);
	delay_ms(100);
	LCD_Put_Str(" Esta es Linea 2");
 
  
while(1)
	{
	}
}

//////////////////   Fin del Programa     /////////

