/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_des.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 11:05:16 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/28 13:05:48 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "readfile.h"
#include "des.h"

t_des_cmd	g_des_cmds[] = {
	{ "des-ecb", des_ecb },
	/*
	{ "des-cbc", des_cbc },
	{ "des", des_cbc },
	*/
	{ NULL, NULL }
};

static int	init_context(const t_command *cmd, t_des_ctx *ctx)
{
	t_des_cmd	*cmds;

	ft_bzero((void *)ctx, sizeof(t_des_ctx));
	cmds = g_des_cmds;
	while (cmds->name && ft_strcmp(cmds->name, cmd->name))
		++cmds;
	ctx->process_block = cmds->process_block;
	return (!ctx->process_block);
}

int	cmd_des(const t_command *cmd, t_cmdopt *opt, char **args)
{
	t_des_ctx	ctx;

	(void)opt;
	(void)args;
	if (init_context(cmd, &ctx))
		return (1);
	return (0);
}
