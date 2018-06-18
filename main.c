#include <msp430.h> 
#include "lcd.h"
#include "MSPack.h"


volatile int eixo_x=0, eixo_y=0;


void main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// P1.0 LED Vermelho
	setPin(P1_0, OUTPUT);
	// P4.7 LED Verde
	setPin(P4_7, OUTPUT);

	// Led de ligado
	writePin(P4_7, HIGH);
	writePin(P1_0, LOW);

	__enable_interrupt();
	lcd_init(0x3F);
	lcd_create_arrows();

	// P4.2 SW ativo como input
	setPin(P4_2, INPUT_PULL_UP);

	for(;;){

	    eixo_x = analogReadPin(A0);
	    eixo_y = analogReadPin(A1);

	    lcd_handler(eixo_x, eixo_y);

	    if(!readPin(P4_2)){
	        // Toogle LEDs
	        tooglePin(P1_0);

	        lcd_toogle_view();

	        // Debounce
	        delay(25);
	        while(!readPin(P4_2)){};
	        delay(25);
	    }
	}
}
