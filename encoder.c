/**
  ******************************************************************************
  * @file    encoder.c
  * @brief   Contains the function to be called whenever a pulse is received.
  ******************************************************************************
  */

#include <stdint.h>
#include <stdbool.h>
#include "stm32f446xx_gpio_mort.h"
#include "stupid_delay.h"
#include "stm32f446xx_timer_mort.h"
#include "stm32f446xx_interruptcontroller_mort.h"
#include "encoder.h"          /* prototypes for everything defined below */


// MACROS
#define PRESCALER 8999            // 90MHz / 9000 = 10kHz
#define AUTO_RELOAD_REGISTER 65535
#define COUNTS_PER_SECOND 10000  

// alpha value for velocity smoothing 
#define VELOCITY_SMOOTH_ALPHA 0.01f

// VARIABLES 
volatile bool    lastEncoder0Val = false;
volatile bool    lastEncoder1Val = false;
volatile int32_t encoderCount    = 0;
volatile int32_t velocity        = 0;
volatile int32_t velocitySmooth  = 0;
static   float   velocitySmoothF = 0.0f;   // filter state kept as float


// METHODS

// Note: we are using pins B8 and B9 for the interrupt pins
void initEncoderInterruptsB8B9(void){

    // init pins as inputs
    initGpioAsInput(PORTB, 8, PULLUP);
    initGpioAsInput(PORTB, 9, PULLUP);

    // Set up Timer 2 for one shot with specified timeout
    setT2OneShot(PRESCALER, AUTO_RELOAD_REGISTER);

    lastEncoder0Val = getEncoder0Val() != 0;
    lastEncoder1Val = getEncoder1Val() != 0;

    // enable interrupt last
    setExtInterrupt8And9ToPB();
}


// call this from EXTI9_5_IRQHandler
void ISR_B8B9Encoder(void){
    //read B8 and B9
    bool currentEncoder0Val = (getEncoder0Val() != 0);
    bool currentEncoder1Val = (getEncoder1Val() != 0);

    // compare old AB and new AB to find direction
    // (A,B) if A changes, it's forward if A!=B after and if B changes, it's forward if A=B after.
    uint32_t index = (lastEncoder0Val << 3) | (lastEncoder1Val << 2) |
                     (currentEncoder0Val << 1) | (currentEncoder1Val);
    int32_t step = 0;                 // signed: it holds +1 / -1

    switch (index){
        case 0b0010:
        case 0b1011:
        case 0b1101:
        case 0b0100:
            step = 1;
            break;

        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            step = -1;
            break;

        default: // no change or both A and B change
            step = 0;
            break;
    }

    // Based on direction:
    if (step != 0){
        // 1. Increment or decrement position
        encoderCount += step;

        // 2. update velocity
        if (!isVelocityTimedOut()){ // If velocity timer is still running
            // Set velocity based on elapsed time
            int32_t v = (int32_t)unsignedVelocityTicksPerSecond() * step;
            velocity = v;
            // Update smoothed velocity
            velocitySmoothF += VELOCITY_SMOOTH_ALPHA * ((float)v - velocitySmoothF);
            velocitySmooth = (int32_t)velocitySmoothF;
        }

        // 3. Reset and start encoder timer
        startEncoderTimer();
    }
    // Finally, update the last values for the next edge
    lastEncoder0Val = currentEncoder0Val;
    lastEncoder1Val = currentEncoder1Val;
}


// Convenience functions
uint32_t getEncoder0Val(void){
    return checkGpioPin(PORTB, 8);
}
uint32_t getEncoder1Val(void){
    return checkGpioPin(PORTB, 9);
}

uint32_t unsignedVelocityTicksPerSecond(void){
    uint32_t cnt = T2CNT();

    //to avoid dividing with 0
    if (cnt == 0){
        cnt = 1;
    }

    // encoder edges per second
    return COUNTS_PER_SECOND / cnt;
}

// Zeroes encoder position and all velocity measurements. Also resets step timer.
void zeroEncoder(void){
    encoderCount = 0;
    velocity = 0;
    velocitySmooth = 0;
    velocitySmoothF = 0.0f;
    startEncoderTimer();
}

bool isVelocityTimedOut(void){
    return isT2Overflowed();
}

void startEncoderTimer(void){
    startT2();
}

// Getters
int32_t getEncoderCount(void){
    return encoderCount;
}
int32_t getEncoderVelocity(void){
    return isVelocityTimedOut() ? 0 : velocity;   // stopped motor reads 0
}
int32_t getSmoothEncoderVelocity(void){
    if (isVelocityTimedOut()){
        velocitySmoothF = 0.0f;
        velocitySmooth = 0;
    }
    return velocitySmooth;
}