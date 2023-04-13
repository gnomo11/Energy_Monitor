

#include <xc.h>  
#include <stdint.h>     
#define _XTAL_FREQ 48000000 

#define TRIS_KEYPAD    TRISD        // Definimos TRIS_KEYPAD = Registro TRISB 
#define PORT_KEYPAD    PORTD        // Definimos PORT_KEYPAD = Regitro PORTB
#define LAT_KEYPAD     LATD         // Definimos LAT_KEYPAD = Registro LATB

#define RENGLON0    0                 
#define RENGLON1    1
#define RENGLON2    2
#define RENGLON3    3
#define COLUMNA0    4
#define COLUMNA1    5
#define COLUMNA2    6
#define COLUMNA3    7

void kbd_init(void);
char kbd_getc(void);
void AntiRebote(void);              
__bit Columnas(void);             



