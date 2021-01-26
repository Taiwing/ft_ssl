/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 14:39:52 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/26 15:12:36 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md.h"
#include "libft.h"

/* DEBUG */
/*
void print_block(unsigned char *block, size_t size)
{
	uint32_t	*nbrs;

	for (size_t i = 0; i < size; ++i)
	{
		// ft_printf("%02x%s", block[i], !((i + 1) % 4) ? " " : "");
		ft_printf("%08hhb%s", block[i], !((i + 1) % 4) && ((i + 1) % 8) ? " " : "");
		if (!((i + 1) % 8))
			ft_printf("\n");
	}
	size /= 4;
	nbrs = (uint32_t *)block;
	for (size_t i = 0; i < size; ++i)
		ft_printf("%032b%c", nbrs[i], ((i + 1) % 2) ? ' ' : '\n');
}
*/
/* DEBUG */

void	init_registers(t_md_ctx *ctx)
{
	ft_memcpy((void *)ctx->regs, (void *)ctx->regs_start_state,
		sizeof(uint32_t) * ctx->regs_size);
}

static void	reverse_bytes(unsigned char *src, size_t n)
{
	unsigned char	*dst;
	unsigned char	tmp;

	dst = src + n - 1;
	while (dst > src)
	{
		tmp = *src;
		*src++ = *dst;
		*dst-- = tmp;
	}
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
		reverse_bytes((unsigned char *)&size, sizeof(size));
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
			reverse_bytes((unsigned char *)ctx->buf + (i * sizeof(uint32_t)),
				sizeof(uint32_t));
	}
	ft_memcpy((void *)tmp_regs, (void *)ctx->regs,
		sizeof(uint32_t) * ctx->regs_size);
	ctx->process_block(tmp_regs, ctx->block);
	for (size_t i = 0; i < ctx->regs_size; ++i)
		ctx->regs[i] += tmp_regs[i];
}
