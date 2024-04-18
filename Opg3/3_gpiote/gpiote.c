#include <stdint.h>
#include <stdio.h>
#include "gpiote.h"
#include "gpio.h"

void gpiote_init() {
    // Konfigurer BUTTON 1-pinne som input med pull-up motstand
    GPIO->PIN_CNF[13] = (0) | (3 << 2);

    // Initialiser GPIOTE-modulen
    GPIOTE->CONFIG[0] = (1) |
                        (13 << 8) |
                        (1 << 16);

    // Aktiver GPIOTE-modulen
    GPIOTE->INTENSET = 1;
    NVIC_EnableIRQ(GPIOTE_IRQn); // Aktiverer interrupt for GPIOTE
}
