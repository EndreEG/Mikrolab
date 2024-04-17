#include "uart.h"

int press_button1();
int press_button2();

int main(){
    uart_init();
    uart_send(4);

    //Konfigurer LED-matrisen og skrur de av ved oppstart
	for(int i = 17; i <= 20; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTSET = (1 << i);
	}
    volatile int LED_tilstand = 0;

    //Konfigurer knappene 1 og 2
    GPIO->PIN_CNF[13] = 3 << 2;
    GPIO->PIN_CNF[14] = 3 << 2;

    int sleep = 0;  //Definerer en variabel for delay-tid
    char received_char; //Definerer en variabel for mottatt bokstav
    while(1){
        //Sender bokstaver ut fra hvilken knapp som har blitt trykket
        if(press_button1()){
            uart_send("A");
        }
        if(press_button2()){
            uart_send("B");
        }

        //Sjekker om det har blitt sendt en bokstav fra datamaskinen til mikrokontrolleren
        //Skrur deretter enten av eller på LED-lysene, avhengig av deres forrige tilstand
        received_char = uart_read();
        // Sjekker om det mottatte tegnet er en gyldig bokstav
        if ((received_char >= 'A' && received_char <= 'Z') || (received_char >= 'a' && received_char <= 'z')){
            if(LED_tilstand == 0){
                for (int i = 17; i <= 20; i++){
			        GPIO->OUTCLR = (1 << i); // Slår på LED-lysene
		        }
                LED_tilstand = 1;
            }
            if(LED_tilstand == 1){
                for (int i = 17; i <= 20; i++){
			        GPIO->OUTSET = (1 << i); // Slår av LED-lysene
		        }
                LED_tilstand = 0;
            }
            
        }

        //Delay
        sleep = 10000;
		while(--sleep);
    }

    return 0;
}


int press_button1(){
    return(!(GPIO->IN & (1 << 13)));
}
int press_button2(){
    return(!(GPIO->IN & (1 << 14)));
}