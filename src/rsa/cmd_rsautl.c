/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_rsautl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 15:11:34 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/12 17:58:55 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "rsa.h"

static int	parse_options(const t_command *cmd, t_cmdopt *opt, t_rsa_key *key)
{
	opt[RSAUTL_ENCRYPT].is_set = !opt[RSAUTL_DECRYPT].is_set
		|| opt[RSAUTL_DECRYPT].ind < opt[RSAUTL_ENCRYPT].ind;
	if (!opt[RSAUTL_ENCRYPT].is_set && opt[RSAUTL_PUBIN].is_set)
		return (!!ft_dprintf(2, "ft_ssl: %s: A private key is needed "
			"for this operation\n", cmd->name));
	if (!opt[RSAUTL_INKEY].is_set || parse_rsa_key(key,
		opt[RSAUTL_INKEY].value, opt[RSAUTL_PUBIN].is_set, cmd->name))
		return (!!ft_dprintf(2, "ft_ssl: %s: unable to load %s Key\n",
			cmd->name, opt[RSAUTL_PUBIN].is_set ? "Public" : "Private"));
	return (0);
}

int			cmd_rsautl(const t_command *cmd, t_cmdopt *opt, char **args)
{
	//int			outfd;
	int			ret;
	t_rsa_key	key;

	(void)args;
	ret = 0;
	//outfd = 1;
	if (parse_options(cmd, opt, &key))
		return (1);
	return (ret);
}
