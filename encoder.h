#ifndef __ENCODER_H_
#define __ENCODER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*Function definitions---------------------------------------------------------*/
void     initEncoderInterruptsB8B9(void);
void     ISR_B8B9Encoder(void);          // Called by EXTI9_5_IRQHandler

int32_t  getEncoderCount(void);          // Signed position in edges
int32_t  getEncoderVelocity(void);       // Signed edges per second
int32_t  getSmoothEncoderVelocity(void); // Filtered version
void     zeroEncoder(void);              // Zeroes position and velocity, resets step timer

// helpers used inside encoder.c
uint32_t getEncoder0Val(void);
uint32_t getEncoder1Val(void);
uint32_t unsignedVelocityTicksPerSecond(void);
bool     isVelocityTimedOut(void);
void     startEncoderTimer(void);

#endif /*__ENCODER_H_ */