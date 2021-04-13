/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:28:14 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/13 16:59:16 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"

const t_opt g_mdc_options[] = {
	{ "help", 0, NULL, MDC_HELP },
	{ "p", 0, NULL, MDC_PRINT },
	{ "q", 0, NULL, MDC_QUIET },
	{ "r", 0, NULL, MDC_REVERSE },
	{ "s", 1, NULL, MDC_STRING },
	{ NULL, 0, NULL, 0 }
};

const t_opt g_cc_options[] = {
	{ "help", 0, NULL, CC_HELP },
	{ "a", 0, NULL, CC_BASE64 },
	{ "k", 1, NULL, CC_KEY },
	{ "p", 1, NULL, CC_PASSWORD },
	{ "s", 1, NULL, CC_SALT },
	{ "v", 1, NULL, CC_INIT_VECTOR },
	{ "d", 0, NULL, CC_DECRYPT },
	{ "e", 0, NULL, CC_ENCRYPT },
	{ "i", 1, NULL, CC_INPUT },
	{ "o", 1, NULL, CC_OUTPUT },
	{ "P", 0, NULL, CC_PRINT },
	{ NULL, 0, NULL, 0 }
};

/* base64 is part of cc but does not have the first 5 options after HELP */
const t_opt g_base64_options[] = {
	{ "help", 0, NULL, BASE64_HELP },
	{ "d", 0, NULL, BASE64_DECRYPT },
	{ "e", 0, NULL, BASE64_ENCRYPT },
	{ "i", 1, NULL, BASE64_INPUT },
	{ "o", 1, NULL, BASE64_OUTPUT },
	{ NULL, 0, NULL, 0 }
};

const t_opt g_genrsa_options[] = {
	{ "help", 0, NULL, GENRSA_HELP },
	{ "i", 1, NULL, GENRSA_INPUT },
	{ "o", 1, NULL, GENRSA_OUTPUT },
	{ "des", 0, NULL, GENRSA_DES },
	{ "passout", 1, NULL, GENRSA_PASSOUT },
	{ NULL, 0, NULL, 0 }
};

const t_opt g_rsa_options[] = {
	{ "help", 0, NULL, RSA_HELP },
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
	{ NULL, 0, NULL, 0 }
};

const t_opt g_rsautl_options[] = {
	{ "help", 0, NULL, RSAUTL_HELP },
	{ "in", 1, NULL, RSAUTL_IN },
	{ "out", 1, NULL, RSAUTL_OUT },
	{ "inkey", 1, NULL, RSAUTL_INKEY },
	{ "pubin", 0, NULL, RSAUTL_PUBIN },
	{ "encrypt", 0, NULL, RSAUTL_ENCRYPT },
	{ "decrypt", 0, NULL, RSAUTL_DECRYPT },
	{ "hexdump", 0, NULL, RSAUTL_HEXDUMP },
	{ "raw", 0, NULL, RSAUTL_RAW },
	{ "passin", 1, NULL, RSAUTL_PASSIN },
	{ NULL, 0, NULL, 0 }
};
