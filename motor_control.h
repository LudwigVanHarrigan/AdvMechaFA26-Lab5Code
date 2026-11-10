#ifndef __MOTOR_CONTROL_H_
#define __MOTOR_CONTROL_H_



/* Includes ------------------------------------------------------------------*/


/* Macros for Everyone--------------------------------------------------------*/

/*Function definitions---------------------------------------------------------*/

// Set pins for motor, set up timed loop timer and stuff like that
// Direction pins: B3 and A4
void initMotorPosControl(void);

// Set the speed of the motor.
// Range (-1000,1000)
void setMotorPWM(int16_t PWM);

// Set the position target in counts
void setPositionTarget(uint32_t target);

// Set the proportional and derivative gain
void setKp(float kP_new);
void setKd(float kD_new);

// Functions to enable or disable the position controller
void enableController();
void disableController();

//The loop. This should get run about 1000hz by timer 4.
void updateMotorControl(void);


#endif /*__MOTOR_CONTROL_H_ */