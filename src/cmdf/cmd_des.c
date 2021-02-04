/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_des.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 11:05:16 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/04 19:11:51 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_des_utils.h"
#include "readfile.h"
#include "base64.h"
#include "debug.h"

t_des_cmd	g_des_cmds[] = {
	{ "des-ecb", des_ecb },
	{ "des-cbc", des_cbc },
	{ "des", des_cbc },
	{ NULL, NULL }
};

int			des_readfile(const char *file_name, char *buf, size_t bufsize,
	t_cmdopt *opt)
{
	if (!opt[CC_ENCRYPT].is_set && opt[CC_BASE64].is_set)
		return (base64_readfile(file_name, buf, bufsize, 0));
	return (readfile(file_name, buf, bufsize));
}

int			des_writefile(int fd, char *buf, size_t n, t_cmdopt *opt)
{
	if (opt[CC_ENCRYPT].is_set && opt[CC_BASE64].is_set)
		return (base64_writefile(fd, buf, n, 0));
	return (write(fd, buf, n));
}
static int	des_decrypt(int outfd, t_des_ctx *ctx,
	t_cmdopt *opt, const char *cmd)
{
	int			rd;
	int			wr;
	uint64_t	block;
	char		*prev;

	(void)cmd;
	wr = 1;
	prev = NULL;
	while ((rd = des_readfile(opt[CC_INPUT].value, (char *)&ctx->plaintext,
		sizeof(uint64_t), opt)) > 0 && wr >= 0)
	{
		if (prev)
			wr = des_writefile(outfd, prev, sizeof(uint64_t), opt);
		else
			prev = (char *)&block;
		ft_memswap((void *)&ctx->plaintext, sizeof(uint64_t));
		ctx->process_block(ctx);
		block = ctx->cyphertext;
		ft_memswap((void *)&block, sizeof(uint64_t));
	}
	if (!rd && wr >= 0 && prev
		&& prev[sizeof(uint64_t) - 1] < (int)sizeof(uint64_t))
		wr = des_writefile(outfd, prev,
			sizeof(uint64_t) - prev[sizeof(uint64_t) - 1], opt);
	return (rd == -1);
}

static int	des_encrypt(int outfd, t_des_ctx *ctx,
	t_cmdopt *opt, const char *cmd)
{
	int			c;
	int			rd;
	int			wr;
	uint64_t	block;

	(void)cmd;
	wr = 1;
	c = 0;
	while ((rd = des_readfile(opt[CC_INPUT].value, (char *)&ctx->plaintext,
		sizeof(uint64_t), opt)) >= 0 && wr >= 0 && !c)
	{
		if ((c = sizeof(uint64_t) - rd))
			ft_memset((void *)&ctx->plaintext + rd, c, c);
		ft_memswap((void *)&ctx->plaintext, sizeof(uint64_t));
		ctx->process_block(ctx);
		block = ctx->cyphertext;
		ft_memswap((void *)&block, sizeof(uint64_t));
		wr = des_writefile(outfd, (char *)&block, sizeof(uint64_t), opt);
	}
	if (opt[CC_BASE64].is_set && rd >= 0 && wr >= 0)
		return (base64_writefile(outfd, NULL, 0, 1));
	return (rd == -1);
}

static int	init_context(t_des_ctx *ctx, const t_command *cmd, t_cmdopt *opt)
{
	t_des_cmd	*cmds;

	ft_bzero((void *)ctx, sizeof(t_des_ctx));
	cmds = g_des_cmds;
	while (cmds->name && ft_strcmp(cmds->name, cmd->name))
		++cmds;
	if ((ctx->process_block = cmds->process_block))
		return (parse_des_options(ctx, cmd, opt));
	return (1);
}

int	cmd_des(const t_command *cmd, t_cmdopt *opt, char **args)
{
	t_des_ctx	ctx;
	int			ret;
	int			outfd;

	(void)args;
	ret = 1;
	outfd = 1;
	base64_readfile(NULL, NULL, 0, 1);
	base64_writefile(1, NULL, 0, 1);
	if (init_context(&ctx, cmd, opt))
		return (ret);
	if (opt[CC_PRINT].is_set)
		return (print_des_ctx(&ctx));
	if (opt[CC_OUTPUT].is_set
		&& (outfd = output_option(opt[CC_OUTPUT].value, cmd->name)) == -1)
		return (ret);
	if (write_salt(outfd, &ctx, opt))
		return (ret);
	des_keygen(&ctx);
	ret = opt[CC_ENCRYPT].is_set ? des_encrypt(outfd, &ctx, opt, cmd->name)
		: des_decrypt(outfd, &ctx, opt, cmd->name);
	if (outfd > 1)
		close(outfd);
	return (ret);
}
