/**********************************************************************
  * Project Name: ISP_polling
  * File Name: Arduino_Slave_ISP.ino
  * Revision:
  * IDE de Arduino v.1.8.3
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * =====================================================================
  * 
  * PROGRAMA DE EJEMPLO DE PROGRAMACION DEL PUERTO ISP
  * 
  *  Configuramos una placa Arduino como esclavo en una comunicaion ISP
  *  con una placa STM32.
  *
  **********************************************************************/

// Conectamos un LED en el Pin 9 que se encenderá de acuerdo
// a las ordenes transmitidas desde la placa master STM32
//----------------------------------------------------------
#define LED_PIN  9
volatile uint8_t led_blink = 0;

/*   Funcion que configura la IRQ del ISP
 *   que detectara cuando se recibe un mensaje desde la
 *   placa SMT32                                          */
 //---------------------------------------------------------
ISR(SPI_STC_vect)
{
    // Leemos el mensaje recibido y lo guardamos en la variable
    uint8_t data_byte = SPDR;

    // Establecemos una seleccion de acuerdo al mensaje recibido
    switch (data_byte)
    {
        case '0':
            led_blink = 0;
            SPDR = 0;  
        break;
        case '1':
            led_blink = 1;
            SPDR = 0;  
        break;
        case '?':
            // Place LED blinking status in SPDR register for next transfer
            SPDR = led_blink;  
        break;
    }  
}

void setup()
{    
    pinMode(LED_PIN, OUTPUT);   // Establecemos el LED_PIN como Salida
    pinMode(MISO, OUTPUT);      // Establecemos la señal MISO, como Salida
    
    SPCR |= (1 << SPE);         // Desabilitamos el modo Slave del bus ISP
    SPCR |= (1 << SPIE);        // Habilitamos la interrupcion ISP
}

void loop() 
{   
    // Establecemos el estado del LED según la seleccion
    if (led_blink == 1)
    {
        digitalWrite(LED_PIN, HIGH);
        delay(250);     // Parpadeo cada 250 ms
        digitalWrite(LED_PIN, LOW);
        delay(250);  
    }
    else if (led_blink == 0)
    {
        digitalWrite(LED_PIN, LOW);   // Apagamos el LED
    }
}

///////////////////  Fin del Programa    //////////////////////////
