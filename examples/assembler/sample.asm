; The first eight Fibonacci numbers, on four registers.
;
;   r0  a          r2  how many are left
;   r1  b          r3  a scratch register, reloaded when it is needed

        load r0, 1
        load r1, 1
        load r2, 8

loop:   jmpz r2, done
        print r0
        load r3, 1        ; r3 is the temp below, so the 1 is made here each time
        sub r2, r3
        copy r3, r0       ; t = a
        add r3, r1        ; t = a + b
        copy r0, r1       ; a = b
        copy r1, r3       ; b = t
        jmp loop

done:   halt
