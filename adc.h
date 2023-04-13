  

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>
#define _XTAL_FREQ 48000000 

void adc_init(void);
uint16_t adc_read_ch(uint8_t ch);
//uint16_t adc_read(uint8_t ch);
void adc_start(void);
void adc_stop(void);

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