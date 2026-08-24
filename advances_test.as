.entry START
.entry RESULT
.extern EXTERNAL_LBL

mcro advanced_macro
    sub $10, $11, $12
    addi $5, 100, $6
    mvhi $8, $9
mcroend

START:  la DATA_ARR
        jmp $14               

        advanced_macro       
        
 BACK:  beq $4, $5, SKIP
        blt $1, $2, EXTERNAL_LBL
        lb $15, -4, $20
        sh $16, 8, $21
        
SKIP:   call EXTERNAL_LBL
        hlt                   

DATA_ARR: .dw 100000, -99999
STR_NEW:  .asciz "AssemblerTest"
RESULT:   .db 1, 2, 3, 4
          .dh 500, -500