; inputs: 
    ; n = unsigned integer 
    ; k = unsigned integer (n >= k)
; output:
    ; $3 = stirling number 
; base cases: 
    ; f(0,0) = 1
    ; f(i,0) = f(0,i) = 0
    ; f(n, k) = 0 if n < k
; recursive case : f(n,k) = (n-1) * f(n-1,k) + f(n-1, k-1)

; variables
;..................................................

;..................................................

stirling: 

    sw $1, -4($30)
    sw $2, -8($30)
    sw $10, -12($30)
    sw $4, -16($30)
    sw $31, -20($30)             ; caller address (starts at loader)

    lis $10
    .word 1

    lis $31
    .word 20

    sub $30, $30, $31

    ;base cases
    ;..................................................
    case1:  ; n < k : output = 0 

        slt $3, $1, $2          ; is n < k ? 

        beq $3, $0, case2       ; if no => check case2 

        lis $3                  ; n < k : output = 0
        .word 0 

        beq $0, $0, end         ; => go to end 

    case2:  ; n = k = 0

        bne $1, $0, case3           ; if ( n != 0 ) => case3 
        bne $2, $0, case3           ; if ( k != 0 ) => case3 

        lis $3                      ; n = k = 0 : output = 1
        .word 1

        beq $0, $0, end             ; => go to end 
    
    case3:  ; n = 0, k != 0  (we know already that theyre ! both zero)  

        bne $1, $0, case4           ; if(n != 0) => case4

        lis $3                      ; n == 0 : output = 0
        .word 0

        beq $0, $0, end             ; => go to end 

    case4:  ; n != 0, k = 0

        bne $2, $0, recursiveCase   ; if(k != 0) => end (no base cases apply) 

        lis $3                      ; k ==0 : output = 0 
        .word 0

        beq $0, $0, end             ; => go to end

    ;..................................................

    ; recursive case f(n,k) = [ (n-1) * f(n-1,k) ] + f(n-1, k-1)
    ;..................................................
    recursiveCase: 

        sub $1, $1, $10         ; n = n-1

        lis $31
        .word stirling

        jalr $31                ; => $3 = f(n-1, k)

        mult $1, $3             ; $4 = (n-1) * f(n-1, k)
            mflo $4         

        sub $2, $2, $10         ; k = k - 1

        lis $31
        .word stirling

        jalr $31                ; => $3 f(n-1, k-1) 

        add $3, $3, $4          ; output = [ (n-1) * f(n-1,k) ] + f(n-1, k-1)
    ;..................................................

end: 
    lis $31
    .word 20
    add $30, $30, $31
    lw $1, -4($30)
    lw $2, -8($30)
    lw $10, -12($30)
    lw $4, -16($30)
    lw $31, -20($30)
    jr $31
