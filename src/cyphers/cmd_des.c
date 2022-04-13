/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_des.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 11:05:16 by yforeau           #+#    #+#             */
/*   Updated: 2022/04/13 16:04:43 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_des_utils.h"
#include "base64.h"
#include "debug.h"

t_des_cmd	g_des_cmds[] = {
	{ "des-ecb", des_ecb },
	{ "des-cbc", des_cbc },
	{ "des-cfb", des_cfb },
	{ "des-ofb", des_ofb },
	{ "des-ctr", des_ctr },
	{ "des-pcbc", des_pcbc },
	{ "des", des_cbc },
	{ NULL, NULL }
};

static int	init_context(t_des_ctx *ctx, const t_command *cmd, t_cmdopt *opt)
{
	t_des_cmd	*cmds;

	ft_bzero((void *)ctx, sizeof(t_des_ctx));
	ctx->first_block = 1;
	cmds = g_des_cmds;
	while (cmds->name && ft_strcmp(cmds->name, cmd->name))
		++cmds;
	ctx->padding = cmds->process_block == des_ecb
		|| cmds->process_block == des_cbc || cmds->process_block == des_pcbc;
	if ((ctx->process_block = cmds->process_block))
		return (parse_des_options(ctx, cmd, opt));
	return (1);
}

int	cmd_des(const t_command *cmd, t_cmdopt *opt, char **args)
{
	t_des_ctx	ctx;
	int			ret, outfd, reverse;

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
	reverse = ctx.reverse;
	ctx.reverse = ctx.padding ? ctx.reverse : 0;
	des_keygen(&ctx);
	ctx.reverse = reverse;
	ret = opt[CC_ENCRYPT].is_set || !ctx.padding
		? des_encrypt(outfd, &ctx, opt, cmd->name)
		: des_decrypt(outfd, &ctx, opt, cmd->name);
	if (outfd > 1)
		close(outfd);
	return (ret);
}
