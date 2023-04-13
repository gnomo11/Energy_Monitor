#include <xc.h>

#define RS RC2
#define EN RC1
#define D4 RD0
#define D5 RD1
#define D6 RD2
#define D7 RD3

#define _XTAL_FREQ 48000000

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void lcd_clear(void);
void lcd_gotoxy(char a, char b);
void lcd_init(void);
void lcd_write_char(char a);
void lcd_write_string(char *a);

