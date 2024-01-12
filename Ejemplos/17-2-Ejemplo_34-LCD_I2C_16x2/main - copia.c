/**********************************************************************
  * Project Name: LCD_I2C_16x2
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
  * PROGRAMA DE EJEMPLO DE UITLIZACION DE UN PANTALLA LCD CON EL
  * ADAPTADOR DE I2C
  * 
  *  Programa de ejemplo de la libreria "LiquidCrystal_I2C.c" con
  * el adaptador I2c en una pantalla LCD de 16x2
  *
  **********************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"

#include "delay.h"               // Cargamos la librería ‘delay’
#include "I2C.h"                 // Cargamos la librería del ‘I2C’
#include "LiquidCrystal_I2C.h"   // Cargamos la libreria del LCD por I2c

#include <string.h>

#define LCD_I2C_ADDR 0x38   // Establecemos la dirección del modulo I2c
#define LCD_I2C_Long   16   // Establecemos longitud de caracteres del LCD
#define LCD_I2C_Lines   4   // Establecemos el número de líneas del LCD

// Creamos una serie de caracteres para representar en el LCD
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t face[8]  = {0x0,0xa,0xa,0xa,0x0,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = {0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

uint8_t batt000[8]  = {0x0E,0x1B,0x11,0x11,0x11,0x11,0x11,0x1F};
uint8_t batt010[8]  = {0x0E,0x1B,0x11,0x11,0x11,0x11,0x1F,0x1F};
uint8_t batt030[8]  = {0x0E,0x1B,0x11,0x11,0x11,0x1F,0x1F,0x1F};
uint8_t batt050[8]  = {0x0E,0x1B,0x11,0x11,0x1F,0x1F,0x1F,0x1F};
uint8_t batt070[8]  = {0x0E,0x1B,0x11,0x1F,0x1F,0x1F,0x1F,0x1F};
uint8_t batt090[8]  = {0x0E,0x1B,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};
uint8_t batt100[8]  = {0x0E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};
uint8_t battcrg[8]  = {0x0E,0x1F,0x1B,0x17,0x1D,0x1B,0x1F,0x1F};

/* Creamos una matiz que contendrá los nombres de los
   Símbolos creados para carga en la memoria del LCD   */
uint8_t const *simbol[] = {"bateria:","nota :  ","reloj : ","cara :  ","corazon:","pato :  ","control:","cruz :  ","retorno:"};

/* Definimos una varible con el abecedario */
static char caracteres[] = "0123456789abcdefghijklmnopqrstuvwxyz";

//void* memcpy(void *dst, const void *src, size_t count);

/*           Modulo principal                */
//---------------------------------------------
int main(void)
{  
  /*  Configuramos la Direccion y el tipo de pantalla LCD   */
  //-------------------------------------------------------------
  LCDI2C_init(LCD_I2C_ADDR, LCD_I2C_Long, LCD_I2C_Lines);
  
  LCDI2C_backlight(); // Encendemos la retroiluminacion en la pantalla
  LCDI2C_clear();     // Borramos el contenido de la pantalla
  
  LCDI2C_setCursor(0, 0);  // Cursor en columna 0 linea 1 de la pantalla
  LCDI2C_write_String("- PRUEBA LCD I2C-");
  
  LCDI2C_setCursor(0,1);   // Cursor en columna 0 linea 2 de la pantalla
  LCDI2C_write_String("Linea 2 -ABCDEFGH");
  
  // Cuando especificamos al principio una pantalla con mas líneas
  if (LCD_I2C_Lines == 4)
  {
    LCDI2C_setCursor(0,2);    // Cursor en columna 0 linea 3 de la pantalla
    LCDI2C_write_String("01234567890123456");
    LCDI2C_setCursor(0,3);    // Cursor en columna 0 linea 4 de la pantalla
    LCDI2C_write_String("abcdefghijklmnopq");
  }
  delay_ms(1000);
  LCDI2C_clear();
  
  //LLenamos la pantalla de caracteres de prueba
  //----------------------------------------------------------------------
  char caracter[33];
  for (int i=0; i < LCD_I2C_Lines; i++)
  {  
    LCDI2C_setCursor(0,i);
    LCDI2C_write_String(memcpy(caracter, caracteres, LCD_I2C_Long));   
  }
  delay_ms(1000);
  
  /* Ejemplo de desplazamiento de texto hacia la izquierda */  
  //-----------------------------------------------------------------------
  LCDI2C_setCursor(0,1);                          
  LCDI2C_write_String("MOVER A IZQUIERD");
  
  for (int i=0; i < 20;i++)
  {
    LCDI2C_scrollDisplayLeft();   
    delay_ms(500);
  }
  delay_ms(1000);  
  
  /* Ejemplo de desplazamiento de texto hacia la derecha */  
  //-----------------------------------------------------------------------  
  LCDI2C_setCursor(0,1);                          
  LCDI2C_write_String("MOVER A DERECHA ");
  
  for (int i=0; i < 20;i++)
  {
    LCDI2C_scrollDisplayRight();    
    delay_ms(500);
  }
    
  /* LLenamos la pantalla de caracteres                    */  
  //----------------------------------------------------------------------- 
   delay_ms(1000);
  LCDI2C_clear();   
  for (int i=0; i < LCD_I2C_Lines; i++)
  {  
    LCDI2C_setCursor(0,i);
    LCDI2C_write_String(memcpy(caracter, caracteres, LCD_I2C_Long));   
  }
  delay_ms(1000);
  LCDI2C_clear(); 
  
  /*  Mostrar caracteres predefinidos por el usuario */  
  // Guardamos los simbolos de Bateria en la memoria de la pantalla LCD  
  LCDI2C_createChar(0, batt000);
  LCDI2C_createChar(1, batt010);
  LCDI2C_createChar(2, batt030);
  LCDI2C_createChar(3, batt050);
  LCDI2C_createChar(4, batt070);
  LCDI2C_createChar(5, batt090);
  LCDI2C_createChar(6, batt100);
  LCDI2C_createChar(7, battcrg);    
    
  // Los mostramos en la pantalla
  for (int i=0; i <= 7; i++)
  {
    LCDI2C_setCursor(0,0); 
    LCDI2C_write_String((char*) simbol[0]);

    LCDI2C_setCursor(10,1); 
    LCDI2C_write(i);
    delay_ms(1000);
  }
  delay_ms(1500);
  LCDI2C_clear();
  
  // Guardamos los otros simbolos   
  LCDI2C_createChar(0, note);
  LCDI2C_createChar(1, clock);
  LCDI2C_createChar(2, face);
  LCDI2C_createChar(3, heart);
  LCDI2C_createChar(4, duck);
  LCDI2C_createChar(5, check);
  LCDI2C_createChar(6, cross);
  LCDI2C_createChar(7, retarrow);  
  
  // Los mostramos en la pantalla
  for (int i=0; i < 8; i++)
  {        
    LCDI2C_setCursor(0,0);
    LCDI2C_write_String((char*) simbol[i+1]);    
    LCDI2C_setCursor(10,1); 
    LCDI2C_write(i);
    delay_ms(1000);   
  }      
  LCDI2C_clear();
  
}


////////////////  Fin del Programa ////////////////////////////////

