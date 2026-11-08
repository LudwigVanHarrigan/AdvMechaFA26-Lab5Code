/**
  ******************************************************************************
  * @file    stm32f446xx_interruptcontroller_mort.c 
  * @author  Kevin Cotellesso
  * @brief   Controls STM32F446ze NVIC access and interrupt control
  ******************************************************************************
  */

/* Includes----------------------------------------------------------*/
#include "stm32f446xx_interruptcontroller_mort.h"
#include "stm32f446xx_gpio_mort.h"
#include "stm32f446xx_rcc_mort.h"



/* MACRO definitions----------------------------------------------------------*/

//REGISTER ADDRESSES:
// (from core_cm4.h)
// DO NOT FUCK THIS UP
#define SYSTEM_CONTROL_BASE_ADDRESS (0xE000E000)
#define NVIC_BASE_ADDRESS (SYSTEM_CONTROL_BASE_ADDRESS + 0x100)

#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31 (NVIC_BASE_ADDRESS)
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63 (NVIC_BASE_ADDRESS+0x4)
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_64_95 (NVIC_BASE_ADDRESS+0x8)
#define TIM3_INTERRUPT_BIT      (0x20000000) // bit 29 pg.239
#define TIM4_INTERRUPT_BIT      (0x40000000) // bit 30
#define EXTI9_5_INTERRUPT_BIT   (0x800000)


// SYSCONFIG controller stuff
#define SYSCFG_BASE_ADDRESS         ((uint32_t)0x40013800)
#define SYSCFG_EXTICR2_REGISTER     (SYSCFG_BASE_ADDRESS+0x0C)
#define SYSCFG_EXTICR3_REGISTER     (SYSCFG_BASE_ADDRESS+0x10) // pg.199

#define SYSCFG_EXTICR2_6BITS        ((uint32_t)0xF00) //Resets channel 6
#define SYSCFG_EXTICR2_6_PORTC      ((uint32_t)0x200)
#define SYSCFG_EXTICR3_8BITS        ((uint32_t)0x00F) //Resets channel 8
#define SYSCFG_EXTICR3_8_PORTB      ((uint32_t)0x001) // pg. 199 set EXTI channel 8 to listen for PORTB (i.e.PB8)

// EXTI controller stuff (external interrupt)
#define EXTI_BASE_ADDRESS           (uint32_t)0x40013C00
#define EXTI_IMR_REGISTER           (EXTI_BASE_ADDRESS + 0x00) // Interrupt mask register
#define EXTI_RTSR_REGISTER          (EXTI_BASE_ADDRESS + 0x08) // Rising Trigger Selection
#define EXTI_PR_REGISTER            (EXTI_BASE_ADDRESS + 0x14) // Pending register

#define EXTI_IMR_MR6                ((uint32_t)0x40)
#define EXTI_RTSR_TR6               ((uint32_t)0x40)
#define EXTI_PR_PR6                 ((uint32_t)0x40)

#define EXTI_IMR_MR8                ((uint32_t)0x100) // Interrupt mask register (pg. 247)
#define EXTI_RTSR_TR8               ((uint32_t)0x100) // Rising trigger selection register (pg.248)
#define EXTI_PR_PR8                 ((uint32_t)0x100) // Pending register (pg. 249)

// FLAGS:

/* function definitions-------------------------------------------------------*/
void enableNVIC_T3(void){
  uint32_t * reg_pointer;
  reg_pointer = (uint32_t *)NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31;
  *reg_pointer = TIM3_INTERRUPT_BIT; // We can just write ...0010000000... since 0 has no effect 
}
void enableNVIC_T4(void){
  uint32_t * reg_pointer;
  reg_pointer = (uint32_t *)NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31;
  *reg_pointer = TIM4_INTERRUPT_BIT; // We can just write ...0100000000... since 0 has no effect 
}

void setExtInterrupt6ToPC6(void){
  // Set external interrupt 6 to portC pin 6
  // Requires PortC pin 6 set to input
  initGpioAsInput(PORTC, 6, PULLDOWN);

  // Set PortB pin 0 to output for light flashing
  initGpioAsOutput(PORTB, 0);

  // Now enable EXTI6 (external interrupt muxer 6 for PA6, PB6, PC6 etc.)
  uint32_t * reg_pointer;

  // 1. Enable the SYSCFG clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // 2. Set EXTI6 using EXTICR2
  reg_pointer = (uint32_t *)SYSCFG_EXTICR2_REGISTER;
  *reg_pointer = *reg_pointer & ~SYSCFG_EXTICR2_6BITS; // Clear
  *reg_pointer = *reg_pointer | SYSCFG_EXTICR2_6_PORTC; // Enable PORTC for EXTI6

  // 3. Unmask EXTI6
  reg_pointer = (uint32_t *)EXTI_IMR_REGISTER;
  *reg_pointer = *reg_pointer | EXTI_IMR_MR6; // pg. 247

  // 4. Set trigger on rising edge using Rising Trigger Selection Register (RTSR, pg. 248)
  reg_pointer = (uint32_t *)EXTI_RTSR_REGISTER;
  *reg_pointer = *reg_pointer | EXTI_RTSR_TR6;

  // 5. Tell NVIC (big interrupt boss) to respond to interrupts from the EXTI controllers 5 through 9
  reg_pointer = (uint32_t *)NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31;
  *reg_pointer = EXTI9_5_INTERRUPT_BIT;

}

void setExtInterrupt8ToPB8(void){
  // Set external interrupt 8 to portB pin 8
  // Requires PortB pin 8 set to input
  // initGpioB8AsInput(); // We already do this (specifying pullup) in the gummy_io setup routine. 
  // This should be separate from attaching interrupt.

  // Now enable EXTI8 (external interrupt muxer 8 for PA8, PB8, PC8 etc.)
  uint32_t * reg_pointer;

  // 1. Enable the SYSCFG clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // 2. Set EXTI8 using EXTICR2 (EXTICR for 5,6,7,8,9)
  reg_pointer = (uint32_t *)SYSCFG_EXTICR3_REGISTER;
  *reg_pointer = *reg_pointer & ~SYSCFG_EXTICR3_8BITS; // Clear
  *reg_pointer = *reg_pointer | SYSCFG_EXTICR3_8_PORTB; // Enable PORTC for EXTI6

  // 3. Unmask EXTI8
  reg_pointer = (uint32_t *)EXTI_IMR_REGISTER;
  *reg_pointer = *reg_pointer | EXTI_IMR_MR8; // pg. 247

  // 4. Set trigger on rising edge using Rising Trigger Selection Register (RTSR, pg. 248)
  reg_pointer = (uint32_t *)EXTI_RTSR_REGISTER;
  *reg_pointer = *reg_pointer | EXTI_RTSR_TR8;

  // 5. Tell NVIC (big interrupt boss) to respond to interrupts from the EXTI controllers 5 through 9
  reg_pointer = (uint32_t *)NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31;
  *reg_pointer = EXTI9_5_INTERRUPT_BIT;
  // TODO is this the right thing to enable?
}





//EOF