; file sample2.as

.entry BEGIN
.extern fn3
BEGIN:  clr r1
        jsr fn3
LOOP2:  prn #25
	macr loop2_macr
		add r4, #-3
		bne FINISH
	endmacr
	lea TEXT, r7
	inc r7
	mov *r7, L5
	sub r2, r5
	loop2_macr
	dec r4
	clr Y
	sub L5, L5
.entry LOOP2
	jmp LOOP2
FINISH: stop
TEXT:   .string "xyz"
L5:     .data 12, -7
        .data -20
Y:      .data 40
