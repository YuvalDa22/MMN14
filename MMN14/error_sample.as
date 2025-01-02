; file error_sample.as

.entry LIST
.extern fn1
MAIN:	add r3, LIST, r8
	jsr fn1
LOOP:	prn #48
	macr m_macr
		cmp r3, 55
		bne END
	endmacr
	lea STR, r6, 8
	inc r6, 5
	mov *r6, L32
	sub r1, r4, 3
	m_macr
	add r7, *r6, r9
	clr K
	sub L3, L3
.entry MAIN
	jmp LOOP
END:	stop
STR:	.string "abcdefg"
LIST:	.data 6, -9
	.data -100
K:	.data 31
