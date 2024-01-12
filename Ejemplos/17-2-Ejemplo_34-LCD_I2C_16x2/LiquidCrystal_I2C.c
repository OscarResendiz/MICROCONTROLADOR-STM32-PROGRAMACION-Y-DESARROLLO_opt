/**********************************************************************
  * Project Name: LCD_I2C_16x2
  * File Name: LiquidCrystal_I2C.c  
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
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"

#include "I2C.h"
#include "LiquidCrystal_I2C.h"
#include "delay.h"

LiquidCrystal_I2C_Def LCD2c_InitStructure;

/*  Funcion que inicializa el LCD con los parametros del usuario */
//-----------------------------------------------------------------
void LCDI2C_init(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows)
{
  LCD2c_InitStructure.Addr = lcd_Addr;
  LCD2c_InitStructure.cols = lcd_cols;
  LCD2c_InitStructure.rows = lcd_rows;
  LCD2c_InitStructure.backlightval = LCD_NOBACKLIGHT;

  I2C1_LCD16x2_Init(); 
  LCD2c_InitStructure.displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  LCDI2C_begin(lcd_cols, lcd_rows);
}

void LCDI2C_begin(uint8_t cols, uint8_t lines) {
	if (lines > 1) {
		LCD2c_InitStructure.displayfunction |= LCD_2LINE;
	}
  
	LCD2c_InitStructure.numlines = lines;	
	delay_ms(50);

	LCDI2C_expanderWrite(LCD2c_InitStructure.backlightval);	
	delay_ms(1000);

  LCDI2C_write4bits(0x03 << 4);
  delay_Mc(4500); 

  LCDI2C_write4bits(0x03 << 4);
  delay_Mc(4500); 

  LCDI2C_write4bits(0x03 << 4);
  delay_Mc(150);

  LCDI2C_write4bits(0x02 << 4);

 	LCDI2C_command(LCD_FUNCTIONSET | LCD2c_InitStructure.displayfunction);

	LCD2c_InitStructure.displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LCDI2C_display();
	
	LCDI2C_clear();

	LCD2c_InitStructure.displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	LCDI2C_command(LCD_ENTRYMODESET | LCD2c_InitStructure.displaymode);

	LCDI2C_home();
}

void LCDI2C_write(uint8_t value){
	LCDI2C_send(value, Rs);
}

void LCDI2C_clear(void){
	LCDI2C_command(LCD_CLEARDISPLAY);
	delay_Mc(3000);  
}

void LCDI2C_home(void){
	LCDI2C_command(LCD_RETURNHOME);  
	delay_Mc(3000);  
}

void LCDI2C_setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > LCD2c_InitStructure.numlines ) {
		row = LCD2c_InitStructure.numlines-1;    
	}
	LCDI2C_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LCDI2C_noDisplay(void) {
	LCD2c_InitStructure.displaycontrol &= ~LCD_DISPLAYON;
	LCDI2C_command(LCD_DISPLAYCONTROL | LCD2c_InitStructure.displaycontrol);
}

void LCDI2C_display(void) {
	LCD2c_InitStructure.displaycontrol |= LCD_DISPLAYON;
	LCDI2C_command(LCD_DISPLAYCONTROL | LCD2c_InitStructure.displaycontrol);
}

void LCDI2C_noCursor(void) {
	LCD2c_InitStructure.displaycontrol &= ~LCD_CURSORON;
	LCDI2C_command(LCD_DISPLAYCONTROL | LCD2c_InitStructure.displaycontrol);
}

void LCDI2C_cursor(void) {
	LCD2c_InitStructure.displaycontrol |= LCD_CURSORON;
	LCDI2C_command(LCD_DISPLAYCONTROL | LCD2c_InitStructure.displaycontrol);
}

void LCDI2C_noBlink(void) {
	LCD2c_InitStructure.displaycontrol &= ~LCD_BLINKON;
	LCDI2C_command(LCD_DISPLAYCONTROL | LCD2c_InitStructure.displaycontrol);
}

void LCDI2C_blink(void) {
	LCD2c_InitStructure.displaycontrol |= LCD_BLINKON;
	LCDI2C_command(LCD_DISPLAYCONTROL | LCD2c_InitStructure.displaycontrol);
}

void LCDI2C_scrollDisplayLeft(void) {
	LCDI2C_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LCDI2C_scrollDisplayRight(void) {
	LCDI2C_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCDI2C_leftToRight(void) {
	LCD2c_InitStructure.displaymode |= LCD_ENTRYLEFT;
	LCDI2C_command(LCD_ENTRYMODESET | LCD2c_InitStructure.displaymode);
}

void LCDI2C_rightToLeft(void) {
	LCD2c_InitStructure.displaymode &= ~LCD_ENTRYLEFT;
	LCDI2C_command(LCD_ENTRYMODESET | LCD2c_InitStructure.displaymode);
}

void LCDI2C_autoscroll(void) {
	LCD2c_InitStructure.displaymode |= LCD_ENTRYSHIFTINCREMENT;
	LCDI2C_command(LCD_ENTRYMODESET | LCD2c_InitStructure.displaymode);
}

void LCDI2C_noAutoscroll(void) {
	LCD2c_InitStructure.displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	LCDI2C_command(LCD_ENTRYMODESET | LCD2c_InitStructure.displaymode);
}

void LCDI2C_createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; 
	LCDI2C_command(LCD_SETCGRAMADDR | (location << 3));
	int i;
	for (i=0; i<8; i++) {
		LCDI2C_write(charmap[i]);
	}
}

void LCDI2C_noBacklight(void) {
	LCD2c_InitStructure.backlightval=LCD_NOBACKLIGHT;
	LCDI2C_expanderWrite(0);
}

void LCDI2C_backlight(void) {
	LCD2c_InitStructure.backlightval=LCD_BACKLIGHT;
	LCDI2C_expanderWrite(0);
}

void LCDI2C_command(uint8_t value) {
	LCDI2C_send(value, 0);
}

void LCDI2C_send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
  LCDI2C_write4bits((highnib)|mode);
	LCDI2C_write4bits((lownib)|mode);
}

void LCDI2C_write4bits(uint8_t value) {
	LCDI2C_expanderWrite(value);
	LCDI2C_pulseEnable(value);
}

void LCDI2C_expanderWrite(uint8_t _data){
	I2C_StartTransmission (I2C1, I2C_Direction_Transmitter, LCD2c_InitStructure.Addr); 
	I2C_WriteData(I2C1, (int)(_data) | LCD2c_InitStructure.backlightval);  
	I2C_GenerateSTOP(I2C1, ENABLE); 
}

void LCDI2C_pulseEnable(uint8_t _data){
	LCDI2C_expanderWrite(_data | En);	
	delay_Mc(1);		

	LCDI2C_expanderWrite(_data & ~En);	
	delay_Mc(50);		
}

void LCDI2C_cursor_on(void){
	LCDI2C_cursor();
}

void LCDI2C_cursor_off(void){
	LCDI2C_noCursor();
}

void LCDI2C_blink_on(void){
	LCDI2C_blink();
}

void LCDI2C_blink_off(void){
	LCDI2C_noBlink();
}

void LCDI2C_load_custom_character(uint8_t char_num, uint8_t *rows){
		LCDI2C_createChar(char_num, rows);
}

void LCDI2C_write_String(char* str) {
  uint8_t i=0;
  while(str[i])
  {
    LCDI2C_write(str[i]);
    i++;
  }
}

////////////////////  Fin del Programa //////////////////////////////
