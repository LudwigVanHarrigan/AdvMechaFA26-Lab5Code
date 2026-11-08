/**
  ******************************************************************************
  * @file    stm32f446xx_adc_mort.c 
  * @author  Kevin Cotellesso
  * @version 0.000000000000000000000000000000000000000000000000000000000000457783
  * @date    
  * @brief   Controls STM32F446ze ADC
  ******************************************************************************
  */

/* Includes----------------------------------------------------------*/
#include "stm32f446xx_adc_mort.h"
#include "stm32f446xx_dma_controller_mort.h"
#include "stm32f446xx_rcc_mort.h"
#include "stm32f446xx_gpio_mort.h"



/* MACRO definitions----------------------------------------------------------*/





//FLAGS: 



/* Module Variables----------------------------------------------------------*/


/* Global Variables----------------------------------------------------------*/
//???? I thought we weren't supposed to do that

/* Functions----------------------------------------------------------*/

// Initialize the ADC for gpioF pins 7,8,9 (ADC channels 5,6,7)
// The datasheet (not big fucking manual) describes this connectivity in table "pin and ball descriptions"
void initADC3_567_withDMA (void){
  volatile uint32_t * reg_pointer;
  // Turn on the ADC3 bus clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  // Initialize GPIO F 7, 8, 9 as analog
  initGpioAsAnalog(PORTF, 7, PULLDOWN);
  initGpioAsAnalog(PORTF, 8, PULLDOWN);
  initGpioAsAnalog(PORTF, 9, PULLDOWN);

  initDMAForAdc3_3channels(); // Initialize the DMA for ADC3
  enableDMAForAdc3_3channels(); // Enable the DMA

  // Setup clock prescaler
  reg_pointer = (uint32_t *)ADC_COMMON_CCR_REGISTER;
  *reg_pointer = ADC_CCR_ADCPRE_4; // Set prescaler to 4x

  // Configure 12bit resolution, end of conversion interrupt DISABLED, scan mode enabled (we scanning group of channels)
  reg_pointer = (uint32_t *)(ADC3_BASE_ADDRESS+ADC_CR1_OFFSET);
  *reg_pointer = ADC_CR1_SCAN;
  // End of conversion interrupt (EOCIE) would be enabled if we wanted to deal with
  // ADC stuff using interrupts to grab the most recent value instead of DMA.

  // Configure ADC external trigger disabled, right data alignment, dma,
  // end of conversion set at the end of each regular conversion, continuous conversion enabled.
  reg_pointer = (uint32_t *)(ADC3_BASE_ADDRESS+ADC_CR2_OFFSET);
  *reg_pointer = ADC_CR2_EOCS + ADC_CR2_CONT + ADC_CR2_DDS + ADC_CR2_DMA;

  // Each sequence of conversions has three channels
  reg_pointer = (uint32_t *)(ADC3_BASE_ADDRESS+ADC_SQR1_OFFSET);
  *reg_pointer = ADC_SQR1_3CONVERSIONS;

  // Configure ADC channels 5,6,7 to max amount of sampling time (480 clock cycles)
  reg_pointer = (uint32_t *)(ADC3_BASE_ADDRESS+ADC_SMPR2_OFFSET);
  *reg_pointer  = (ADC_SMPR2_SMP_480 << ADC_SMPR2_SMP5_SHIFT)
                + (ADC_SMPR2_SMP_480 << ADC_SMPR2_SMP6_SHIFT)
                + (ADC_SMPR2_SMP_480 << ADC_SMPR2_SMP7_SHIFT);

  // Configure the sequence of conversion (5,6,7)
  reg_pointer = (uint32_t *)(ADC3_BASE_ADDRESS+ADC_SQR3_OFFSET);
  *reg_pointer  = (ADC_CHANNEL_5<<ADC_SQR3_SQ1_SHIFT)
                + (ADC_CHANNEL_6<<ADC_SQR3_SQ2_SHIFT)
                + (ADC_CHANNEL_7<<ADC_SQR3_SQ3_SHIFT);

  // Enable ADC3
  enableADC3();

}

// Enable ADC3 by setting ADON bit
void enableADC3(void){
  //"Turn the ADC on by setting the ADON bit in the CR2 register in ADC 3"
  // (Melisa's slides)
  volatile uint32_t * reg_pointer;
  // CR2 register
  reg_pointer = (uint32_t *)(ADC3_BASE_ADDRESS+ADC_CR2_OFFSET);
  *reg_pointer = *reg_pointer | (uint32_t)ADC_CR2_ADON; // Set ADON bit

}

// Start a conversion or a chain of conversions by setting SWSTART bit
void startADCConversion(void){
  // Start conversion by setting SWSTART bit in CR2
  volatile uint32_t * reg_pointer;
  // CR2 register
  reg_pointer = (uint32_t *)(ADC3_BASE_ADDRESS+ADC_CR2_OFFSET);
  *reg_pointer = *reg_pointer | (uint32_t)ADC_CR2_SWSTART; // Set SWSTART bit
}

// Get the current value of the ADC3 data register (for debug)
uint16_t getCurrentADC3Value(void){
    volatile uint32_t * reg_pointer;
  reg_pointer = (uint32_t *)(ADC3_BASE_ADDRESS + ADC_DR_OFFSET); // ADC3 Data Register
  return *reg_pointer;
}



/* Interrupt Handler----------------------------------------------------------*/



//EOF