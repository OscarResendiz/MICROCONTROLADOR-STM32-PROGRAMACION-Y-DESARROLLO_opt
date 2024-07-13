/**********************************************************************
D:\Cursos\STM32\Cocox\GIT\Ejemplos\20-1_Ejemplo_40-USB_puerto_com\hw_config.h
D:\Cursos\STM32\Cocox\GIT\Ejemplos\20-1_Ejemplo_40-USB_puerto_com\STM32_USB-FS-Device_Lib\hw_config.h
  * Project Name: USB_com_port
  * File Name: hw_config.h
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Important:Select in "Options for Target" the option
  *           (x) Use MicroLib
  * =====================================================================
  *  ESTE PROGRAMA ES PARTE DE LA LIBRERIA STMICROELECTRONICS y no debe
  *  ser modificado libremente, ya que pertenece a la estructura de la 
  *  libreria 'STM_USB_Device_Lib" de STMicroelectronics.
  * 
  * *********************************************************************
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL PUERTO USB
  * 
  * Programa ejemplo donde se configura el puerto USB de la
  * placa de pruebas para utilizarlo como puerto de comunicacion
  * con nuestro PC.
  *
  * Cuando se detecta la recepción de un '1' se enciende el LED
  * de pruebas de la placa, se apaga con la recepción de cualquier
  * otro caracter.
  * Todos los caracteres recibidos se reenvian de nuevo por el
  * mismo puerto hacia el ordenador.
  *
  **********************************************************************/   

#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "stm32f10x.h"               
#include "stm32f10x_pwr.h"           

//#include "platform_config.h"
#include "hw_config.h"
#include "usb_type.h"

#define MASS_MEMORY_START     0x04002000
//#define BULK_MAX_PACKET_SIZE  0x00000040
#define LED_ON                0xF0
#define LED_OFF               0xFF

/*Unique Devices IDs register set*/
/*STM32F1x*/
#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)

#define USB_DISCONNECT                    GPIOD
#define USB_DISCONNECT_PIN                GPIO_Pin_9
#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOD

void Set_System(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Cable_Config (FunctionalState NewState);
//void Get_SerialNum(void);
uint32_t CDC_Receive_DATA(void);
uint32_t USB_data_Send (uint8_t *ptrBuffer, uint8_t Send_length);

#endif  

/////////////////   Fin del Programa   /////////////////////////////////////
