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
#include "stm32f446xx_timer_mort.h"
#include <stdbool.h>
#include "encoder.h"

// MACROS


// VARIABLES
// bool reverse;
int32_t desiredEncoderCount;
int32_t controlInput;
int32_t error;
int32_t derivative;
float kP = 0.0f;
float kD = 0.0f;
bool enabled;



// METHODS

// Set pins for motor, set up timed loop timer and stuff like that
// Direction pins: B3 and A4
void initMotorPosControl(void){
    
    // Set direction pins as output
    initGpioAsOutput(PORTB, 3);
    initGpioAsOutput(PORTA, 4);
    
    // Set up timer4 to run continously and overflow 1000 times per second or so.
                                    // Start with 90MHz
    uint16_t prescale   = 900;      // Pop down to 100Khz
    uint16_t autoreload = 100;      // Reloads at 1000hz
    uint16_t cc3r       = 50;       // Also happens at 1000hz but a different 1000hz
    bool onePulseMode   = false;    // That shit run continuously fam
    initT4C3OutputCompareInterrupt(prescale, autoreload, cc3r, onePulseMode);
    
    // Set up timer3 for PWM and enable the connected alternate function pin
    initT3C2PWM();
    initGpioAsAF2(PORTB, 5, NOPULL);

    // Initialize variables such as error, positionTarget, etc.
    desiredEncoderCount = 0;

    initEncoderInterruptsB8B9(); // Initialize the encoder and all that comes with it
}

// Set the speed and direction of the motor.
// Range (-1000,1000)
void setMotorPWM(int16_t PWM){
    
    // Intelligently run the motor one way or the other depending on PWM value sign
    if(PWM > 0){
        // Direction 1
        setGpioPin(PORTB, 3);
        clearGpioPin(PORTA, 4);
        setT3C2PWM(PWM);
    } else if(PWM < 0){
        // Direction 2
        setGpioPin(PORTA, 4);
        clearGpioPin(PORTB, 3);
        setT3C2PWM(-PWM);
    } else {
        // Stopped
        setT3C2PWM(0);
        clearGpioPin(PORTA, 4);
        clearGpioPin(PORTB, 3);
    }
}

// Set the position target in counts
void setPositionTarget(int32_t target){
    desiredEncoderCount = target;
}

// Set the proportional gain
void setKp(float kP_new){
    kP = kP_new;
}
void setKd(float kD_new){
    kD = kD_new;
}

void enableController(){
    enabled = true;
}
void disableController(){
    enabled = false;
}

// Call this either with TIM4 OVF or 
// in the main loop using some millis() function
void updateMotorControl(void){
    
    if(enabled){
        // Calculate error
        error = getEncoderCount()-desiredEncoderCount;

        // Estimate derivative using velocity
        if(isVelocityTimedOut()){
            derivative = 0;
        } else{
            derivative = getSmoothEncoderVelocity();
        }

        // Calculate control input
        controlInput = kP*error + kD*derivative;

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

}


//
