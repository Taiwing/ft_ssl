/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 16:02:27 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 17:18:44 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

static int	get_options(t_command *command, char ***argv,
	t_cmdopt *options, int *status)
{
	t_optdata	opt;
	int			ret;
	int			argc;
	int			opt_id;

	opt_id = 0;
	argc = ft_wtlen(*argv);
	ft_bzero((void *)&opt, sizeof(t_optdata));
	init_getopt(&opt, OPTSTRING, command.options, &opt_id);
	while ((ret = ft_getopt_long_only(argc, *argv, &opt)) != -1)
	{
		if (ret == '?')
		{
			*status = 1;
			return (*status);
		}
		options[opt_id].ind = opt.optind;
		options[opt_id].is_set = 1;
		options[opt_id].value = opt.optarg;
	}
	*argv += opt.optind;
	return (0);
}

static int	invalidate_command(const char *cmd_name)
{
	char	*argv[2] = { "help", NULL };

	ft_dprintf(2, "ft_ssl: error: '%s' is an invalid command\n", cmd_name);
	exec_cmd(argv);
	return (1);
}

int			exec_cmd(char **argv)
{
	int			status;
	t_command	*command;
	const char	*cmd_name;
	t_cmdopt	options[MAX_OPT];

	ft_bzero((void *)options, sizeof(options));
	status = 0;
	cmd_name = argv[0];
	command = g_commands;
	if (!cmd_name)
		return (status);
	while (command->name && ft_strcmp(command->name, cmd_name))
		++command;
	if (!command->name)
		status = invalidate_command(cmd_name);
	else if (!get_options(command, &argv, options, &status))
	{
		if (command->cmdf)
			status = command->cmdf(command, options, argv);
		else
			ft_printf("command '%s' has not been implemented yet!\n", cmd_name);
	}
	return (status);
}
