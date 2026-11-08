/**
  ******************************************************************************
  * @file    nucleo_led.c 
  * @author  
  * @version 
  * @date    
  * @brief   Controls the LED's on the nucleo board
  ******************************************************************************
  */
  
#include "stm32f446xx_gpio_mort.h"
// #include "hardware_stm_gpio.h"
#include "stupid_delay.h"


// Simple delay function using software loops, not very good.
// you should implement your own eventually using timers once
// you learn about them.
void delay(volatile int count) {
    while (count--) {
        __asm__("nop");
    }
}
