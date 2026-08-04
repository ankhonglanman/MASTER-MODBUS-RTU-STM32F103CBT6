#include "stm32f1xx_hal.h"
#include "application.h"

void Deep_Sleep(){
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

    __WFI();

    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}