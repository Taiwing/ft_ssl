/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 03:17:04 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/05 01:56:04 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "des.h"

#define ROTATE_LEFT(x, n) (((x << n) | (x >> (28-n))) & 0x0fffffff)
#define BIT(x, n) (((x) >> (n)) & 0x01)

void		des_keygen(t_des_ctx *ctx)
{
	uint64_t	left;
	uint64_t	right;
	int			shift;

	left = des_permute(ctx->key, &g_left_pc1);
	right = des_permute(ctx->key, &g_right_pc1);
	for (int i = 0; i < DES_ROUNDS; ++i)
	{
		shift = (i == 0 || i == 1 || i == 8 || i == 15) ? 1 : 2;
		left = ROTATE_LEFT(left, shift);
		right = ROTATE_LEFT(right, shift);
		ctx->subkey[ctx->reverse ? DES_ROUNDS - i - 1 : i] =
			des_permute((left << 28) | right, &g_pc2);
	}
}

uint64_t	des_sbox(uint64_t x)
{
	uint64_t	y;
	size_t		j;
	size_t		k;

	y = 0;
	for (size_t i = 0; i < SBOX_NB; ++i)
	{
		y = y << 4;
		j = (BIT(x, 47) << 1) | BIT(x, 42);
		k = (BIT(x, 46) << 3) | (BIT(x, 45) << 2)
			| (BIT(x, 44) << 1) | BIT(x, 43);
		y |= g_sbox[i][j][k] & 0x0f;
		x = x << 6;
	}
	return (y);
}

uint64_t	des_permute(uint64_t x, const t_des_const *p)
{
	uint64_t	y;

	y = 0;
	for (size_t i = 0; i < p->output_size; ++i)
	{
		y = y << 1;
		y |= BIT(x, p->input_size - p->bit[i]);
	}
	return (y);
}

uint64_t	des_f(uint64_t x, uint64_t key)
{
	x = des_permute(x, &g_e) ^ key;
	return (des_permute(des_sbox(x), &g_p));
}

uint64_t	des(t_des_ctx *ctx)
{
	uint64_t	tmp;
	uint64_t	left;
	uint64_t	right;

	tmp = des_permute(ctx->plaintext, &g_ip);
	left = (tmp >> 32) & 0xffffffff;
	right = tmp & 0xffffffff;
	for (int i = 0; i < DES_ROUNDS; ++i)
	{
		tmp = right;	
		right = left ^ des_f(right, ctx->subkey[i]);
		left = tmp;
	}
	return (des_permute((right << 32) | left, &g_reverse_ip));
}
