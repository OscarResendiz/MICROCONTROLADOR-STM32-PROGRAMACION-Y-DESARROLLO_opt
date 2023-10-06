/**********************************************************************
  * Project Name: ISP_Master
  * File Name: main.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * =====================================================================*/

#ifndef SPI_MIO_H
#define SPI_MIO_H

#include "spi_mio.h"

#define SPIx_RCC			RCC_APB2Periph_SPI1
#define SPIx				SPI1
#define SPI_GPIO_RCC		RCC_APB2Periph_GPIOA
#define SPI_GPIO			GPIOA
#define SPI_PIN_MOSI		GPIO_Pin_7
#define SPI_PIN_MISO		GPIO_Pin_6
#define SPI_PIN_SCK			GPIO_Pin_5
#define SPI_PIN_SS			GPIO_Pin_4

/*  Conexion: ====================

  STM32    Arduino-Uno
   PA4 ---- 10
   PA5 ---- 13
   PA6 ---- 12
   PA7 ---- 11
            09 -- LED -[ ]-- >| -- Gnd
 ==================================*/

void SPIx_Init(void);
void SPIx_EnableSlave(void);
void SPIx_DisableSlave(void);

#endif

/////////////////////////////////////////////////////////////////

