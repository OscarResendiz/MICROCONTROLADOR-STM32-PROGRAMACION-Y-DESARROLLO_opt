/**********************************************************************
  * Project Name: USART2
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
  * PROGRAMA DE PRUEBA DE RECEPCION Y ENVIO DE TEXTO POR EL USART2
  *
  *  Codigo que reenvia por el mismo puerto USART2 los caracteres
  *  que se reciben.
  *
  *  Pines del USART2:   Tx PA2
  *                      Rx PA3
  *
  **********************************************************************/
 
// Libreria principal del microcontrolador
#include "stm32f10x.h"		

// Las librerias para los perifericos
#include "stm32f10x_rcc.h"		
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"     

// Llamada a la libreria USART2 externa
#include "usart2.h"

// Libreria axuliar del Keil para el uso de la 
// instruccion 'memset'
#include <string.h>

// Creamos la variable 'MESSAGE_SIZE' con el valor 16 como
// maximo de caracteres a recibir en el buffer de comunicacion
#define MESSAGE_SIZE	16

// Creamos la variable indexada 'message' con un maximo de 
// de caracteres indicado en la variable 'MESSAGE_SIZE'
char message[MESSAGE_SIZE];

int i = 0;  // Cramos la variable numerica 'i' para el contador

int main(void)
{
	
  USART2_Config();    // Inicializamos el USART2
	
	while (1)
	{
		// Lee un caracter recibido en el USART2 y lo guarda
    // en la variable 'c'
		char c = USART2_GetChar();
		
		// Comprueba si no se ha recibido el caracter de fin de linea
		if (c != '\n')
		{
			// Concatenamos a la variable 'message' cada uno de los
            // caracteres recibidos 'c' hasta el maximo indicado
            // descontandose cada uno hasta llegar a 0
			if (i < MESSAGE_SIZE - 1)
			{
				message[i] = c;   
				i++;      
			}
			else    
			{
				message[i] = c;
				i = 0;
			}
		}
		// Si se ha recibido el caracter '\n' de fin de linea
    else
		{
			// Se reproduce por el mismo puerto USART2 el mensaje recibido
			USART2_PutString(message);
			USART2_PutChar('\n');       // Se envia el caracter de fin de linea '\n'
			
			// Reseteamos el 'buffer' con el borrado mediante el comando
            // 'memset' de la variable indexada 'message[]'
			memset(&message[0], 0, sizeof(message));
			i = 0;
		}
	}
}

/////////////////////  Fin del Programa   ///////////////////////

