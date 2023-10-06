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
  **********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

#include "spi_mio.h"

void SPIx_Init(void);
uint8_t SPIx_Transfer(uint8_t data);
void SPIx_EnableSlave(void);
void SPIx_DisableSlave(void);
void delay(unsigned int nCount);

uint8_t receivedByte;

//========================================================
int main(void)
{		
	SPIx_Init();
  
	while (1)
	{
		// Activamos señal SS Slave (PA4 Low)
		SPIx_EnableSlave();
    		
		// Enviamos un '1' en el esclavo para enceder el LED		
		SPIx_Transfer((uint8_t) '1');
		delay(10);
    
		// Enviamos el caracter '?' para solicitar reenvio de nuestro mensaje
		SPIx_Transfer((uint8_t) '?');
		delay(10);
    
		// Recibimos el mensaje desde el esclavo 
		receivedByte = SPIx_Transfer(0);
    		
		// Desactivamos la señal SS para el esclavo (PA4 High)
		SPIx_DisableSlave();
    
	    // SI Recibimos '0' se enciende led
		if (receivedByte == 0)
		{			
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
		// Si se recibe '1' se apaga led
		else if (receivedByte == 1)
		{			
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		delay(2500);
		
		// Activamos señal SS para el esclavo (PA4 Low)
		SPIx_EnableSlave();
    
		// Enviamos caracter '0' para apagar el LED
		SPIx_Transfer((uint8_t) '0');
		delay(10);
    
		// Enviamos el caracter '?' para solicitar el reenvio del mensaje
		SPIx_Transfer((uint8_t) '?');
		delay(10);
    		
		// Recibimos el mensaje del esclavo con el estao del LED
		receivedByte = SPIx_Transfer(0);
    		
		// Desactivamos la señal SS del esclavo (PA4 High)
		SPIx_DisableSlave();

		// SI Recibimos '0' se enciende led
		if (receivedByte == 0)
		{			
			GPIO_ResetBits(GPIOC, GPIO_Pin_13); 
		}
		// Si se recibe '1' se apaga led
		else if (receivedByte == 1)
		{			
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		delay(2500);
	}
}


// Modulo que genera retardo 
//--------------------------------------
void delay(unsigned int nCount){ 
		unsigned int i, j;
		for(i = 0; i < nCount; i++)
		{  
				for(j = 0; j < 0x2AFF; j++){;} 
		}
} 

///////////////////////////////////////////////////////////

