/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_keygen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:46:18 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/04 14:16:39 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "des.h"
#include "des_const.h"

#define ROTATE_LEFT(x, n) (((x << n) | (x >> (28-n))) & 0x0fffffff)

void	des_keygen(t_des_ctx *ctx)
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
