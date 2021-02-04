/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 03:17:04 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/04 01:07:37 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "des.h"
#include "des_const.h"

static uint64_t	des_f(uint64_t x, uint64_t key)
{
	x = des_permute(x, &g_e) ^ key;
	return (des_permute(des_sbox(x), &g_p));
}

uint64_t		des(t_des_ctx *ctx)
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
