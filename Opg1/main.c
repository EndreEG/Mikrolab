#include <stdint.h>
#include <stdio.h>

#define GPIO ((NRF_GPIO_REGS*)0x50000000)

typedef struct {
	volatile uint32_t RESERVED0[321];
	volatile uint32_t OUT;
	volatile uint32_t OUTSET;
	volatile uint32_t OUTCLR;
	volatile uint32_t IN;
	volatile uint32_t DIR;
	volatile uint32_t DIRSET;
	volatile uint32_t DIRCLR;
	volatile uint32_t RESERVED1[120];
	volatile uint32_t PIN_CNF[32];
} NRF_GPIO_REGS;

int ubit_button_press_a();
int ubit_button_press_b();

int main(){
	// Configure LED Matrix
	for(int i = 17; i <= 20; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTSET = (1 << i);
	}

	// Configure buttons
	GPIO->PIN_CNF[13] = 3 << 2;
	GPIO->PIN_CNF[14] = 3 << 2;


	int sleep = 0;
	while(1){
	if (ubit_button_press_a()) {
		for (int i = 17; i <= 20; i++)
		{
			GPIO->OUTCLR = (1 << i); // Slår på LED-lyset
		}
	}
	if(ubit_button_press_b()) {
		for (int i = 17; i <= 20; i++)
		{
			GPIO->OUTSET = (1 << i); // Slår på LED-lyset
		}
	}
		/*
		if(ubit_button_press_a() == 0){
			for(int i = 17; i <= 20; i++){
				GPIO->OUTCLR = (1 << i);
			}
		}
		if(ubit_button_press_b() == 0){
			for(int i = 17; i <= 20; i++){
				GPIO->OUTSET = (1 << i);
			}
		}
		*/
		
		


		sleep = 10000;
		while(--sleep); // Delay
		//GPIO->OUTSET = (1 << 20);
	}
	return 0;
}

int ubit_button_press_a(){
	return (!(GPIO->IN & (1 << 13)));
}

int ubit_button_press_b(){
	return (!(GPIO->IN & (1 << 14)));
}
