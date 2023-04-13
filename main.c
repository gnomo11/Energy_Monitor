/* PROGRAMA PRINCIPAL
 * File:   main.c
 * Author: 
 * Created on XX de XX de 2021, XX:XX PM
 */

#include <xc.h>         //compilador
#include <pic18f4550.h> //hardware
#include <stdio.h>      //standar IO
#include <stdint.h>     //para poder uint8_t = 8 bits || int = 16 bits
#include <stdlib.h>     //para las atof -> Cadena a flotante
#include <math.h>       //librería math
#include "Fuses.h"      //Configuración de los Fuses
#include "KBD_4X4.h"    //librería del teclado matricial
#include "lcd.h"        //librería LCD
#include "timer0.h"     //Librería para el TMR 0
#include "adc.h"        //Librería ADC


#define _XTAL_FREQ 48000000

#define val_ini_sp 50.0           // xx W
#define val_ini_tol 10.0          //tolerancia inicial 10%

#define n_muestras_I 500        //# de muestras que captura la Irms
#define ICAL 10.0                 // FACTOR DE CALIBRACIÓN (modificar en caso necesario)
#define I_conversion_val 0.49   // (5/1023)*(100A/1V)

#define n_muestras_V 500        //# de muestras que captura la Vrms
#define V_conversion_val 0.24   // (5/1023)*(250/5)

#define segundos 90             //segundos necesarios para hacer muestreo 

#define RELE_1 LATBbits.LATB0
#define RELE_2 LATBbits.LATB1
#define RELE_3 LATBbits.LATB2
#define RELE_4 LATBbits.LATB3

typedef struct{
    float I0;
    float V1;
    uint16_t muestras_adc0;
    uint16_t muestras_adc1;
}adc_variables;


adc_variables adc_result;



enum{
    false,
    true
};

enum{
    activado,
    desactivado
};

__bit ch1,ch2,ch3,ch4; //valores 0 y 1
float Irms=0.0,Vrms=0.0,P=0.0,sp = val_ini_sp, tolerancia = val_ini_tol/100.0;
char buffer_lcd[16],k,sp_c[8],tol_c[2];
uint8_t canal = 0, screen = 1, condicion=0;
volatile uint8_t seg = 0;


adc_variables Read_ADC(uint8_t ch);
void display_selector(char c);
void pantalla1(void);
void pantalla2(void);
void pantalla3(void);
float entry_setpoint(char k);
float entry_tol(char k);
void control_cargas(float carga_act, float setpoint);
void reles_init(void);

void main(void) {
    
    INTCONbits.GIE=1;//Habilitar INT globales
    INTCONbits.PEIE = 1; //Habilitar INT  periféricas
    RCONbits.IPEN=1; //HABILITAR PRIORIDAD EN LAS INTERRUPCIONES
    INTCONbits.GIEH=1; //HABILITAR INT GLOB ALTAS
    INTCONbits.GIEL=1; //HABILITAR INT GLOB BAJAS
    TRISB = 0x00; //PORTB como salidas dig
    
    kbd_init();
    reles_init();
    
    lcd_init();                     // Inicializamos la librería LCD
    lcd_clear();                    // Limpiamos la pantalla LCD
    __delay_ms(100);
    
    adc_init();
    adc_start();
    adc_result.I0=0.0;
    adc_result.V1=0.0;
    
    tmr0_init();
    
    while(1){

            if(canal>1){canal=0;}
            adc_result = Read_ADC(canal); 
            
            Irms = (adc_result.I0);
            Vrms = (adc_result.V1);
            P = Irms*Vrms;
            


        if(seg >= segundos){
            control_cargas(P,sp);
            seg=0;
        }

       display_selector(kbd_getc());

    }
    return;
}

void display_selector(char c){
    
    static uint8_t screen_ant = 1;
    
    if(c == 'D'){
        screen++;
        __delay_ms(100);    
        if(screen > 3){screen = 1;}
    }
    
    if(screen_ant != screen){lcd_clear();__delay_ms(10);}
    
    switch (screen){
        
        case 1: pantalla1(); break;
        case 2: pantalla2(); break;
        case 3: pantalla3(); break;
     
    }
    
    screen_ant=screen;
}

void pantalla1(void){
    static float I_ant=0.0,V_ant=0.0,P_ant=0.0;
     
    if(I_ant != Irms){
        lcd_gotoxy(1,3);
        lcd_write_string("     ");  
    }
    lcd_gotoxy(1,1);
    sprintf(buffer_lcd,"I=%.1fA",Irms);
    lcd_write_string(buffer_lcd);
    
    if(V_ant != Vrms){
        lcd_gotoxy(1,11);
        lcd_write_string("      ");  
    }
    lcd_gotoxy(1,9);
    sprintf(buffer_lcd,"V=%.1fV",Vrms);
    lcd_write_string(buffer_lcd);
    
    if(P_ant != P){
        lcd_gotoxy(2,6);
        lcd_write_string("      ");  
    }
    lcd_gotoxy(2,4);
    sprintf(buffer_lcd,"P=%.2fkW",P/1000.0);
    lcd_write_string(buffer_lcd);
    
    I_ant = Irms;V_ant = Vrms;P_ant=P;
}

void pantalla2(void){

    lcd_gotoxy(1,1);
    sprintf(buffer_lcd,"Ch1=%u",ch1);
    lcd_write_string(buffer_lcd);
    
    lcd_gotoxy(1,12);
    sprintf(buffer_lcd,"Ch2=%u",ch2);
    lcd_write_string(buffer_lcd);
    
    lcd_gotoxy(2,1);
    sprintf(buffer_lcd,"Ch3=%u",ch3);
    lcd_write_string(buffer_lcd);

    lcd_gotoxy(2,12);
    sprintf(buffer_lcd,"Ch4=%u",ch4);
    lcd_write_string(buffer_lcd);
  
}

void pantalla3(void){
     static float P_ant=0.0;
    sp = entry_setpoint(kbd_getc());
    tolerancia = entry_tol(kbd_getc());
    tolerancia = tolerancia/100.0;
    
    lcd_gotoxy(1,1);
    sprintf(buffer_lcd,"sp:%.1fkW", sp/1000.0);
    lcd_write_string(buffer_lcd);
    
    lcd_gotoxy(1,10);
    sprintf(buffer_lcd,"|%.2fk|", (sp-(tolerancia*sp))/1000.0 );
    lcd_write_string(buffer_lcd);
    
    lcd_gotoxy(2,10);
    sprintf(buffer_lcd,"|%.2fk|", (sp+(tolerancia*sp))/1000.0 );
    lcd_write_string(buffer_lcd);
    
    if(P_ant != P){
        lcd_gotoxy(2,4);
        lcd_write_string("      ");  
    }
    lcd_gotoxy(2,1);
    sprintf(buffer_lcd,"P=%.2fkW",P/1000.0);
    lcd_write_string(buffer_lcd);
    
    P_ant = P;
}

float entry_setpoint(char k){

    static __bit new_sp;
    char cmd=0;
    uint8_t j = 0, i= 0;
    static float nuevo_sp = val_ini_sp;
    
    if(k == '#'){
        new_sp = false;
        i = 0;
        lcd_clear(); __delay_ms(10);
        lcd_gotoxy(1,1);
        lcd_write_string("Nuevo S.P: ");
        
        while(new_sp == false){
        
            cmd = kbd_getc();
            
            if(cmd == 'C')//Clear all
			{
				lcd_gotoxy(2,1);
                lcd_write_string("        ");
                i=0;
				for(j=0;j<=7;j++){sp_c[j]=0;}
                lcd_gotoxy(2,1);
			}

			else if (cmd == 'B')//Borrar
			{
                if(i>0){lcd_gotoxy(2,i);i--;}
                else{lcd_gotoxy(2,1);}
                lcd_write_char(' ');
                sp_c[i]=' ';
                
                __delay_ms(250);
			}

            else if (cmd == 'A') //Enter-Aceptar
			{
                
				nuevo_sp = atof(sp_c);
				__delay_ms(200);
				for(j=0;j<=7;j++){sp_c[j]=0;}
				lcd_clear();__delay_ms(10);
				new_sp=true;
                seg=0;
			}

            else if(cmd!= 0 && cmd != '#' && i <=7){
                lcd_gotoxy(2,1+i);
				lcd_write_char(cmd);
				sp_c[i]=cmd;
				i++;
                __delay_ms(250);
			}   
        }
    }
    
    return nuevo_sp;
    
    
}

float entry_tol(char k){
    
    static __bit new_tol;
    char cmd=0;
    uint8_t j = 0, i= 0;
    static float nueva_tol = val_ini_tol;
    
    if(k == '.'){
        new_tol = false;
        i = 0;
        lcd_clear(); __delay_ms(10);
        lcd_gotoxy(1,1);
        lcd_write_string("Tolerancia (%)=");
        
        while(new_tol == false){
        
            cmd = kbd_getc();
            
            if(cmd == 'C')//Clear all
			{
				lcd_gotoxy(2,1);
                lcd_write_string("   ");
                i=0;
				for(j=0;j<2;j++){tol_c[j]=0;}
                lcd_gotoxy(2,1);
			}

			else if (cmd == 'B')//Borrar
			{
                if(i>0){lcd_gotoxy(2,i);i--;}
                else{lcd_gotoxy(2,1);}
                lcd_write_char(' ');
                tol_c[i]=' ';
                
                __delay_ms(250);
			}

            else if (cmd == 'A') //Enter-Aceptar
			{
                
				nueva_tol = atof(tol_c);
				__delay_ms(200);
				for(j=0;j<2;j++){tol_c[j]=0;}
				lcd_clear();__delay_ms(10);
				new_tol=true;
                seg=0;
			}

            else if(cmd!= 0 && cmd!= '.' && cmd != '#' && i <2){
                lcd_gotoxy(2,1+i);
				lcd_write_char(cmd);
				tol_c[i]=cmd;
				i++;
                __delay_ms(250);
			}   
        }
    }
    
    return nueva_tol;

}

void control_cargas(float carga_actual, float setpoint){

    if( carga_actual >= (setpoint +(tolerancia*setpoint)) ){ if(condicion<4){condicion++;} }
    else if( (carga_actual < setpoint-(tolerancia*setpoint)) ) { if(condicion>0){condicion--;} }
    
    switch (condicion){
        
        case 0:
            RELE_1 = activado;
            ch1 = ~activado;
            break;
            
        case 1:
            RELE_1 = desactivado; RELE_2 = activado;
            ch1 = ~desactivado; ch2 = ~activado;
            break;
            
        case 2:
            RELE_2 = desactivado; RELE_3 = activado;
            ch2 = ~desactivado; ch3 = ~activado;
            break;
            
        case 3:
            RELE_3 = desactivado; RELE_4 = activado;
            ch3 = ~desactivado; ch4 = ~activado;
            break;
            
        case 4:
            RELE_4 = desactivado;
            ch4 = ~desactivado;
            break;
        
        default:
            break;
    }
    
}


/*
void __interrupt(high_priority)ADC_INT(void){

    if(PIR1bits.ADIF){
        conversion = lista;
        adc_stop();
        PIR1bits.ADIF=0;//limpiar bandera
    }
}
*/

adc_variables Read_ADC(uint8_t ch){
    
   static adc_variables result = {.I0=0.0, .V1=0.0, .muestras_adc0=0, .muestras_adc1=0};
   uint16_t adc0=0,adc1=0;
   float Ic=0.0,Vc=0.0,suma0=0.0,suma1=0.0;
   uint16_t i;


    switch(ch){
        
        case 0:
            
            for(i=0;i<n_muestras_I;i++){
                adc0 = adc_read_ch(0);
                Ic = (float)adc0*(I_conversion_val);//Ic=adc0*(5/1023)*(100A/1V)
                suma0 += sqrt(Ic*Ic);
                result.muestras_adc0++;
                
                __delay_us(1000);
                
            }
            
            suma0=(suma0*ICAL);//Para compensar los cuadrados de los semiciclos negativos
            Ic=sqrt(suma0/(float)result.muestras_adc0++);//ecuación del RMS
            result.I0 = Ic;
            suma0=0.0;//resetear sumador
            Ic=0.0;
            result.muestras_adc0=0; canal++;
 
            break;
                    
        case 1:
            
            for(i=0;i<n_muestras_V;i++){
                adc1 = adc_read_ch(1);
                suma1 += (float)adc1*V_conversion_val;
                result.muestras_adc1++;
            }

            result.V1=suma1;
            suma1=0;
            result.V1=(result.V1/result.muestras_adc1++);
            result.muestras_adc1=0; canal++;
            
            break;  
    }
         
    return result;

}


//interrupción cada 1 segundo
void __interrupt(high_priority) ISR_TIMER_0(void){
    
    if(INTCONbits.TMR0IF){
        seg++;
        TMR0L = tmr0_load;
        TMR0H = (tmr0_load>> 8);
        INTCONbits.TMR0IF=0;
    }
}

void reles_init(void){

    RELE_1 = activado; RELE_2 = activado;
    RELE_3 = activado; RELE_4 = activado;
    
    ch1 = ~activado; ch2 = ~activado;
    ch3 = ~activado; ch4 = ~activado;
    
}