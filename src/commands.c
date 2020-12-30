/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 12:27:58 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 13:39:06 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "options.h"
#include "help.h"

const t_command	g_command[] = {
	{ "md5", NULL, g_mdc_options, g_mdc_help, MDC },
	{ "sha256", NULL, g_mdc_options, g_mdc_help, MDC },
	{ "base64", NULL, g_base64_options, g_base64_help, CIC },
	{ "des", NULL, g_cc_options, g_cc_help, CIC },
	{ "des-ecb", NULL, g_cc_options, g_cc_help, CIC },
	{ "des-cbc", NULL, g_cc_options, g_cc_help, CIC },
	{ "genrsa", NULL, g_genrsa_options, g_genrsa_help, STC },
	{ "rsa", NULL, g_rsa_options, g_rsa_help, STC },
	{ "rsautl", NULL, g_rsautl_options, g_rsautl_help, STC },
	{ NULL, NULL, NULL, NULL, NONE }
};
