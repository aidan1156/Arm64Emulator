
add x0, x0, #0x3F200000
add x3, x3, #1
add x1, x1, x3 lsl #0x11
ldr x1 [x0, #4]!

loop:
    // Turn GPIO17 on
    // call delay

    // Turn GPIO17 off
    // call delay

    b loop


delay:
    add x2, x2, #0xffffffff
delay_loop:
    subs x2, x2, #1
    bne delay_loop
    ret