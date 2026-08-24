.entry START
.extern EXTERNAL_VAL

START:  add $3, $5, $9
LOOP:   sub $4, $2, $1
        
LOOP:   ori $9, -5, $2        
        jmp MISSING_LBL        
        
        la EXTERNAL_VAL
        bne $1, $2, START
        hlt