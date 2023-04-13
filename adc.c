#include "adc.h"

uint8_t ch_1=0;

void adc_init(void){
    
    //PIE1bits.ADIE=1;//HABILITAR INT DEL ADC
    //IPR1bits.ADIP=1; //Prioridad de interrupciín alta
    //PIR1bits.ADIF=0;//LIMPIAR LA BANDERA DE INT DEL ADC
    ADCON1bits.PCFG=0b1101; //AN-0 Y AN-1 como Entradas Analógicas
    ADCON1bits.VCFG=0; //Tomar Vref el voltaje del uC 0-5 V
    ADCON0=0; //ADC OFF and CH0 selected
    ADCON2bits.ACQT=0b010; //4TAD
    ADCON2bits.ADCS=0b110;//de acuerdo a tabla del DataSheet
    ADCON2bits.ADFM=1;//justificado a la derecha

}

void adc_start(void){
     ADCON0bits.ADON=1;//encender adc
     __delay_us(30);
     ADCON0bits.GO=1; //iniciar conversión
}

void adc_stop(void){
        ADCON0bits.ADON=0;//apagar adc
}
/*
uint16_t adc_read(uint8_t ch){
    
    uint16_t adc_=0;

    if(ch_1!=ch){
        adc_stop();
        ADCON0bits.CHS = ch;
        __delay_us(500);
        adc_start();
    }
    adc_= ADRES;
    ch_1=ch;
    
    return adc_;

}
*/

uint16_t adc_read_ch(uint8_t ch){
    uint16_t adc_=0;

    if(ch_1!=ch){
        adc_stop();
        ADCON0bits.CHS = ch;
        __delay_us(500);
    }
    
    ADCON0bits.ADON=1;
    ADCON0bits.GO_DONE=1;
    while(ADCON0bits.GO_DONE);//wait until conversion is finished
    ADCON0bits.ADON=0;//apagar adc
    adc_=ADRES;
    
    ch_1=ch;
    
    return adc_;
}

/*
*************** CONFIGURACIÓN DEL TAD *****************+
     *48MHz --> Tosc = 1/48MHz = 20.833333 ns
     * TAD(mín) > 0.7 us 
     * entonces TAD(min)< xTosc (donde x = 2,4,8,16,32 y 64)
     * 0.7us <= 64 Tosc (1.333us)
     * entonces 1 TAD = 64 TOSC = 1.333us
     * EL TIEMPO DE ADQUISICIÓN MÍNIMO PARA EL ADC ES 2.45us
     * entonces 2.45 us < xTAD (donde x = 0 a 20)
     * 2.45us < (4 *TAD) = 4*1.333us --> 010
     
 */
