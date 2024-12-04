PORTB = $6000
PORTA = $6001
DDRB = $6002
DDRA = $6003
PCR = $600C
IFR = $600D
IER = $600E
	
E  = %10000000
RW = %01000000
RS = %00100000

ACIA_DATA = $5000
ACIA_STATUS = $5001
ACIA_CMD = $5002
ACIA_CTRL = $5003

	.org $8000
reset:
	ldx #$ff
	txs
	cli
	
	lda #$82
	sta IER
	lda #$00
	sta PCR
	
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

	lda #$00
	sta ACIA_STATUS 	; soft reset

	lda #$1f		; N-8=1, 19200 baud
	sta ACIA_CTRL

	lda #$0b		;no parity, no echo, no interrupts
	sta ACIA_CMD

	ldx #0
send_msg:	
	lda message,x
	beq done
	jsr send_char
	inx
	jmp send_msg
done:	
rx_wait:	
	lda ACIA_STATUS
	and #$08		;check rx buffer status flag
	beq rx_wait		;loop if rx buffer empty

	lda ACIA_DATA
	jsr print_char
	jsr send_char
	jmp rx_wait

message:	.asciiz "Hello, world!"

send_char:	
	sta ACIA_DATA
	pha
tx_wait:	
	lda ACIA_STATUS
	and #$10
	beq tx_wait			; check tx buffer status flag
	jsr tx_delay
	pla
	rts

tx_delay:
	phx
	ldx #100
tx_delay_1:	
	dex
	bne tx_delay_1
	plx
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
	pha
	jsr lcd_wait
	sta PORTB
	lda #RS        ; Set RS
	sta PORTA  
	lda #(RS | E)  ; Set E bit to send instruction
	sta PORTA
	lda #RS        ; Clear Enable
	sta PORTA
	pla
	rts

nmi:
irq:
	rti
	
	;; reset and irq vectors
	.org $fffa
	.word nmi
	.word reset
	.word irq
