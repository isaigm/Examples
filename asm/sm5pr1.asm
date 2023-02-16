__CONFIG  _FOSC_XT & _WDTE_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _WRT_OFF & _CP_OFF 

LIST P=16f877a
#include <p16f877a.inc>
; RC7 RC6 RC5 RC4 RC3 RC2 RC1 RC0
;	  		  	  D7  D6  D5  D4 	
; RD7 RD5 RD6
; RW  RS  E
#define RW 		PORTD, 7
#define E  		PORTD, 6
#define RS 		PORTD, 5
#define OPTO 	PORTC, 7
#define	BTN		PORTD, 4
cblock 0x20
contador1
contador2
ncarros
unidades
decenas
dato
temp
endc
ORG 0x00 

;-------------inicializando LCD--------------------
banksel TRISC 
bcf 	TRISC, 0
bcf 	TRISC, 1
bcf 	TRISC, 2
bcf 	TRISC, 3
bsf		OPTO
bsf		BTN
bcf		RS
bcf		E

banksel PORTC

call 	retardo_15ms
movlw 	0x30
call 	enviar_comando
call	retardo_1ms
movlw	0x30
call 	enviar_comando
call 	retardo_1ms
movlw 	0x32
call 	enviar_comando
call 	retardo_1ms
movlw 	0x28 ; 2 lineas, fuente 5x8
call 	enviar_comando
call 	retardo_1ms
movlw 	0xC; lcd prendido, cursor apagado, cursor no parpadea
call 	enviar_comando
call 	retardo_1ms
movlw 	0x6; el cursor se desplaza a la derecha cuando se escribe un nuevo caracter, no se hace shift cuando se escribe un caracter que no cabe en la linea
call 	enviar_comando
call 	retardo_1ms
movlw 	0x1
call 	enviar_comando
call 	retardo_1ms
clrf	unidades
clrf	decenas

loop
	movlw 	0x1
	
	call 	enviar_comando
	call 	retardo_1ms
	 
	btfss	OPTO	
	incf	ncarros, F
	
	movf	ncarros, w
	sublw	d'13'
	btfsc	STATUS, Z
	call	lleno
	
	btfss	BTN
	call	sacar_carro
	btfss	STATUS, C
	call 	vacio
	movf	ncarros, W
	movwf	temp
	digitos
			incf 	decenas, F
			movf	temp, W
			movwf	unidades
			movlw	d'10'
			subwf	temp, F 
			btfsc	STATUS, Z
			goto 	salir
		    btfss   STATUS, C
			goto 	salir
			goto    digitos
	salir

	btfsc	STATUS, Z
	clrf	unidades
	btfss	STATUS, C
	decf	decenas, F
	movlw 	0x80
	call	enviar_comando
	call 	retardo_1ms
	movlw 	'C' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'a' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'r' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'r' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'o' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	's' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	':' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	' ' 
	call 	enviar_caracter
	call 	retardo_1ms
	movf	decenas, W
	addlw	'0'
	call 	enviar_caracter
	call 	retardo_1ms

	movf	unidades, W
	addlw	'0'
	call 	enviar_caracter
	call 	retardo_1ms

	clrf	unidades
	clrf	decenas
	call 	retardo_200ms
	call 	retardo_200ms
	call 	retardo_200ms
	goto 	loop
lleno
	decf	ncarros, F
	movlw 	0xC0 
	call 	enviar_comando
	call 	retardo_1ms
	movlw 	'L' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'l' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'e' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'n' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'o' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'!' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'!' 
	call 	enviar_caracter
	call 	retardo_1ms
	return
vacio
	clrf	ncarros
	movlw 	0xC0 
	call 	enviar_comando
	call 	retardo_1ms
	movlw 	'V' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'a' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'c' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'i' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'o' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'!' 
	call 	enviar_caracter
	call 	retardo_1ms
	movlw 	'!' 
	call 	enviar_caracter
	call 	retardo_1ms
	return
enviar_comando
	bcf 	RS
	movwf 	dato
	swapf 	dato, W
	andlw 	0xF	
	movwf 	PORTC
	call 	retardo_1ms
	call 	retardo_1ms
	bsf 	E ; habilitar y deshabilitar E necesario para enviar info al lcd
	call 	retardo_1ms
	bcf 	E
	movf 	dato, W
	andlw 	0xF
	movwf 	PORTC
	call 	retardo_1ms
	call 	retardo_1ms
	bsf 	E ; habilitar y deshabilitar E necesario para enviar info al lcd
	call 	retardo_1ms
	bcf 	E
	return 
enviar_caracter
	bsf 	RS
	movwf 	dato
	swapf 	dato, W
	andlw 	0xF	
	movwf 	PORTC
	call 	retardo_1ms
	call 	retardo_1ms
	bsf 	E ; habilitar y deshabilitar E necesario para enviar info al lcd
	call 	retardo_1ms
	bcf 	E
	movf 	dato, W
	andlw 	0xF
	movwf 	PORTC
	call 	retardo_1ms
	call 	retardo_1ms
	bsf 	E ; habilitar y deshabilitar E necesario para enviar info al lcd
	call 	retardo_1ms
	bcf 	E
	return
sacar_carro
	movlw 	d'1'
	subwf 	ncarros
	return
retardo_200ms				
	movlw	d'200'	;repetimos 200 veces el retardo de un 1 ms		
	goto	retardo_ms		
retardo_15ms
	movlw	d'15'
	goto	retardo_ms
retardo_1ms
	movlw	d'1'
	goto	retardo_ms
retardo_100ms
	movlw	d'100'
	goto	retardo_ms
retardo_5ms
	movlw	d'5'
	goto	retardo_ms
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

End
