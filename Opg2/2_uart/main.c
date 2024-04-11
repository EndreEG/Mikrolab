#include "uart.h"

int main(){
    uart_init();
    uart_send(4);
    return 0;
}