/**********************************************************************
  * Project Name: LCD_I2C_16x2
  * File Name: I2C.h  
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

#include "stm32f10x.h"

void I2C1_LCD16x2_Init(void);
void I2C_StartTransmission(I2C_TypeDef* I2Cx, uint8_t transmissionDirection,  uint8_t slaveAddress);
void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);

////////////////////// Fin del Programa //////////////////////////////////

