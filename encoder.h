#ifndef __ENCODER_H_
#define __ENCODER_H_



/* Includes ------------------------------------------------------------------*/


/* Macros for Everyone--------------------------------------------------------*/

/*Function definitions---------------------------------------------------------*/
void delay(volatile int count);

void initEncoderInterruptsB8B9();
void ISR_B8B9Encoder(); // Will get called by EXTI ISR
uint32_t getEncoderCount(); // Returns encoder count number
uint32_t getEncoderVelocity(); // Returns encoder velocity
void zeroEncoder(); // Zeroes encoder position and all velocity measurements. Also resets step timer.

#endif /*__ENCODER_H_ */