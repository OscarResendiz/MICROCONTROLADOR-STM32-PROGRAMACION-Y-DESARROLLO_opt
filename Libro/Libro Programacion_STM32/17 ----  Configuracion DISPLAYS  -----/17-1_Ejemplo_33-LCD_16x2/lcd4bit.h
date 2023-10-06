 /**********************************************************************
  * Project Name: LCD_16x2
  * File Name: lcd4bit.h
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

#ifndef __lcd4bit_H
#define __lcd4bit_H

/*   Definimos los pines donde conectaremos la pantalla LCD   */
//--------------------------------------------------------------
// Definimos los pines a conectar con la pantalla LCD
#define LCD_RS_PIN GPIO_Pin_0	  // Pin RS_LCD PB0
#define LCD_RW_PIN GPIO_Pin_1	  // Pin RW_LCD PB1
#define LCD_EN_PIN GPIO_Pin_10	// Pin EN_LCD PB10

#define LCD_D4_PIN GPIO_Pin_12  // Pin DATA4_LCD PB12
#define LCD_D5_PIN GPIO_Pin_13  // Pin DATA5_LCD PB13
#define LCD_D6_PIN GPIO_Pin_14  // Pin DATA6_LCD PB14
#define LCD_D7_PIN GPIO_Pin_15  // Pin DATA7_LCD PB15

#define LCD_RS_PORT	GPIOB       // Puerto Pin RS GPIOB
#define LCD_RW_PORT	GPIOB       // Puerto Pin RW GPIOB
#define LCD_EN_PORT	GPIOB       // Puerto Pin EN GPIOB
#define LCD_DATA_PORT	GPIOB     // Puerto Pines DATAx GPIOB

#define RCC_APB2Periph_GPIO_RS RCC_APB2Periph_GPIOB // Reloj para pin RS_LCD APB2
#define RCC_APB2Periph_GPIO_RW RCC_APB2Periph_GPIOB // Reloj para pin RW_LCD APB2
#define RCC_APB2Periph_GPIO_EN RCC_APB2Periph_GPIOB // Reloj para pin EN_LCD APB2
#define RCC_APB2Periph_GPIO_DATA RCC_APB2Periph_GPIOB//Reloj para pines Data APB2

// Definimos que nombres para activar los pines a nivel 'High' y 'Low'
#define LCD_EN_HI() GPIO_WriteBit(LCD_EN_PORT,LCD_EN_PIN,Bit_SET)
#define LCD_EN_LO()	GPIO_WriteBit(LCD_EN_PORT,LCD_EN_PIN,Bit_RESET)
#define LCD_RW_HI() GPIO_WriteBit(LCD_RW_PORT,LCD_RW_PIN,Bit_SET) 
#define LCD_RW_LO() GPIO_WriteBit(LCD_RW_PORT,LCD_RW_PIN,Bit_RESET)
#define LCD_RS_HI() GPIO_WriteBit(LCD_RS_PORT,LCD_RS_PIN,Bit_SET)
#define LCD_RS_LO() GPIO_WriteBit(LCD_RS_PORT,LCD_RS_PIN,Bit_RESET)
#define LCD_D4_HI() GPIO_WriteBit(LCD_DATA_PORT,LCD_D4_PIN,Bit_SET)
#define LCD_D4_LO() GPIO_WriteBit(LCD_DATA_PORT,LCD_D4_PIN,Bit_RESET)
#define LCD_D5_HI() GPIO_WriteBit(LCD_DATA_PORT,LCD_D5_PIN,Bit_SET)
#define LCD_D5_LO() GPIO_WriteBit(LCD_DATA_PORT,LCD_D5_PIN,Bit_RESET)
#define LCD_D6_HI() GPIO_WriteBit(LCD_DATA_PORT,LCD_D6_PIN,Bit_SET)
#define LCD_D6_LO() GPIO_WriteBit(LCD_DATA_PORT,LCD_D6_PIN,Bit_RESET)
#define LCD_D7_HI() GPIO_WriteBit(LCD_DATA_PORT,LCD_D7_PIN,Bit_SET)
#define LCD_D7_LO() GPIO_WriteBit(LCD_DATA_PORT,LCD_D7_PIN,Bit_RESET)

/*   Definimos los nombres de los comandos para el LCD                  */
//-------------------------------------------------------------------------
#define lcd_clear() LCD_Write_Cmd(0x01)	      // Borra la pantalla 
#define lcd_cursor_home() LCD_Write_Cmd(0x02)	// Establecer el cursor a 'Home'
#define lcd_display_on() LCD_Write_Cmd(0x0E)	// Pantalla LCD Activa
#define lcd_display_off() LCD_Write_Cmd(0x08)	// Pantalla LCD Inactiva
#define lcd_cursor_blink() LCD_Write_Cmd(0x0F)// Cursor intermitente
#define lcd_cursor_on() LCD_Write_Cmd(0x0E)	  // Cursor visible activo
#define lcd_cursor_off() LCD_Write_Cmd(0x0C)	// Cursor inactivo
#define lcd_cursor_left() LCD_Write_Cmd(0x10)	// Movimiento hacia la izquierda del cursor
#define lcd_cursor_right() LCD_Write_Cmd(0x14)// Movimiento hacia la derecha del cursor
#define lcd_display_sleft() LCD_Write_Cmd(0x18)	// Movimiento a la izquierda de la pantalla
#define lcd_display_sright() LCD_Write_Cmd(0x1C)// Movimiento a la derecha de la pantalla

// Lista de funciones ----------------------------------------------------
void LCD_Out_Data4(unsigned char val);
void LCD_Write_Byte(unsigned char val);
void LCD_Write_Cmd(unsigned char val);
void LCD_Put_Char(unsigned char c);
void LCD_Init(void);
void LCD_Set_Cursor(unsigned char column, unsigned char line);
void LCD_Put_Str(char* str);
void LCD_Put_Num(int num);
char LCD_Busy(void);
void LCD_pulse_EN(void);
void LCD_BarGraphic (int value, int size);
void LCD_BarGraphicXY (int pos_x, int pos_y, int value);

#endif /* __lcd4bit_H */
