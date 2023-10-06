/**********************************************************************
  * Project Name: I2C_HMC5883L
  * File Name: i2c.h
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
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL PUERTO I2C 
  * 
  *  Nuestro código configura el bus I2C para que se conecte a un sensor
  *  HMC5883 del que leeremos los datos de sus registros
  *
  **********************************************************************/
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"

/*   Definimos el puerto I2C de nuestro microcontrolador   */
#define I2Cx	I2C2

/* Lista de funciones ---------------------------------------*/
void I2C_Config(void);
void I2C_single_Write(uint8_t address, uint8_t data);
void I2C_write_Reg(uint8_t address, uint8_t reg, uint8_t data);
void i2c_write_multi_no_reg(uint8_t address, uint8_t* data, uint8_t len);
void i2c_write_multi_with_reg(uint8_t address, uint8_t reg, uint8_t* data, uint8_t len);
void i2c_read_no_reg(uint8_t address, uint8_t* data);
void i2c_read_with_reg(uint8_t address, uint8_t reg, uint8_t* data);
void I2C_read_multiVal(uint8_t address, uint8_t len, uint8_t* data);
void I2C_read_multiValReg(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data);

#ifdef __cplusplus
}
#endif
#endif

/////////////////////////////////////////////////////
