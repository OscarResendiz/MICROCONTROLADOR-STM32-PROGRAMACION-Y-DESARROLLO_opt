/**********************************************************************
  * Project Name: LCD_I2C_16x2
  * File Name: LiquidCrystal_I2C.h  
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5  
  * Date :    2018.01.08
  * Select in "Options for Target" the option
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

#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"

#include "delay.h"
#include "I2C.h"

#define En 0x04 
#define Rw 0x02 
#define Rs 0x01 

// Comandos para manejo del LCD normal
#define LCD_CLEARDISPLAY  0x01
#define LCD_RETURNHOME    0x02
#define LCD_ENTRYMODESET  0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT   0x10
#define LCD_FUNCTIONSET   0x20
#define LCD_SETCGRAMADDR  0x40
#define LCD_SETDDRAMADDR  0x80

// Banderas de estado del LCD
#define LCD_ENTRYRIGHT    0x00
#define LCD_ENTRYLEFT     0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Comandos de manejo del cursor
#define LCD_DISPLAYON     0x04
#define LCD_DISPLAYOFF    0x00
#define LCD_CURSORON      0x02
#define LCD_CURSOROFF     0x00
#define LCD_BLINKON       0x01
#define LCD_BLINKOFF      0x00

// Comandos para manejo de la pantalla
#define LCD_DISPLAYMOVE   0x08
#define LCD_CURSORMOVE    0x00
#define LCD_MOVERIGHT     0x04
#define LCD_MOVELEFT      0x00

// Comandos de inicializacion de la pantalla
#define LCD_8BITMODE      0x10
#define LCD_4BITMODE      0x00
#define LCD_2LINE         0x08
#define LCD_1LINE         0x00
#define LCD_5x10DOTS      0x04
#define LCD_5x8DOTS       0x00

// Comandos para activar/desactivar la retroiluminacion
#define LCD_BACKLIGHT     0x08
#define LCD_NOBACKLIGHT   0x00

/* Estructura para configuracion de la pantalla */
typedef struct
{
  uint8_t Addr;
  uint8_t displayfunction;
  uint8_t displaycontrol;
  uint8_t displaymode;
  uint8_t numlines;
  uint8_t cols;
  uint8_t rows;
  uint8_t backlightval;
} LiquidCrystal_I2C_Def;

// Lista de funciones que ejecutan los comandos en el I2c
  void LCDI2C_write(uint8_t value);
  void LCDI2C_init(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
  void LCDI2C_begin(uint8_t cols, uint8_t rows);
  void LCDI2C_clear(void);
  void LCDI2C_home(void);
  void LCDI2C_noDisplay(void);
  void LCDI2C_display(void);
  void LCDI2C_noBlink(void);
  void LCDI2C_blink(void);
  void LCDI2C_noCursor(void);
  void LCDI2C_cursor(void);
  void LCDI2C_scrollDisplayLeft(void);
  void LCDI2C_scrollDisplayRight(void);
  void LCDI2C_printLeft(void);
  void LCDI2C_printRight(void);
  void LCDI2C_leftToRight(void);
  void LCDI2C_rightToLeft(void);
  void LCDI2C_shiftIncrement(void);
  void LCDI2C_shiftDecrement(void);
  void LCDI2C_noBacklight(void);
  void LCDI2C_backlight(void);
  void LCDI2C_autoscroll(void);
  void LCDI2C_noAutoscroll(void);
  void LCDI2C_createChar(uint8_t location, uint8_t charmap[]);
  void LCDI2C_setCursor(uint8_t col, uint8_t row);
  void LCDI2C_write_String(char* str);
  void LCDI2C_command(uint8_t value);

void LCDI2C_blink_on(void);             
void LCDI2C_blink_off(void);            
void LCDI2C_cursor_on(void);      	     
void LCDI2C_cursor_off(void);           
void LCDI2C_setBacklight(uint8_t new_val);				
void LCDI2C_load_custom_character(uint8_t char_num, uint8_t *rows);	
void LCDI2C_printstr(const char[]);

void LCDI2C_send(uint8_t, uint8_t);
void LCDI2C_write4bits(uint8_t);
void LCDI2C_expanderWrite(uint8_t);
void LCDI2C_pulseEnable(uint8_t);

#endif

///////////////// Fin del Programa ////////////////////////////
