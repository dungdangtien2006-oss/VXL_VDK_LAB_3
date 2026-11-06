#include "timer.h"

#define NO_OF_TIMERS 2
static volatile uint32_t timer_counter[NO_OF_TIMERS] = {0, 0};
static volatile uint8_t  timer_flag[NO_OF_TIMERS] = {0, 0};

void setTimer(uint8_t index, uint32_t duration_ms) {
    if (index >= NO_OF_TIMERS) return;
    timer_counter[index] = duration_ms / TIMER_CYCLE;
    timer_flag[index] = 0;
}

uint8_t timer_expired(uint8_t index) {
    if (index >= NO_OF_TIMERS) return 0;
    if (timer_flag[index]) {
        timer_flag[index] = 0;
        return 1;
    }
    return 0;
}

void timer_run(void) {
    for (uint8_t i = 0; i < NO_OF_TIMERS; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] == 0) {
                timer_flag[i] = 1;
            }
        }
    }
}
