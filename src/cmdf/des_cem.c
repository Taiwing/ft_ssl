/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_cem.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 13:07:20 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/04 14:34:50 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "des.h"

uint64_t	des_ecb(struct s_des_ctx *ctx)
{
	ctx->cyphertext = des(ctx);
	ctx->plaintext = ctx->cyphertext;
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
