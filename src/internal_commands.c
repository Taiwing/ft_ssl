/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 19:46:19 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 20:07:16 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

int	cmd_help(const t_command *cmd, t_cmdopt *opt, char **args)
{
	enum e_type	type;

	(void)opt;
	(void)args;
	type = NONE;
	cmd = g_commands;
	while (cmd->name && cmd->type != INC)
	{
		if (type != cmd->type)
		{
			type = cmd->type;
			ft_printf("%c%s:\n", type ? '\n' : 0, g_type_strings[type]);
		}
		ft_printf("%s\n", cmd->name);
		++cmd;
	}
	return (0);
}
