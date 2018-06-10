#include <msp430.h> 
#include "lcd.h"
#include "pins.h"
#include "msp_timers.h"


volatile int eixo_x=0, eixo_y=0;

#pragma vector=ADC12_VECTOR
__interrupt void ADC12_A0_ISR(){

    // Pausa a amostragem e conversao
    CLEAR_REG(ADC12CTL0, (ADC12ENC | ADC12SC));

    if(ADC12IFG0)   // caso interrupcao de M0
        eixo_x = ADC12MEM0;

    if(ADC12IFG1)   // caso interrupcao de M1
        eixo_y = ADC12MEM1;

    lcd_handler(eixo_x, eixo_y);


    ADC12IV = 0;   // zera td mundo
    ADC12IFG = 0;
}

void readADC12(){
    SET_REG(ADC12CTL0, (ADC12ENC | ADC12SC));
}


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

	//-------------SETUPS------------------

	// ADC12ON | 256 ciclos de para amostragem | Modo continuo de amostragem e conversao
	ADC12CTL0 = (ADC12ON | ADC12SHT0_3 | ADC12MSC);

	// ADC12_A Clock = ADC12OSC | ADC12_A Sample-and-hold = ADC12SC | Pulse Sample Mode | Repeat-Sequence-of-Channels | Channel 0
	ADC12CTL1 = (ADC12SHP | ADC12CONSEQ_3 | ADC12CSTARTADD_0);

	// Armazena conversão sem sinal 0 to 255
	ADC12CTL2 = (ADC12RES_0);

	// CANAL A0 -> P6.0 | AVcc and AVss
	ADC12MCTL0 = (ADC12INCH_0);

	// CANAL A1 -> P6.1 | AVcc and AVss | Final de sequencia
	ADC12MCTL1 = (ADC12INCH_1 | ADC12EOS);

	// Ativa interrupcao para A0 e A1
	ADC12IE = (BIT0 | BIT1);

	// P6.0 (Eixo_X) ativo como input e selecionado para canal A0
	setPin(P6_0, INPUT);
	SET_REG(P6SEL, BIT0);

	// P6.1 (Eixo_Y) ativo como input e selecionado para canal A1
	setPin(P6_1, INPUT);
	SET_REG(P6SEL, BIT1);

	// P4.2 SW ativo como input
	setPin(P4_2, INPUT_PULL_UP);

	for(;;){
	    readADC12();

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
