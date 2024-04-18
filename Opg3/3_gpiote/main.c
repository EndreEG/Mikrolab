#include <stdint.h>
#include <stdio.h>
#include "gpio.h"

int main(){
    //Skru av LED-matrisen ved oppstart
	for(int i = 17; i <= 20; i++){
		GPIO->OUTSET = (1 << i);
	}

    return 0;
}

