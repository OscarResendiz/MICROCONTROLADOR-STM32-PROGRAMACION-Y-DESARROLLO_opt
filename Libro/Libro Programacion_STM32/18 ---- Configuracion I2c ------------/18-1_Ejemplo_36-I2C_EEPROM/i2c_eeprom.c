/**********************************************************************
  * Project Name: I2c_EEPROM
  * File Name: i2c_eeprom.c
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
  * Programa ejemplo de utilizacion del puerto I2C que
  * lee y escribe un byte en una memoria EEPROM y muestra
  * el valor leido por el puerto serie USART1
  *
  **********************************************************************/

#include "i2c_eeprom.h"

/*   Funcion que inicializa GPIO e I2C1    */
//-------------------------------------------
void I2C1_EE_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
   I2C_InitTypeDef I2C_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
   
   // Configuracion de GPIO para SCL y SDA
   GPIO_InitStructure.GPIO_Pin        = GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Speed      = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode       = GPIO_Mode_AF_OD;  
   GPIO_InitStructure.GPIO_Speed      = GPIO_Speed_50MHz;     
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	
   // Configuracion de parametros del I2C_1	
   I2C_DeInit(I2C1);
   I2C_InitStructure.I2C_Mode         = I2C_Mode_I2C;
   I2C_InitStructure.I2C_DutyCycle    = I2C_DutyCycle_2;
   I2C_InitStructure.I2C_OwnAddress1  = 0x00;
   I2C_InitStructure.I2C_Ack          = I2C_Ack_Enable;
   I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
   I2C_InitStructure.I2C_ClockSpeed   = 100000;
  	
   I2C_Init(I2C1, &I2C_InitStructure);
   I2C_Cmd(I2C1, ENABLE);	   
 
}

/*  Funcion que Lee un byte de la eeprom                       
    -----------------------------------------------------
Protocolo:

 Maestro - [START][ADDRES|R/W]          [ACK][STOP]    
 Esclavo -                    [ACK|DATA]                   */
 
//--------------------------------------------------------------
int I2C_eeprom_Read(I2C_TypeDef* I2Cx, uint8_t HWAddress, uint8_t WRAddress)
{
    // Creamos la variable que contendrá el dato leido
  	uint8_t DataByte = 0;
  
    /* Esperamos a que el bus I2c este operativo*/
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	
    /* Establecemos condicion de START */
    I2C_GenerateSTART(I2Cx, ENABLE);
        
    /* Comprobamos si el registro EV5 confirma el bus libre */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)); 
    
    /* Enviamos direccion del dispotivo 'esclavo'              */        
    I2C_Send7bitAddress(I2Cx, HWAddress<<1, I2C_Direction_Transmitter);    
    	
    /* Comprobamos que el dispositivo 'esclavo' confirma la comunicacion */   
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 
	   
    ///////////////////    
    /*  Borramos la bandera del registro EV6 para reiniciar el registro PE */
    //I2C_Cmd(I2C1, ENABLE);
    //////////////////////////
  
    /* Enviamos la direccion interna de la EEPROM para leer          */
    I2C_SendData(I2Cx, WRAddress);
    
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 
    
    /* Regeneramos una nueva condicion de START */
    I2C_GenerateSTART(I2Cx, ENABLE);
    
    /* Comprobamos si el registro EV5 confirma el bus libre */  
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)); 
        
    /* Enviamos direccion del dispositivo 'esclavo'                    */    
    I2C_Send7bitAddress(I2Cx, HWAddress<<1, I2C_Direction_Receiver);
    	
    /* Comprobamos que el dispositivo 'esclavo' confirma la comunicacion */
  	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /* Disabilitamos la señal de ACK (Acknowledgement) */
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    
    /* Esperamos a que se genere el evento EV7 para leer los datos */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
 
    /* Leemos un byte de la EEPROM */
  	DataByte = I2C_ReceiveData(I2Cx);
    
    /* Habilitar ACK para estar listo para otra recepción */    
    //I2C_AcknowledgeConfig(I2C1, ENABLE);
	 
    /* Establecemos condicion de STOP */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    
	return DataByte;	
}


/*  Funcion que escribe un byte en la eeprom              
   Protocolo:

   Maestro - [START][ADDRES|R/W]    [DATA]    [STOP]    
   Esclavo -                    [ACK]    [ACK]                           */

//-------------------------------------------------------------------------
int I2C_eeprom_Write(I2C_TypeDef* I2Cx, uint8_t HWAddress, uint8_t DataByte, uint8_t WRAddress)
{
     /* Esperamos a que el bus I2c este operativo*/
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
  
    /* Establecemos condicion de START                                   */
    I2C_GenerateSTART(I2Cx, ENABLE);
	
    /* Comprobamos si el registro EV5 confirma el bus libre              */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
      
    /* Enviamos la direccion de la EEPROM para escribir                  */        
    I2C_Send7bitAddress(I2C1,HWAddress<<1, I2C_Direction_Transmitter);
        	  
    /* Comprobamos si el registro EV6 en el que el dispositivo 'esclavo' confirma la comunicacion */   
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    /* Enviamos la direccion interna de la EEPROM para escribir          */    
    I2C_SendData(I2Cx, WRAddress);
      	
    /* Comprobamos que el dispositivo 'esclavo' confirma la comunicacion */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	  
    /* Enviamos un bytye de datos para escribir en la EEPROM             */
    I2C_SendData(I2Cx, DataByte);    
         	
    /* Comprobamos que el dispositivo 'esclavo' confirma la comunicacion */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
    /* Establecemos condicion de STOP                                    */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    
    return 0;
}
		
/////////////////////////  Fin del Programa  //////////////////////////////////

