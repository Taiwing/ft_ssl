/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 14:39:52 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/05 01:35:00 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md.h"
#include "libft.h"

void	init_registers(t_md_ctx *ctx)
{
	ft_memcpy((void *)ctx->regs, (void *)ctx->regs_start_state,
		sizeof(uint32_t) * ctx->regs_size);
}

void	add_md_padding(t_md_ctx *ctx, int rd, uint64_t size)
{
	size += rd * 8;
	ft_bzero((void *)ctx->buf + rd, MD_BUF_SIZE - rd);
	ctx->buf[rd++] = 0x80;
	if ((size_t)(MD_BUF_SIZE - rd) < sizeof(size))
	{
		exec_md(ctx);
		ft_bzero((void *)ctx->buf, MD_BUF_SIZE);
	}
	if (ctx->is_be)
		ft_memswap((void *)&size, sizeof(size));
	ft_memcpy((void *)ctx->buf + MD_BUF_SIZE - sizeof(size),
		(void *)&size, sizeof(size));
	exec_md(ctx);
}

void	exec_md(t_md_ctx *ctx)
{
	uint32_t	tmp_regs[REGS_MAX_SIZE];

	if (ctx->is_be)
	{
		for (int i = 0; i < MD_BLOCK_SIZE; ++i)
			ft_memswap((void *)ctx->buf + (i * sizeof(uint32_t)),
				sizeof(uint32_t));
	}
	ft_memcpy((void *)tmp_regs, (void *)ctx->regs,
		sizeof(uint32_t) * ctx->regs_size);
	ctx->process_block(tmp_regs, ctx->block);
	for (size_t i = 0; i < ctx->regs_size; ++i)
		ctx->regs[i] += tmp_regs[i];
}
