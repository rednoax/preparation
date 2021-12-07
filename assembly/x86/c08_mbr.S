.code16
.text
.equ app_lba_start, 100 #LBA start from [0,...
.globl _start
_start:
    mov $0, %bx
    mov %bx, %ss
    mov %bx, %sp #sp is full-decrement, so the most high addr holding pushed data is [0xffff, 0xfffe,...
    mov app_start, %ax  #nasm:mov ax, [app_start], ds<<4, ds:0 now
    mov app_start + 2, %dx
    divw divisor
    mov %ax, %ds    #ds: 0x1000
    mov %ax, %es    #es: 0x1000

    mov $app_lba_start, %si
    mov $0, %di
    call read_hard_disk_0 #cs: 0
    mov %ds:(0), %ax
    mov %ds:(2), %dx
    divw divisor + 4
#511 512 513 1024
#0   1   1   2 qutient
#511 0   1   0 remainder
    cmp $1, %ax #unsgined: ax - 1 <= 0
    jna fin
    dec %ax
    mov %ax, %cx
1:
    inc $si
    add divisor + 4, %bx
    call read_hard_disk_0
    loop 1b
fin:
    mov (6), %ax #ds:0x1000
    mov (8), %dx
    call calc_segment_base #fix section.code_1.start
    mov %ax, (6)
    ljmp (4)#$0x1000, $start
read_hard_disk_0:   #read LBA[DI:SI] to mem DS:BX, DI:[27:16] SI[15:0], DI[31:28]
    push %ax
    push %bx
    push %cx
    push %dx
    mov $0x1f2, %dx #0x1f2: 8bit port, read sector#
    mov $1, %al
    out %al, %dx
    inc %dx
    mov %si, %ax
    out %al, %dx    #0x1f3: 8bit port, LBA[7:0]
    inc %dx
    out %ah, %dx    #0x1f4: 8bit port, LBA[15:8]
    inc %dx
    mov %di, %ax
    out %al, %dx    #0x1f5: 8bit port, LBA[23:16]
    inc %dx
    or $0xe0, %ah	#ah [31:28] is empty
    out %ah, %dx    #0x1f6: 8bit port, LBA[27:24]
    inc %dx
    mov $0x20, %al  #0x20: read command
    out %al, %dx    #0x1f7: 8bit port, command port
.waits:
    in %al, %dx     #0x1f7: status port
    and $0x89, %al   #1000 1001:BUSY, Data read ready, err
    cmp $0x80, %al  #al - 0x80
    jnz .waits
    cmp $0x8, %al
    jz 1f
    jmp .
1:
    mov $256, %cx
    mov $0x1f0, %dx #0x1f0: data port, 16bit
.read:
    in %dx, %ax
    mov %ax, %ds:(%bx)
    add $2, %bx
    loop .read
    pop %dx
    pop %cx
    pop %bx
    pop %ax
    ret
calc_segment_base:#in DX:AX, return AX
    push %dx
    add %cs:(app_start), %ax
    adc %cs:(app_start + 2), %dx #TODO: 2 number's adding's carry is 1 at most
    divw divisor + 2
    pop %dx
    ret
.data
divisor:
    .word 16
    .word 10
    .word 512
app_start:
    .int 0x10000 #le: 0x0000 0x0001

