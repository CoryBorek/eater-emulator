PORTB = $6000
PORTA = $6001
DDRB = $6002
DDRA = $6003

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

	lda #"H"    
	jsr print_char
	lda #"e"
	jsr print_char
	lda #"l"
	jsr print_char
	lda #"l"
	jsr print_char
	lda #"o"
	jsr print_char
	lda #","
	jsr print_char
	lda #" "
	jsr print_char
	lda #"w"
	jsr print_char
	lda #"o"
	jsr print_char
	lda #"r"
	jsr print_char
	lda #"l"
	jsr print_char
	lda #"d"
	jsr print_char
	lda #"!"
	jsr print_char

loop:
	jmp loop

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
