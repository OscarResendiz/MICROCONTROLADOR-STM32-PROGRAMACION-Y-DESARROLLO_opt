#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
//llamada a la libreria externa
#include "usart2.h"
#include <string.h>
//creamos la variable MESSAGE_SIZE con el valor 16 como maximo
//de caracteres a recibir el en buffer de comunicacion
#define MESSAGE_SIZE 16
//creamos la variable indexada message con un maximo de caracteres indicado en MESSAGE_SIZE
char message[MESSAGE_SIZE];
//creamos la variable numerica i para el contador
int i=0;
//funcion que configura el USART2
int main(void)
{
	//inicializamos el puerto USART2
	USART2_Config();
    while(1)
    {
    	// lee un caracter recibido en el USART2 y lo guarda el la variable c
    	char c=USART2_GetChar();
    	//comprueba si no es el fin de linea
    	if(c!='\n'&&c!='\r')
    	{
    		//concatenamos a la variable message cada uno de los caracteres hasta el maximo indicado
    		// descontando 1 hasta llegar a 0
    		if(i<MESSAGE_SIZE-1)
    		{
    			message[i]=c;
    			i++;
    		}
    		else
    		{
    			message[i]=c;
    			i=0;
    		}
    	}
    	else
    	{
    		//se ha recibo el caracter de fin de linea
    		// se envia por el mismo puerto USAR2 el mensaje recibido
    		USART2_PutString(message);
    		USART2_PutChar('\r');//se envia el fin de linea
    		USART2_PutChar('\n');//se envia el fin de linea
    		//reseteamos el buffer con el borrado mediante el comando memset de la variable indexada message
    		memset(&message[0],0,sizeof(message));
    		i=0;
    	}
    }
}

