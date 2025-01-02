; file sample1.as

.extern fn2
.entry START
START:  mov r2, r3
        jsr fn2
LOOP1:  prn #100
	macr loop_macr
		sub r1, #-4
		bne EXIT
	endmacr
	lea MSG, r5
	inc r5
	mov *r5, L4
	add r2, r3
	loop_macr
	dec r6
	clr Z
	sub L4, L4
.entry LOOP1
	jmp LOOP1
EXIT:   stop
MSG:    .string "hello"
L4:     .data 10, -15
        .data 5
Z:      .data 20
