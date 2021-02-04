/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:28:24 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/04 15:52:31 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONS_H
# define OPTIONS_H

# include "libft.h"

/* maximum length of the long options array for a command */
# define MAX_OPT 0x30
/* there will be no short options so just set the leave flag */
# define OPTSTRING "+"

/*
** The result of option parsing will be stored in a static array of t_cmdopt.
** This is a way of sending a formatted structure of options to the commands
** without having to make a special procedure for each one.
**
** ind: keeps the index of the option in argv so the value of optind at the
** moment the option is passed, this will keep track of which option has been
** called last since some overwrite each other (this works because option
** parsing only uses long options so each element of argv corresponds to an
** option or to an argument)
** is_set: boolean indicating if the option has been passed
** value: keeps optarg value if applicable
*/

typedef struct	s_cmdopt
{
	int			ind;
	int			is_set;
	char		*value;
}				t_cmdopt;

/*
** Option codes for commands
**
** They will be used to identify option in the parsed array options
** passed to command functions (that's why they all must start with
** 0 as they are the index in this array).
**
** HELP is always the first one so that it can be identified
** before executing the command
**
*/

enum e_mdc_option_codes {
	MDC_HELP = 0,
	MDC_PRINT,
	MDC_QUIET,
	MDC_REVERSE,
	MDC_STRING
};

enum e_cc_option_codes {
	CC_HELP = 0,
	CC_BASE64,
	CC_KEY,
	CC_PASSWORD,
	CC_SALT,
	CC_INIT_VECTOR,
	CC_DECRYPT,
	CC_ENCRYPT,
	CC_INPUT,
	CC_OUTPUT,
	CC_PRINT
};

enum e_base64_option_codes {
	BASE64_HELP = 0,
	BASE64_DECRYPT,
	BASE64_ENCRYPT,
	BASE64_INPUT,
	BASE64_OUTPUT
};

enum e_genrsa_option_codes {
	GENRSA_HELP = 0,
	GENRSA_INPUT,
	GENRSA_OUTPUT
};

enum e_rsa_option_codes {
	RSA_HELP = 0,
	RSA_INFORM,
	RSA_OUTFORM,
	RSA_IN,
	RSA_PASSIN,
	RSA_OUT,
	RSA_PASSOUT,
	RSA_DES,
	RSA_TEXT,
	RSA_NOOUT,
	RSA_MODULUS,
	RSA_CHECK,
	RSA_PUBIN,
	RSA_PUBOUT
};

enum e_rsautl_option_codes {
	RSAUTL_HELP = 0,
	RSAUTL_IN,
	RSAUTL_OUT,
	RSAUTL_INKEY,
	RSAUTL_PUBIN,
	RSAUTL_ENCRYPT,
	RSAUTL_DECRYPT,
	RSAUTL_HEXDUMP
};

/*
** Options constants in options.c
*/

extern const t_opt g_mdc_options[];
extern const t_opt g_cc_options[];
extern const t_opt g_base64_options[];
extern const t_opt g_genrsa_options[];
extern const t_opt g_rsa_options[];
extern const t_opt g_rsautl_options[];

#endif
