/**********************************************************************
  * Project Name: I2C_HMC5883L
  * File Name: i2c.c
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
#include "i2c.h"

// Lista de funciones
void I2C_Write_addrss(uint8_t address, uint8_t direction);
uint8_t I2C_readVal(void);
uint8_t I2C_readVal_NAck(void);

void I2C_Config()
{
   GPIO_InitTypeDef GPIO_InitStructure;
   I2C_InitTypeDef  I2C_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
   RCC_APB1PeriphClockCmd(I2Cx == I2C1 ? RCC_APB1Periph_I2C1 : 
                                         RCC_APB1Periph_I2C2, ENABLE);
										 
   /* Configuracion de GPIO para SCL y SDA--------------------------*/
   if (I2Cx == I2C1)
   {//  ----------------------------     SCL_Pin      SDA_Pin       
      GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6 | GPIO_Pin_7;
   } else {
      GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10 | GPIO_Pin_11;
   }
   
   GPIO_InitStructure.GPIO_Speed      = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode       = GPIO_Mode_AF_OD;
   GPIO_Init(GPIOB, &GPIO_InitStructure);   

   /* Configuracion de parametros del I2C_1	-------------------------*/
   I2C_InitStructure.I2C_Mode         = I2C_Mode_I2C;
   I2C_InitStructure.I2C_DutyCycle    = I2C_DutyCycle_2;
   I2C_InitStructure.I2C_Ack          = I2C_Ack_Enable;   
   I2C_InitStructure.I2C_OwnAddress1  = 0x00;
   I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
   I2C_InitStructure.I2C_ClockSpeed   = 100000;	 
		  
   I2C_Init(I2Cx,&I2C_InitStructure);
   I2C_Cmd(I2Cx,ENABLE);    
}

/*  Funcion que graba un valor en el registro del dispotivo */
void i2C_single_Write(uint8_t address, uint8_t data)
{
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
	I2C_Write_addrss(address << 1, I2C_Direction_Transmitter);
	
    I2C_SendData(I2Cx, (uint8_t) data);	
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

/*  Funcion que graba un dato en el disposito indicando el registro  */
//---------------------------------------------------------------------
void I2C_write_Reg(uint8_t address, uint8_t reg, uint8_t data)
{
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
	I2C_Write_addrss(address << 1, I2C_Direction_Transmitter);
	
    I2C_SendData(I2Cx, (uint8_t) reg);	
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
	I2C_SendData(I2Cx, (uint8_t) data);	
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

/* Funcion que escribe multiples valores en el dispositivo */
//-------------------------------------------------------------
void i2c_write_multi_no_reg(uint8_t address, uint8_t* data, uint8_t len)
{
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
	I2C_Write_addrss(address << 1, I2C_Direction_Transmitter);
	for (int i = 0; i < len; i++)
	{
		I2C_SendData(I2Cx, (uint8_t) data[i]);	
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));      
	}
  
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

/* Funcion que escribe multiples valores en una dirección de registro del dispositivo */
//--------------------------------------------------------------------------------------
void i2c_write_multi_with_reg(uint8_t address, uint8_t reg, uint8_t* data, uint8_t len)
{
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
	I2C_Write_addrss(address << 1, I2C_Direction_Transmitter);
	
    I2C_SendData(I2Cx, (uint8_t) reg);	
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));      
  
	for (int i = 0; i < len; i++)
	{		
    I2C_SendData(I2Cx, (uint8_t) data[i]);	
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));       
	}
  
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

/* Funcion que lee un valor del dispostivo   */
//----------------------------------------------
void i2c_read_no_reg(uint8_t address, uint8_t* data)
{
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
	I2C_Write_addrss(address << 1, I2C_Direction_Receiver);
	*data = I2C_readVal_NAck();
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

/* Funcion que lee un valor de un registro del dispositivo       */
//-----------------------------------------------------------------
void i2c_read_with_reg(uint8_t address, uint8_t reg, uint8_t* data)
{
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
  
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
	I2C_Write_addrss(address << 1, I2C_Direction_Transmitter);
	
    I2C_SendData(I2Cx, (uint8_t) reg);	
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));      
 
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
  
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Write_addrss(address << 1, I2C_Direction_Receiver);
	*data = I2C_readVal_NAck();
  
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

/*    Funcion que lee varios valores del dispositivo    */
//--------------------------------------------------------------------
void I2C_read_multiVal(uint8_t address, uint8_t len, uint8_t* data)
{	
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
	I2C_Write_addrss(address << 1, I2C_Direction_Receiver);
	for (int i = 0; i < len; i++)
	{
		if (i == (len - 1))
		{
			data[i] = I2C_readVal_NAck();
		}
		else
		{
			data[i] = I2C_readVal();
		}
	}
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

/*   Funcion que lee varios valores de diferentes registros del dispositivo   */
//---------------------------------------------------------------------------
void I2C_read_multiValReg(uint8_t address, uint8_t reg, uint8_t len, uint8_t* data)
{	
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
	I2C_Write_addrss(address << 1, I2C_Direction_Transmitter);
	
    I2C_SendData(I2Cx, (uint8_t) reg);	
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));      
  
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
  
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
	I2C_Write_addrss(address << 1, I2C_Direction_Receiver);
	for (int i = 0; i < len; i++)
	{
		if (i == (len - 1))
		{
			data[i] = I2C_readVal_NAck();
		}
		else
		{
			data[i] = I2C_readVal();
		}
	}
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

/*   Funcion que escribe un valor en el dispositivo      */
//----------------------------------------------------
void I2C_Write_addrss(uint8_t address, uint8_t direction)
{	
	I2C_Send7bitAddress(I2Cx, address, direction);
	if (direction == I2C_Direction_Transmitter)
	{
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if (direction == I2C_Direction_Receiver)
	{	
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

/*   Funcion que recibe un byte desde dispositivo tras un ACK del maestro  */
//-------------------------------------------------------------------------
uint8_t I2C_readVal()
{	
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
	return I2C_ReceiveData(I2Cx);
}

/* Funcion que lee el valor enviado por el dispositivo  */
//---------------------------------------------------------
uint8_t I2C_readVal_NAck()
{	
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
	return I2C_ReceiveData(I2Cx);
}


/////////////////////////////////////////////////////////////////////
