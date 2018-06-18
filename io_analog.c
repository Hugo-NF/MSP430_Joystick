/*
 * ADC.c
 *
 *  Created on: 17 de jun de 2018
 *      Author: JoséLuiz
 */

#define PORT_MAPS
#include "io.h"
#include "pins.h"

int analogReadPin(unsigned char Apin){

    int ret = 0;

    setupADC_single_mode(Apin);                 // Setup do ADC12 para single mode

    //------------INICIA AMOSTRAGE E CONVERSAO-----------------
    SET_REG(ADC12CTL0, (ADC12ENC | ADC12SC));

    while(!ADC12IFG0){}                          // Aguarda conversao

    ret = ADC12MEM0;

    ADC12IV = 0;                                // zera td mundo
    ADC12IFG = 0;

    CLEAR_REG(ADC12CTL0, (ADC12ENC | ADC12SC)); // Pausa a amostragem e conversao

    return ret;
}

int analogReadTemp(){
    return 0;
}


//---------------FUNCOES PRIVADAS------------------------
void setupADC_single_mode(unsigned char Apin){
    uint8_t pin = inchToPin(Apin);
    uint8_t bit = Mask(pin);
    uint8_t port = Port(pin);

    volatile uint8_t *sel = portToSel(port);

    setPin(pin, INPUT);
    *sel |= bit;            //Seta SEL do pino p/ entrada analogica

    //-------------SETUP ADC ONE SINGLE MODE------------------

    // ADC12ON | 32 ciclos para amostragem
    ADC12CTL0 = (ADC12ON | ADC12SHT0_3);

    // ADC12_A Clock = ADC12OSC | ADC12_A Sample-and-hold = ADC12SC | Pulse Sample Mode | Single-channel, single-conversion | Channel 0
    ADC12CTL1 = (ADC12SHP | ADC12CONSEQ_0 | ADC12CSTARTADD_0);

    // Armazena conversão sem sinal 0 to 255
    ADC12CTL2 = (ADC12RES_0);

    //------------SETUP MEMORIA ADC ---------------------------
    // CANAL Apin | AVcc and AVss
    ADC12MCTL0 = (Apin);
}


