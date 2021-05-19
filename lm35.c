#pragma config FOSC = XT   // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF  // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF   // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF   // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF    // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void enviar_comando(int cmd);
void enviar_caracter(char c);
void enviar_dato(int dato);
void print_string(const char *str);
void main(void)
{
    TRISAbits.TRISA0    = 1;
    ADCON1bits.PCFG3    = 1;//unicamente ra0 como entrada analogica y los demas digitales
    ADCON1bits.PCFG2    = 1;
    ADCON1bits.PCFG1    = 1;
    ADCON1bits.PCFG0    = 0;
    
    ADCON1bits.ADCS2    = 1;//fosc/64
    
    ADCON1bits.ADFM     = 0;//justificacion a la izquierda, ADRESH tendra los 8 bits mas significativos y ADRESL los 2 bits menos significativos
    
    ADCON0bits.ADCS1    = 1;//fosc/64
    ADCON0bits.ADCS0    = 0;
    
    ADCON0bits.CHS2     = 0;//seleccion del canal 0
    ADCON0bits.CHS1     = 0;
    ADCON0bits.CHS0     = 0;
    ADCON0bits.ADON     = 1;//adc activo
    uint16_t adc        = 0;//para guardar los 10 bits
    char buff[10]; //para guardar el numero flotante convertido a string
    TRISD = 0; //a diferencia de ASM, aqui no hay necesidad de cambiar de banco
    TRISC = 0;
    PORTC = 0;
    PORTD = 0;
    __delay_ms(15);
    enviar_comando(0x30); //inicializando LCD
    enviar_comando(0x30);
    enviar_comando(0x32);
    enviar_comando(0x2C); 
    enviar_comando(0xC);
    enviar_comando(0x6);
    enviar_comando(0x1);
    unsigned char punto[8] = { 0b00000, 0b01110, 0b01010, 0b01110, 0b00000, 0b00000, 0b00000, 0b00000 }; //para mostrar el simbolo ° mas o menos XD
    enviar_comando(0x40); //escribir los bytes anteriores en la cgram
    for(int i = 0; i < 8; i++)
        
    {
        enviar_caracter(punto[i]);
    }

    while(1)
    {
        ADCON0bits.GO = 1;
        enviar_comando(0x1); //limpiar pantalla
        while(ADCON0bits.GO) //mientras no este lista lo conversion ADC
        {
            
        }
        //rotamos ADRESH 2 veces a la izquierda para dejar espacio para poner ahi los 2 bits menos significativos del ADRESL
        //los cuales se encuentran en los bits 7 y 6 de ADRESL, asi que debemos rotar este registro 6 veces a la derecha para poner
        //esos 2 bits en los bits 1 y 0, finalmente hacer un or para combinar los registros ya rotados
        adc = (uint16_t)(ADRESH << 2) | (uint16_t)(ADRESL >> 6);
        float temp = (float )adc * 5 * 100 / 1023; // por cada bit hay 5/1023 volts, multiplicamos por 100 para obtener los grados centigrados
        sprintf(buff, "%.1f", temp);//parecido a printf, solo que lo que se imprime no se va a la salida estandar, sino al buffer que le pasamos en el primer parametro
        print_string("Temp: ");
        print_string(buff);
        enviar_caracter(' ');
        enviar_caracter(0); //mostrar el simbolo ° mas o menos XD
        enviar_caracter('C');
        
        __delay_ms(500);
    }
    return;
      
}
void enviar_comando(int cmd)
{
    PORTDbits.RD5 = 0;
    enviar_dato(cmd);
}
void enviar_caracter(char c)
{
    PORTDbits.RD5 = 1;
    enviar_dato(c);
}
void enviar_dato(int dato) //ya que trabajamos en modo de 4 bits, primero enviamos el nibble alto y luego el bajo
{
    PORTC = (dato >> 4) & 0xF;
    __delay_ms(1);
    __delay_ms(1);
    PORTDbits.RD6 = 1;
    __delay_ms(1);
    PORTDbits.RD6 = 0;
    PORTC = dato & 0xF;
    __delay_ms(1);
    __delay_ms(1);
    PORTDbits.RD6 = 1;
    __delay_ms(1);
    PORTDbits.RD6 = 0;
    __delay_ms(1);
}
void print_string(const char *str)
{
    while(*str != '\0')
    {
        enviar_caracter(*str);
        str++;
        __delay_ms(10);
    }
}
