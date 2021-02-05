/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 15:59:59 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/26 15:06:17 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md.h"
#include "libft.h"
#define OPS_COUNT	64

#define ROTATE_RIGHT(x, n) (((x) >> n) | ((x) << (32-n)))
#define a regs[0]
#define b regs[1]
#define c regs[2]
#define d regs[3]
#define e regs[4]
#define f regs[5]
#define g regs[6]
#define h regs[7]

const uint32_t g_sha256_T[OPS_COUNT] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void	init_block(uint32_t block[64], uint32_t *raw_block)
{
	uint32_t	s0, s1;

	ft_memcpy((void *)block, (void *)raw_block, 16 * sizeof(uint32_t));
	for (int i = 16; i < 64; ++i)
	{
		s0 = ROTATE_RIGHT(block[i - 15], 7)
			^ ROTATE_RIGHT(block[i - 15], 18)
			^ (block[i - 15] >> 3);
		s1 = ROTATE_RIGHT(block[i - 2], 17)
			^ ROTATE_RIGHT(block[i - 2], 19)
			^ (block[i - 2] >> 10);
		block[i] = block[i - 16] + s0 + block[i - 7] + s1;
	}
}

void	sha256(uint32_t regs[REGS_MAX_SIZE], uint32_t *raw_block)
{
	uint32_t	block[64];
	uint32_t	tmp, tmp2;

	(void)regs;
	init_block(block, raw_block);
	for (int i = 0; i < OPS_COUNT; ++i)
	{
		tmp = (ROTATE_RIGHT(e, 6) ^ ROTATE_RIGHT(e, 11) ^ ROTATE_RIGHT(e, 25))
			+ ((e & f) ^ (~e & g)) + h + g_sha256_T[i] + block[i];
		tmp2 = (ROTATE_RIGHT(a, 2) ^ ROTATE_RIGHT(a, 13) ^ ROTATE_RIGHT(a, 22))
			+ ((a & b) ^ (a & c) ^ (b & c));
		h = g;
		g = f;
		f = e;
		e = d + tmp;
		d = c;
		c = b;
		b = a;
		a = tmp + tmp2;
	}
}
