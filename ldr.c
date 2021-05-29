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
    uint16_t adc        = 0;
    TRISD               = 0;
    PORTD               = 0;
    
    while(1)
    {
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO
        {
            
        }
        adc = (uint16_t)(ADRESH << 2) | (uint16_t)(ADRESL >> 6);
        float volts = (float ) adc * 5 / 1023; 
        if(volts >= 4)
        {
            PORTDbits.RD2 = 0;
        }else{
            PORTDbits.RD2 = 1;
        }
        
        __delay_ms(100);
    }
    return; 
}
