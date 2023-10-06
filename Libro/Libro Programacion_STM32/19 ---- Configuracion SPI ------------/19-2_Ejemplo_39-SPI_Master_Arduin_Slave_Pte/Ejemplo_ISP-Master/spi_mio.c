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
  * =====================================================================
  * 
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL PUERTO ISP
  * 
  * El programa configura nuestra placa como maestro en una comunicacion
  * ISP con otra placa en modo sclavo. Como equipo esclavo se utiliza una
  * placa Arduino
  *
  * ======================================================================
  * Secuencia de uso:

   SPIx_Init();         // Inicializamos el GPIO y SPI
   
   SPIx_EnableSlave();  // Activamos SS Slave (PA4 Low)
   
   SPIx_Transfer((uint8_t) '1') // Enviamos dato='1' al Slave
      
   SPIx_Transfer((uint8_t) '?') // Enviamos dato='?' al Slave
      
   receivedByte = SPIx_transfer(0); // Leemos dato del Slave
   
   SPIx_DisableSlave()  // Desactivamos SS Slave (PA4 High)

   **************************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"        
#include "stm32f10x_gpio.h"   
#include "stm32f10x_rcc.h"    
#include "stm32f10x_spi.h"    

#include "spi_mio.h"

void SPIx_Init(void)
{
	SPI_InitTypeDef   SPI_StructureInit;
	GPIO_InitTypeDef  GPIO_StructureInit;
    
	// Habilitamos los relojes del GPIOC y SPI-----------------------------
	RCC_APB2PeriphClockCmd(SPI_GPIO_RCC |
                         RCC_APB2Periph_GPIOC, ENABLE);
  	
  // Creamos el GPIO de los pines MOSI, MISO y SCK ----------------------
	GPIO_StructureInit.GPIO_Pin     = SPI_PIN_MOSI | SPI_PIN_MISO | SPI_PIN_SCK;
	GPIO_StructureInit.GPIO_Mode    = GPIO_Mode_AF_PP;
	GPIO_StructureInit.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_StructureInit);
	
  // Creamos el GPIO para el pin SS ------------------------------------
	GPIO_StructureInit.GPIO_Pin     = SPI_PIN_SS;
	GPIO_StructureInit.GPIO_Mode    = GPIO_Mode_Out_PP;
	GPIO_StructureInit.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_StructureInit);
  
  // Creamos el GPIO para el pin del LED de pruebas --------------------
    GPIO_StructureInit.GPIO_Pin     = GPIO_Pin_13;						
	GPIO_Init(GPIOC, &GPIO_StructureInit);	
  
  // Configuracion del SPI ---------------------------------------------	
	RCC_APB2PeriphClockCmd(SPIx_RCC, ENABLE);
  
	SPI_StructureInit.SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_128;
	SPI_StructureInit.SPI_CPHA               = SPI_CPHA_1Edge;
	SPI_StructureInit.SPI_CPOL               = SPI_CPOL_Low;
	SPI_StructureInit.SPI_DataSize           = SPI_DataSize_8b;
	SPI_StructureInit.SPI_Direction          = SPI_Direction_2Lines_FullDuplex;
	SPI_StructureInit.SPI_FirstBit           = SPI_FirstBit_MSB;
	SPI_StructureInit.SPI_Mode               = SPI_Mode_Master;
	SPI_StructureInit.SPI_NSS                = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI_Init(SPIx, &SPI_StructureInit); 
	SPI_Cmd(SPIx, ENABLE);
		
  // Desabilitamos la linea de SS del esclavo
	SPIx_DisableSlave();
}

/*     Funcion que envia un byte por el bus SPI       */
//------------------------------------------------------
uint8_t SPIx_Transfer(uint8_t data)
{	
  // Envia un byte por el bus SPI	
  SPI_I2S_SendData (SPIx, data);
  
	// Esperamos a que se ha realizado la transimision  
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) != RESET);
  
	// Esperamos a recibir un byte desde el esclavo
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) != RESET);
  
	// Wait until SPI is not busy anymore
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_SR_BSY) != RESET);
  
	// Leemos el dato recibido
  data = SPI1->DR;
  	
  return data;
}

/*     Funcion que habilita la señal SS del esclavo    */
//-------------------------------------------------------
void SPIx_EnableSlave(void)
{
	// Activamos SS (PA4 Low)
	SPI_GPIO->BRR = SPI_PIN_SS;
}

/*     Funcion que deshabilita la señal SS del esclavo  */
//--------------------------------------------------------
void SPIx_DisableSlave(void)
{	
  // Desactivamos SS (PA4 High)
	SPI_GPIO->BSRR = SPI_PIN_SS;
}

/////////////////   Fin del Programa      /////////////////////////////////

