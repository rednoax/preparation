#ifndef _DEFINES_H
#define _DEFINES_H

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define VIC_BASE_ADDR 0x10140000
#define VIC_STATUS (*(volatile int*)VIC_BASE_ADDR)
#define VIC_INTENABLE (*(volatile int*)(VIC_BASE_ADDR + 0x10))//* and (type) are at the same level in c operator precedence table, from right to left
#define SIC_BASE_ADDR 0x10003000
#endif
