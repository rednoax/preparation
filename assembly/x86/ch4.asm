bits 16
mov bx, 0xb800
mov ds, bx
mov byte [0x0], 'A'
mov byte [0x1], 0x3
mov byte [0x2], 'S'
mov byte [0x3], 0x3
mov byte [0x4], 'M'
mov byte [0x5], 0x3
