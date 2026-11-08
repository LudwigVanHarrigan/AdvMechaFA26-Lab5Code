/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F446XX_GPIO_MORT_H_
#define __STM32F446XX_GPIO_MORT_H_


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f446xx_mort.h"

/* Macros for Everyone--------------------------------------------------------*/
#define PIN_0   0
#define PIN_1   1
#define PIN_2   2
#define PIN_3   3
#define PIN_4   4
#define PIN_5   5
#define PIN_6   6
#define PIN_7   7
#define PIN_8   8
#define PIN_9   9
#define PIN_10  10

// Enum for ports
typedef enum {
	PORTA,
	PORTB,
	PORTC,
	PORTD,
	PORTE,
	PORTF,
	PORTG
} port_t;

#define PUPDR_CLEAR 0b11
#define PULLUP 0b01
#define PULLDOWN 0b10
#define NOPULL 0b00



/*Function declarations---------------------------------------------------------*/

void enablePortClock(uint8_t port_number);
void initGpioAsOutput(uint8_t port_number, uint8_t pin);
void toggleGpioPin(uint8_t port_number, uint8_t pin);
void setGpioPin(uint8_t port_number, uint8_t pin);
void clearGpioPin(uint8_t port_number, uint8_t pin);
void initGpioAsInput(uint8_t port_number, uint8_t pin, uint8_t pupdr);
uint32_t checkGpioPin(uint8_t port_number, uint8_t pin);
void initGpioAsAnalog(uint8_t port_number, uint8_t pin, uint8_t pupdr);
void initGpioAsAF2(uint8_t port_number, uint8_t pin, uint8_t pupdr);


void initGpioB0AsOutput( void );

void initGpioB12AsOutput( void );
void initGpioB13AsOutput( void );
void initGpioB15AsOutput( void );

void initGpioC6AsInput( void );

void initGpioB3AsInput(uint16_t pupdr);
void initGpioB8AsInput(uint16_t pupdr);

void initGpioD8D9AsAF7( void );
void toggleGPIOB0( void );
void setGPIOB0( void );
void clearGPIOB0( void );

void setGPIOB12( void );
void clearGPIOB12( void );
void setGPIOB13( void );
void clearGPIOB13( void );
void setGPIOB15( void );
void clearGPIOB15( void );


uint32_t checkGPIOC6(void);

uint32_t checkGPIOB3(void);
uint32_t checkGPIOB8(void);

void setGPIOB0_withC6( void );


#endif /*__STM32F446XX_GPIO_MORT_H_ */
