#ifndef COMMANDS_H
# define COMMANDS_H

# include "libft.h"


/* cmd function that will execute the command */
typedef void (* t_cmdf)(void); /* unsure about parameters for now */

typedef struct	s_command
{
	char		*name;
	t_cmdf		cmd;
	t_opt		*options;
	char		*usage;
	char		*help;
}				t_command;

extern const t_command	g_commands[];

#endif
