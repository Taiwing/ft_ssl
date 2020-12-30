#ifndef OPTIONS_H
# define OPTIONS_H

# include "libft.h"

/* maximum length of the long options array for a command */
# define MAX_OPT 0x40
/* there will be no short options so just set the leave flag */
# define OPTSTRING "+"

/*
** The result of option parsing will be stored in a static array of t_cmdopt.
** This is a way of sending a formatted structure of options to the commands
** without having to make a special procedure for each one.
**
** set: boolean indicating if the option has been passed
** ind: keeps the index of the option in argv so the value of optind at the
** moment the option is passed, this will keep track of which option has been
** called last since some overwrite each other (this works because option
** parsing only uses long options so each element of argv corresponds to an
** option or to an argument)
** value: keeps optarg value if applicable
*/

typedef struct	s_cmdopt
{
	int			set;
	int			ind;
	char		*value;
}				t_cmdopt;

/*
** Options constants in options.c
*/

extern enum e_mdc_option_codes;
extern const t_opt g_mdc_options[];
extern enum e_cc_option_codes;
extern const t_opt g_cc_options[];
extern const t_opt *g_base64_options;
extern enum e_genrsa_option_codes;
extern const t_opt g_genrsa_options[];
extern enum e_rsa_option_codes;
extern const t_opt g_rsa_options[];
extern enum e_rsautl_option_codes;
extern const t_opt g_rsa_options[];

#endif
