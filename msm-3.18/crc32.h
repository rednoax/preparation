/*
 * crc32.h
 * See linux/lib/crc32.c for license and changes
 */
#ifndef _LINUX_CRC32_H
#define _LINUX_CRC32_H
#if 0
#include <linux/types.h>
#include <linux/bitrev.h>
#else
#define __pure			__attribute__((pure))
//#define __attribute_const__	__attribute__((__const__))
#define __force
#define __bitwise
#define __HAVE_BUILTIN_BSWAP32__
#define __cacheline_aligned
//#define __LITTLE_ENDIAN
typedef unsigned char u8;
typedef unsigned int __u32;
typedef __u32 u32;
typedef __u32 __bitwise __be32;
typedef long		__kernel_long_t;
typedef unsigned long	__kernel_ulong_t;
typedef __kernel_ulong_t __kernel_size_t;
typedef __kernel_size_t		size_t;
typedef __u32 __bitwise __le32;
#define unlikely(x) (x)
static inline __attribute_const__ __u32 __fswab32(__u32 val)
{
#ifdef __HAVE_BUILTIN_BSWAP32__
	return __builtin_bswap32(val);
#elif defined(__arch_swab32)
	return __arch_swab32(val);
#else
	return ___constant_swab32(val);
#endif
}

#define ___constant_swab32(x) ((__u32)(				\
	(((__u32)(x) & (__u32)0x000000ffUL) << 24) |		\
	(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |		\
	(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |		\
	(((__u32)(x) & (__u32)0xff000000UL) >> 24)))

#define __swab32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swab32(x) :			\
	__fswab32(x))

#define cpu_to_le64(x)	(x)
#define cpu_to_le32(x)	(x)
#define cpu_to_be32(x) __cpu_to_be32(x)
#define __cpu_to_le32(x) ((__force __le32)(__u32)(x))
#define __cpu_to_be32(x) ((__force __be32)__swab32((x)))
#define __be32_to_cpu(x) __swab32((__force __u32)(__be32)(x))
#define __le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_LICENSE(x)
#define EXPORT_SYMBOL(x)
#endif
u32 __pure crc32_le(u32 crc, unsigned char const *p, size_t len);
u32 __pure crc32_be(u32 crc, unsigned char const *p, size_t len);

/**
 * crc32_le_combine - Combine two crc32 check values into one. For two
 * 		      sequences of bytes, seq1 and seq2 with lengths len1
 * 		      and len2, crc32_le() check values were calculated
 * 		      for each, crc1 and crc2.
 *
 * @crc1: crc32 of the first block
 * @crc2: crc32 of the second block
 * @len2: length of the second block
 *
 * Return: The crc32_le() check value of seq1 and seq2 concatenated,
 * 	   requiring only crc1, crc2, and len2. Note: If seq_full denotes
 * 	   the concatenated memory area of seq1 with seq2, and crc_full
 * 	   the crc32_le() value of seq_full, then crc_full ==
 * 	   crc32_le_combine(crc1, crc2, len2) when crc_full was seeded
 * 	   with the same initializer as crc1, and crc2 seed was 0. See
 * 	   also crc32_combine_test().
 */
u32 __attribute_const__ crc32_le_shift(u32 crc, size_t len);

static inline u32 crc32_le_combine(u32 crc1, u32 crc2, size_t len2)
{
	return crc32_le_shift(crc1, len2) ^ crc2;
}

u32 __pure __crc32c_le(u32 crc, unsigned char const *p, size_t len);

/**
 * __crc32c_le_combine - Combine two crc32c check values into one. For two
 * 			 sequences of bytes, seq1 and seq2 with lengths len1
 * 			 and len2, __crc32c_le() check values were calculated
 * 			 for each, crc1 and crc2.
 *
 * @crc1: crc32c of the first block
 * @crc2: crc32c of the second block
 * @len2: length of the second block
 *
 * Return: The __crc32c_le() check value of seq1 and seq2 concatenated,
 * 	   requiring only crc1, crc2, and len2. Note: If seq_full denotes
 * 	   the concatenated memory area of seq1 with seq2, and crc_full
 * 	   the __crc32c_le() value of seq_full, then crc_full ==
 * 	   __crc32c_le_combine(crc1, crc2, len2) when crc_full was
 * 	   seeded with the same initializer as crc1, and crc2 seed
 * 	   was 0. See also crc32c_combine_test().
 */
u32 __attribute_const__ __crc32c_le_shift(u32 crc, size_t len);

static inline u32 __crc32c_le_combine(u32 crc1, u32 crc2, size_t len2)
{
	return __crc32c_le_shift(crc1, len2) ^ crc2;
}

#define crc32(seed, data, length)  crc32_le(seed, (unsigned char const *)(data), length)

/*
 * Helpers for hash table generation of ethernet nics:
 *
 * Ethernet sends the least significant bit of a byte first, thus crc32_le
 * is used. The output of crc32_le is bit reversed [most significant bit
 * is in bit nr 0], thus it must be reversed before use. Except for
 * nics that bit swap the result internally...
 */
#define ether_crc(length, data)    bitrev32(crc32_le(~0, data, length))
#define ether_crc_le(length, data) crc32_le(~0, data, length)

#endif /* _LINUX_CRC32_H */
