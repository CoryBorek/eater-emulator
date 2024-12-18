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
  sta PORTB
  
  lda #0         ; Clear RS/RW/E bits
  sta PORTA
  
  lda #E         ; Set E bit to send instruction
  sta PORTA
  
  lda #0         ; Clear RS/RW/E bits
  sta PORTA 

  lda #%00001110 ; Display on; cursor on; blink off
  sta PORTB
  
  lda #0         ; Clear RS/RW/E bits
  sta PORTA
  
  lda #E         ; Set E bit to send instruction
  sta PORTA
  
  lda #0         ; Clear RS/RW/E bits
  sta PORTA
  
  lda #%00000110 ; Increment and shift cursor; don't shift entire display.
  sta PORTB
  
  lda #0         ; Clear RS/RW/E bits
  sta PORTA
  
  lda #E         ; Set E bit to send instruction
  sta PORTA
  
  lda #0         ; Clear RS/RW/E bits
  sta PORTA

  lda #%00000001 ; Clear display
  sta PORTB
  
  lda #0         ; Clear RS/RW/E bits
  sta PORTA
  
  lda #E         ; Set E bit to send instruction
  sta PORTA
  
  lda #0         ; Clear RS/RW/E bits
  sta PORTA

  lda #"H"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA
  lda #"e"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA
  lda #"l"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA
  lda #"l"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA
  lda #"o"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA
  lda #","    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA
  lda #" "    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA
  lda #"W"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA
  lda #"o"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA

  lda #"r"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA
	
  lda #"l"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA

  lda #"d"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA

  lda #"!"    
  sta PORTB
  
  lda #RS        ; Set RS
  sta PORTA
  
  lda #(RS | E)  ; Set E bit to send instruction
  sta PORTA
  
  lda #RS        ; Clear Enable
  sta PORTA

loop:
  jmp loop
  
  .org $fffc
  .word reset
  .word $0000
