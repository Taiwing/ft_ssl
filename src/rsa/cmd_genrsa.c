/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_genrsa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 23:59:02 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/10 16:30:37 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include "commands.h"
#include "rsa_math.h"
#include "rsa.h"

int	print_rsa_key(int fd, t_rsa_key *key)
{
	(void)fd;
	(void)key;
	ft_dprintf(2, "lol dis all good\n");
	return (0);
}

int	rsa_keygen(t_rsa_key *key)
{
	uint64_t	gcd;
	uint64_t	totient;;

	ft_bzero((void *)key, sizeof(t_rsa_key));
	key->e = E_VALUE;
	if (!(key->p = find_prime(12, 32)) || !(key->q = find_prime(12, 32)))
		return (!!ft_printf("\n"));
	if (key->p == key->q || key->p <= E_VALUE || key->q <= E_VALUE)
		return (1);
	key->n = key->p * key->q;
	totient = (key->p - 1) * (key->q - 1);
	key->d = modinv((int128_t)key->e, (int128_t)totient, &gcd);
	if (!key->d || gcd != 1)
		return (1);
	key->exp1 = key->d % (key->p - 1);
	key->exp2 = key->d % (key->q - 1);
	key->coeff = modinv((int128_t)key->q, (int128_t)key->p, &gcd);
	if (!key->coeff || gcd != 1)
		return (1);
	return (0);
}

int	cmd_genrsa(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int			outfd;
	int			ret;
	t_rsa_key	key;

	(void)args;
	ret = 0;
	outfd = 1;
	if (opt[GENRSA_INPUT].is_set && !fill_rand_buf(opt[GENRSA_INPUT].value, 0))
		return (!!ft_dprintf(2, "ft_ssl: %s: %s\n",
			cmd->name, strerror(errno)));
	if (opt[GENRSA_OUTPUT].is_set
		&& (outfd = output_option(opt[GENRSA_OUTPUT].value, cmd->name)) < 0)
		ret = 1;
	if (!ret && (ret = rsa_keygen(&key)))
		ft_dprintf(2, "ft_ssl: %s: failed to generate key\n", cmd->name);
	else if ((ret = print_rsa_key(outfd, &key)))
		ft_dprintf(2, "ft_ssl: %s: failed to print key\n", cmd->name);
	if (outfd > 1)
		close(outfd);
	fill_rand_buf(NULL, 1);
	return (ret);
}
