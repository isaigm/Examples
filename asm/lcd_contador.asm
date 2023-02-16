 __CONFIG  _FOSC_XT & _WDTE_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _WRT_OFF & _CP_OFF 

LIST P=16f877a
#include <p16f877a.inc>
; RC7 RC6 RC5 RC4 RC3 RC2 RC1 RC0
;	  		  	  D7  D6  D5  D4 	
; RD7 RD5 RD6
; RW  RS  E
#define RW PORTD, 7
#define E  PORTD, 6
#define RS PORTD, 5

cblock 0x20
contador 
contador1
contador2
cnt
cnt2
cnt3  
endc
ORG 0x00 
;-------------inicializando LCD--------------------
banksel TRISC 
clrf TRISC
clrf TRISD
banksel PORTC
clrf PORTC
clrf PORTD
call retardo_15ms
movlw 0x30
call enviar_comando
call retardo_1ms
movlw 0x30
call enviar_comando
call retardo_1ms
movlw 0x30
call enviar_comando
call retardo_1ms
movlw 0x38 ; 2 lineas, fuente 5x8
call enviar_comando
call retardo_1ms
movlw 0xC; lcd prendido, cursor apagado, cursor no parpadea
call enviar_comando
call retardo_1ms
movlw 0x1 ; limpiar lcd
call enviar_comando
call retardo_1ms
movlw 0x6; el cursor se desplaza a la derecha cuando se escribe un nuevo caracter, no se hace shift cuando se escribe un caracter que no cabe en la linea
call enviar_comando
call retardo_1ms
loop
	movlw 	0x1
	call 	enviar_comando
	call 	retardo_1ms
	movf 	cnt3, W
	addlw 	'0'
	call 	enviar_caracter
	call 	retardo_1ms
	movf 	cnt2, W
	addlw 	'0'
	call 	enviar_caracter
	call 	retardo_1ms
	movf 	cnt, W
	addlw 	'0'
	call 	enviar_caracter
	call 	retardo_1ms
	incf 	cnt,F
	movf 	cnt,W
	sublw 	d'10'
	btfsc	STATUS,Z
	call 	add_d2
	call 	retardo_100ms
	call 	retardo_100ms
	call 	retardo_100ms
	goto 	loop
add_d2
	incf 	cnt2, F
	movf 	cnt2,W
	sublw 	d'10'
	btfsc	STATUS,Z
	call 	add_d3
	clrf 	cnt
	return
add_d3
	incf 	cnt3, F
	movf 	cnt3, W
	sublw 	d'10'
	btfsc 	STATUS, Z
	clrf 	cnt3
	clrf 	cnt2
	return
enviar_comando
	bcf 	RS 
	movwf 	PORTC
	call 	retardo_1ms
	call 	retardo_1ms
	bsf 	E
	call 	retardo_1ms
	bcf 	E
	return 
enviar_caracter
	bsf 	RS	
	movwf 	PORTC
	call 	retardo_1ms
	call 	retardo_1ms
	bsf 	E ; habilitar y deshabilitar E necesario para enviar info al lcd
	call 	retardo_1ms
	bcf 	E
	return
retardo_200ms				
	movlw	d'200'	;repetimos 200 veces el retardo de un 1 ms		
	goto	retardo_ms		
retardo_15ms
	movlw d'15'
	goto retardo_ms
retardo_1ms
	movlw d'1'
	goto retardo_ms
retardo_100ms
	movlw d'100'
	goto retardo_ms
retardo_5ms
	movlw d'5'
	goto retardo_ms
retardo_ms
	movwf	contador2		
loop1
	movlw	d'249'	;numero de iteraciones que necesitamos hacer para conseguir un tiempo aproximado de 1 ms		
	movwf	contador1		
loop2
	nop
	decfsz	contador1,F	
	goto	loop2		
	decfsz	contador2,F		
	goto	loop1 	
	return
retardo_100micros
	movlw d'100'
	goto  retardo_micros
retardo_5micros
	movlw d'5'
	goto  retardo_micros
retardo_micros
	movwf contador1
micros
	nop
	decfsz  contador1, F
	goto    micros
	return
End
