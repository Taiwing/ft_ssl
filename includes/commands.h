/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:28:21 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 13:42:26 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_H
# define COMMANDS_H

# include "libft.h"


/* cmd function that will execute the command */
typedef void (* t_cmdf)(void); /* unsure about parameters for now */

/*
** type of command
**
** NONE: for the last NULL t_command element
** MDC: Message Digest Command
** CIC: Cipher Command
** STC: Standard Command
** INC: Internal Command (for openssl builtins like help, exit, etc..)
*/
enum e_type = { NONE, MDC, CIC, STC, INC };

typedef struct	s_command
{
	const char	*name;
	t_cmdf		cmd;
	t_opt		*options;
	const char	*usage;
	const char	*help;
	enum e_type	type;
}				t_command;

extern const t_command	g_commands[];

#endif
