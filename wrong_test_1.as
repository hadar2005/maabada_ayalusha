.entry MAIN
.extern EXT_FUNC

MAIN:   add $35, $5, $9       
        sub $3, $-1, $4        
        addi $4, 50000, $5     
        fakecmd $1, $2, $3     
        hlt