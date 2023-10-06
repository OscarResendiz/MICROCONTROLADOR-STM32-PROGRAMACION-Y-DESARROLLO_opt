/**********************************************************************
  * Project Name: SPI_MAX7912
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
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL PUERTO SPI
  * 
  *  Programa de ejemplo de programacion del bus SPI en la que 
  *  conectamos nuestra placa a una matriz de 8x8 led, controlado
  *  por un integrado MAX7219 que se comunica por SPI1
  
  **********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"          
#include "stm32f10x_rcc.h"           
#include "stm32f10x_spi.h"           

// Listado de caracteres
// https://xantorohara.github.io/led-matrix-editor/
 unsigned char caracter[38][8]={
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},  //0
{0x10,0x18,0x14,0x10,0x10,0x10,0x10,0x10},  //1
{0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},    //2
{0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},       //3
{0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},      //4
{0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},     //5
{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},   //6
{0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},        //7
{0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},   //8
{0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},     //9
{0x8,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},   //A
{0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x0},   //B
{0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x0},   //C
{0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x0},   //D
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},  //E
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},  //F
{0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},  //G
{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},  //H
{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},  //I
{0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30},       //J
{0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24},   //K
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},  //L
{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},  //M
{0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0},    //N
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},  //O
{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},  //P
{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},  //Q
{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},  //R
{0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C},     //S
{0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8},         //T
{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},  //U
{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},  //V
{0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0},    //W
{0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41},    //X
{0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8},       //Y
{0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F},      //Z
};

// Modulo que genera retardo 
//--------------------------------------
void delay_us(unsigned int nCount){ 
		unsigned int i, j;
		for(i = 0; i < nCount; i++)
		{  
				for(j = 0; j < 0x2AFF; j++){;} 
		}
} 

/*  Funcion que inicializa el Bus SPI                          */
//----------------------------------------------------------------
void SPI1_Init(void)	
{
	SPI_InitTypeDef  SPI1_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	// Configuramos los reloj de los modulos
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_SPI1, ENABLE);
    // Pin pa4 (CS PA4)----------------------------------------------
    GPIO_InitStructure.GPIO_Pin       = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed     = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);	
  
	// Pines de PA5(SCK),  PA6 (MISO)   y   PA7 (MOSI)---------------
    GPIO_InitStructure.GPIO_Pin       = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	 
	// Inicializamos el bus ISP (1)----------------------------------  
    SPI1_InitStructure.SPI_Direction          = SPI_Direction_2Lines_FullDuplex;
	SPI1_InitStructure.SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_64;
	SPI1_InitStructure.SPI_DataSize           = SPI_DataSize_8b;
	SPI1_InitStructure.SPI_Mode               = SPI_Mode_Master;
	SPI1_InitStructure.SPI_FirstBit           = SPI_FirstBit_MSB; 
	SPI1_InitStructure.SPI_CPOL               = SPI_CPOL_High;	
	SPI1_InitStructure.SPI_CPHA               = SPI_CPHA_2Edge;
    SPI1_InitStructure.SPI_NSS                = SPI_NSS_Soft;
	SPI1_InitStructure.SPI_CRCPolynomial      = 7;
	
	SPI_I2S_DeInit(SPI1);
	
	SPI_Init(SPI1, &SPI1_InitStructure); 
	
	SPI_Cmd(SPI1, ENABLE);   
}


/*     Funcion que escribe un byte en el bus ISP             */
//-------------------------------------------------------------
void Send_Max7219(unsigned char address,unsigned char data)
{			
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);		
        
    SPI_I2S_SendData(SPI1, address);  //Escribir la dirección del registro
    delay_us(10);
    		    
    SPI_I2S_SendData(SPI1, data);    	//Escribir datos 
    delay_us(10);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_4);	
}


/*      Funcion que inicializa los registros del MAX7219      */
//--------------------------------------------------------------
void Init_MAX7219(void)
{  /*  Registros de control: -----------------------------------
     0x9 -- Registro de modo de codificacion (Decode Mode)
     0xA -- Registro de brillo de pantalla (Intensity)
     0xB -- Registro de límite de escaneo (Scan Limit)
     0xC -- Registro de modo de apagado (Shutdwn)
     0xF -- Registro de detección de pantalla (Display Test)
   ------------------------------------------------------------*/
	//Send_Max7219(0x09, 0xFF); // Método de decodificación BCD
  Send_Max7219(0x09, 0x00); // Método de decodificación: desabilita BCD
  Send_Max7219(0x0A, 0x03); // Luminosidad
  Send_Max7219(0x0B, 0x07); // Limite de escaneo; Pantalla de 8 LEDs
  Send_Max7219(0x0C, 0x01); // Modo de apagado: 0, modo normal 1
  Send_Max7219(0x0F, 0x00); // Test de pantalla: 1 
}

/*            Modulo Principal                                 */
//---------------------------------------------------------------
int main(void)
 { 	 
	 SPI1_Init();     // Inicializamos el bus ISP
	 
	 delay_us(800);    // Retardo para que se reinicie
   
	 Init_MAX7219();  // Enviamos la secuencia de configuracion del MAX7219
   
     delay_us(200);    // Retardo para que se reinicie
   
     
   while(1)  // Bucle infinito para que se repita automaticamente
   {
     
      // Bucle que va mostrando los caracteres predefinidos
      for(int NLetra=0; NLetra<38; NLetra++)
      {
        // Selecciona cada registro de la columna a escribir
        for(int NRow=1; NRow<9; NRow++)
        Send_Max7219(NRow, caracter[NLetra][NRow-1]);
        delay_us(500);
      } 	  
  }    
}


//////////////  Fin del Programa //////////////////////////////////////////
