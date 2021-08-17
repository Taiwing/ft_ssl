/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_rsautl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 15:11:34 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/17 19:52:11 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "rsa.h"
#include "readfile.h"
#include "rsa_math.h"

/*
static int	parse_options(const t_command *cmd, t_cmdopt *opt,
		t_rsa_key_64 *key, int *outfd)
{
	t_des_getkey	gk = { opt[RSAUTL_INKEY].value, opt[RSAUTL_PASSIN].value,
		opt[RSAUTL_INKEY].value, "Enter pass phrase for %s:", 0 };

	opt[RSAUTL_ENCRYPT].is_set = !opt[RSAUTL_DECRYPT].is_set
		|| opt[RSAUTL_DECRYPT].ind < opt[RSAUTL_ENCRYPT].ind;
	if (!opt[RSAUTL_ENCRYPT].is_set && opt[RSAUTL_PUBIN].is_set)
		return (!!ft_dprintf(2, "ft_ssl: %s: A private key is needed "
			"for this operation\n", cmd->name));
	key->is_pub = opt[RSAUTL_PUBIN].is_set;
	if (!opt[RSAUTL_INKEY].is_set)
		return (!!ft_dprintf(2, "ft_ssl: %s: no keyfile specified\n",
			cmd->name));
	if (parse_rsa_key_64(key, cmd->name, &gk))
		return (!!ft_dprintf(2, "ft_ssl: %s: unable to load %s Key\n",
			cmd->name, key->is_pub ? "Public" : "Private"));
	if (opt[RSAUTL_OUT].is_set)
		*outfd = output_option(opt[RSAUTL_OUT].value, cmd->name);
	return (*outfd < 0);
}

static int	get_input(uint64_t *input, t_cmdopt *opt,
		t_rsa_key_64 *key, const t_command *cmd)
{
	int			rd;
	int			skip;
	char		rsabuf[RSABUF_SIZE];

	*input = 0;
	if ((rd = readfile(opt[RSAUTL_IN].value, (char *)rsabuf, RSABUF_SIZE)) < 0)
	{
		print_readfile_error(cmd->name, opt[RSAUTL_IN].value
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
			"ft_ssl: %s: data greater than mod len\n", cmd->name));
	if (opt[RSAUTL_ENCRYPT].is_set && rd < (int)sizeof(uint64_t))
		return (!!ft_dprintf(2,
			"ft_ssl: %s: data too small for key size\n", cmd->name));
	return (0);
}

int			cmd_rsautl(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int				fd;
	int				ret;
	t_rsa_key_64	key;
	uint64_t		out;
	uint64_t		in;

	(void)args;
	fd = 1;
	ft_bzero((void *)&key, sizeof(t_rsa_key_64));
	ret = parse_options(cmd, opt, &key, &fd) || get_input(&in, opt, &key, cmd);
	if (!ret)
	{
		if (opt[RSAUTL_ENCRYPT].is_set)
			out = modexp((uint128_t)in, (uint128_t)key.e, (uint128_t)key.n);
		else
			out = modexp((uint128_t)in, (uint128_t)key.d, (uint128_t)key.n);
		ft_memswap((void *)&out, sizeof(uint64_t));
		if (opt[RSAUTL_HEXDUMP].is_set)
			rsa_hexdump(fd, (uint8_t *)&out, sizeof(uint64_t));
		else
			write(fd, (void *)&out, sizeof(uint64_t));
	}
	if (fd > 1)
		close(fd);
	return (ret);
}
*/

/*
** TODO: un-comment and adapt the real cmd_rsautl function to
** the new key format and remove this
*/
int			cmd_rsautl(const t_command *cmd, t_cmdopt *opt, char **args)
{
	(void)cmd;
	(void)opt;
	(void)args;

	return (0);
}
