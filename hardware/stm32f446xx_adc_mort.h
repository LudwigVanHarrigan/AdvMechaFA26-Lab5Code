/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F446XX_ADC_MORT_H_
#define __STM32F446XX_ADC_MORT_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32f446xx_mort.h"

/* Macros for Everyone--------------------------------------------------------*/

//REGISTER ADDRESSES that other modules may need
// (DMA needs ADC3_base_address and ADC_DR_OFFSET)

// Register Map: pg. 399
// ADC_whichone_registername_REGISTER
#define ADC_BASE_ADDRESS ADC1_BASE_MORT // Don't ask me, but this is how it is
#define ADC1_BASE_ADDRESS ADC_BASE_ADDRESS+0x000
#define ADC2_BASE_ADDRESS ADC_BASE_ADDRESS+0x100
#define ADC3_BASE_ADDRESS ADC_BASE_ADDRESS+0x200
#define ADC_COMMON_ADDRESS ADC_BASE_ADDRESS+0x300 // melisa calls this ADC_BASE_MORT for some reason

#define ADC_COMMON_CCR_REGISTER (ADC_COMMON_ADDRESS + 0x04) // pg 396
#define ADC_CCR_ADCPRE_4 (uint32_t)(0b01 << 16) // Sets prescaler to 4

#define ADC_CR1_OFFSET    0x04  
#define ADC_CR1_SCAN      (0x01 << 8) // pg.386

#define ADC_CR2_OFFSET    0x08        // pg.387
#define ADC_CR2_SWSTART   (0x01 << 30)
#define ADC_CR2_EOCS      (0x01 << 10)  // when set: EOC set per conversion instead of at the end of a sequence 
#define ADC_CR2_ADON      (0x01 << 0)   // Enable ADC out of power save mode
#define ADC_CR2_CONT      (0x01 << 1)   // Continuous conversion mode
#define ADC_CR2_DDS       (0x01 << 9)   // DMA Disable Selection: make DMA requests be issued as long as data are converted
#define ADC_CR2_DMA       (0x01 << 8)   //Enable DMA (duu-uuuh)

#define ADC_SMPR1_OFFSET 0x0C  
#define ADC_SMPR2_OFFSET 0x10  	// ADC Sample Time Register 2 (pg. 390)
#define ADC_SMPR2_SMP_480 0b111 // Just check the datasheet
#define ADC_SMPR2_SMP5_SHIFT 15
#define ADC_SMPR2_SMP6_SHIFT 18
#define ADC_SMPR2_SMP7_SHIFT 21


#define ADC_JOFR1_OFFSET 0x14  
#define ADC_JOFR2_OFFSET 0x18  
#define ADC_JOFR3_OFFSET 0x1C  
#define ADC_JOFR4_OFFSET 0x20  
#define ADC_HTR_OFFSET   0x24  
#define ADC_LTR_OFFSET   0x28 

#define ADC_SQR1_OFFSET  0x2C // Regular Sequence Register (pg. 391)
#define ADC_SQR1_3CONVERSIONS ((3-1) << 20)



#define ADC_SQR2_OFFSET  0x30 
#define ADC_SQR3_OFFSET  0x34 // Regular Sequence Register 3 (pg.393)
#define ADC_SQR3_SQ1_SHIFT 0
#define ADC_SQR3_SQ2_SHIFT 5
#define ADC_SQR3_SQ3_SHIFT 10
#define ADC_SQR3_SQ4_SHIFT 15
#define ADC_SQR3_SQ5_SHIFT 20
#define ADC_SQR3_SQ6_SHIFT 25
// NOTE to get the rest of these see SQR1 and SQR2
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2
#define ADC_CHANNEL_3 3
#define ADC_CHANNEL_4 4
#define ADC_CHANNEL_5 5 // No shit, Sherlock
#define ADC_CHANNEL_6 6
#define ADC_CHANNEL_7 7
// NOTE this goes up to 18
// NOTE to set channel 5 as the first channel and channel 2 as the second:
// ADC_3_SQR3_REGISTER = (ADC_CHANNEL_5 << ADC_SQR3_SQ1_SHIFT) + (ADC_CHANNEL_2 << ADC_SQR3_SQ2_SHIFT)


#define ADC_JSQR_OFFSET  0x38 
#define ADC_JDR1_OFFSET  0x3C 
#define ADC_JDR2_OFFSET  0x40 
#define ADC_JDR3_OFFSET  0x44 
#define ADC_JDR4_OFFSET  0x48 
#define ADC_DR_OFFSET    0x4C





/*Function definitions---------------------------------------------------------*/
void initADC3_567_withDMA (void);
void enableADC3(void);
void startADCConversion(void);
uint16_t getCurrentADC3Value(void);


#endif /*__STM32F446XX_ADC_MORT_H_ */