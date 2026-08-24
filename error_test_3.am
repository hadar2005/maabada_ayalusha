WARN1:  .entry MAIN          \
WARN2:  .extern EXT_LBL        

MAIN:   move $1, $2
        bgt $4, $2, END
        call EXT_LBL
        
STR1:   .asciz "missing_quote  
        .db 5, 10, 15
END:    hlt