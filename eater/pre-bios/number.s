PORTB = $6000
PORTA = $6001
DDRB = $6002
DDRA = $6003

value = $0200			; 2 bytes
mod10 = $0202			; 2 bytes
message = $0204			; 6 bytes
E  = %10000000
RW = %01000000
RS = %00100000

	.org $8000
reset:
	lda #%11111111 ; Set all pins on port B to output
	sta DDRB
	lda #%11100000 ; set top 3 pins on port A to output
	sta DDRA
  
	lda #%00111000 ; set 8-bit mode; 2-line display; 5x8 font
	jsr lcd_instruction
	lda #%00001110 ; Display on; cursor on; blink off
	jsr lcd_instruction
	lda #%00000110 ; Increment and shift cursor; don't shift entire display.
	jsr lcd_instruction
	lda #%00000001 ; Clear display
	jsr lcd_instruction

	lda #0
	sta message

	;; Initialize value to be the number to convert
	lda number
	sta value
	lda number + 1
	sta value + 1

divide:	
	;;  Initialize the remainder to zero
	lda #0
	sta mod10
	sta mod10 + 1
	clc
	
	ldx #16
divloop:	
	;; Rotate quotient and remainder
	rol value
	rol value + 1
	lda value + 1
	rol mod10
	lda mod10
	rol mod10 + 1
	
	;; a,y = dividend - divisor
	sec
	lda mod10
	sbc #10
	tay			;save low byte
	lda mod10 + 1
	sbc #0
	bcc ignore_result 	; branch if dividend < divisor
	.byte 0
	sty mod10
	sta mod10 + 1
	
ignore_result:
	
	dex
	bne divloop
	rol value		;shift in the last bit of the quotient
	rol value + 1

	lda mod10
	clc
	adc #"0"
	jsr push_char

	;; if Value != 0, continue dividing
	lda value
	ora value + 1
	bne divide		; branch if value not zero

	ldx #0
print:
	lda message,x
	beq loop
	jsr print_char
	inx
	jmp print
	
loop:
	jmp loop

number:	 .word 1729

;;; Add the charracter in the A register to the beginning of the
;;; null terminated string message
push_char:
	pha			;put the first character on the stack
	ldy #0

char_loop:	
	lda message,y		;get char on string and put into x
	tax
	pla
	sta message,y		; pull char off stack and add to the str
	iny
	txa
	pha			;push char from string onto stack
	bne char_loop

	pla
	sta message,y 		; pull the null off the stack and add to the end of the string
	rts
	
lcd_wait:
	pha
	lda #%00000000		;Port B is input
	sta DDRB
lcdbusy:	
	lda #RW
	sta PORTA
	lda #(RW | E)
	sta PORTA
	lda PORTB
	and #%10000000
	bne lcdbusy
	lda #$FF
	lda #RW
	sta PORTA
	lda #%11111111		; PORTB is output
	sta DDRB
	pla
	rts
	
lcd_instruction:
	jsr lcd_wait
	sta PORTB  
	lda #0         ; Clear RS/RW/E bits
	sta PORTA
	lda #E         ; Set E bit to send instruction
	sta PORTA
	lda #0         ; Clear RS/RW/E bits
	sta PORTA
	rts

print_char:
	jsr lcd_wait
	sta PORTB
	lda #RS        ; Set RS
	sta PORTA  
	lda #(RS | E)  ; Set E bit to send instruction
	sta PORTA
	lda #RS        ; Clear Enable
	sta PORTA
	rts

	;; reset and irq vectors
	.org $fffc
	.word reset
	.word $0000
