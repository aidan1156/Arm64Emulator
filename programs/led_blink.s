
eor x0, x0, x0
eor x1, x1, x1
eor x2, x2, x2
eor x3, x3, x3


ldr w0, d0
add w1, w1, #0x2
str w1 [w0, #1]

eor x1, x1, x1
add w1, w1, #0x8

str w1 [w0, #0x1c]
and x0, x0, x0

d0:
.int 0x3f200000
and x0, x0, x0