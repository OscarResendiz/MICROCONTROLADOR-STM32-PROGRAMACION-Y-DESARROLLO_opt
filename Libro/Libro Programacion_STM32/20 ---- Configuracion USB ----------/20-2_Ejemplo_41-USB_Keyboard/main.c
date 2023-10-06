/**********************************************************************
  * Project Name: USB_keyboard
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
  *  En el programa, una vez cargado en nuestra placa, al conectarlo
  *  en un puerto USB de nuestro ordenador simulará el funcionamiento
  *  de un ratón y un teclado, produciendo el movimiento del cursor en
  *  nuestra pantalla y la escritura continua de una frase en un programa
  *  de edición de texto que tengamos abierto.
  *
  **********************************************************************/
// Libreria principal del micro
#include "stm32f10x.h"

// Librerias de getion del puerto USB
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "keycodes.h"

__IO uint8_t PrevXferComplete = 1;

extern uint8_t key_buf[9];

void MOUSE_move(int8_t x, int8_t y){
	/*
	 * buf[0]: 1 - report ID
	 * buf[1]: bit2 - middle button, bit1 - right, bit0 - left
	 * buf[2]: move X
	 * buf[3]: move Y
	 * buf[4]: wheel
	 */
	uint8_t buf[5] = {1,0,0,0,0};
	buf[2] = x; buf[3] = y;
	USB_SIL_Write(EP1_IN, buf, 5);
	PrevXferComplete = 0;
	SetEPTxValid(ENDP1);
}

void KEYBOARD_SEND_key_buf(void) {
    USB_SIL_Write(EP1_IN, key_buf, 9);
    PrevXferComplete = 0;
    SetEPTxValid(ENDP1);
    while (PrevXferComplete == 0)
    {}
}

void KEYBOARD_SEND_Char(char ch) {
    press_key(ch);
    KEYBOARD_SEND_key_buf();
    release_key();
    KEYBOARD_SEND_key_buf();
}

void KEYBOARD_SEND_word(char *wrd){
	do {
		KEYBOARD_SEND_Char(*wrd);
	} while(*(++wrd));
}

void SetSysClockTo72(void)
{
	ErrorStatus HSEStartUpStatus;
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
    	//FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);

        /* Flash 2 wait state */
        //FLASH_SetLatency( FLASH_Latency_2);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);

        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config( RCC_HCLK_Div2);

        /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLConfig(0x00010000, RCC_PLLMul_9);

        /* Enable PLL */
        RCC_PLLCmd( ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
    else
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
     User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1)
        {
        }
    }
}

/*   Modulo principal                       */
//--------------------------------------------
int main(void)
{
  Set_System();         // Inicializa la configuracion de perifericos
  SetSysClockTo72();    // Configura el reloj del sistema
  USB_Interrupts_Config();  // Crea la interrupcion USB
  Set_USBClock();       // Establece el reloj del USB
  USB_Init();           // Inicializa el modulo USB

  while (1)
  {
    // Comprueba si el USB esta operativo
    if (bDeviceState == CONFIGURED)
    {
      
      if (PrevXferComplete)
      {
        // Reproduce la pulsacion de una frase con el teclado
    	  KEYBOARD_SEND_word("HOla desde STM32!!!");
    	  MOUSE_move(1,-1);  // Reproduce un movimiento de raton
    
      }
    }
  }
}

