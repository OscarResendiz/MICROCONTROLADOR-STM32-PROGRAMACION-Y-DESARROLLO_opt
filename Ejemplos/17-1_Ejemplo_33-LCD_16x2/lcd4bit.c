 /**********************************************************************
  * Project Name: LCD_16x2
  * File Name: lcd4bit.c
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
  * PROGRAMA DE EJEMPLO DE LIBRERIA DEL LCD 16x2
  *   
  * Ejemplo del empleo de la libreria para el manejo de una
  * pantalla LCD.
  *
  **********************************************************************/

// Libreria principal del micro
#include "stm32f10x.h"

#include "lcd4bit.h"
#include "delay.h"

/*   Caracter definido por usuario para cargar en la memoria CGRAM del LCD */
const char UserFont[8][8] = 
{
{ 0x11,0x0A,0x04,0x1B,0x11,0x11,0x11,0x0E },
{ 0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10 },
{ 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18 },
{ 0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C },
{ 0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E },
{ 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }
};

/*    Funcion que inicializa un LCD a 4 Bits   */
//-----------------------------------------------
void LCD_Init(void)
{
  int i;
  char const *p;
  
  // Configuracion de los pines de salida hacia el LCD 
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_EN, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = LCD_EN_PIN;
  GPIO_Init(LCD_EN_PORT, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_RW, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = LCD_RW_PIN;
  GPIO_Init(LCD_RW_PORT, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_RS, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
  GPIO_Init(LCD_RS_PORT, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DATA, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = LCD_D4_PIN |
                                LCD_D5_PIN |
                                LCD_D6_PIN |
                                LCD_D7_PIN;
  GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);

  delay_init(72);

  LCD_D4_HI();
  LCD_D5_HI();
  LCD_D6_LO();
  LCD_D7_LO();
  delay_ms(15);  // Delay de 15 ms

  LCD_D4_HI();
  LCD_D5_HI();
  LCD_D6_LO();
  LCD_D7_LO();
  LCD_pulse_EN(); 
  delay_us(4100); // Delay de 4.1 ms

  LCD_D4_HI();
  LCD_D5_HI();
  LCD_D6_LO();
  LCD_D7_LO();
  LCD_pulse_EN(); 
  delay_us(100); // Delay de 100 us

  LCD_D4_HI();
  LCD_D5_HI();
  LCD_D6_LO();
  LCD_D7_LO();
  LCD_pulse_EN(); 

  while(LCD_Busy()); // Espera hasta que el LCD este operativo
  LCD_D4_LO();
  LCD_D5_HI();
  LCD_D6_LO();
  LCD_D7_LO();
  LCD_pulse_EN(); 
  
  while(LCD_Busy()); // Espera hasta que se complete 
  LCD_Write_Cmd(0x28); // Establecemos LCD como (Datos 4-Bit, N_lineas=2, Font=0 5X7)
  LCD_Write_Cmd(0x0C); // Enciende el LCD sin cursor.
  LCD_Write_Cmd(0x06); // Inicializa cursor 

  /* Cargamos el caracter definidos por usuario en la CGRRAM */
  LCD_Write_Cmd(0x40); /* Establece la direccion CGRAM desde 0 */
  p = &UserFont[0][0];
  for (i = 0; i < sizeof(UserFont); i++, p++)
  LCD_Put_Char(*p);

  LCD_Write_Cmd(0x80); 
}

/*  Funcion que genera un Strobe en el LCD   */
//---------------------------------------------
void LCD_Out_Data4(unsigned char val)
{ 
  if((val&0x01)==0x01) // Bit[0] 
  {
    LCD_D4_HI(); 
  } 
  else
  {
    LCD_D4_LO();
  }

  if((val&0x02)==0x02) // Bit[1] 
  {
    LCD_D5_HI(); 
  } 
  else
  {
    LCD_D5_LO();
  }

  if((val&0x04)==0x04) // Bit[2] 
  {
    LCD_D6_HI(); 
  } 
  else
  {
    LCD_D6_LO();
  } 

  if((val&0x08)==0x08) // Bit[3]
  {
    LCD_D7_HI(); 
  } 
  else
  {
    LCD_D7_LO();
  } 

}

/*  FUncion que escribe 1 byte de datos en el LCD  */
//---------------------------------------------------
void LCD_Write_Byte(unsigned char val)
{ 
  LCD_Out_Data4((val>>4)&0x0F); 
  LCD_pulse_EN(); 

  LCD_Out_Data4(val&0x0F); 
  LCD_pulse_EN(); 

  while(LCD_Busy()); 
}

/*  Funcionlque escribe un comando en el LCD   */
//-----------------------------------------------
void LCD_Write_Cmd(unsigned char val)
{ 
  LCD_RS_LO();         // pin RS = 0 (Seleccion comando)
  LCD_Write_Byte(val); 
}

/*  Escribe un Caracter (ASCII) en el LCD      */
//-----------------------------------------------
void LCD_Put_Char(unsigned char c)
{ 
  LCD_RS_HI();        // pin RS = 1 (Seleccion caracteres)
  LCD_Write_Byte(c); 
}

/*   Funcion que establece el cursor en una posicion de la pantalla LCD */
//------------------------------------------------------------------------
void LCD_Set_Cursor(unsigned char line, unsigned char column)
{
  unsigned char address;
  column--;line--;
  address = (line * 40) + column;
  address = 0x80 + (address & 0x7F);
  LCD_Write_Cmd(address); 
}

/*  Funcion que envia una cadena de caracteres (ASCII) al LCD  */
//---------------------------------------------------------------
void LCD_Put_Str(char* str)
{
  int i;

  for (i=0;i<16 && str[i]!=0;i++) 
  {
    LCD_Put_Char(str[i]);    // Envia un Byte al LCD
  }
}

/*   Funcion que envia un caracter numerico al LCD  */
//----------------------------------------------------
void LCD_Put_Num(int num)
{
  int i,j;
  int p,f=0;
  char ch[5];
  for(i=0;i<5;i++)
  {
    p=1;
    for(j=4-i;j>0;j--)
      p = p*10;
      ch[i] = (num/p);
      if (num>=p && !f){
        f=1;
      }
   num =num - ch[i] *p ;
   ch[i] = ch[i] +48;
   if(f) LCD_Put_Char(ch[i]);
  }
}

/*  Funcion que provoca tiempos de espera en el LCD */
//----------------------------------------------------
char LCD_Busy(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = LCD_D7_PIN;
  GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);

  LCD_RS_LO(); 
  LCD_RW_HI(); 
  LCD_EN_HI(); 

  delay_us(100); 
  if (GPIO_ReadInputDataBit(LCD_DATA_PORT, LCD_D7_PIN) == Bit_SET)
  {
    LCD_EN_LO(); 
    LCD_RW_LO(); 

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = LCD_D7_PIN;
    GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);
  
    return 1; 
  }
  else
  {
    LCD_EN_LO(); 
    LCD_RW_LO(); 

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = LCD_D7_PIN;
    GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);
    
    return 0; 
  } 
}

/*  Funcion que genera un pulso en el pin EN del LCD  */
void LCD_pulse_EN(void) 
{ 
  delay_init(72);
  LCD_EN_HI(); // Habilita pin EN ON
  delay_us(50); 
  LCD_EN_LO(); // Habilita pin EN Off
}

/*   Funcion que muestra un caracter gráfico en el LCD   
     en 'value' el valor en su posicion CGRAM y en
     'size' especificamos su tamaño                      */
//----------------------------------------------------------
void LCD_BarGraphic (int value, int size) 
{
  int i;

  value = value * size / 20; /* Matriz de 5 x 8 pixels */
  for (i = 0; i < size; i++) 
  {
    if (value > 5) {
      LCD_Put_Char (0x05);
      value -= 5;
    }
  else {
    LCD_Put_Char (value);
    
    break;
  } 
  }
}

/*    Funcion que muestra un caracter grafico en la pantalla LCD
      especificando la posicion pos_x horizontal de inicio y la
      posicion pos_y vertical de la pantalla LCD                 */
//-----------------------------------------------------------------
void LCD_BarGraphicXY (int pos_x, int pos_y, int value) 
{
  int i;

  LCD_Set_Cursor(pos_x, pos_y);
  for (i = 0; i < 16; i++) 
  {
    if (value > 5) {
      LCD_Put_Char (0x05);
      value -= 5;
    } else {
      LCD_Put_Char (value);
      while (i++ < 16) LCD_Put_Char (0);
    }
  }
}



////////////////    Fin del Programa ////////////////////////////
