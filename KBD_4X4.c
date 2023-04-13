#include <xc.h>
#include "KBD_4X4.h"

char const TECLAS[4][4]={            
                        {'1','2','3','A'},
                        {'4','5','6','B'},
                        {'7','8','9','C'},
                        {'.','0','#','D'}};  

void kbd_init(void){
    
    TRIS_KEYPAD = 0xF0; // Configuramos RB0-3 como salidas y RB4-7 como entradas 
    LAT_KEYPAD = 0x0F;    // Inicializamos RB0, RB1, RB2, RB3  a nivel alto 1
    
    PORT_KEYPAD&(1<<COLUMNA0);
    PORT_KEYPAD&(1<<COLUMNA1);
    PORT_KEYPAD&(1<<COLUMNA2);
    PORT_KEYPAD&(1<<COLUMNA3);
    
}

char kbd_getc(void){
 static uint8_t     contador=0;             //Cuenta el número de filas
 static uint8_t     n=1;                    //Desplaza los ceros de las filas.
 char               K=0;                      //Acepta el valor de la tecla pulsada
 static uint8_t     C;                      //Número de Columnas.
 static uint8_t     R;                      //Número de Filas.

 if(contador<4)                // Si contador es menor que 4
      {
          R=contador;               // F recibe 0,1,2,3 según contador incrementa.
          LAT_KEYPAD=~n;            // Invertimos el valor de n y lo cargamos en el Puerto B
          n=n<<1;                   // n desplaza su valor 1 bit a la izquierda.
          ++contador;               // Incrementamos el valor de contador en una unidad.
          Columnas();               // Detecta si hay cambios en las columnas.
      }
      else                          // Reiniciamos las variables a sus valores iniciales 
      {
          contador=0;
          LAT_KEYPAD=0xFF;
          n=1;
      }
      
      if(Columnas())                // Si hay un cambio en la columna (columnas=1)...
      {
          if((PORT_KEYPAD&(1<<COLUMNA0))==0)
          {
              C=0;
          }
          else if((PORT_KEYPAD&(1<<COLUMNA1))==0)
          {
              C=1;
          }
          else if((PORT_KEYPAD&(1<<COLUMNA2))==0)
          {
              C=2;
          }
          else if((PORT_KEYPAD&(1<<COLUMNA3))==0)
          {
              C=3;
          }
          AntiRebote();             // LLamamos a la función AntiRebote
          K=TECLAS[R][C];                      // Asigna a K el valor de la tecla ubicada enla fila F, columna C.
          
          
      }
      return K;
}

__bit Columnas(void)              //Función Columnas censa el cambio de estado.
{
    if((PORT_KEYPAD&(1<<COLUMNA0))&&(PORT_KEYPAD&(1<<COLUMNA1))&&(PORT_KEYPAD&(1<<COLUMNA2))&&(PORT_KEYPAD&(1<<COLUMNA3)))
        return 0;
    else
        return 1;
}

void AntiRebote(void)               // Función AntiRebote
{
    while(PORT_KEYPAD&(1<<COLUMNA0)==0); //No realiza nada hasta que el pulsador esté inactivo 
    while(PORT_KEYPAD&(1<<COLUMNA1)==0);
    while(PORT_KEYPAD&(1<<COLUMNA2)==0);
    while(PORT_KEYPAD&(1<<COLUMNA3)==0);
    return;
}

