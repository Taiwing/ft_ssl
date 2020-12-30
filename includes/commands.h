/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:28:21 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 20:01:57 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_H
# define COMMANDS_H

# include "options.h"

/*
** type of command
**
** MDC: Message Digest Command
** CIC: Cipher Command
** STC: Standard Command
** INC: Internal Command (for openssl builtins like help, exit, etc..)
** NONE: for the last NULL t_command element
*/
enum e_type { MDC = 0, CIC, STC, INC, NONE };
extern const char *g_type_strings[];

typedef struct	s_command
{
	const char	*name;
	int			(*cmdf)(const struct s_command *, t_cmdopt *, char **);
	const t_opt	*options;
	const char	*help;
	enum e_type	type;
}				t_command;

extern const t_command	g_commands[];

/* command utility functions */
void	input_cmd(void);
int		exec_cmd(char **argv);

/* commands */
int		cmd_help(const t_command *cmd, t_cmdopt *opt, char **args);

#endif
