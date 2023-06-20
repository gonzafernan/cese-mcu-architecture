
#ifndef __PERF_COUNTER_H
#define __PERF_COUNTER_H

#include <stdint.h>

void perf_counter_init(void);
void perf_counter_reset(void);
uint32_t perf_counter_get(void);

#endif /* __PERF_COUNTER_H */