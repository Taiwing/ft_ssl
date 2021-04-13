/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_genrsa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 23:59:02 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/13 10:29:28 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include "commands.h"
#include "rsa_math.h"
#include "rsa.h"
#include "base64.h"

int			print_rsa_key(int fd, t_rsa_key *key)
{
	int		ret;
	uint8_t	len;
	uint8_t	der[KEY_BUFLEN];

	encode_der_key(der, &len, key);
	if ((ret = ft_dprintf(fd, BEGIN_PRIV"\n")) > 0
		&& (ret = base64_writefile(fd, (char *)der, (size_t)len, 1)) >= 0)
		ret = ft_dprintf(fd, END_PRIV"\n");
	return (ret <= 0);
}

#define MAX_TRY	5

int			rsa_keygen(t_rsa_key *key)
{
	uint64_t	gcd;
	uint64_t	totient;;

	ft_dprintf(2, "Generating RSA private key, "
		"64 bit long modulus (2 primes)\n");
	ft_bzero((void *)key, sizeof(t_rsa_key));
	key->e = E_VALUE;
	for (int i = 0; i < MAX_TRY && (key->p == key->q || !(key->n >> 63)) ; ++i)
	{
		if (!(key->p = find_prime(12, 32)) || !(key->q = find_prime(12, 32)))
			return (!!ft_printf("\n"));
		key->n = key->p * key->q;
	}
	if (key->p == key->q || !(key->n >> 63))
		return (!!ft_printf("\n"));
	ft_dprintf(2, "e is %1$lu (0x%1$06lx)\n", key->e);
	totient = (key->p - 1) * (key->q - 1);
	key->d = modinv((int128_t)key->e, (int128_t)totient, &gcd);
	if (!key->d || gcd != 1)
		return (1);
	key->exp1 = key->d % (key->p - 1);
	key->exp2 = key->d % (key->q - 1);
	key->coeff = modinv((int128_t)key->q, (int128_t)key->p, &gcd);
	return (!key->coeff || gcd != 1);
}

int			cmd_genrsa(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int			outfd;
	int			ret;
	t_rsa_key	key;

	(void)args;
	ret = 0;
	outfd = 1;
	if (opt[GENRSA_INPUT].is_set && !fill_rand_buf(opt[GENRSA_INPUT].value, 0))
		return (!!ft_dprintf(2, "ft_ssl: %s: %s: %s\n",
			cmd->name, opt[GENRSA_INPUT].value, strerror(errno)));
	if (opt[GENRSA_OUTPUT].is_set
		&& (outfd = output_option(opt[GENRSA_OUTPUT].value, cmd->name)) < 0)
		ret = 1;
	if (!ret && (ret = rsa_keygen(&key)))
		ft_dprintf(2, "ft_ssl: %s: failed to generate key\n", cmd->name);
	if (!ret && (ret = print_rsa_key(outfd, &key)))
		ft_dprintf(2, "ft_ssl: %s: failed to print key\n", cmd->name);
	if (outfd > 1)
		close(outfd);
	fill_rand_buf(NULL, 1);
	return (ret);
}
