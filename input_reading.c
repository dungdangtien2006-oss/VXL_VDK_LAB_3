#include "input_reading.h"
#include "timer.h"

#define NO_OF_BUTTONS  3
#define BUTTON_READ_MS ((TIMER_CYCLE > 10) ? TIMER_CYCLE : 10)
#define READ_DIVIDER   (BUTTON_READ_MS / TIMER_CYCLE)
#define DURATION_FOR_AUTO_INCREASING (1000 / BUTTON_READ_MS)
#define AUTO_REPEAT_TICKS            (500 / BUTTON_READ_MS)
#define BUTTON_IS_PRESSED  GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

static const uint16_t buttonPin[NO_OF_BUTTONS] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2};
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

static volatile uint8_t flagForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};
static volatile uint8_t clickFlag[NO_OF_BUTTONS] = {0, 0, 0};
static volatile uint8_t repeatFlag[NO_OF_BUTTONS] = {0, 0, 0};
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};
static uint16_t repeatCounter[NO_OF_BUTTONS] = {0, 0, 0};
static uint8_t readDivCounter = 0;

void button_reading(void) {
    if (++readDivCounter < READ_DIVIDER) return;
    readDivCounter = 0;

    for (uint8_t i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOA, buttonPin[i]);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            buttonBuffer[i] = debounceButtonBuffer1[i];
        }

        if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
            if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
                counterForButtonPress1s[i]++;
                if (counterForButtonPress1s[i] == 1) {
                    clickFlag[i] = 1;
                }
            } else {
                flagForButtonPress1s[i] = 1;
                if (++repeatCounter[i] >= AUTO_REPEAT_TICKS) {
                    repeatCounter[i] = 0;
                    repeatFlag[i] = 1;
                }
            }
        } else {
            counterForButtonPress1s[i] = 0;
            repeatCounter[i] = 0;
            flagForButtonPress1s[i] = 0;
        }
    }
}

unsigned char is_button_pressed(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0xff;
    return (flagForButtonPress1s[index] == 1);
}

unsigned char is_button_clicked(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    if (clickFlag[index]) {
        clickFlag[index] = 0;
        return 1;
    }
    return 0;
}

unsigned char is_button_repeat(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    if (repeatFlag[index]) {
        repeatFlag[index] = 0;
        return 1;
    }
    return 0;
}
