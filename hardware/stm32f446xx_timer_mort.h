/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F446XX_TIMER_H_
#define __STM32F446XX_TIMER_H_


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "stm32f446xx_mort.h"
#include "stm32f446xx_interruptcontroller_mort.h"

/* Macros ------------------------------------------------------------------*/

// OUTPUT B0 STUFF
//#define PORTB_BASE_ADDRESS ((uint32_t)0x40020400)
#define PORTB_BASE_ADDRESS      GPIOB_BASE_MORT
#define PORTB_MODER_REGISTER (PORTB_BASE_ADDRESS + 0x00)
#define PORTB_OTYPER_REGISTER (PORTB_BASE_ADDRESS + 0x04)
#define PORTB_OSPEEDR_REGISTER (PORTB_BASE_ADDRESS + 0x08)
#define PORTB_PUPDR_REGISTER (PORTB_BASE_ADDRESS + 0x0C)
// To output to a pin in PORTB, use the Output Data Register ODR
#define PORTB_ODR_REGISTER (PORTB_BASE_ADDRESS + 0x14)
// Alternate functions
#define PORTB_AFR1_REGISTER (PORTB_BASE_ADDRESS + 0x20)

#define PORTC_BASE_ADDRESS    GPIOC_BASE_MORT
#define PORTC_MODER_REGISTER (PORTC_BASE_ADDRESS + 0x00)
#define PORTC_OTYPER_REGISTER (PORTC_BASE_ADDRESS + 0x04)
#define PORTC_OSPEEDR_REGISTER (PORTC_BASE_ADDRESS + 0x08)
#define PORTC_PUPDR_REGISTER (PORTC_BASE_ADDRESS + 0x0C)
// To input from a pin in PORTC, use the Input Data Register IDR
#define PORTC_IDR_REGISTER (PORTC_BASE_ADDRESS + 0x10)
// To output to a pin in PORTC, use the Output Data Register ODR
#define PORTC_ODR_REGISTER (PORTC_BASE_ADDRESS + 0x14)
// Alternate functions
#define PORTC_AFR1_REGISTER (PORTC_BASE_ADDRESS + 0x20) // good

//flags MODER Register:
#define GPIO_0_MODER                0x03
#define GPIO_0_MODER_OUT            0x01
#define GPIO_0_MODER_AF             0x02
#define GPIO_6_MODER                0x3000
#define GPIO_6_MODER_AF             0x2000
#define GPIO_6_MODER_OUT            0x1000
#define GPIO_6_MODER_IN             0x0000

//flags OTYPER Register:
#define GPIO_0_OTYPER               0x01
#define GPIO_0_OTYPER_PP            0x00
#define GPIO_6_OTYPER               0x40
#define GPIO_6_OTYPER_PP            0x00

//flags OSPEEDR Register:
#define GPIO_0_OSPEEDR              0x03 // aka 0b11
#define GPIO_0_OSPEEDR_HIGH_SPEED   0x03 // also 0b11
#define GPIO_6_OSPEEDR              0x3000
#define GPIO_6_OSPEEDR_HIGH_SPEED   0x3000

//flags PUPDR Register:
#define GPIO_0_PUPDR                0x03
#define GPIO_0_PUPDR_NOPULL         0x00
#define GPIO_6_PUPDR                0x3000
#define GPIO_6_PUPDR_NOPULL         0x00
#define GPIO_6_PUPDR_PD             0x2000
#define GPIO_6_PUPDR_PU             0x1000

//flags AFR1 (AFRL) Register:
#define GPIO_0_AFR1_AF2             2
#define GPIO_0_AFR1                 0xF
#define GPIO_6_AFR1                 0xF000000
#define GPIO_6_AFR1_AF2             0x2000000 //good?

// TIMER 2 STUFF (32-bit timer, same register layout as TIM3)
#define TIM2_BASE_ADDRESS       ((uint32_t)0x40000000)
#define TIM2_CR1_REGISTER       (TIM2_BASE_ADDRESS + 0x00)
#define TIM2_CR2_REGISTER       (TIM2_BASE_ADDRESS + 0x04)
#define TIM2_SMCR_REGISTER      (TIM2_BASE_ADDRESS + 0x08)
#define TIM2_DIER_REGISTER      (TIM2_BASE_ADDRESS + 0x0C)
#define TIM2_SR_REGISTER        (TIM2_BASE_ADDRESS + 0x10)
#define TIM2_EGR_REGISTER       (TIM2_BASE_ADDRESS + 0x14)
#define TIM2_CCMR1_REGISTER     (TIM2_BASE_ADDRESS + 0x18)
#define TIM2_CCMR2_REGISTER     (TIM2_BASE_ADDRESS + 0x1C)
#define TIM2_CCER_REGISTER      (TIM2_BASE_ADDRESS + 0x20)
#define TIM2_CNT_REGISTER       (TIM2_BASE_ADDRESS + 0x24)
#define TIM2_PSC_REGISTER       (TIM2_BASE_ADDRESS + 0x28)
#define TIM2_ARR_REGISTER       (TIM2_BASE_ADDRESS + 0x2C)
#define TIM2_CCR1_REGISTER      (TIM2_BASE_ADDRESS + 0x34)
#define TIM2_CCR2_REGISTER      (TIM2_BASE_ADDRESS + 0x38)
#define TIM2_CCR3_REGISTER      (TIM2_BASE_ADDRESS + 0x3C)
#define TIM2_CCR4_REGISTER      (TIM2_BASE_ADDRESS + 0x40)
#define TIM2_DCR_REGISTER       (TIM2_BASE_ADDRESS + 0x48)
#define TIM2_DMAR_REGISTER      (TIM2_BASE_ADDRESS + 0x4C)

// Individual bits for those registers
#define TIM2_CR1_CEN            ((uint32_t)0b01)
#define TIM2_CR1_CEN_ENABLE     ((uint32_t)0b01)
#define TIM2_CR1_OPM            ((uint32_t)0b1000)   // pg. 558
#define TIM2_CR1_OPM_ENABLE     ((uint32_t)0b1000)   // pg. 558

#define TIM2_EGR_UG             ((uint32_t)0b01)     // force update event

#define TIM2_DIER_UIE_ENABLE    ((uint32_t)0b01)
#define TIM2_DIER_CC3IE_ENABLE  ((uint32_t)0b01 << 3)

#define TIM2_SR_UIF             ((uint32_t)0b01)
#define TIM2_SR_CC1IF           ((uint32_t)0b01 << 1)
#define TIM2_SR_CC3IF           ((uint32_t)0b01 << 3)

#define TIM2_CCMR1_CC1          ((uint32_t)0b11111111)          // Resets entire CCMR1 register for channel 1
#define TIM2_CCMR1_CC1S_IN_TI1  ((uint32_t)0b01)

#define TIM2_CCMR1_CC2          ((uint32_t)0b1111111100000000)  // Resets entire CCMR1 register for channel 2
#define TIM2_CCMR1_CC2S_OUT     ((uint32_t)0b00 << 0)           // pg. 567
#define TIM2_CCMR1_OC2M_PWM     ((uint32_t)0b111 << 12)
#define TIM2_CCMR1_OC2PE_ENABLE ((uint32_t)0b01 << 11)

#define TIM2_CCMR2_CC3          ((uint32_t)0b11111111)          // Resets entire CCMR2 register for CC3
#define TIM2_CCMR2_CC3S_OUT     ((uint32_t)0b00)
#define TIM2_CCMR2_OC3M_TOGGLE  ((uint32_t)0b011 << 4)
#define TIM2_CCMR2_OC3M_PWM     ((uint32_t)0b111 << 4)
#define TIM2_CCMR2_OC3M_FROZEN  ((uint32_t)0b000 << 4)
#define TIM2_CCMR2_OC3PE_ENABLE ((uint32_t)0b01 << 3)

#define TIM2_CCER_CC1E          ((uint32_t)0b1011)              // Reset all CC1 stuff
#define TIM2_CCER_CC1E_ENABLE   ((uint32_t)0b01)
#define TIM2_CCER_CC2E          ((uint32_t)0b1011 << 4)         // Reset all CC2 stuff
#define TIM2_CCER_CC2E_ENABLE   ((uint32_t)0b01 << 4)
#define TIM2_CCER_CC3E          ((uint32_t)0b01 << 8)
#define TIM2_CCER_CC3E_ENABLE   ((uint32_t)0b01 << 8)

// TIMER 3 STUFF
// See TIMx Register Map on page 579-580
#define TIM3_BASE_ADDRESS (uint32_t)0x40000400
#define TIM3_CR1_REGISTER (TIM3_BASE_ADDRESS + 0x00)
#define TIM3_CR2_REGISTER (TIM3_BASE_ADDRESS + 0x04)
#define TIM3_SMCR_REGISTER (TIM3_BASE_ADDRESS + 0x08)
#define TIM3_DIER_REGISTER (TIM3_BASE_ADDRESS + 0x0C)
#define TIM3_SR_REGISTER (TIM3_BASE_ADDRESS + 0x10) // good
#define TIM3_EGR_REGISTER (TIM3_BASE_ADDRESS + 0x14)
#define TIM3_CCMR1_REGISTER (TIM3_BASE_ADDRESS + 0x18) //good
#define TIM3_CCMR2_REGISTER (TIM3_BASE_ADDRESS + 0x1C)
#define TIM3_CCER_REGISTER (TIM3_BASE_ADDRESS + 0x20) //good
#define TIM3_CNT_REGISTER (TIM3_BASE_ADDRESS + 0x24)
#define TIM3_PSC_REGISTER (TIM3_BASE_ADDRESS + 0x28)
#define TIM3_ARR_REGISTER (TIM3_BASE_ADDRESS + 0x2C)
#define TIM3_CCR1_REGISTER (TIM3_BASE_ADDRESS + 0x34)
#define TIM3_CCR2_REGISTER (TIM3_BASE_ADDRESS + 0x38)
#define TIM3_CCR3_REGISTER (TIM3_BASE_ADDRESS + 0x3C)
#define TIM3_CCR4_REGISTER (TIM3_BASE_ADDRESS + 0x40)
#define TIM3_DCR_REGISTER (TIM3_BASE_ADDRESS + 0x48)
#define TIM3_DMAR_REGISTER (TIM3_BASE_ADDRESS + 0x4C)
// Wow that's a lot of controls for one timer...


// Individual bits for those registers
#define TIM3_CR1_CEN            (uint32_t)0b01
#define TIM3_CR1_CEN_ENABLE     (uint32_t)0b01
#define TIM3_CR1_OPM            (uint32_t)0b1000 // pg. 558
#define TIM3_CR1_OPM_ENABLE     (uint32_t)0b1000 // pg. 558

#define TIM3_DIER_UIE_ENABLE    (uint32_t)0b01
#define TIM3_DIER_CC3IE_ENABLE  (uint32_t)0b01 << 3

#define TIM3_SR_UIF             (uint32_t)0b01 // good
#define TIM3_SR_CC1IF           (uint32_t)0b01 << 1
#define TIM3_SR_CC3IF           (uint32_t)0b01 << 3

#define TIM3_CCMR1_CC1          (uint32_t)0b11111111  // Resets entire CCMR1 register for channel 1
#define TIM3_CCMR1_CC1S_IN_TI1  (uint32_t)0b01  //     

#define TIM3_CCMR1_CC2        	(uint32_t)0b1111111100000000  // Resets entire CCMR1 register for channel 2
#define TIM3_CCMR1_CC2S_OUT     (uint32_t)0b00 << 0 // pg. 567
#define TIM3_CCMR1_OC2M_PWM     (uint32_t)0b111 << 12
#define TIM3_CCMR1_OC2PE_ENABLE (uint32_t)0b01  << 11
 
#define TIM3_CCMR2_CC3          (uint32_t)0b11111111  // Resets entire CCMR2 register for CC3
#define TIM3_CCMR2_CC3S_OUT     (uint32_t)0b00
#define TIM3_CCMR2_OC3M_TOGGLE  (uint32_t)0b011 << 4  // Same as OC1M on pg. 494
#define TIM3_CCMR2_OC3M_PWM     (uint32_t)0b111 << 4
#define TIM3_CCMR2_OC3M_FROZEN  (uint32_t)0b000 << 4
#define TIM3_CCMR2_OC3PE_ENABLE (uint32_t)0b01  << 3

#define TIM3_CCER_CC1E          (uint32_t)0b1011 // Reset all CC1 stuff
#define TIM3_CCER_CC1E_ENABLE   (uint32_t)0b01
#define TIM3_CCER_CC2E          (uint32_t)0b1011 << 4 // Reset all CC1 stuff
#define TIM3_CCER_CC2E_ENABLE   (uint32_t)0b01 << 4
#define TIM3_CCER_CC3E          (uint32_t)0b01 << 8
#define TIM3_CCER_CC3E_ENABLE   (uint32_t)0b01 << 8


// TIMER 4 STUFF
// See TIMx Register Map on page 579-580
#define TIM4_BASE_ADDRESS (uint32_t)0x40000800 // pg. 59 "register boundary addresses"
#define TIM4_CR1_REGISTER (TIM4_BASE_ADDRESS + 0x00)
#define TIM4_CR2_REGISTER (TIM4_BASE_ADDRESS + 0x04)
#define TIM4_SMCR_REGISTER (TIM4_BASE_ADDRESS + 0x08)
#define TIM4_DIER_REGISTER (TIM4_BASE_ADDRESS + 0x0C)
#define TIM4_SR_REGISTER (TIM4_BASE_ADDRESS + 0x10) // good
#define TIM4_EGR_REGISTER (TIM4_BASE_ADDRESS + 0x14)
#define TIM4_CCMR1_REGISTER (TIM4_BASE_ADDRESS + 0x18) //good
#define TIM4_CCMR2_REGISTER (TIM4_BASE_ADDRESS + 0x1C)
#define TIM4_CCER_REGISTER (TIM4_BASE_ADDRESS + 0x20) //good
#define TIM4_CNT_REGISTER (TIM4_BASE_ADDRESS + 0x24)
#define TIM4_PSC_REGISTER (TIM4_BASE_ADDRESS + 0x28)
#define TIM4_ARR_REGISTER (TIM4_BASE_ADDRESS + 0x2C)
#define TIM4_CCR1_REGISTER (TIM4_BASE_ADDRESS + 0x34)
#define TIM4_CCR2_REGISTER (TIM4_BASE_ADDRESS + 0x38)
#define TIM4_CCR3_REGISTER (TIM4_BASE_ADDRESS + 0x3C)
#define TIM4_CCR4_REGISTER (TIM4_BASE_ADDRESS + 0x40)
#define TIM4_DCR_REGISTER (TIM4_BASE_ADDRESS + 0x48)
#define TIM4_DMAR_REGISTER (TIM4_BASE_ADDRESS + 0x4C)
// Wow that's a lot of controls for one timer...


// Individual bits for those registers
#define TIM4_CR1_CEN            (uint32_t)0b01
#define TIM4_CR1_CEN_ENABLE     (uint32_t)0b01
#define TIM4_CR1_OPM            (uint32_t)0b1000 // pg. 558
#define TIM4_CR1_OPM_ENABLE     (uint32_t)0b1000 // pg. 558
#define TIM4_DIER_UIE_ENABLE    (uint32_t)0b01
#define TIM4_DIER_CC3IE_ENABLE  (uint32_t)0b01 << 3
#define TIM4_SR_UIF             (uint32_t)0b01 // good
#define TIM4_SR_CC1IF           (uint32_t)0b01 << 1
#define TIM4_SR_CC3IF           (uint32_t)0b01 << 3
#define TIM4_CCMR1_CC1          (uint32_t)0b11111111  // Resets entire CCMR1 register for CC1
#define TIM4_CCMR1_CC1S_IN_TI1  (uint32_t)0b01  //     
#define TIM4_CCMR2_CC3          (uint32_t)0b11111111  // Resets entire CCMR2 register for CC3
#define TIM4_CCMR2_CC3S_OUT     (uint32_t)0b00
#define TIM4_CCMR2_OC3M_TOGGLE  (uint32_t)0b011 << 4  // Same as OC1M on pg. 494
#define TIM4_CCMR2_OC3M_PWM     (uint32_t)0b111 << 4
#define TIM4_CCMR2_OC3M_FROZEN  (uint32_t)0b000 << 4
#define TIM4_CCMR2_OC3PE_ENABLE (uint32_t)0b01  << 3
#define TIM4_CCER_CC1E          (uint32_t)0b1011 // Reset all CC1 stuff
#define TIM4_CCER_CC1E_ENABLE   (uint32_t)0b01
#define TIM4_CCER_CC3E          (uint32_t)0b01 << 8
#define TIM4_CCER_CC3E_ENABLE   (uint32_t)0b01 << 8


/* Function Definitions-------------------------------------------------------*/



// Set Timer 3 Channel 3 as output compare interrupt
// Optionally set onePulseMode to true to make the timer only run once. in which case it must be started by setting CEN in TIM3_CR1
void initT3C3OutputCompareInterrupt(uint16_t prescale, uint16_t autoReloadRegister, uint16_t capCompare3Register,  bool onePulseMode);

// Set said CEN bit
void startT3();

// Ditto for timer 4
void initT4C3OutputCompareInterrupt(uint16_t prescale, uint16_t autoReloadRegister, uint16_t capCompare3Register, bool onePulseMode);

// Extra ditto
void startT4();

// Initialize Timer 3 Channel 2 as output compare PWM

void initT3C2PWM(void);

// Set the duty cycle (0-1000) of the T3C2 PWM output
void setT3C2PWM(uint16_t duty);

//set timer2 as one shot without starting it
void setT2OneShot(uint32_t prescaler, uint32_t period);

//return timer 2 count
uint32_t T2CNT(void);

//check if timer 2 overflowed
uint32_t isT2Overflowed(void);

//start timer2
void startT2(void);


#endif /*__STM32F446XX_TIMER_H_*/