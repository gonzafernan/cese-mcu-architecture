/**
 * @brief Tools to measure the performance of each implementation.
*/

#include <stdint.h>
#include "stm32f4xx_hal.h"


/**
 * @brief Initialize performance counter.
*/
void perf_counter_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= 1 << DWT_CTRL_CYCCNTENA_Pos;
}

/**
 * @brief Reset performance counter.
*/
void perf_counter_reset(void)
{
    DWT->CYCCNT = 0;
}

/**
 * @brief Get performance counter value.
*/
uint32_t perf_counter_get(void)
{
    return DWT->CYCCNT;
}