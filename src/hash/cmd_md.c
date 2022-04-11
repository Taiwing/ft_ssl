/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_md.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 13:51:44 by yforeau           #+#    #+#             */
/*   Updated: 2022/04/11 16:19:33 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "readfile.h"
#include "md.h"

t_md_cmd	g_md_cmds[] = {
	{
		"md5", "MD5", md5, 4, 0,
		{
			0x67452301,
			0xefcdab89,
			0x98badcfe,
			0x10325476
		}
	},
	{
		"sha256", "SHA256", sha256, 8, 1,
		{
			0x6a09e667,
			0xbb67ae85,
			0x3c6ef372,
			0xa54ff53a,
			0x510e527f,
			0x9b05688c,
			0x1f83d9ab,
			0x5be0cd19
		}
	},
	{ NULL, NULL, NULL, 0, 0, { 0 } }
};

int			init_md_context(const char *md_name, t_md_ctx *ctx)
{
	t_md_cmd	*cmds;

	ctx->block = (uint32_t *)ctx->buf;
	cmds = (t_md_cmd *)g_md_cmds;
	while (cmds->name)
	{
		if (!ft_strcmp(md_name, cmds->name))
		{
			ctx->regs_start_state = (uint32_t *)cmds->regs_start_state;
			ctx->process_block = cmds->process_block;
			ctx->regs_size = cmds->regs_size;
			ctx->name = cmds->name;
			ctx->is_be = cmds->is_be;
			ctx->label = cmds->label;
			return (0);
		}
		++cmds;
	}
	return (1);
}

static void	print_hash(const char *name, t_cmdopt *opt, t_md_ctx *ctx) 
{
	if (name && !opt[MDC_QUIET].is_set && !opt[MDC_REVERSE].is_set)
		ft_printf("%s (%3$s%2$s%3$s) = ", ctx->label, name,
			opt[MDC_STRING].value == name ? "\"" : "");
	else if (!name && !opt[MDC_QUIET].is_set && !opt[MDC_PRINT].is_set)
		ft_printf("(stdin)= ");
	if (ctx->is_be)
		ft_printf("%*t%08x", (int)ctx->regs_size, ctx->regs);
	else
		ft_printf("%*t%02hhx", (int)ctx->regs_size * 4, ctx->regs);
	if (name && !opt[MDC_QUIET].is_set && opt[MDC_REVERSE].is_set)
		ft_printf(" %2$s%1$s%2$s", name,
			opt[MDC_STRING].value == name ? "\"" : "");
	ft_printf("\n");
}

static void	print_stdin(char *buf, int rd, int quiet)
{
	if (quiet)
		ft_printf("%.*s", rd, buf);
	else
		ft_printf("(\"%.*s\")= ",
			rd > 0 && buf[rd - 1] == '\n' ? rd - 1 : rd, buf);
}

static int	md_from_file(t_md_ctx *ctx, const char *file_name, t_cmdopt *opt)
{
	uint64_t	size;
	int			rd;

	size = 0;
	init_registers(ctx);
	while ((rd = readfile(file_name, ctx->buf, MD_BUF_SIZE)) == MD_BUF_SIZE)
	{
		if (!file_name && opt[MDC_PRINT].is_set)
			print_stdin(ctx->buf, rd, opt[MDC_QUIET].is_set);
		exec_md(ctx);
		size += rd * 8;
	}
	if (rd < 0)
	{
		print_readfile_error(ctx->name, file_name ? file_name : "stdin");
		return (1);
	}
	else if ((rd > 0 || (!rd && !size)) && rd < MD_BUF_SIZE
		&& !file_name && opt[MDC_PRINT].is_set)
		print_stdin(ctx->buf, rd, opt[MDC_QUIET].is_set);
	add_md_padding(ctx, rd, size);
	print_hash(file_name, opt, ctx);
	return (0);
}

static int	md_from_string(t_md_ctx *ctx, const char *str, t_cmdopt *opt)
{
	int			len;
	const char	*ptr;

	len = ft_strlen(str);
	ptr = str;
	init_registers(ctx);
	while (len >= MD_BUF_SIZE)
	{
		ft_memcpy((void *)ctx->buf, (void *)ptr, MD_BUF_SIZE);
		exec_md(ctx);
		ptr += MD_BUF_SIZE;
		len -= MD_BUF_SIZE;
	}
	ft_memcpy((void *)ctx->buf, (void *)ptr, len);
	add_md_padding(ctx, len, (ft_strlen(str) - len) * 8);
	print_hash(str, opt, ctx);
	return (0);
}

int	cmd_md(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int			ret;
	t_md_ctx	ctx;

	ret = 0;
	if (init_md_context(cmd->name, &ctx))
		return (1);
	if (opt[MDC_PRINT].is_set || (!opt[MDC_STRING].is_set && !*args))
	{
		ret = md_from_file(&ctx, NULL, opt);
		readfile(NULL, NULL, 0);
	}
	if (opt[MDC_STRING].is_set)
		md_from_string(&ctx, opt[MDC_STRING].value, opt);
	while (*args)
	{
		ret += md_from_file(&ctx, *args++, opt);
		readfile(NULL, NULL, 0);
	}
	return (ret);
}
