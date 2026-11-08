/**
  ******************************************************************************
  * @file    encoder.c 
  * @author  
  * @version 
  * @date    
  * @brief   Contains the function to be called whenever a pulse is received.
  ******************************************************************************
  */
  
#include "stm32f446xx_gpio_mort.h"
// #include "hardware_stm_gpio.h"
#include "stupid_delay.h"
#include <stdbool.h>

// MACROS
// TODO #define the timer 2 prescale, auto reload register values
// so they are changable 
// Also #define COUNTS_PER_SECOND based on these

// alpha value for velocity smoothing
#define VELOCITY_SMOOTH_ALPHA 0.01

// VARIABLES
bool lastEncoder0Val = false;
bool lastEncoder1Val = false;
int32_t encoderCount;
int32_t velocity;
int32_t velocitySmooth;


// METHODS

// Note: we are using pins B8 and B9 for the interrupt pins
void initEncoderB8B9(){
    // TODO check the last part of lab 4 for set up interrupts
    // TODO set up Timer 2 for one shot with specified timeout (see lab4 gummy)

}
// TODO call this with EXTI9_5_IRQHandler
void ISR_B8B9Encoder(){
    // 1. compare lastEncoder0Val and current Encoder 0 val
    // (same for encoder 1)
    // to find direction. (logic tree switch case)

    // Based on direction:
        // 1. Increment or decrement position

        // 2. update velocity
        if(!isVelocityTimedOut()){ // If velocity timer is still running
            // Set velocity based on elapsed time
            // Update smoothed velocity
        }
        // 3. Reset and start encoder timer
        startEncoderTimer();

    // Finally, update lastEncoder0Val and lastEncoder1Val.
    // (boolean values for the encoder lines to check direction next loop)


} 


// Convenience functions
void getEncoder0Val(){
    // Check pin 8
    checkGpioPin(PORTB, 8); // requires gpio_mort
}
void getEncoder1Val(){
    // TODO Value of pin B9
}

uint32_t unsignedVelocityTicksPerSecond(){
    // Get timer 2 CNT value
    // 
    // Return COUNTS_PER_SECOND / TIM2_CNT
    // = encoder ticks per second
}

 // Zeroes encoder position and all velocity measurements. Also resets step timer.

void zeroEncoder(){
    encoderCount = 0;
    velocity = 0;
    velocitySmooth = 0;
    // TODO Reset and start encoder timer
}

bool isVelocityTimedOut(){
    // Check TIM2 OVF bit.
    return false;
}

void startEncoderTimer(){
    // TODO look at lab 4 gummy machine to figure out how to do this
    // FIRST stop and reset the timer
    // CEN bit??
    // Do we need to clear the overflow (OVF) bit?
}

// Getters
// Returns encoder count number
uint32_t getEncoderCount();{
    return encoderCount;
}
// Returns encoder velocity
uint32_t getEncoderVelocity(){
    return velocity;
}
uint32_t getSmoothEncoderVelocity(){
    return velocitySmooth;
}

//
