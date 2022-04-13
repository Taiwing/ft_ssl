/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 23:58:22 by yforeau           #+#    #+#             */
/*   Updated: 2022/04/13 10:39:58 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "commands.h"
#include "readfile.h"
#include "base64.h"
#include "des.h"

int	des_readfile(const char *file_name, char *buf, size_t bufsize,
	t_cmdopt *opt)
{
	if (!opt[CC_ENCRYPT].is_set && opt[CC_BASE64].is_set)
		return (base64_readfile(file_name, buf, bufsize, 0));
	return (readfile(file_name, buf, bufsize));
}

int	des_writefile(int fd, char *buf, size_t n, t_cmdopt *opt)
{
	if (opt[CC_ENCRYPT].is_set && opt[CC_BASE64].is_set)
		return (base64_writefile(fd, buf, n, 0));
	return (write(fd, buf, n));
}

uint64_t	exec_cypher(t_des_ctx *ctx)
{
	uint64_t	block;

	ft_memswap((void *)&ctx->plaintext, sizeof(uint64_t));
	ctx->process_block(ctx);
	block = ctx->cyphertext;
	ft_memswap((void *)&block, sizeof(uint64_t));
	return (block);
}

int	des_decrypt(int outfd, t_des_ctx *ctx, t_cmdopt *opt, const char *cmd)
{
	int			rd;
	int			wr;
	char		last;
	uint64_t	block;
	char		*prev;

	wr = 1;
	prev = NULL;
	while (wr >= 0 && (rd = des_readfile(opt[CC_INPUT].value,
		(char *)&ctx->plaintext, sizeof(uint64_t), opt)) > 0
		&& (!ctx->padding || rd == sizeof(uint64_t)))
	{
		if (prev)
			wr = des_writefile(outfd, prev, sizeof(uint64_t), opt);
		prev = (char *)&block;
		block = exec_cypher(ctx);
	}
	if (rd > 0 && rd < (int)sizeof(uint64_t) && ctx->padding)
		return (!!ft_dprintf(2, "\nft_ssl: %s: error: incomplete %u bytes"
			" input block\n", cmd, sizeof(uint64_t)));
	if (!rd && wr >= 0 && prev && (last = prev[sizeof(uint64_t) - 1])
		< (int)sizeof(uint64_t) && last > 0)
		wr = des_writefile(outfd, prev, sizeof(uint64_t) - last, opt);
	if (rd == -1 || wr == -1)
		return (!!ft_dprintf(2, "ft_ssl: %s: %s\n", cmd, strerror(errno)));
	return (0);
}

int	des_encrypt(int outfd, t_des_ctx *ctx, t_cmdopt *opt, const char *cmd)
{
	int			c;
	int			rd;
	int			wr;
	uint64_t	block;

	wr = 1;
	c = 0;
	while (wr >= 0 && !c && (rd = des_readfile(opt[CC_INPUT].value,
		(char *)&ctx->plaintext, sizeof(uint64_t), opt)) >= 0)
	{
		if ((c = sizeof(uint64_t) - rd))
			ft_memset((void *)&ctx->plaintext + rd, c, c);
		block = exec_cypher(ctx);
		wr = des_writefile(outfd, (char *)&block,
			ctx->padding ? sizeof(uint64_t) : (size_t)rd, opt);
	}
	if (rd >= 0 && wr >= 0 && opt[CC_BASE64].is_set)
		return (base64_writefile(outfd, NULL, 0, 1));
	if (rd == -1 || wr == -1)
		return (!!ft_dprintf(2, "ft_ssl: %s: %s\n", cmd, strerror(errno)));
	return (0);
}
