/**
  ******************************************************************************
  * @file    stm32f446xx_gpio_mort.c 
  * @author  mortamar@andrew.cmu.edu and kcotelle@andrew.cmu.edu
  * @version 2.0
  * @date    June-2026
  * @brief   Controls STM32F446ze GPIO
  ******************************************************************************
  */

/* Includes----------------------------------------------------------*/
#include "stm32f446xx_gpio_mort.h"
#include "stm32f446xx_rcc_mort.h"


// REQUIRED FOR COMMUNICATION !!!
//Port D addresses: //These are needed for communication do not edit
//#define PORTD_BASE_ADDRESS ((uint32_t)0x40020C00U)
#define PORTD_BASE_ADDRESS		GPIOD_BASE_MORT
#define PORTD_MODER_REGISTER (PORTD_BASE_ADDRESS + 0x00)
#define PORTD_OTYPER_REGISTER (PORTD_BASE_ADDRESS + 0x04)
#define PORTD_OSPEEDR_REGISTER (PORTD_BASE_ADDRESS + 0x08)
#define PORTD_PUPDR_REGISTER (PORTD_BASE_ADDRESS + 0x0C)
#define PORTD_IDR_REGISTER (PORTD_BASE_ADDRESS + 0x10)
#define PORTD_ODR_REGISTER (PORTD_BASE_ADDRESS + 0x14)
#define PORTD_BSRRL_REGISTER (PORTD_BASE_ADDRESS + 0x18)
#define PORTD_BSRR_REGISTER (PORTD_BASE_ADDRESS + 0x18)
#define PORTD_BSRRH_REGISTER (PORTD_BASE_ADDRESS + 0x1A)
#define PORTD_LCKR_REGISTER (PORTD_BASE_ADDRESS + 0x1C)
#define PORTD_AFR1_REGISTER (PORTD_BASE_ADDRESS + 0x20)
#define PORTD_AFR2_REGISTER (PORTD_BASE_ADDRESS + 0x24)
#define PORTD_OSPEEDR_REGISTER (PORTD_BASE_ADDRESS + 0x08)

// MODER for communication
#define GPIO_8_MODER                (uint32_t)(0x03<<16)
#define GPIO_8_MODER_OUT            (uint32_t)(0x01<<16)
#define GPIO_8_MODER_AF             (uint32_t)(0x02<<16)
#define GPIO_8_MODER_IN             (uint32_t)(0x00<<16)
#define GPIO_9_MODER                (uint32_t)(0x03<<18)
#define GPIO_9_MODER_OUT            (uint32_t)(0x01<<18)
#define GPIO_9_MODER_AF             (uint32_t)(0x02<<18)

// OTYPER
#define GPIO_8_OTYPER               (((uint32_t)0x01)<<8)
#define GPIO_8_OTYPER_PP            0x00
#define GPIO_9_OTYPER               (((uint32_t)0x01)<<9)
#define GPIO_9_OTYPER_PP            0x00

// OSPEEDR
#define GPIO_8_OSPEEDR              (((uint32_t)0x03)<<16)
#define GPIO_8_OSPEEDR_FAST_SPEED   (((uint32_t)0x02)<<16)
#define GPIO_8_OSPEEDR_HIGH_SPEED   (((uint32_t)0x03)<<16)
#define GPIO_9_OSPEEDR              (((uint32_t)0x03)<<18)
#define GPIO_9_OSPEEDR_FAST_SPEED   (((uint32_t)0x02)<<18)
#define GPIO_9_OSPEEDR_HIGH_SPEED   (((uint32_t)0x03)<<18)

// PUPDR registers
#define GPIO_8_PUPDR_SHIFT         16
#define GPIO_8_PUPDR                (uint32_t)(0x03<<GPIO_8_PUPDR_SHIFT)
#define GPIO_8_PUPDR_NOPULL         0x00
#define GPIO_8_PUPDR_PU            (uint32_t)(0x01<<GPIO_8_PUPDR_SHIFT)
#define GPIO_8_PUPDR_PD            (uint32_t)(0x02<<GPIO_8_PUPDR_SHIFT)
#define GPIO_9_PUPDR                (uint32_t)(0x03<<18)
#define GPIO_9_PUPDR_NOPULL         0x00
#define GPIO_9_PUPDR_PU            (uint32_t)(0x01<<18)
#define GPIO_9_PUPDR_PD            (uint32_t)(0x02<<18)

// IDR register
#define GPIO_8_IDR               (uint32_t)(0x01<<8)
#define GPIO_9_IDR               (uint32_t)(0x01<<9)

//flags AFR1 Register:
#define GPIO_0_AFR1_AF2             2
#define GPIO_0_AFR1                 0xF
#define GPIO_6_AFR1                 0xF000000
#define GPIO_6_AFR1_AF2             0x2000000
//flags AFR2 or AFRH Register
#define GPIO_8_AFR2_SET_ALL				0xF
#define GPIO_8_AFR2_AF7					0x7
#define GPIO_9_AFR2_SET_ALL				(uint32_t)(0xF<<4)
#define GPIO_9_AFR2_AF7				(uint32_t)(0x7<<4)

// ODR:
#define GPIO_8_ODR_HIGH               (uint32_t)(0x01<<8)
#define GPIO_9_ODR_HIGH               (uint32_t)(0x01<<9)
// END DEFINITIONS FOR 

/* ---------------- Unified pin/port definitions ----------------*/
// We store a constant array with all the addresses. This can be indexed with enum port_t.
const uint32_t port_addresses[7] = {GPIOA_BASE_MORT, GPIOB_BASE_MORT, GPIOC_BASE_MORT, GPIOD_BASE_MORT, GPIOE_BASE_MORT,GPIOF_BASE_MORT, GPIOG_BASE_MORT};

// GPIO setup registers
// Access any of these with say ([PORT_REGISTER] + MODER_OFFSET) | (MODER_OUT << (MODER_NUM_BITS*pin))
// MODER: pg. 187
#define MODER_OFFSET                0x00 // Ofset from port base address
#define MODER_NUM_BITS              2    // Number of bits per pin. Multiply this by pin number to get how far to shift the MODER settings
#define MODER_CLEAR                 0b11 // All two bits
#define MODER_OUT                   0b01 // Output
#define MODER_IN                    0b00 // Input
#define MODER_AF                    0b10 // Alternative function
#define MODER_ANALOG                0b11 // Analog

#define OTYPER_OFFSET               0x04
#define OTYPER_NUM_BITS             1
#define OTYPER_CLEAR                0b01
#define OTYPER_PP                   0b00

#define OSPEEDR_OFFSET              0x08
#define OSPEEDR_NUM_BITS            2
#define OSPEEDR_CLEAR               0b11
#define OSPEEDR_HIGH_SPEED          0b11

#define PUPDR_OFFSET                0x0C
#define PUPDR_NUM_BITS              2
#define PUPDR_CLEAR                 0b11
#define PUPDR_NOPULL                0b00

#define IDR_OFFSET                  0x10
#define IDR_NUM_BITS                1
#define IDR_HIGH                    0b01

#define ODR_OFFSET                  0x14
#define ODR_NUM_BITS                1
#define ODR_HIGH                    0b01

#define AFR1_OFFSET                 0x20
#define AFR2_OFFSET                 0x24 // WARNING since AFR has 4 bits, it does not fit in one register. 
// Set AFR2 with (AFR_AF2 << (AFR_NUM_BITS*(pin-8))) since AFR2 holds pins 8,9,10,11,12,13,14,15
#define AFR_NUM_BITS                4
#define AFR_CLEAR                   0b1111
#define AFR_AF2                     0b0010



/* function definitions----------------------------------------------------------*/

/* hardware initializations */
void enablePortClock(uint8_t port_number){
  switch (port_number){
    case PORTA:
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    break;
    case PORTB:
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    break;
    case PORTC:
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    break;
    case PORTD:
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    break;
    case PORTE:
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    break;
    case PORTF:
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    break;
    case PORTG:
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    break;



  }
}

/**
  * @brief  Initializes any GPIO to be an output
  * @note   GPIO B0 is the one connected to LED 1. EDIT THIS function!
  * @param  port_number - 0, 1, 2 ... corresponding to PORTA, PORTB ...
  * @retval None
  */
void initGpioAsOutput(uint8_t port_number, uint8_t pin){
    // Get the address of the port
    uint32_t port_address = port_addresses[port_number];


     volatile uint32_t  * reg_pointer; // Volatile so the compiler doesn't go trying to optimize it

    /* GPIO Peripheral clock enable */
    enablePortClock(port_number);

    /* Pin configured as output */
    reg_pointer = (uint32_t *)(port_address + MODER_OFFSET);                              // Set pointer to the register we would like to change
    *reg_pointer = *reg_pointer & (~((uint32_t)(MODER_CLEAR << (MODER_NUM_BITS*pin))));   // Clear all two bits of GPIO MODER for whatever pin. 
    *reg_pointer = *reg_pointer | (MODER_OUT << (MODER_NUM_BITS*pin));                    // Set MODERx to be out (pg 187)

    /*Pin configured as push-pull */
    reg_pointer = (uint32_t *)(port_address + OTYPER_OFFSET);
    *reg_pointer = *reg_pointer & (~((uint32_t)(OTYPER_CLEAR << (OTYPER_NUM_BITS*pin)))); // Clear the one bit of GPIO OTYPER pin 0
    *reg_pointer = *reg_pointer | (OTYPER_PP << (OTYPER_NUM_BITS*pin));                    // Set OTy to be output push-pull (pg 188)

    /*Pin configured floating */
    reg_pointer = (uint32_t *)(port_address + PUPDR_OFFSET);
    *reg_pointer = *reg_pointer & (~((uint32_t)(PUPDR_CLEAR << (PUPDR_NUM_BITS*pin))));
    *reg_pointer = *reg_pointer | (PUPDR_NOPULL << (PUPDR_NUM_BITS*pin));              // Set output type to be no pullup or pull-down (pg 189)
}



/**
  * @brief  Initializes GPIO to be an input
  * @note   Why the heck didn't I do this sooner...
  * @param  None
  * @retval None
  */
void initGpioAsInput(uint8_t port_number, uint8_t pin, uint8_t pupdr)
{
    volatile uint32_t  * reg_pointer; 
    uint32_t port_address = port_addresses[port_number]; // Get the port address
    /* GPIO Peripheral clock enable */
    enablePortClock(port_number);

    /* GPIOC Pin as input*/
    reg_pointer = (uint32_t *)(port_address + MODER_OFFSET);    // Set pointer to the register we would like to change
    *reg_pointer = *reg_pointer & (~((uint32_t)(MODER_CLEAR << (MODER_NUM_BITS*pin))));   // Clear all two bits of GPIO MODER for the pin. 
    *reg_pointer = *reg_pointer | (MODER_IN << (MODER_NUM_BITS*pin));                      // Set MODERx to be input (pg 187)

    /*PUSH-PULL Pin*/
    /*GPIO pin configured as push-pull */
    reg_pointer = (uint32_t *)(port_address + OTYPER_OFFSET);
    *reg_pointer = *reg_pointer & (~((uint32_t)(OTYPER_CLEAR << (OTYPER_NUM_BITS*pin)))); // Clear the one bit of GPIO OTYPER pin 0
    *reg_pointer = *reg_pointer | (OTYPER_PP << (OTYPER_NUM_BITS*pin));             // Set OT0 to be output push-pull (pg 188)

    /*GPIOB0 configured to be FAST*/
    reg_pointer = (uint32_t *)(port_address + OSPEEDR_OFFSET);
    *reg_pointer = *reg_pointer & (~((uint32_t)(OSPEEDR_CLEAR << (OSPEEDR_NUM_BITS*pin)))); // Clear the bits of OSPEEDR
    *reg_pointer = *reg_pointer | (OSPEEDR_HIGH_SPEED << (OSPEEDR_NUM_BITS*pin));             // Set input to high speed (I wonder what the tradeoffs are)
    
    // Set pullup or pulldown
    reg_pointer = (uint32_t *)(port_address + PUPDR_OFFSET);
    *reg_pointer = *reg_pointer & (~((uint32_t)(PUPDR_CLEAR << (PUPDR_NUM_BITS*pin))));  
    *reg_pointer = *reg_pointer | (pupdr << (PUPDR_NUM_BITS*pin));  // Set pin to be a pulldown or pullup (pg 189)
    
}


/**
  * @brief  Initializes GPIO to be an ANALOG INPUT
  * @note   nothing to see here *smirk*
  * @param  None
  * @retval None
  */
void initGpioAsAnalog(uint8_t port_number, uint8_t pin, uint8_t pupdr)
{
  // FIRST THINGS FIRST let's save some work by setting it up as an input
  // After that the only thing we need to change is the input-y-ness to analog-y-ness
  initGpioAsInput(port_number, pin, pupdr); // This enables the clock and stuff too

  volatile uint32_t  * reg_pointer; 
  uint32_t port_address = port_addresses[port_number]; // Get the port address

  /* GPIO Pin as ANALOG*/
  reg_pointer = (uint32_t *)(port_address + MODER_OFFSET);    // Set pointer to the register we would like to change
  *reg_pointer = *reg_pointer & (~((uint32_t)(MODER_CLEAR << (MODER_NUM_BITS*pin))));   // Clear all two bits of GPIO MODER for the pin. 
  *reg_pointer = *reg_pointer | (MODER_ANALOG << (MODER_NUM_BITS*pin));                      // Set MODERx to be analog (pg 187)
    
}

/** 
 * @brief   Initializes GPIO to be alternate function 2 AKA hooked to timer 3/4/5
 * @note    How do pros do this?
 * @param   port_number - the number (0,1,2...) corresponding to port (A,B,C...)
 * @param   pin - pin number 0-15
 * @param   pupdr - two-bit pullup/pulldown state
 * @retval  None
 */
void initGpioAsAF2(uint8_t port_number, uint8_t pin, uint8_t pupdr){

  // Alternate function 2 can either be an input or an output.
  // Set up pin as input before changing mode 
  // (This gives us control over pullup/pulldown)
  initGpioAsInput(port_number, pin, pupdr); // This also enables clock etc.

  volatile uint32_t  * reg_pointer; 
  uint32_t port_address = port_addresses[port_number]; // Get the port address

  // 2. Set MODER in GPIOx_MODER to alternative function mode
  reg_pointer = (uint32_t *)(port_address + MODER_OFFSET);                              // Set pointer to the register we would like to change
  *reg_pointer = *reg_pointer & (~((uint32_t)(MODER_CLEAR << (MODER_NUM_BITS*pin))));   // Clear all two bits of GPIO MODER pin. 
  *reg_pointer = *reg_pointer | (MODER_AF << (MODER_NUM_BITS*pin));                     // Set MODERx to be alternate function (pg 187)

  // 6. Set AFR  to have alternative function 2

  // First see if we need to use AFR1 or AFR2

  if(pin<8){
    // AFR1 time
    reg_pointer = (uint32_t *)(port_address + AFR1_OFFSET);
    *reg_pointer = *reg_pointer & (~((uint32_t)(AFR_CLEAR << (AFR_NUM_BITS*pin))));     // Clear all FOUR AFRL bits 
    *reg_pointer = *reg_pointer | (AFR_AF2 << (AFR_NUM_BITS*pin));                     // Set GPIO to be alternative function 2 (pg 192)

  } else {
    // AFR2 baby. Subtract 8 from pin before shifting bits
    reg_pointer = (uint32_t *)(port_address + AFR2_OFFSET);
    *reg_pointer = *reg_pointer & (~((uint32_t)(AFR_CLEAR << (AFR_NUM_BITS*(pin-8))))); // Clear all FOUR AFRH bits 
    *reg_pointer = *reg_pointer | (AFR_AF2 << (AFR_NUM_BITS*(pin-8)));                 // Set GPIO to be alternative function 2 (pg 192)
  }

  


}



/**
  * @brief  Initializes GPIO D8 and D9 as alternative function to be used by USART3 to communicate to the computer
  * @note   Do NOT EDIT!
  * @param  None
  * @retval None
  */
void initGpioD8D9AsAF7( void )
{
      volatile uint32_t  * reg_pointer;
      
      /* GPIOD Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    /* GPIO D 8 and 9 as alternative function */ 
    reg_pointer = (volatile uint32_t *)PORTD_MODER_REGISTER;
    *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_8_MODER + (uint32_t)GPIO_9_MODER));
    *reg_pointer = *reg_pointer | GPIO_8_MODER_AF | GPIO_9_MODER_AF;
    //to test that the clock is working:
    //*reg_pointer = *reg_pointer | (uint32_t)GPIO_8_MODER_OUT | (uint32_t)GPIO_9_MODER_OUT;
    
    /* GPIOD pin 8 and 9 push pull -> no, open drain-> No another thing says yes to push pull!*/
    reg_pointer = (volatile uint32_t *)PORTD_OTYPER_REGISTER;
    *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_8_OTYPER + (uint32_t)GPIO_9_OTYPER));
    *reg_pointer = *reg_pointer | GPIO_8_OTYPER_PP | GPIO_9_OTYPER_PP;
    
    /* GPIOD pin 8 and 9 as high speed */
    reg_pointer = (volatile uint32_t *)PORTD_OSPEEDR_REGISTER;
    *reg_pointer = *reg_pointer | (uint32_t)GPIO_8_OSPEEDR_HIGH_SPEED | (uint32_t)GPIO_9_OSPEEDR_HIGH_SPEED;
    
    /* GPIOD pin 8 and 9 pulled up */
    reg_pointer = (volatile uint32_t *)PORTD_PUPDR_REGISTER;
    *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_8_PUPDR + (uint32_t)GPIO_9_PUPDR));
    *reg_pointer = *reg_pointer | (uint32_t)GPIO_8_PUPDR_PU | (uint32_t)GPIO_9_PUPDR_PU;
    
     /*GPIO D 8 and 9 as alternative function 7*/   
     reg_pointer = (volatile uint32_t *)PORTD_AFR2_REGISTER;
    *reg_pointer = *reg_pointer & (~((uint32_t)GPIO_8_AFR2_SET_ALL	+ (uint32_t)GPIO_9_AFR2_SET_ALL));
    *reg_pointer = *reg_pointer | (uint32_t)GPIO_8_AFR2_AF7 | (uint32_t)GPIO_9_AFR2_AF7;    
	
	//Drive them high to test: (this was just to test that the clock was turning on, it is)
	 /* GPIOD 8 and 9 driven high to start out with */
    //reg_pointer = (volatile uint32_t *)PORTD_ODR_REGISTER;
    //*reg_pointer = *reg_pointer | GPIO_8_ODR_HIGH | GPIO_9_ODR_HIGH;

}

/* hardware actions */

/**
  * @brief  Toggles the value of GPIO pin
  * @note         Read the current value of GPIOB0
  *               Change the value depending on the current value
  * @param  None
  * @retval None
  */
void toggleGpioPin(uint8_t port_number, uint8_t pin)
{
    
    volatile uint32_t  * reg_pointer;
    uint32_t port_address = port_addresses[port_number];
    // do some MAGIC
    reg_pointer = (uint32_t *)(port_address + ODR_OFFSET);
    // Toggle the bit using the XOR bitmask
    *reg_pointer = *reg_pointer ^ (ODR_HIGH << (ODR_NUM_BITS*pin));
    // (learned this in Mechatronics 1 so stop me if I'm wrong)

}
/**
  * @brief  Sets the value of GPIOB0 high
  * @note   Set the value of GPIOB0 high
  * @param  None
  * @retval None
  */
void setGpioPin(uint8_t port_number, uint8_t pin)
{
    volatile uint32_t * reg_pointer;
    uint32_t port_address = port_addresses[port_number]; // Get the port address
    reg_pointer = (uint32_t *)(port_address + ODR_OFFSET);
    // Set the bit using an OR bitmask
    *reg_pointer = *reg_pointer | (ODR_HIGH << (ODR_NUM_BITS*pin));
    
}
/**
  * @brief  Clears the value of GPIOB0 
  * @note   Seems pretty clear to me                                        (I'll see myself out)
  * @param  None
  * @retval None
  */
void clearGpioPin(uint8_t port_number, uint8_t pin)
{
    volatile uint32_t  * reg_pointer;
    uint32_t port_address = port_addresses[port_number]; // Get the port address
    reg_pointer = (uint32_t *)(port_address + ODR_OFFSET);
    // Set the bit using an AND bitmask
    *reg_pointer = *reg_pointer & (~(ODR_HIGH << (ODR_NUM_BITS*pin)));
    
        
}

/**
  * @brief  Check the value of GPIO input pin (IDR)
  * @note   glub glub ><> . '
  * @param  Port number, pin
  * @retval >0 if pin is being pulled high
  */
uint32_t checkGpioPin(uint8_t port_number, uint8_t pin)
{
    uint32_t value;
    volatile uint32_t  * reg_pointer;
    uint32_t port_address = port_addresses[port_number];
    reg_pointer = (uint32_t *)(port_address + IDR_OFFSET); // Input data register
    //get the current value of the pin
    // To read a bit, do a little AND action
    value = *reg_pointer & (IDR_HIGH << (IDR_NUM_BITS*pin)); // >0 iff the pin-th bit of PORTx_IDR is set

    return value;
    
}
//EOF
