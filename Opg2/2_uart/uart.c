#include <stdint.h>
#include <stdio.h>
#include <gpio.h>
#include "uart.h"

#define UART ((NRF_UART_REG*)0x40002000)

typedef struct {
    volatile uint32_t TASKS_STARTRX;
    volatile uint32_t TASKS_STOPRX;
    volatile uint32_t TASKS_STARTTX;
    volatile uint32_t TASKS_STOPTX;
    volatile uint32_t RESERVED0[3];
    volatile uint32_t TASKS_SUSPEND;
    volatile uint32_t RESERVED1[56];
    volatile uint32_t EVENTS_CTS;
    volatile uint32_t EVENTS_NCTS;
    volatile uint32_t EVENTS_RXDRDY;
    volatile uint32_t RESERVED2[4];
    volatile uint32_t EVENTS_TXDRDY;
    volatile uint32_t RESERVED3[1];
    volatile uint32_t EVENTS_ERROR;
    volatile uint32_t RESERVED4[7];
    volatile uint32_t EVENTS_RXTO;
    volatile uint32_t RESERVED5[46];
    volatile uint32_t SHORTS;
    volatile uint32_t RESERVED6[64];
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t RESERVED7[93];
    volatile uint32_t ERRORSRC;
    volatile uint32_t RESERVED8[31];
    volatile uint32_t ENABLE;
    volatile uint32_t RESERVED9[1];
    volatile uint32_t PSELRTS;
    volatile uint32_t PSELTXD;
    volatile uint32_t PSELCTS;
    volatile uint32_t PSELRXD;
    volatile uint32_t RXD;
    volatile uint32_t TXD;
    volatile uint32_t RESERVED10[1];
    volatile uint32_t BAUDRATE;
    volatile uint32_t RESERVED11[17];
    volatile uint32_t CONFIG;
} NRF_UART_REG;


void uart_init(){
    GPIO->PIN_CNF[6] = 1; //Transmitter, output
    GPIO->PIN_CNF[8] = 0; //Receiver, input

    UART->PSELTXD = 6;
    UART->PSELRXD = 8;

    UART->BAUDRATE = 0x00275000; //Baudrate på 9600

    UART->PSELRTS = 0xFFFFFFFF; //Fortelle at vi ikke har RTS
    UART->PSELCTS = 0xFFFFFFFF; //Fortelle at vi ikke har CTS

    UART->ENABLE = 4; //Enable UART

    UART->TASKS_STARTRX = 1; //Starter å motta meldinger
}

void uart_send(char letter){
    UART->TASKS_STARTTX = 1;
    UART->TXD = letter;
    while (!UART->EVENTS_TXDRDY); // Vent til sendingen er ferdig
    UART->EVENTS_TXDRDY = 0; // Nullstill TXDRDY-eventet
    UART->TASKS_STOPTX;
}

char uart_read(){
    char received_char = '\0'; // Default return value hvis ingen bokstav er klar

    if (UART->EVENTS_RXDRDY) { // Sjekker om det er data tilgjengelig for lesing
        received_char = UART->RXD; // Leser data fra RXD-registeret
        UART->EVENTS_RXDRDY = 0; // Nullstiller RXDRDY-eventet for å unngå pakketap
        UART->TASKS_STARTRX = 1; // Starter mottaksprosessen igjen
    }

    return received_char;
}