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
#include "encoder.h"

// MACROS


// VARIABLES
bool reverse;
int32_t desiredEncoderCount;
int32_t controlInput;
float kP;


// METHODS

// Set pins for motor, set up timed loop timer and stuff like that
// Direction pins: B3 and A4
void initMotor(void){
    // TODO set direction and enable pins as output
    // TODO set up timer4 to run continously and overflow 1000 times per second or so.
    // TODO set up timer3 for PWM
    // Initialize variables such as error, positionTarget, etc.
    initEncoderB8B9(); // Initialize the encoder and all that comes with it
}

// Set the speed and direction of the motor.
// Range (-1000,1000)
void setMotorPWM(int16_t PWM){
    // TODO:
    // if (PWM > 0):
        // set direction pins one way
        // setT3C2PWM(PWM)
    // else if (PWM < 0):
        // set direction pins the other way
        // setT3C2PWM(-PWM)
    // else set direction pins both low

    

}

// Set the position target in counts
void setPositionTarget(int32_t target){
    desiredEncoderCount = target;
}

// Set the proportional gain
void setKp(float kP_new){
    kP = kP_new;
}

// TODO call this either with TIM4 OVF or 
// in the main loop using some millis() function
void updateMotorControl(void){
    // Calculate error
    error = getEncoderCount()-desiredEncoderCount;

    // Calculate control input
    controlInput = kP*error;

    // Clamp it
    if(controlInput > 1000){
        controlInput = 1000;
    }
    if(controlInput < -1000){
        controlInput = -1000;
    }

    // Apply to motor
    setMotorPWM(controlInput);

}

// TODO put the timer4 ISR here. Nobody else should be using timer4.

//
