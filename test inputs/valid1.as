; Sample valid assembly input file for testing

.entry LIST
.extern W

; Macro definition and deployment
mcro m_print
    add $3, $5, $9
    or $1, $2, $4
endmcro

MAIN:   add $3, $5, $9
        LOOP:   ori $1, -5, $2
        m_print
        bne $31, $9, LOOP
        jmp W
        la $t1
        stop

; Data declarations
STR:    .asciz "abcdef"
LIST:   .db 6, -9, 15
        .dh 270, -150
        .dw 33554, -40000
K:      .dw 31