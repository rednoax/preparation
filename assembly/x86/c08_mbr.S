.code16
.text
.equ app_lba_start, 100
.globl _start
_start:
    mov $0, %bx
    mov %bx, %ss
    mov %bx, %sp
    mov app_start, %ax  #mov ax, [app_start]:ds<<4
    mov app_start + 2, %dx
    divw divisor
    mov %ax, %ds    #ds: 0x1000
    mov %ax, %es    #es: 0x1000

    mov $app_lba_start, %si
    mov $0, %di
    call read_hard_disk_0
    mov %ax, %ds:(0)
    mov %dx, %ds:(2)
    mov $512, $divisor
    divw divisor
#511 512 513 1024
#0   1   1   2 qutient
#511 0   1   0 remainder
    cmp $0, %ax
    jz fin
    dec %ax
    cmp $0, %ax
    jz fin
    mov %ax, %cx
1:
    inc $si
    add $512, %bx
    call read_hard_disk_0
    loop 1b
fin:
    ljmp $0x1000, $0
read_hard_disk_0:   #read LBA[DI:SI] to mem DS:BX, DI:[27:16] SI[15:0]
    push %ax
    push %bx
    push %dx
    push %cx
    mov $0x1f2, %dx #0x1f2: 8bit port, read one sector
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
    or $0xe0, %ah
    out %al, %dx    #0x1f6: 8bit port, LBA[27:24]
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
    pop %cx
    pop %bx
    pop %dx
    pop %ax
    ret
.data
divisor:
    .word 16
app_start:
    .int 0x10000

