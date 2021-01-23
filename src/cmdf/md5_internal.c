/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_internal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 15:59:59 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/22 20:19:44 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "libft.h"
#define OPS_COUNT	64
#define ROTATE_LEFT(x, n) (((x) << n) | ((x) >> (32-n)))
#define MD5_OP(a, b, c, d, k, s, j) \
	a += g_roundf[j/16](b, c, d) + block[k] + g_T[j]; \
	a = b + ROTATE_LEFT(a, s)

static uint32_t	f(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & y) | (~x & z));
}

static uint32_t	g(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & z) | (y & ~z));
}

static uint32_t	h(uint32_t x, uint32_t y, uint32_t z)
{
	return (x ^ y ^ z);
}

static uint32_t	i(uint32_t x, uint32_t y, uint32_t z)
{
	return (y ^ (x | ~z));
}

/* this will dispatch the right function depending on j/16 */
uint32_t (*g_roundf[4])(uint32_t, uint32_t, uint32_t) = { f, g, h, i };

/*
 * These integer arrays each represent an operation:
 * a = b + ((a + g_roundf[j/16](b,c,d) + block[k] + g_T[j]) <<< s)
 * 0: k
 * 1: s
*/
const int g_ops[OPS_COUNT][2] =
{
	/* Round 1. (f) */
	{ 0,  7}, { 1, 12}, { 2, 17}, { 3, 22},
	{ 4,  7}, { 5, 12}, { 6, 17}, { 7, 22},
	{ 8,  7}, { 9, 12}, {10, 17}, {11, 22},
	{12,  7}, {13, 12}, {14, 17}, {15, 22},

	/* Round 2. (g) */
	{ 1,  5}, { 6,  9}, {11, 14}, { 0, 20},
	{ 5,  5}, {10,  9}, {15, 14}, { 4, 20},
	{ 9,  5}, {14,  9}, { 3, 14}, { 8, 20},
	{13,  5}, { 2,  9}, { 7, 14}, {12, 20},

	/* Round 3. (h) */
	{ 5,  4}, { 8, 11}, {11, 16}, {14, 23},
	{ 1,  4}, { 4, 11}, { 7, 16}, {10, 23},
	{13,  4}, { 0, 11}, { 3, 16}, { 6, 23},
	{ 9,  4}, {12, 11}, {15, 16}, { 2, 23},

	/* Round 4. (i) */
	{ 0,  6}, { 7, 10}, {14, 15}, { 5, 21},
	{12,  6}, { 3, 10}, {10, 15}, { 1, 21},
	{ 8,  6}, {15, 10}, { 6, 15}, {13, 21},
	{ 4,  6}, {11, 10}, { 2, 15}, { 9, 21}
};

const uint32_t g_T[OPS_COUNT] =
{
	/* Round 1 */
	0xd76aa478,	0xe8c7b756,	0x242070db,	0xc1bdceee,
	0xf57c0faf,	0x4787c62a,	0xa8304613,	0xfd469501,
	0x698098d8,	0x8b44f7af,	0xffff5bb1,	0x895cd7be,
	0x6b901122,	0xfd987193,	0xa679438e,	0x49b40821,

	/* Round 2 */
	0xf61e2562,	0xc040b340,	0x265e5a51,	0xe9b6c7aa,
	0xd62f105d,	0x02441453,	0xd8a1e681,	0xe7d3fbc8,
	0x21e1cde6,	0xc33707d6,	0xf4d50d87,	0x455a14ed,
	0xa9e3e905,	0xfcefa3f8,	0x676f02d9,	0x8d2a4c8a,

	/* Round 3 */
	0xfffa3942,	0x8771f681,	0x6d9d6122,	0xfde5380c,
	0xa4beea44,	0x4bdecfa9,	0xf6bb4b60,	0xbebfbc70,
	0x289b7ec6,	0xeaa127fa,	0xd4ef3085,	0x04881d05,
	0xd9d4d039,	0xe6db99e5,	0x1fa27cf8,	0xc4ac5665,

	/* Round 4 */
	0xf4292244,	0x432aff97,	0xab9423a7,	0xfc93a039,
	0x655b59c3,	0x8f0ccc92,	0xffeff47d,	0x85845dd1,
	0x6fa87e4f,	0xfe2ce6e0,	0xa3014314,	0x4e0811a1,
	0xf7537e82,	0xbd3af235,	0x2ad7d2bb,	0xeb86d391
};

void	md5(uint32_t regs[4], uint32_t *block)
{
	int	jp;

	for (int j = 0; j < OPS_COUNT; ++j)

	{
		jp = OPS_COUNT - j;
		MD5_OP(regs[jp & 3], regs[(jp + 1) & 3], regs[(jp + 2) & 3],
			regs[(jp + 3) & 3], g_ops[j][0], g_ops[j][1], j);
	}
}
