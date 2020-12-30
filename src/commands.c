/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:27:58 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 20:01:28 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "help.h"

/* type of commands for cmd_help */
const char		*g_type_strings[] = {
	"Message Digest commands",
	"Cipher commands",
	"Standard commands",
	NULL
};

/* every command ordered by type */
const t_command	g_commands[] = {
	/* Message Digest Commands */
	{ "md5", NULL, g_mdc_options, g_mdc_help, MDC },
	{ "sha256", NULL, g_mdc_options, g_mdc_help, MDC },

	/* Cipher Commands */
	{ "base64", NULL, g_base64_options, g_base64_help, CIC },
	{ "des", NULL, g_cc_options, g_cc_help, CIC },
	{ "des-ecb", NULL, g_cc_options, g_cc_help, CIC },
	{ "des-cbc", NULL, g_cc_options, g_cc_help, CIC },

	/* Standard Commands */
	{ "genrsa", NULL, g_genrsa_options, g_genrsa_help, STC },
	{ "rsa", NULL, g_rsa_options, g_rsa_help, STC },
	{ "rsautl", NULL, g_rsautl_options, g_rsautl_help, STC },

	/* Internal Commands */
	{ "help", cmd_help, NULL, NULL, INC },
	{ "exit", NULL, NULL, NULL, INC },
	{ "quit", NULL, NULL, NULL, INC },

	{ NULL, NULL, NULL, NULL, NONE }
};
