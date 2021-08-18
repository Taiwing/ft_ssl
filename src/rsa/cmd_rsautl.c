/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_rsautl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 15:11:34 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 22:39:24 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "rsa.h"
#include "readfile.h"
#include "rsa_math.h"

static int	parse_options(const t_command *cmd, t_cmdopt *opt,
		t_rsa_key *key, int *outfd)
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
	if (parse_rsa_key(key, cmd->name, &gk))
		return (!!ft_dprintf(2, "ft_ssl: %s: unable to load %s Key\n",
			cmd->name, key->is_pub ? "Public" : "Private"));
	if (opt[RSAUTL_OUT].is_set)
		*outfd = output_option(opt[RSAUTL_OUT].value, cmd->name);
	return (*outfd < 0);
}

static int	get_input(t_bint input, t_cmdopt *opt,
		t_rsa_key *key, const t_command *cmd)
{
	int			rd;
	uint64_t	byte_size;
	char		rsabuf[RSABUF_SIZE];

	if ((rd = readfile(opt[RSAUTL_IN].value, (char *)rsabuf, RSABUF_SIZE)) < 0)
	{
		print_readfile_error(cmd->name, opt[RSAUTL_IN].value
			? opt[RSAUTL_IN].value : "stdin");
		return (1);
	}
	readfile(NULL, NULL, 0);
	if (bintset_bytes(input, (uint8_t *)rsabuf, (uint64_t)rd) == BINT_FAILURE)
		return (1);
	if (bintcmp(input, key->n) >= 0)
		return (!!ft_dprintf(2,
			"ft_ssl: %s: data greater than mod len\n", cmd->name));
	byte_size = (key->size / 8) + !!(key->size % 8);
	if (opt[RSAUTL_ENCRYPT].is_set && rd < (int)byte_size)
		return (!!ft_dprintf(2,
			"ft_ssl: %s: data too small for key size\n", cmd->name));
	return (0);
}

static int	print_rsautl_output(int fd, t_bint out, t_cmdopt *opt)
{
	uint8_t		*ptr;
	uint64_t	byte_len;

	ptr = (uint8_t *)(out + 1);
	byte_len = BINT_LEN(out) * sizeof(uint32_t);
	if (!ft_memswap((void *)ptr, byte_len))
		return (1);
	if (opt[RSAUTL_HEXDUMP].is_set)
		return (rsa_hexdump(fd, ptr, byte_len) < 0);
	return (write(fd, (void *)ptr, byte_len) < 0);
}

int			cmd_rsautl(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int			fd;
	int			ret;
	t_rsa_key	key = RSA_KEY_DEFAULT;
	uint32_t	in[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	out[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	(void)args;
	INIT_RSA_KEY(key);
	fd = 1;
	ret = parse_options(cmd, opt, &key, &fd) || get_input(in, opt, &key, cmd);
	if (!ret)
	{
		if (opt[RSAUTL_ENCRYPT].is_set)
			ret = bint_modexp(out, in, key.e, key.n) == BINT_FAILURE;
		else
			ret = bint_modexp(out, in, key.d, key.n) == BINT_FAILURE;
		if (!ret)
			ret = print_rsautl_output(fd, out, opt);
	}
	if (fd > 1)
		close(fd);
	return (ret);
}
