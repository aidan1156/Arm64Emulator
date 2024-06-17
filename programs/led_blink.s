
eor w0, w0, w0
eor w1, w1, w1
eor w2, w2, w2
eor w3, w3, w3
eor w4, w4, w4

add w4, w4, #0x3ff lsl #12

ldr w0, address
ldr w1, fsel
str w1, [x0, #0]

ldr w2, gpset

loop:
str w2, [x0, #0x1c]
eor w3, w3, w3

delay1:
add w3, w3, #1
cmp w3, w4
b.ne delay1

str w2, [x0, #0x28]

eor w3, w3, w3

delay2:
add w3, w3, #1
cmp w3, w4
b.ne delay2

b loop

address:
.int 0x3f200000

fsel:
.int #0x200000

gpset:
.int 0x80

