/**********************************************************************
  * Project Name: USB_com_port
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
  * ESTE CODIGO DE EJEMPLO ES PROPIEDAD DE STMicroelectronics y esta
  * incluido como ejemplo en la libreria STM32_USB_Device_Lib.
  * *********************************************************************
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL PUERTO USB
  * 
  *  En el programa configura nuestra placa para que se conecte con un puerto
  *  USB de nuestro ordenador y se comunique como un puerto de comunicaciones
  *  estandar.
  *  Se reenvian hacia el ordenador todos los carcteres que se reciben por el
  *  puerto USB del micrcotrolador. Cuando se recibe un '1' el LED de pruebas
  *  de nuestro placa se encenderá y se apaga al recibir cualquier otro caracter.
  *
  **********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias auxialiares para los perifericos estandar      
#include "stm32f10x_gpio.h"            
#include "stm32f10x_rcc.h"    
         
// Librerias de getion del puerto USB    
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"

extern volatile uint8_t Receive_Buffer[64];
extern volatile uint32_t Receive_length ;
extern volatile uint32_t length ;
uint32_t Data_send  = 1;
uint32_t packet_sent  = 1;
uint32_t packet_receive = 1;

int main(void)
{
  Set_System();     // Inicializamos el GPIO del USB
  
  USB_Init();       // Inicializamos la libreria USB

  while (1)
  {
    // Comprobamos si el puerto USB esta operativo
    if (bDeviceState == CONFIGURED)
    {
      CDC_Receive_DATA();    // Leemos la entrada del USB
      
      // Comprobamos si se ha recibido un dato
      if (Receive_length  != 0)
      {
    	  // Enciende el LED de pruebas al recibir un '1'
    	  if (Receive_Buffer[0]=='1') {
    		  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    	  }
    	  else {
          // Apaga el LED de pruebas al recibir cualquier otro numero
    		  GPIO_SetBits(GPIOC, GPIO_Pin_13);
    	  }

    	  // Reproducimos ECHO con los datos recibidos por el USB
    	  if (Data_send == 1) {
    		  USB_data_Send ((uint8_t*)Receive_Buffer, Receive_length);
    	  }

    	  Receive_length = 0;
      }
    }
  }
}


/////////////////////  Fin del Programa ////////////////////////////////////

