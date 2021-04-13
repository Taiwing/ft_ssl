/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_genrsa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 23:59:02 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/13 18:40:48 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include "commands.h"
#include "rsa_math.h"
#include "rsa.h"
#include "base64.h"

int			print_rsa_key(int fd, t_rsa_key *key,
	const char *cmd, t_des_getkey *gk)
{
	int		ret;
	uint8_t	len;
	uint8_t	der[KEY_BUFLEN];

	encode_der_key(der, &len, key);
	if (!key->is_pub && key->is_enc)
	{
		if (!get_rand(&key->des.salt, 0, UINT64_MAX)
			|| rsa_des_getkey(key, cmd, gk))
			return (1);
		ft_memswap((void *)&key->des.salt, sizeof(uint64_t));
		key->des.process_block = des_cbc;
		key->des.iv = key->des.salt;
		rsa_des_encrypt(der, &len, key);
	}
	ret = ft_dprintf(fd, "%s\n", key->is_pub ? BEGIN_PUB : BEGIN_PRIV) <= 0;
	if (!ret && !key->is_pub && key->is_enc)
		ret = ft_dprintf(fd, "%s\n%s%016llX\n\n",
			PROC_TYPE, DEK_INFO, key->des.salt) <= 0;
	if (!ret)
		ret = base64_writefile(fd, (char *)der, (size_t)len, 1) < 0
			|| ft_dprintf(fd, "%s\n", key->is_pub ? END_PUB : END_PRIV) <= 0;
	return (ret);
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

static void	genrsa_init_getkey(t_des_getkey *gk, t_cmdopt *opt)
{
	ft_bzero((void *)gk, sizeof(t_des_getkey));
	gk->pass = opt[GENRSA_PASSOUT].value;
	gk->prompt = "Enter pass phrase:";
	gk->verify = 1;
}

int			cmd_genrsa(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int				outfd;
	int				ret;
	t_rsa_key		key;
	t_des_getkey	gk;

	(void)args;
	ret = 0;
	outfd = 1;
	genrsa_init_getkey(&gk, opt);
	if (opt[GENRSA_INPUT].is_set && !fill_rand_buf(opt[GENRSA_INPUT].value, 0))
		return (!!ft_dprintf(2, "ft_ssl: %s: %s: %s\n",
			cmd->name, opt[GENRSA_INPUT].value, strerror(errno)));
	if (opt[GENRSA_OUTPUT].is_set
		&& (outfd = output_option(opt[GENRSA_OUTPUT].value, cmd->name)) < 0)
		ret = 1;
	if (!ret && (ret = rsa_keygen(&key)))
		ft_dprintf(2, "ft_ssl: %s: failed to generate key\n", cmd->name);
	key.is_enc = opt[GENRSA_DES].is_set;
	if (!ret && (ret = print_rsa_key(outfd, &key, cmd->name, &gk)))
		ft_dprintf(2, "ft_ssl: %s: failed to print key\n", cmd->name);
	if (outfd > 1)
		close(outfd);
	fill_rand_buf(NULL, 1);
	return (ret);
}
