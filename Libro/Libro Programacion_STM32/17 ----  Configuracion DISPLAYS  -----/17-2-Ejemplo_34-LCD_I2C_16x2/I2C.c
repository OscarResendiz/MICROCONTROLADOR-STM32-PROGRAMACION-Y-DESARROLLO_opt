/**********************************************************************
  * Project Name: LCD_I2C_16x2
  * File Name: I2c.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Select in "Options for Target" the option
  *           (x) Use MicroLib
  * =====================================================================
  * 
  * PROGRAMA DE EJEMPLO DE UITLIZACION DE UN PANTALLA LCD CON EL
  * ADAPTADOR DE I2C
  * 
  *  Programa de ejemplo de la libreria "LiquidCrystal_I2C.c" con
  * el adaptador I2c en una pantalla LCD de 16x2
  *
  **********************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de los perifericos
#include "stm32f10x_gpio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"

#include "delay.h"
#include "I2C.h"

/*      Funcion que inicializa el GPIO y puerto I2C_1     */
//-----------------------------------------------------------
void I2C1_LCD16x2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;  
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    // Configuracion de GPIO para SCL y SDA
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Configuracion de parametros del I2C_1	
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;    
    I2C_InitStructure.I2C_OwnAddress1 = 0x15;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}

/*******************************************************************/
void I2C_StartTransmission(I2C_TypeDef* I2Cx, uint8_t transmissionDirection,  uint8_t slaveAddress)
{
    /* Esperamos a que el bus I2c este operativo*/
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
    
    /* Establecemos condicion de START */
    I2C_GenerateSTART(I2Cx, ENABLE);
    
    /* Comprobamos si el registro EV5 confirma el bus libre */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    
    /* Enviamos direccion del dispotivo 'esclavo'              */     
    I2C_Send7bitAddress(I2Cx, slaveAddress<<1, transmissionDirection);
  
     /* Comprobamos que el dispositivo 'esclavo' confirma la comunicacion */   
    if(transmissionDirection == I2C_Direction_Transmitter)
    {
    	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    /* Comprobamos que el dispositivo 'esclavo' confirma la comunicacion */
    if(transmissionDirection == I2C_Direction_Receiver)
    {
  	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data)
{
    /* Enviamos el dato del comando para la pantalla             */   
    I2C_SendData(I2Cx, data);
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t I2C_ReadData(I2C_TypeDef* I2Cx)
{
    /* Esperamos a que se genere el evento EV7 para leer los datos */  
    while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    uint8_t data;
  
    /* Leemos un byte de la pantalla */
    data = I2C_ReceiveData(I2Cx);
  
    return data;
}

//////////////////////  Fin del Programa //////////////////////////////////////

