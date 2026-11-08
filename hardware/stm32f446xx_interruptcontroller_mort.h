/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F446XX_INTERRUPTCONTROLLER_MORT_H_
#define __STM32F446XX_INTERRUPTCONTROLLER_MORT_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32f446xx_mort.h"


/*Function definitions--------------------------------------------------------*/
// Tell NVIC it needs to keep an eye out for timer 3 and 4 interrupt stuff
void enableNVIC_T3(void);
void enableNVIC_T4(void);
void setExtInterrupt6ToPC6(void); // vestigal from part 1
void setExtInterrupt8ToPB8(void);

#endif /*__STM32F446XX_INTERRUPTCONTROLLER_MORT_H_*/