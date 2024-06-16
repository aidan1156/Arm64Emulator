
sub x0, x0, x0
sub x1, x1, x1
sub x2, x2, x2
sub x3, x3, x3
sub x4, x4, x4

address:
.int 0x3f200000

fsel:
.int 0x1000

gpset:
.int 0x4000

ldr x0, address
ldr x1, fsel
str w1, [x0, #4]

ldr x2, gpset

lmao:
str w2, [x0, #0x1c]
str w10, [x0, #0x1c]
b lmao

