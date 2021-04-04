/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_rsautl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 15:11:34 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/04 14:20:28 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "rsa.h"
#include "readfile.h"

static int	parse_options(const t_command *cmd, t_cmdopt *opt,
		t_rsa_key *key, int *outfd)
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
	if (opt[RSAUTL_OUT].is_set)
		*outfd = output_option(opt[RSAUTL_OUT].value, cmd->name);
	return (*outfd < 0);
}

static int	get_input(uint64_t *input, t_cmdopt *opt,
		t_rsa_key *key, const char *cmd)
{
	int			rd;
	int			skip;
	char		rsabuf[RSABUF_SIZE];

	*input = 0;
	if ((rd = readfile(opt[RSAUTL_IN].value, (char *)rsabuf, RSABUF_SIZE)) < 0)
	{
		print_readfile_error(cmd, opt[RSAUTL_IN].value
			? opt[RSAUTL_IN].value : "stdin");
		return (1);
	}
	readfile(NULL, NULL, 0);
	skip = rd > (int)sizeof(uint64_t)
		? sizeof(uint64_t) : sizeof(uint64_t) - rd;
	ft_memcpy((void *)input + skip, (void *)rsabuf, sizeof(uint64_t) - skip);
	ft_memswap((void *)input, sizeof(uint64_t));
	if (rd > (int)sizeof(uint64_t) || *input >= key->n)
		return (!!ft_dprintf(2,
			"ft_ssl: %s: data greater than mod len\n", cmd));
	if (opt[RSAUTL_ENCRYPT].is_set && rd < (int)sizeof(uint64_t))
		return (!!ft_dprintf(2,
			"ft_ssl: %s: data too small for key size\n", cmd));
	return (0);
}

int			cmd_rsautl(const t_command *cmd, t_cmdopt *opt, char **args)
{
	uint64_t	input;
	int			outfd;
	t_rsa_key	key;

	(void)args;
	outfd = 1;
	if (parse_options(cmd, opt, &key, &outfd))
		return (1);
	if (get_input(&input, opt, &key, cmd->name))
		return (1);
	return (0);
}
