/* find_next_bit.c: fallback find next bit implementation
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#if 0
#include <linux/bitops.h>
#include <linux/module.h>
#include <asm/types.h>
#include <asm/byteorder.h>
#else
#include <stdio.h>
#define BITS_PER_LONG 32
#define CONFIG_GENERIC_FIND_NEXT_BIT
#define CONFIG_GENERIC_FIND_FIRST_BIT
#define EXPORT_SYMBOL(x) 
#undef __BIG_ENDIAN

/**
 * ffs - find first bit set
 * @x: the word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */
static inline int ffs(int x)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

/**
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static /*__always_inline*/ unsigned long __ffs(unsigned long word)
{
	int num = 0;

#if BITS_PER_LONG == 64
	if ((word & 0xffffffff) == 0) {
		num += 32;
		word >>= 32;
	}
#endif
	if ((word & 0xffff) == 0) {
		num += 16;
		word >>= 16;
	}
	if ((word & 0xff) == 0) {
		num += 8;
		word >>= 8;
	}
	if ((word & 0xf) == 0) {
		num += 4;
		word >>= 4;
	}
	if ((word & 0x3) == 0) {
		num += 2;
		word >>= 2;
	}
	if ((word & 0x1) == 0)
		num += 1;
	return num;
}

/*
 * ffz - find first zero in word.
 * @word: The word to search
 *
 * Undefined if no zero exists, so code should check against ~0UL first.
 */
#define ffz(x)  __ffs(~(x))

//
void print_bitmap(unsigned long *addr, unsigned long nr)
{
	unsigned long i;
	for (i = 0; i < (nr * 32); i += 32)
		printf("[%02ld:%02ld]  ", i + 31, i);
	printf("\n");
	for (i = 0; i < nr; i++)
		printf("%08lx ", addr[i]);
	printf("\n");
}
//
#endif
#define BITOP_WORD(nr)		((nr) / BITS_PER_LONG)

#ifdef CONFIG_GENERIC_FIND_NEXT_BIT
/*
 * Find the next set bit in a memory region.
 */
unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
			    unsigned long offset)
{
	const unsigned long *p = addr + BITOP_WORD(offset);
	unsigned long result = offset & ~(BITS_PER_LONG-1);
	unsigned long tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset %= BITS_PER_LONG;
	if (offset) {
		tmp = *(p++);
		tmp &= (~0UL << offset);
		if (size < BITS_PER_LONG)
			goto found_first;
		if (tmp)
			goto found_middle;
		size -= BITS_PER_LONG;
		result += BITS_PER_LONG;
	}
	while (size & ~(BITS_PER_LONG-1)) {
		if ((tmp = *(p++)))
			goto found_middle;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;
	tmp = *p;

found_first:
	tmp &= (~0UL >> (BITS_PER_LONG - size));
	if (tmp == 0UL)		/* Are any bits set? */
		return result + size;	/* Nope. */
found_middle:
	return result + __ffs(tmp);
}
EXPORT_SYMBOL(find_next_bit);

/*
 * This implementation of find_{first,next}_zero_bit was stolen from
 * Linus' asm-alpha/bitops.h.
 */
unsigned long find_next_zero_bit(const unsigned long *addr, unsigned long size,
				 unsigned long offset)
{
	const unsigned long *p = addr + BITOP_WORD(offset);
	unsigned long result = offset & ~(BITS_PER_LONG-1);
	unsigned long tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset %= BITS_PER_LONG;
	if (offset) {
		tmp = *(p++);
		tmp |= ~0UL >> (BITS_PER_LONG - offset);
		if (size < BITS_PER_LONG)
			goto found_first;
		if (~tmp)
			goto found_middle;
		size -= BITS_PER_LONG;
		result += BITS_PER_LONG;
	}
	while (size & ~(BITS_PER_LONG-1)) {
		if (~(tmp = *(p++)))
			goto found_middle;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;
	tmp = *p;

found_first:
	tmp |= ~0UL << size;
	if (tmp == ~0UL)	/* Are any bits zero? */
		return result + size;	/* Nope. */
found_middle:
	return result + ffz(tmp);
}
EXPORT_SYMBOL(find_next_zero_bit);
#endif /* CONFIG_GENERIC_FIND_NEXT_BIT */

#ifdef CONFIG_GENERIC_FIND_FIRST_BIT
/*
 * Find the first set bit in a memory region.
 */
unsigned long find_first_bit(const unsigned long *addr, unsigned long size)
{
	const unsigned long *p = addr;
	unsigned long result = 0;
	unsigned long tmp;

	while (size & ~(BITS_PER_LONG-1)) {
		if ((tmp = *(p++)))
			goto found;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;

	tmp = (*p) & (~0UL >> (BITS_PER_LONG - size));
	if (tmp == 0UL)		/* Are any bits set? */
		return result + size;	/* Nope. */
found:
	return result + __ffs(tmp);
}
EXPORT_SYMBOL(find_first_bit);

/*
 * Find the first cleared bit in a memory region.
 */
unsigned long find_first_zero_bit(const unsigned long *addr, unsigned long size)
{
	const unsigned long *p = addr;
	unsigned long result = 0;
	unsigned long tmp;

	while (size & ~(BITS_PER_LONG-1)) {
		if (~(tmp = *(p++)))
			goto found;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;

	tmp = (*p) | (~0UL << size);
	if (tmp == ~0UL)	/* Are any bits zero? */
		return result + size;	/* Nope. */
found:
	return result + ffz(tmp);
}
EXPORT_SYMBOL(find_first_zero_bit);
#endif /* CONFIG_GENERIC_FIND_FIRST_BIT */

#ifdef __BIG_ENDIAN

/* include/linux/byteorder does not support "unsigned long" type */
static inline unsigned long ext2_swabp(const unsigned long * x)
{
#if BITS_PER_LONG == 64
	return (unsigned long) __swab64p((u64 *) x);
#elif BITS_PER_LONG == 32
	return (unsigned long) __swab32p((u32 *) x);
#else
#error BITS_PER_LONG not defined
#endif
}

/* include/linux/byteorder doesn't support "unsigned long" type */
static inline unsigned long ext2_swab(const unsigned long y)
{
#if BITS_PER_LONG == 64
	return (unsigned long) __swab64((u64) y);
#elif BITS_PER_LONG == 32
	return (unsigned long) __swab32((u32) y);
#else
#error BITS_PER_LONG not defined
#endif
}

unsigned long generic_find_next_zero_le_bit(const unsigned long *addr, unsigned
		long size, unsigned long offset)
{
	const unsigned long *p = addr + BITOP_WORD(offset);
	unsigned long result = offset & ~(BITS_PER_LONG - 1);
	unsigned long tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset &= (BITS_PER_LONG - 1UL);
	if (offset) {
		tmp = ext2_swabp(p++);
		tmp |= (~0UL >> (BITS_PER_LONG - offset));
		if (size < BITS_PER_LONG)
			goto found_first;
		if (~tmp)
			goto found_middle;
		size -= BITS_PER_LONG;
		result += BITS_PER_LONG;
	}

	while (size & ~(BITS_PER_LONG - 1)) {
		if (~(tmp = *(p++)))
			goto found_middle_swap;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;
	tmp = ext2_swabp(p);
found_first:
	tmp |= ~0UL << size;
	if (tmp == ~0UL)	/* Are any bits zero? */
		return result + size; /* Nope. Skip ffz */
found_middle:
	return result + ffz(tmp);

found_middle_swap:
	return result + ffz(ext2_swab(tmp));
}

EXPORT_SYMBOL(generic_find_next_zero_le_bit);

unsigned long generic_find_next_le_bit(const unsigned long *addr, unsigned
		long size, unsigned long offset)
{
	const unsigned long *p = addr + BITOP_WORD(offset);
	unsigned long result = offset & ~(BITS_PER_LONG - 1);
	unsigned long tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset &= (BITS_PER_LONG - 1UL);
	if (offset) {
		tmp = ext2_swabp(p++);
		tmp &= (~0UL << offset);
		if (size < BITS_PER_LONG)
			goto found_first;
		if (tmp)
			goto found_middle;
		size -= BITS_PER_LONG;
		result += BITS_PER_LONG;
	}

	while (size & ~(BITS_PER_LONG - 1)) {
		tmp = *(p++);
		if (tmp)
			goto found_middle_swap;
		result += BITS_PER_LONG;
		size -= BITS_PER_LONG;
	}
	if (!size)
		return result;
	tmp = ext2_swabp(p);
found_first:
	tmp &= (~0UL >> (BITS_PER_LONG - size));
	if (tmp == 0UL)		/* Are any bits set? */
		return result + size; /* Nope. */
found_middle:
	return result + __ffs(tmp);

found_middle_swap:
	return result + __ffs(ext2_swab(tmp));
}
EXPORT_SYMBOL(generic_find_next_le_bit);
#endif /* __BIG_ENDIAN */

int main(int argc, char** argv)
{
	unsigned long bitmap[][4] = {
		//[31:0] [63:32] [95:64] [127:96]
		{0x0, 0x0, 1<<31, 0x0},
		{0x0, 0x0, 1|(1<<31), ~0},
		{0x0, 0x0, 1<<31, 0x1},
		{~0x1, ~0x1, ~0x0, ~0x1},
	};
	unsigned long offset, ret, size, nr, *addr;
	size = sizeof(bitmap[0])<<3;
	printf("total bits:%ld\n", size);
	print_bitmap(bitmap[0], 4);
	offset = 95;
	ret = find_next_bit(bitmap[0], size, offset);
	printf("next bit: [%ld @%ld\n", offset, ret);
	offset = 32;
	ret = find_next_bit(bitmap[0], size, offset);
	printf("next bit: [%ld @%ld\n", offset, ret);
	offset = 96;
	ret = find_next_bit(bitmap[0], size, offset);
	printf("next bit: [%ld @%ld\n", offset, ret);
	
	print_bitmap(bitmap[1], 4);
	offset = 65;
	ret = find_next_zero_bit(bitmap[1], size, offset);
	printf("zero bit: [%ld @%ld\n", offset, ret);
	offset = 64;
	ret = find_next_zero_bit(bitmap[1], size, offset);
	printf("zero bit: [%ld @%ld\n", offset, ret);
	offset = 95;
	ret = find_next_zero_bit(bitmap[1], size, offset);
	printf("zero bit: [%ld @%ld\n", offset, ret);

	addr = &bitmap[2][3];
	nr = 1;
	print_bitmap(addr, nr);
	ret = find_first_bit(addr, nr * 32);
	printf("first bit: %ld\n", ret);
	addr = bitmap[2];
	nr = 4;
	print_bitmap(addr, nr);
	ret = find_first_bit(addr, nr * 32);
	printf("first bit: %ld\n", ret);
	addr = bitmap[2];
	nr = 2;
	print_bitmap(addr, nr);
	ret = find_first_bit(addr, nr * 32);
	printf("first bit: %ld\n", ret);
	
	addr = bitmap[3];
	nr = 2;
	print_bitmap(addr, nr);
	ret = find_first_zero_bit(addr, nr * 32);
	printf("first zero bit: %ld\n", ret);
	addr = &bitmap[3][2];
	nr = 2;
	print_bitmap(addr, nr);
	ret = find_first_zero_bit(addr, nr * 32);
	printf("first zero bit: %ld\n", ret);
	addr = &bitmap[3][2];
	nr = 1;
	print_bitmap(addr, nr);
	ret = find_first_zero_bit(addr, nr * 32);
	printf("first zero bit: %ld\n", ret);
	
	addr = &bitmap[1][2];
	ret = ffs(addr[0]);
	printf("ffs %lx:%ld\n", addr[0], ret);
	addr = &bitmap[0][2];
	ret = ffs(addr[0]);
	printf("ffs %lx:%ld\n", addr[0], ret);
	addr = &bitmap[0][0];
	ret = ffs(addr[0]);
	printf("ffs %lx:%ld\n", addr[0], ret);
	offset = 0x8086;
	ret = ffs(offset);
	printf("ffs %lx:%ld\n", offset, ret);
	offset = 0x400;
	ret = ffs(offset);
	printf("ffs %lx:%ld\n", offset, ret);
	
	addr = &bitmap[1][2];
	ret = __ffs(addr[0]);
	printf("__ffs %lx:%ld\n", addr[0], ret);
	addr = &bitmap[0][2];
	ret = __ffs(addr[0]);
	printf("__ffs %lx:%ld\n", addr[0], ret);
	addr = &bitmap[0][0];
	ret = __ffs(addr[0]);
	printf("__ffs %lx:%ld\n", addr[0], ret);
	offset = 0x8086;
	ret = __ffs(offset);
	printf("__ffs %lx:%ld\n", offset, ret);
	offset = 0x400;
	ret = __ffs(offset);
	printf("__ffs %lx:%ld\n", offset, ret);

	offset = 0x400;
	ret = ffz(offset);
	printf("ffz %lx:%ld\n", offset, ret);
	offset = 0xff;
	ret = ffz(offset);
	printf("ffz %lx:%ld\n", offset, ret);
	offset = ~0x0;
	ret = ffz(offset);
	printf("ffz %lx:%ld\n", offset, ret);
	

	return 0;
}