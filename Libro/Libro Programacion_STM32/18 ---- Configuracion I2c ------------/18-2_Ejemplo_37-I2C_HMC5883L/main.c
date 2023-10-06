/**********************************************************************
  * Project Name: I2C_HMC5883L
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
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL PUERTO I2C 
  * 
  *  Nuestro código configura el bus I2C para que se conecte a un sensor
  *  HMC5883 del que leeremos los datos de sus registros
  *
  **********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_usart.h"     

#include "i2c.h"
#include "usart1.h"
#include <stdio.h>
#include <math.h>

#define HMC5883_Address		0x1E	// Direccion I2c del sensor

void HMC5883_Init(void);
void LeerRegistros_HMC5883(void);
void Delay(int nTime);

int16_t ejeX, ejeY, ejeZ;

char buf[8];

float const PI=3.1415926536;

int main(void)
{	  
  USART1_Config();
  printf("  PRUEBA DE HMC5883L  \r\n");
  
	// Initialize HMC5883L
	HMC5883_Init();
	Delay(4000);
	
	while (1)
	{
    LeerRegistros_HMC5883();		// Leemos los valores
	
    // Imprimimos los valores por el puerto USART1
    printf(" Valores de X,Y,Z = %3d  %3d  %3d\n", ejeX, ejeY, ejeZ);	
    
    // Calculo del angulo respecto al Norte geografico
    // para averiguar la declinación de vuestra zona en esta pagina:
    // https://www.ngdc.noaa.gov/geomag-web/
    // Mi valor de Declinacion = -0.30º
    double valorx, valory;   
	
    //declinacion en grados en tu posición geografica
    float angulo = atan2((double) ejeY, (double) ejeX);    
    angulo = angulo * (180.0 / PI);  // Pasamos los radianes a grados
    angulo = angulo - 0.30;          // Descontamos nuestra declinacion
    printf("     Angulo respecto Norte: %2.f %c \r\n", angulo, 'º');
    		
	Delay(6000);   // retraso 
	}
}

/*      Funcion que configura el Magnetonemtro HMC5883  */
//---------------------------------------------------------
void HMC5883_Init()
{
	// Inicializamos la comunicaion I2c
	I2C_Config();	
	// Establecemos la configurarion del sensor en modo muestras promedio/medicion
    // Configura el registro CRA 0x00 de Modo de medicion
	I2C_write_Reg(HMC5883_Address, 0x00, 0x70);
	
    // Configura el registro CRB 0x01 para la ganancia del campo magnetico
	I2C_write_Reg(HMC5883_Address, 0x01, 0xA0);
	
	// Configura el registro 0x02  para el modo Captura continuo
	I2C_write_Reg(HMC5883_Address, 0x02, 0x00);
}

/*      Funcion que lee los valores de los registros del HMC5883  */
//------------------------------------------------------------------
void LeerRegistros_HMC5883()
{
	// Leemos los valores de los registros del HMC5883
	uint8_t valorHMC5883[6];    // matriz para guardar los datos
  
    // Indicamos al HMC5883L dónde empezar a leer datos
    // Leer en Address-0x1E , 0x03, ValorLeido
	I2C_read_multiValReg(HMC5883_Address, 0x03, 6, valorHMC5883);
		
    // Cargamos valores MSB y LSB del valor leido eje X
	ejeX = valorHMC5883[0] << 8;
	ejeX |= valorHMC5883[1];
		
    // Cargamos valores MSB y LSB del valor leido eje Z
	ejeZ = valorHMC5883[2] << 8;
	ejeZ |= valorHMC5883[3];
		
    // Cargamos valores MSB y LSB del valor leido eje Y
	ejeY = valorHMC5883[4] << 8;
	ejeY |= valorHMC5883[5];
}


/*   Funcion que configura un retraso        */
void Delay(int nTime)
{
     unsigned int i;
     unsigned long j;
     for(i = nTime;i > 0;i--)
          for(j = 1000;j > 0;j--);     
}

///////////////    Fin del Programa      ///////////////////

