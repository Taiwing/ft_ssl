/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_cem.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 13:07:20 by yforeau           #+#    #+#             */
/*   Updated: 2022/04/13 12:04:30 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "des.h"

uint64_t	des_ecb(struct s_des_ctx *ctx)
{
	ctx->cyphertext = des(ctx);
	return (ctx->cyphertext);
}

uint64_t	des_cbc(struct s_des_ctx *ctx)
{
	if (!ctx->reverse)
		ctx->plaintext ^= ctx->iv;
	ctx->cyphertext = des(ctx);
	if (ctx->reverse)
	{
		ctx->cyphertext ^= ctx->iv;
		ctx->iv = ctx->plaintext;
	}
	else
		ctx->iv = ctx->cyphertext;
	return (ctx->cyphertext);
}

uint64_t	des_cfb(struct s_des_ctx *ctx)
{
	uint64_t	plaintext = ctx->plaintext;

	ctx->plaintext = ctx->iv;
	ctx->cyphertext = des(ctx);
	ctx->cyphertext ^= plaintext;
	ctx->iv = ctx->reverse ? plaintext : ctx->cyphertext;
	return (ctx->cyphertext);
}

uint64_t	des_pcbc(struct s_des_ctx *ctx)
{
	uint64_t	last_plaintext;

	if (!ctx->reverse)
	{
		last_plaintext = ctx->plaintext;
		ctx->plaintext ^= ctx->iv;
		if (!ctx->first_block)
			ctx->plaintext ^= ctx->last_plaintext;
		ctx->first_block = 0;
		ctx->last_plaintext = last_plaintext;
	}
	ctx->cyphertext = des(ctx);
	if (ctx->reverse)
	{
		ctx->cyphertext ^= ctx->iv;
		ctx->iv = ctx->plaintext;
		if (!ctx->first_block)
			ctx->cyphertext ^= ctx->last_plaintext;
		ctx->first_block = 0;
		ctx->last_plaintext = ctx->cyphertext;
	}
	else
		ctx->iv = ctx->cyphertext;
	return (ctx->cyphertext);
}
