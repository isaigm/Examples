 __CONFIG  _FOSC_XT & _WDTE_OFF & _PWRTE_OFF & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _WRT_OFF & _CP_OFF 
;este script realiza un contador en el puerto a y muestra el resultado en el puerto C en un display de 7 segmentos
;Isai Garcia Mendoza 9/05/2021
;Libreria para el PIC16F877a
LIST P=16f877a
#include <p16f877a.inc>
;c7 c6 c5 c4 c3 c2 c1 c0
;   b  a  f  g  c   d  e
;0  1  1	 1  0  1   1  1 ; 0 77 
;0  1  0  0  0  1   0  0 ; 1 44
;0  1  1  0  1  0   1  1 ; 2 6b
;0  1  1  0  1  1   1  0 ; 3 6e
;0  1  0  1  1  1   0  0 ; 4 5c
;0  0  1  1  1  1   1  0 ; 5 3e
;0  0  1  1  1  1   1  1 ; 6 3f
;0  1  1  0  0  1   0  0 ; 7 64
;0  1  1  1  1  1   1  1 ; 8 7f
;0  1  1  1  1  1   1  0 ; 9 7e
;0  1  1  1  1  1   0  1 ; A 7d
;0  0  0  1  1  1   1  1 ; b 1f
;0  0  1  1  0  0   1  1 ; C 33 
;0  1  0  0  1  1   1  1 ; d 4f
;0  0  1  1  1  0   1  1 ; e 3b
;0  0  1  1  1  0   0  1 ; f 39
;***** Variables *****
cblock 0x20
contador 
contador1
contador2 
cnt
endc
;;***** Estructura *****
ORG 0x00 ; Vector de reset
BCF STATUS, RP0 ;
BCF STATUS, RP1 ; seleccion banco 0
CLRF PORTA
BSF STATUS, RP0 ; Seleccion banco 1
MOVLW 0x06 
MOVWF ADCON1 
MOVLW 0xFF ; los 6 bits del puerto A como entradas digitales
MOVWF TRISA
banksel TRISC ; cambiamos de banco del 0 al 1
clrf TRISC; Habilitamos el puerto c como salida
banksel PORTC ; cambiamos del banco 1 al 0
clrf PORTC
loop
	btfsc PORTA, 1 ; si esta activado el RA1 se muestra en el display el valor actual del registro cnt (counter)
	call mostrar_display
	goto loop

tabla
	addwf PCL, F
	dt    0x77, 0x44, 0x6b, 0x6e, 0x5c, 0x3e, 0x3f, 0x64, 0x7f, 0x7e, 0x7d, 0x1f, 0x33, 0x4f, 0x3b, 0x39
mostrar_display
	movf  cnt, W
	call  tabla
	movwf PORTC
	incf  cnt, F ;para comprobar cuando el contador se pasa de 16
	movf  cnt, W
	sublw d'16'
	btfsc STATUS, Z
	clrf  cnt
	call  retardo_1s
	return
retardo_1s
	movlw 5
	movwf contador
	loop3
		call    retardo_200ms
		decfsz  contador, F
		goto    loop3
	return 
retardo_200ms				
	movlw	d'200'	;repetimos 200 veces el retardo de un 1 ms		
	goto	retardo_ms		
retardo_20ms
	movlw d'20'
	goto  retardo_ms

retardo_ms
	movwf	  contador2		
loop1
	movlw	  d'249'	;numero de iteraciones que necesitamos hacer para conseguir un tiempo aproximado de 1 ms		
	movwf	  contador1		
loop2
	nop
	decfsz	contador1,F	
	goto	  loop2		
	decfsz  contador2,F		
	goto	  loop1 	
	return
End
