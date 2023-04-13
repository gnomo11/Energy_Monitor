
/**
 *                          TIMER 0
 * 
 * TIEMPO = (4/Fosc) * (prescaler) * ( (2^16 -1) - TMR0_H_L)
 * solving for TMR0_H_L
 *
 */
#include "timer0.h"
void tmr0_init(void){
    
    T0CONbits.T0CS=0;//Internal instruction cycle clock (CLKO) 
    T0CONbits.PSA=0;//Timer0 prescaler is assigned!
    
    T0CONbits.T0PS=0b111; //prescaler = 256
    T0CONbits.T08BIT=0; //Timer de 16bits
    
    INTCONbits.TMR0IF = 0; //Flag del Timer 0 deshabilitado
    INTCONbits.TMR0IE = 1; //Habilitra interrupciones por desbordamiento
    INTCON2bits.TMR0IP = 1; //Prioridad baja de interrupci�n
    TMR0L = tmr0_load;
    TMR0H = (tmr0_load >> 8);
    T0CONbits.TMR0ON=1; //Start Timer0
}

/*
//interrupcion cada 1 segundos
void __interrupt(low_priority) ISR_TIMER_0(void){
    
    if(INTCONbits.TMR0IF){

        INTCONbits.TMR0IF=0;
    }
}
 */