; Sample error assembly file to test error handling

; 1. Undefined / illegal command
MAIN:   notacommand $1, $2, $3

; 2. Register number out of range (greater than 31)
        add $32, $5, $9

; 3. Immediate value out of signed 16-bit range (-32768 to 32767)
        addi $1, $2, 40000

; 4. Invalid register name (missing $)
        or 5, $2, $4

; 5. Missing comma between operands
        bne $1 $2, LOOP

; 6. Undefined target label in jump/branch instruction
        jmp NON_EXISTENT_LABEL

; 7. Invalid string declaration (missing closing quote)
STR:    .asciz "missing end quote

; 8. Redefinition of existing label
MAIN:   stop