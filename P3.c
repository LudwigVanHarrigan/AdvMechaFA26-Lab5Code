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

  
#include "P3.h"

#include "stm32f446xx_gpio_mort.h"
#include "stm32f446xx_timer_mort.h"
#include "stm32f446xx_adc_mort.h"
#include "stm32f446xx_dma_controller_mort.h"
#include "stm32f446xx_system_mort.h"
#include "stm32f446xx_usart3_mort.h" //always include usart where you want to do communications
#include "stupid_delay.h"
#include "encoder.h"
#include "motor_control.h"



void P3 (void)
{
    /* Variables*/
    /* Initializations */
    initUSART3(); //always init USART if you want communication!

    // Initialize encoder stuffs
    // initEncoderInterruptsB8B9();

    // Initialize pushbutton
    initGpioAsInput(PORTD, 1, PULLUP);

    // Initialize the motor controller (and by corollary the encoder)
    initMotorPosControl();

    // Set the proportional gain
    setKp(50.0f);
    setKd(1.1f);

    delay(80000); //give some time for USART to come alive.
    transmitLineUSART3("USART3 is alive!");
    delay(800000); //Vestigal ADC wait time
    enableController(); // Start controlling with P controller


    // State tracking
    typedef enum {
        POS1,
        POS1_db,
        POS2,
        POS2_db,
    }state_t;
    state_t state = POS1;


    while(1)
    {
        delay(80000);
        // Very very simple state machine. Runs slowly(tm)
        // for "dumb debounce"
        switch (state){
            case POS1:
                // position target is 0.

                // Trigger: button is pressed
                if(checkGpioPin(PORTD, 1) == 0){
                    // Go to debounce state
                    state = POS1_db;
                }

            break;
            case POS1_db:  
                // Debounce button.

                // Trigger: button is released
                if(checkGpioPin(PORTD, 1) > 0){
                    // Go to POS2
                    state = POS2;
                    // Set position target to 90 degrees
                    //48*4.4 = 211.2 counts per revolution
                    // 211.2/4 = about 53
                    setPositionTarget(53);
                    transmitLineUSART3("Setting position to 90 degrees");

                }                
            break;
            case POS2:

                // Trigger: button is pressed
                if(checkGpioPin(PORTD, 1) == 0){
                    // Go to debounce state
                    state = POS2_db;
                }
                
            break;
            case POS2_db:
                // Trigger: button is released
                if(checkGpioPin(PORTD, 1) > 0){
                    // Go to POS2
                    state = POS1;
                    // Set position target to 90 degrees
                    setPositionTarget(0);
                    transmitLineUSART3("Setting position to 0 degrees");
                } 
            break;




        }


    }

}