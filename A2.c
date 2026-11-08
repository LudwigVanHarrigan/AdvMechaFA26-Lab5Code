
/**
  ******************************************************************************
  * @file    A2.c 
  * @author  MOI (Kevin Cotellesso)
  * @version 
  * @date    
  * @brief   Performs continuous ADC conversion on 3 channels.
  ******************************************************************************
  */
 
#include <stdint.h>

// #include "main.h"
#include "stm32f446xx_gpio_mort.h"
#include "stm32f446xx_timer_mort.h"
#include "stm32f446xx_adc_mort.h"
#include "stm32f446xx_dma_controller_mort.h"
#include "stm32f446xx_system_mort.h"
#include "stm32f446xx_usart3_mort.h" //always include usart where you want to do communications
#include "stupid_delay.h"



void A2 (void)
{
  // Initialize some stuff
  initUSART3(); //always init USART if you want communication!

  initADC3_567_withDMA(); // Initialize adc3 channel 5,6,7
  
  delay(80000); //give some time for USART to come alive.
  transmitLineUSART3("USART3 is alive!");
  delay(800000); // Give some time for ADC to boot up
  startADCConversion(); // Turn on the ADC (SWSTART)

  // Run stuff
  while(1) {
    delay(800000);
     printfUSART3("ADC Value 1: %u  ADC Value 2: %u  ADC Value 3: %u\n", returnADC3StoredValue(0), returnADC3StoredValue(1), returnADC3StoredValue(2));
    
  }
}






