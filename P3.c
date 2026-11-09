/**
  ******************************************************************************
  * @file    P3.c 
  * @author  Kevin Cotellesso
  * @version 0.6
  * @date    October 6, 2026
  * @brief   Performs position control on DC motor with encoder. Lab 5 Part 3
  ******************************************************************************
  */

// Pins to connect to the Nucleo:

// F7 - analog input (potentiometer wiper most likely)
// B3 - direction 1 output
// A4 - direction 2 output
// B5 - PWM / enable pin

// EXTI9-5 is its own interrupt handler. Use B9 and B8 for interrupts

  
#include "P2.h"

#include "stm32f446xx_gpio_mort.h"
#include "stm32f446xx_timer_mort.h"
#include "stm32f446xx_adc_mort.h"
#include "stm32f446xx_dma_controller_mort.h"
#include "stm32f446xx_system_mort.h"
#include "stm32f446xx_usart3_mort.h" //always include usart where you want to do communications
#include "stupid_delay.h"



void P2 (void)
{
    /* Variables*/
    /* Initializations */
    initUSART3(); //always init USART if you want communication!
    // Set up F7,8,9 as analog
    initADC3_567_withDMA(); // Initialize adc3 channel 5,6,7
    // Set up B8 and B9 as output
    initGpioAsOutput(PORTB, 3);
    initGpioAsOutput(PORTA, 4);
    // Set up PWM on T3C2 and enable the connected alternate function pin
    initT3C2PWM();
    initGpioAsAF2(PORTB, 5, NOPULL);
    delay(80000); //give some time for USART to come alive.
    transmitLineUSART3("USART3 is alive!");
    delay(800000); //give some time for ADC to wake up from deeeep slumber
        startADCConversion(); // Turn on the ADC (SWSTART)
    uint32_t adcValue; //uint32_t so we can multiply it by 1000 later


    // Set motor direction
    setGpioPin(PORTB, 8);
    clearGpioPin(PORTB, 9);
    while(1)
    {
        // Nab the ADC value
        adcValue = returnADC3StoredValue(0); // Pin F7

        // Convert the 0-4095 to 0-1000 for PWM function
        adcValue = (adcValue*1000)/4095; // This order works best for precision

        // Apply this (0-1000) value directly to the pwm
        // setT3C2PWM(adcValue);
        setT3C2PWM(adcValue);


    }

}