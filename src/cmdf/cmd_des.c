/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_des.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 11:05:16 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/02 21:57:36 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_des_utils.h"
#include "readfile.h"
#include "base64.h"

t_des_cmd	g_des_cmds[] = {
	{ "des-ecb", des_ecb },
	{ "des-cbc", des_cbc },
	{ "des", des_cbc },
	{ NULL, NULL }
};

int			des_readfile(const char *file_name, char *buf, size_t bufsize,
	t_cmdopt *opt)
{
	if (opt[CC_BASE64].is_set)
		return (base64_readfile(file_name, buf, bufsize, 0));
	return (readfile(file_name, buf, bufsize));
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

	(void)args;
	if (init_context(&ctx, cmd, opt))
		return (1);
	des_keygen(&ctx);
	return (0);
}
