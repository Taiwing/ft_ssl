/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_rsa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 06:54:37 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/14 08:07:03 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "rsa.h"

static int	parse_options(t_rsa_key *key, int *outfd,
		t_cmdopt *opt, const t_command *cmd)
{
	t_des_getkey	gk = { opt[RSA_IN].value, opt[RSA_PASSIN].value,
		NULL, "Enter pass phrase:", 0 };

	key->is_pub = opt[RSA_PUBIN].is_set;
	if (key->is_pub && opt[RSA_CHECK].is_set)
		return (!!ft_dprintf(2,
			"ft_ssl: %s: Only private keys can be checked\n", cmd->name));
	if (opt[RSA_INFORM].is_set
			&& ft_ignore_case_strcmp(opt[RSA_INFORM].value, "DER"))
		return (!!ft_dprintf(2, "ft_ssl: %s: Invalid format \"%s\" "
			"for -inform\n", cmd->name, opt[RSA_INFORM].value));
	if (opt[RSA_OUTFORM].is_set
			&& ft_ignore_case_strcmp(opt[RSA_OUTFORM].value, "DER"))
		return (!!ft_dprintf(2, "ft_ssl: %s: Invalid format \"%s\" "
			"for -outform\n", cmd->name, opt[RSA_OUTFORM].value));
	if (parse_rsa_key(key, cmd->name, &gk))
		return (!!ft_dprintf(2, "ft_ssl: %s: unable to load %s Key\n",
			cmd->name, key->is_pub ? "Public" : "Private"));
	if (opt[RSA_OUT].is_set)
		*outfd = output_option(opt[RSA_OUT].value, cmd->name);
	return (*outfd < 0);
}

int		cmd_rsa(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int			ret;
	int			outfd;
	t_rsa_key	key_in;
	t_rsa_key	key_out;

	(void)args;
	(void)key_out;
	outfd = 1;
	ft_bzero((void *)&key_in, sizeof(t_rsa_key));
	ret = parse_options(&key_in, &outfd, opt, cmd);
	if (outfd > 1)
		close(outfd);
	return (ret);
}
