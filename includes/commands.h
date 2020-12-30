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
*/
enum e_type = { NONE, MDC, CIC, STC };

typedef struct	s_command
{
	char		*name;
	t_cmdf		cmd;
	t_opt		*options;
	char		*usage;
	char		*help;
	enum e_type	type;
}				t_command;

extern const t_command	g_commands[];

#endif
