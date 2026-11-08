/**
  ******************************************************************************
  * @file    stm32f446xx_dma_controller_mort.c 
  * @author  YOURNAME
  * @version 
  * @date    
  * @brief   Controls STM32F446ze Direct Memory Address
  ******************************************************************************
  */

/* Includes----------------------------------------------------------*/
#include "stm32f446xx_dma_controller_mort.h"
#include "stm32f446xx_rcc_mort.h"
#include "stm32f446xx_adc_mort.h"



/* MACRO definitions----------------------------------------------------------*/
//REGISTER ADDRESSES:
#define DMA2_BASE_ADDRESS                     ((uint32_t)0x40026400)
#define DMA2_LISR_REGISTER                    (DMA2_BASE_ADDRESS + 0x00)
#define DMA2_HISR_REGISTER                    (DMA2_BASE_ADDRESS + 0x04)
#define DMA2_LIFCR_REGISTER                   (DMA2_BASE_ADDRESS + 0x08)
#define DMA2_HIFCR_REGISTER                   (DMA2_BASE_ADDRESS + 0x0C)
#define DMA2_S0CR_REGISTER                    (DMA2_BASE_ADDRESS + 0x10)
#define DMA2_S0NDTR_REGISTER                  (DMA2_BASE_ADDRESS + 0x14)
#define DMA2_S0PAR_REGISTER                   (DMA2_BASE_ADDRESS + 0x18)
#define DMA2_S0M0AR_REGISTER                  (DMA2_BASE_ADDRESS + 0x1C)
#define DMA2_S0M1AR_REGISTER                  (DMA2_BASE_ADDRESS + 0x20)
#define DMA2_S0FCR_REGISTER                   (DMA2_BASE_ADDRESS + 0x24)

// Bits and flags and stuff
//DMA_SxCR: Dma stream x configuration register
#define DMA_SxCR_CHANNEL_2_SELECT             (((uint32_t)2)<<25)  // Channel 2
#define DMA_SxCR_MSIZE_HALF_WORD              (((uint32_t)1)<<13)  //the location memory is 16 bits in length
#define DMA_SxCR_PSIZE_HALF_WORD              (((uint32_t)1)<<11)  //the peripheral data size is 16 bits in length
#define DMA_SxCR_MINC_INCREMENT               (((uint32_t)1)<<10)  //increment the place where you store the data each transfer
#define DMA_SxCR_CIRC_ENABLE                  (((uint32_t)1)<<8)   //ENABLE circular mode
#define DMA_SxCR_DIR_PERTOMEM                 0
#define DMA_SxCR_STREAM_ENABLE                1


// FLAGS:


/* Module Variables----------------------------------------------------------*/
volatile uint16_t adcDmaDataStorageBufferSize3[3]; // To store yer ADC data. 
// I had to tag it as volatile. Otherwise, since the code never explicitly changes it,
// compiler will somehow optimize it to a constant.

/* Functions----------------------------------------------------------*/
// Set up direct memory access controller to yoink data from ADC3 data register
// and stick it in adcDmaDataStorageBufferSize3.
void initDMAForAdc3_3channels(void){
  volatile uint32_t * reg_pointer;
  // Start DMA clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);


  // Configure DMA stream 0 to use ADC3 channel 2
  reg_pointer = (uint32_t *)DMA2_S0CR_REGISTER;
  *reg_pointer = DMA_SxCR_CHANNEL_2_SELECT + DMA_SxCR_MSIZE_HALF_WORD
    + DMA_SxCR_PSIZE_HALF_WORD + DMA_SxCR_MINC_INCREMENT + DMA_SxCR_DIR_PERTOMEM
    + DMA_SxCR_CIRC_ENABLE; // TODO understand these

  // Set number of data registers to transfer
  reg_pointer = (uint32_t *)DMA2_S0NDTR_REGISTER;
  *reg_pointer = 3; // That would be THREE

  // Pick register to transfer from (ADC3 data register)
  reg_pointer = (uint32_t *)DMA2_S0PAR_REGISTER;
  *reg_pointer = (ADC3_BASE_ADDRESS + ADC_DR_OFFSET); // put a memory address in the memory address

  // Transfer it to the storage buffer variable
  reg_pointer = (uint32_t *)DMA2_S0M0AR_REGISTER;
  *reg_pointer = (uint32_t)&adcDmaDataStorageBufferSize3[0]; // The memory address of the first item in it.
}

// Set the DMA_S0CR_STREAM_ENABLE bit
void enableDMAForAdc3_3channels(void){
  volatile uint32_t * reg_pointer;
  reg_pointer = (uint32_t *)DMA2_S0CR_REGISTER;
  *reg_pointer = *reg_pointer | DMA_SxCR_STREAM_ENABLE;
}

// Return any of the three stored values in the ADC DMA storage buffer
uint16_t returnADC3StoredValue(uint8_t index){
  return adcDmaDataStorageBufferSize3[index];
}

/* Interrupt Handler----------------------------------------------------------*/



//EOF