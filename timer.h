#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define TIMER_CYCLE 10

void setTimer(uint8_t index, uint32_t duration_ms);
uint8_t timer_expired(uint8_t index);
void timer_run(void);

#endif
