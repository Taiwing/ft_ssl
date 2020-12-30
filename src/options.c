/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:28:14 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 12:28:15 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

enum e_mdc_option_codes = {
	MDC_PRINT = 0,
	MDC_QUIET,
	MDC_REVERSE,
	MDC_STRING,
	MDC_HELP
};

const t_opt g_mdc_options[] = {
	{ "p", 0, NULL, MDC_PRINT },
	{ "q", 0, NULL, MDC_QUIET },
	{ "r", 0, NULL, MDC_REVERSE },
	{ "s", 1, NULL, MDC_STRING },
	{ "h", 0, NULL, MDC_HELP },
	{ NULL, 0, NULL, 0 }
}

enum e_cc_option_codes = {
	CC_BASE64 = 0,
	CC_KEY,
	CC_PASSWORD,
	CC_SALT,
	CC_INIT_VECTOR,
	CC_DECRYPT,
	CC_ENCRYPT,
	CC_INPUT,
	CC_OUTPUT,
	CC_HELP
};

const t_opt g_cc_options[] = {
	{ "a", 0, NULL, CC_BASE64 = 0 },
	{ "k", 1, NULL, CC_KEY },
	{ "p", 1, NULL, CC_PASSWORD },
	{ "s", 1, NULL, CC_SALT },
	{ "v", 1, NULL, CC_INIT_VECTOR },
	{ "d", 0, NULL, CC_DECRYPT },
	{ "e", 0, NULL, CC_ENCRYPT },
	{ "i", 1, NULL, CC_INPUT },
	{ "o", 1, NULL, CC_OUTPUT },
	{ "h", 0, NULL, CC_HELP },
	{ NULL, 0, NULL, 0 }
};

/* base64 is part of cc but hasnt the first 5 options */
const t_opt *g_base64_options = &g_cc_options[CC_DECRYPT];

enum e_genrsa_option_codes = {
	GENRSA_INPUT = 0,
	GENRSA_OUTPUT,
	GENRSA_HELP
};

const t_opt g_genrsa_options[] = {
	{ "i", 1, NULL, GENRSA_INPUT },
	{ "o", 1, NULL, GENRSA_OUTPUT },
	{ "h", 0, NULL, GENRSA_HELP },
	{ NULL, 0, NULL, 0 }
};

enum e_rsa_option_codes = {
	RSA_INFORM = 0,
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
	RSA_PUBOUT,
	RSA_HELP
};

const t_opt g_rsa_options[] = {
	{ "inform", 1, NULL, RSA_INFORM },
	{ "outform", 1, NULL, RSA_OUTFORM },
	{ "in", 1, NULL, RSA_IN },
	{ "passin", 1, NULL, RSA_PASSIN },
	{ "out", 1, NULL, RSA_OUT },
	{ "passout", 1, NULL, RSA_PASSOUT },
	{ "des", 0, NULL, RSA_DES },
	{ "text", 0, NULL, RSA_TEXT },
	{ "noout", 0, NULL, RSA_NOOUT },
	{ "modulus", 0, NULL, RSA_MODULUS },
	{ "check", 0, NULL, RSA_CHECK },
	{ "pubin", 0, NULL, RSA_PUBIN },
	{ "pubout", 0, NULL, RSA_PUBOUT },
	{ "help", 0, NULL, RSA_HELP },
	{ NULL, 0, NULL, 0 }
};

enum e_rsautl_option_codes = {
	RSAUTL_IN,
	RSAUTL_OUT,
	RSAUTL_INKEY,
	RSAUTL_PUBIN,
	RSAUTL_ENCRYPT,
	RSAUTL_DECRYPT,
	RSAUTL_HEXDUMP,
	RSAUTL_HELP
};

const t_opt g_rsa_options[] = {
	{ "in", 1, NULL, RSAUTL_IN },
	{ "out", 1, NULL, RSAUTL_OUT },
	{ "inkey", 1, NULL, RSAUTL_INKEY },
	{ "pubin", 0, NULL, RSAUTL_PUBIN },
	{ "encrypt", 0, NULL, RSAUTL_ENCRYPT },
	{ "decrypt", 0, NULL, RSAUTL_DECRYPT },
	{ "hexdump", 0, NULL, RSAUTL_HEXDUMP },
	{ "help", 0, NULL, RSAUTL_HELP },
	{ NULL, 0, NULL, 0 }
};
