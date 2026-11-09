/**
  ******************************************************************************
  * @file    stm32f446xx_timer_mort.c 
  * @author  Kevin Cotellesso
  * @version 
  * @date    
  * @brief   Controls STM32F446ze timers
  ******************************************************************************
  */

/* Includes----------------------------------------------------------*/
#include "stm32f446xx_timer_mort.h"
#include "stm32f446xx_rcc_mort.h"

#include "stm32f446xx_usart3_mort.h" // usart for comms

/* function definitions-------------------------------------------------------*/

/**
  * @brief  Sets up Timer 3 to throw interrupts on capture compare 3 and on overflow.
  * @note   PAIN AND SUFFERING
  * @param  prescale sets the timer 3 prescale.
  * @param autoReloadRegister sets the timer 3 autoReloadRegister.
  * @param capCompare3Register sets the timer 3 capture/compare register 3 value.
  * @param onePulseMode sets the timer to one pulse mode, in which case startT3() must be called to start the single pulse.
  * @retval None
  */
void initT3C3OutputCompareInterrupt(uint16_t prescale, uint16_t autoReloadRegister, uint16_t capCompare3Register, bool onePulseMode){
  volatile uint32_t  * reg_pointer; // volatile so the compiler doesn't try to cache it



  // Timer registers for TIM2 to TIM5 start on pg. 558 of the big fat STM32 book
  // 1. Enable APB1 clock (see stm32..._rcc_mort.h)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);

  // 2. Enable timer 3 interrupt
  enableNVIC_T3();

  // 3. Clear all flags in TIM3_SR (status register) (pg. 564)
  reg_pointer = (uint32_t *)TIM3_SR_REGISTER;
  // UIF is rc_w0
  *reg_pointer = 0;

       
  // 4. Upload prescale value to TIM3_PSC
  reg_pointer = (uint32_t *)TIM3_PSC_REGISTER;
  *reg_pointer = (uint32_t)prescale; // This works.

  // 5. Set wanted period to TIM3_ARR auto reload register
  reg_pointer = (uint32_t *)TIM3_ARR_REGISTER;
  *reg_pointer = (uint32_t)autoReloadRegister;

  // CAPTURE/CONTROL REGISTER STUFF

  // 6. Set output compare mode in CCMR2 register (want interrupt)
  // page 570 
  // Note we can only set CC3S while CC3E = 0 in TIM3_CCER
  reg_pointer = (uint32_t *)TIM3_CCMR2_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM3_CCMR2_CC3);      // Reset!
  *reg_pointer = *reg_pointer | TIM3_CCMR2_CC3S_OUT;              // Set Capture/Compare 3 as output
  *reg_pointer = *reg_pointer | TIM3_CCMR2_OC3M_FROZEN;           // Default value (pg. 568, same as OC1M)
  *reg_pointer = *reg_pointer | TIM3_CCMR2_OC3PE_ENABLE;          // Enable preload register in case we want to change compare value

  // 7. Set CCR3 compare register
  reg_pointer = (uint32_t *)TIM3_CCR3_REGISTER;
  *reg_pointer = (uint32_t)capCompare3Register; 


  // 8. Enable TIM3 by setting CC3E bit in CCER register
  reg_pointer = (uint32_t *)TIM3_CCER_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM3_CCER_CC3E);  // Reset it
  *reg_pointer = *reg_pointer | TIM3_CCER_CC3E_ENABLE;        // Enable capture/control register 3

  // 9. Enable the interrupt for CC3 and overflow
  reg_pointer = (uint32_t *)TIM3_DIER_REGISTER;
  // In the slides it's just written directly instead of bitmasked
  // which I suppose is fine since timer 3 uses the whole register?
  *reg_pointer = (TIM3_DIER_CC3IE_ENABLE | TIM3_DIER_UIE_ENABLE);

  // ENABLE SUBSYSTEM

  // if onePulseMode is not set, start the timer
  if(onePulseMode){
    // 8. Set one pulse mode
    reg_pointer = (uint32_t *)TIM3_CR1_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM3_CR1_OPM);  // Reset it
  *reg_pointer = *reg_pointer | TIM3_CR1_OPM_ENABLE;        // Enable one-pulse mode
  }else {

  // Enable timer subsystem by setting CEN bit in TIM3_CR1 (pg. 558)
  startT3();
  }
}

// Set CEN bit high for timer 3, starting counter.
void startT3(){
  // transmitLineUSART3("Starting Timer 3");
    volatile uint32_t  * reg_pointer;

  // Clear update event flag in TIM3_SR (status register) (pg. 564)
  reg_pointer = (uint32_t *)TIM3_SR_REGISTER;
  // UIF is rc_w0 so we can (and should) write straight "...11111110" without worrying about bitmasks
  // *reg_pointer = (~((uint32_t)TIM3_SR_UIF)); 
  *reg_pointer = 0;

  // Enable timer subsystem with CEN
  reg_pointer = (uint32_t *)TIM3_CR1_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM3_CR1_CEN);  // Reset it
  *reg_pointer = *reg_pointer | TIM3_CR1_CEN_ENABLE;        // Enable subsystem

}
/**
  * @brief  Sets up Timer 4 to throw interrupts on capture compare 3 and on overflow.
  * @note   Less pain cause I copied it from Timer 3
  * @param  prescale sets the timer 4 prescale.
  * @param autoReloadRegister sets the timer 4 autoReloadRegister.
  * @param capCompare3Register sets the timer 4 capture/compare register 3 value.
  * @param onePulseMode sets the timer to one pulse mode, in which case startT4() must be called to start the single pulse.
  * @retval None
  */
void initT4C3OutputCompareInterrupt(uint16_t prescale, uint16_t autoReloadRegister, uint16_t capCompare3Register, bool onePulseMode){
  volatile uint32_t  * reg_pointer; // volatile so the compiler doesn't try to cache it

  // Timer registers for TIM2 to TIM5 start on pg. 558 of the big fat STM32 book
  // 1. Enable APB1 clock (see stm32..._rcc_mort.h)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);

  // 2. Enable timer 4 interrupt
  enableNVIC_T4();

  // 3. Clear all flags in TIM4_SR (status register) (pg. 564)
  reg_pointer = (uint32_t *)TIM4_SR_REGISTER;
  // UIF is rc_w0
  *reg_pointer = 0;
       
  // 4. Upload prescale value to TIM4_PSC
  reg_pointer = (uint32_t *)TIM4_PSC_REGISTER;
  *reg_pointer = (uint32_t)prescale; // This works.

  // 5. Set wanted period to TIM4_ARR auto reload register
  reg_pointer = (uint32_t *)TIM4_ARR_REGISTER;
  *reg_pointer = (uint32_t)autoReloadRegister;

  // CAPTURE/CONTROL REGISTER STUFF

  // 6. Set output compare mode in CCMR2 register (want interrupt)
  // page 570 
  // Note we can only set CC3S while CC3E = 0 in TIM4_CCER
  reg_pointer = (uint32_t *)TIM4_CCMR2_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM4_CCMR2_CC3);      // Reset!
  *reg_pointer = *reg_pointer | TIM4_CCMR2_CC3S_OUT;              // Set Capture/Compare 3 as output
  *reg_pointer = *reg_pointer | TIM4_CCMR2_OC3M_FROZEN;           // Default value (pg. 568, same as OC1M)
  *reg_pointer = *reg_pointer | TIM4_CCMR2_OC3PE_ENABLE;          // Enable preload register in case we want to change compare value

  // 7. Set CCR3 compare register
  reg_pointer = (uint32_t *)TIM4_CCR3_REGISTER;
  *reg_pointer = (uint32_t)capCompare3Register; 


  // 8. Enable TIM4 by setting CC3E bit in CCER register
  reg_pointer = (uint32_t *)TIM4_CCER_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM4_CCER_CC3E);  // Reset it
  *reg_pointer = *reg_pointer | TIM4_CCER_CC3E_ENABLE;        // Enable capture/control register 3

  // 9. Enable the interrupt for CC3 and overflow
  reg_pointer = (uint32_t *)TIM4_DIER_REGISTER;
  // In the slides it's just written directly instead of bitmasked
  // which I suppose is fine since timer 4 uses the whole register?
  *reg_pointer = (TIM4_DIER_CC3IE_ENABLE | TIM4_DIER_UIE_ENABLE);

  // ENABLE SUBSYSTEM

  // if onePulseMode is not set, start the timer
  if(onePulseMode){
    // 8. Set one pulse mode
    reg_pointer = (uint32_t *)TIM4_CR1_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM4_CR1_OPM);  // Reset it
  *reg_pointer = *reg_pointer | TIM4_CR1_OPM_ENABLE;        // Enable one-pulse mode
  
  }else {

  // 8. Enable timer subsystem by setting CEN bit in TIM4_CR1 (pg. 558)
  startT4();
  }
}

// Set CEN bit high for timer 4, starting counter.
void startT4(){
  // transmitLineUSART3("Starting Timer 4");
  volatile uint32_t  * reg_pointer;
  reg_pointer = (uint32_t *)TIM4_CR1_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM4_CR1_CEN);  // Reset it
  *reg_pointer = *reg_pointer | TIM4_CR1_CEN_ENABLE;        // Enable subsystem

}

/**
  * @brief  Uses Timer 3 Compare register to toggle output
  * @note   Use Alternate Function Table of datasheet (not book) to find out which pin to set to AF2.
  * @param  None
  * @retval None
  */
void initT3C2PWM(void)
{
  volatile uint32_t  * reg_pointer; // volatile so the compiler doesn't try to cache it
 
  // Calculate autoreload and prescale values. These will be constant so we can use multiple channels
  uint32_t prescale = 4; // This gives us 22.5MHZ. Counts to 1,125 20k times per second or 1000 22.5k times per second.

  // Let's go with 22.5khz so only dogs can hear it.
  uint32_t autoReloadRegister = 999;
  // Conveniently this means we can set CCR2 from 0 to 1000
  // 0 is on, 1000 is off (since CCR turns pin on and ARR turns it off again.)
  uint32_t capCompare2Register = 0; // Start with 0 or off hopefully

  // Following slide 20 on the timer lecture
  // Timer registers for TIM2 to TIM5 start on pg. 558 of the big large STM32 book
  // 1. Enable APB1 clock (see stm32..._rcc_mort.h)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);

  // 2. Clear update event flag in TIM3_SR (status register) (pg. 564)
  reg_pointer = (uint32_t *)TIM3_SR_REGISTER;
  // UIF is rc_w0 so we can (and should) write straight "...11111110" without worrying about bitmasks
  *reg_pointer = (~((uint32_t)TIM3_SR_UIF));  // TODO check if UIF is the right flag 
       
  // 3. Upload prescale value to TIM3_PSC
  reg_pointer = (uint32_t *)TIM3_PSC_REGISTER;
  *reg_pointer = (uint32_t)prescale; // This works.

  // 4. Set wanted period to TIM3_ARR auto reload register
  reg_pointer = (uint32_t *)TIM3_ARR_REGISTER;
  *reg_pointer = (uint32_t)autoReloadRegister;

  // CAPTURE/CONTROL REGISTER STUFF

  // 5. Set output compare mode in CCMR1 register (want pwm)
  // Note we can only set CC2S while CC2E = 0 in TIM3_CCER
  reg_pointer = (uint32_t *)TIM3_CCMR1_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM3_CCMR1_CC2);      // Reset!
  *reg_pointer = *reg_pointer | TIM3_CCMR1_CC2S_OUT;              // Set Capture/Compare 3 as output
  *reg_pointer = *reg_pointer | TIM3_CCMR1_OC2M_PWM;              // (pg. 568, same as OC1M)
  *reg_pointer = *reg_pointer | TIM3_CCMR1_OC2PE_ENABLE;          // Enable preload register since we will want to change compare value

  // 6. Set CCR2 compare register
  reg_pointer = (uint32_t *)TIM3_CCR2_REGISTER;
  *reg_pointer = (uint32_t)capCompare2Register; 


  // 7. Enable TIM3 by setting CC2E bit in CCER register
  reg_pointer = (uint32_t *)TIM3_CCER_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM3_CCER_CC2E);  // Reset it
  *reg_pointer = *reg_pointer | TIM3_CCER_CC2E_ENABLE;        // Enable capture/control register 3

  // ENABLE SUBSYSTEM

  // 8. Enable timer subsystem by setting CEN bit in TIM3_CR1 (pg. 558)
  reg_pointer = (uint32_t *)TIM3_CR1_REGISTER;
  *reg_pointer = *reg_pointer & (~(uint32_t)TIM3_CR1_CEN);  // Reset it
  *reg_pointer = *reg_pointer | TIM3_CR1_CEN_ENABLE;        // Enable subsystem


}

// Duty should be 0-1000.
// 0 should be off, 1000 should be on
void setT3C2PWM(uint16_t duty){
  // First of all, invert it.
  duty = 1000 - duty;
  // Second of all, trust the user to put the right values in
  // since the user is ME.


  volatile uint32_t  * reg_pointer; // volatile so the compiler doesn't try to cache it

  // 1. Set CCR2 compare register
  reg_pointer = (uint32_t *)TIM3_CCR2_REGISTER;
  *reg_pointer = (uint32_t)duty; 
  // Since we enabled preload register, we can set this on the fly.

}

void setT2OneShot(uint32_t prescaler, uint32_t period){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    volatile uint32_t *reg_pointer;

    //disable timer by setting CEN to 0
    reg_pointer = (uint32_t *)TIM2_CR1_REGISTER;
    *reg_pointer = *reg_pointer & 0x00;

    //upload the prescaler value to TIM2_PSC
    reg_pointer = (uint32_t *)TIM2_PSC_REGISTER;
    *reg_pointer = prescaler;

    //set the wanted period value to the autoreload register TIM2_ARR
    reg_pointer = (uint32_t *)TIM2_ARR_REGISTER;
    *reg_pointer = period;

    //set one pulse mode to TIM2 by writing 1 to bit3 of TIM2_CR1
    reg_pointer = (uint32_t *)TIM2_CR1_REGISTER;
    *reg_pointer = *reg_pointer | TIM2_CR1_OPM;

    //load PSC and ARR now
    reg_pointer = (uint32_t *)TIM2_EGR_REGISTER;
    *reg_pointer = *reg_pointer | TIM2_EGR_UG;

    //clear the UIF that UG set
    reg_pointer = (uint32_t *)TIM2_SR_REGISTER;
    *reg_pointer = 0;

}

uint32_t T2CNT(void){
  uint32_t *reg_pointer = (uint32_t *)TIM2_CNT_REGISTER;
  return *reg_pointer;
}

uint32_t isT2Overflowed(void){
  uint32_t *reg_pointer;
  reg_pointer = (uint32_t *)TIM2_SR_REGISTER;
  return (*reg_pointer & TIM2_SR_UIF) != 0;
}

void startT2(void){
  uint32_t *reg_pointer;
  // stop the timer by clearing CEN bit of CR1 register
  reg_pointer = (uint32_t *)TIM2_CR1_REGISTER;
  *reg_pointer = *reg_pointer & (~TIM2_CR1_CEN);
  // reset the timer
  reg_pointer = (uint32_t *)TIM2_CNT_REGISTER;
  *reg_pointer = 0;
  // clear overflow flag
  reg_pointer = (uint32_t *)TIM2_SR_REGISTER;
  *reg_pointer = ~TIM2_SR_UIF;
  // start timer
  reg_pointer = (uint32_t *)TIM2_CR1_REGISTER;
  *reg_pointer = *reg_pointer | TIM2_CR1_CEN_ENABLE;
}
//EOF