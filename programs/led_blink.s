ldr r0, #0x3f200000

ldr r1, #0x8
str r1, [r0, #0x08]

ldr r2, #0x800000

loop:
	ldr r1, #0x200000
	str r1, [r0, #0x1c]

	eor r10, r10, r10
	delay1:
		add r10, r10, #1
		cmp r10, r2
		bne delay1

	ldr r1, #0x200000
	str r1, [r0, #0x28]

	eor r10, r10, r10
	delay2:
		add r10, r10, #1
		cmp r10, r2
		bne delay2

	b loop