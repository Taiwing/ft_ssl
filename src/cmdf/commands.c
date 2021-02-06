/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:27:58 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/07 00:12:14 by yforeau          ###   ########.fr       */
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
	{ "md5", cmd_md, g_mdc_options, g_mdc_help, MDC },
	{ "sha256", cmd_md, g_mdc_options, g_mdc_help, MDC },

	/* Cipher Commands */
	{ "base64", cmd_base64, g_base64_options, g_base64_help, CIC },
	{ "des", cmd_des, g_cc_options, g_cc_help, CIC },
	{ "des-ecb", cmd_des, g_cc_options, g_cc_help, CIC },
	{ "des-cbc", cmd_des, g_cc_options, g_cc_help, CIC },

	/* Standard Commands */
	{ "genrsa", cmd_genrsa, g_genrsa_options, g_genrsa_help, STC },
	{ "rsa", NULL, g_rsa_options, g_rsa_help, STC },
	{ "rsautl", NULL, g_rsautl_options, g_rsautl_help, STC },

	/* Internal Commands */
	{ "help", cmd_help, NULL, NULL, INC },
	{ "exit", cmd_exit, NULL, NULL, INC },
	{ "quit", cmd_exit, NULL, NULL, INC },

	{ NULL, NULL, NULL, NULL, NONE }
};
