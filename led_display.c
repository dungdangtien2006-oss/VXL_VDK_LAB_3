#include "led_display.h"


#define SEG_MASK 0x007F
#define EN_SHIFT 11
#define EN_MASK  (0x0Fu << EN_SHIFT)

static const uint8_t seg_code[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static uint8_t led_buffer[4] = {0, 0, 0, 0};
static uint8_t led_index = 0;

void update_led_buffer(uint8_t left, uint8_t right) {
    led_buffer[0] = (left / 10) % 10;
    led_buffer[1] = left % 10;
    led_buffer[2] = (right / 10) % 10;
    led_buffer[3] = right % 10;
}

void led_scan(void) {
    uint32_t seg = seg_code[led_buffer[led_index]];
    GPIOA->BSRR = EN_MASK;
    GPIOB->BSRR = (~seg & SEG_MASK) | ((seg & SEG_MASK) << 16);
    GPIOA->BSRR = (1u << (EN_SHIFT + led_index)) << 16;
    led_index = (led_index + 1) % 4;
}
