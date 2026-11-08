/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32f446XX_DMA_CONTROLLER_MORT_H_
#define __STM32f446XX_DMA_CONTROLLER_MORT_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f446xx_mort.h"


/* Macros for Everyone--------------------------------------------------------*/



/*Function definitions---------------------------------------------------------*/
void initDMAForAdc3_3channels(void);
void enableDMAForAdc3_3channels(void);
uint16_t returnADC3StoredValue(uint8_t index);


#endif /*__STM32f446XX_DMA_CONTROLLER_MORT_H_ */