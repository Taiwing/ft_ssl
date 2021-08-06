/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_genrsa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 23:59:02 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/06 16:21:18 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include "commands.h"
#include "rsa_math.h"
#include "rsa.h"
#include "base64.h"


/*
** TODO: make this function static when we will have delete all
** external calls to it
*/
int			rsa_key_64_to_bint(t_rsa_key *dst, t_rsa_key_64 *src)
{
	if (bintset_u64(dst->n, src->n) == BINT_FAILURE
		|| bintset_u64(dst->e, src->e) == BINT_FAILURE
		|| bintset_u64(dst->d, src->d) == BINT_FAILURE
		|| bintset_u64(dst->p, src->p) == BINT_FAILURE
		|| bintset_u64(dst->q, src->q) == BINT_FAILURE
		|| bintset_u64(dst->exp1, src->exp1) == BINT_FAILURE
		|| bintset_u64(dst->exp2, src->exp2) == BINT_FAILURE
		|| bintset_u64(dst->coeff, src->coeff) == BINT_FAILURE)
		return (1);
	dst->is_pub = src->is_pub;
	dst->is_enc = src->is_enc;
	ft_memcpy((void *)&dst->des, (void *)&src->des, sizeof(t_des_ctx));
	return (0);
}

int			print_rsa_key(int fd, t_rsa_key *key,
	const char *cmd, t_des_getkey *gk)
{
	int			ret;
	uint64_t	len;
	uint8_t		der[KEY_BUFLEN];

	encode_der_key(der, &len, key);
	if (!key->is_pub && key->is_enc)
	{
		if (!get_rand(&key->des.salt, 0, UINT64_MAX)
			|| rsa_des_getkey(&key->des, cmd, gk))
			return (1);
		key->des.process_block = des_cbc;
		key->des.iv = key->des.salt;
		rsa_des_encrypt(der, &len, &key->des);
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

int			rsa_keygen(t_rsa_key_64 *key)
{
	uint64_t	gcd;
	uint64_t	totient;

	ft_dprintf(2, "Generating RSA private key, "
		"64 bit long modulus (2 primes)\n");
	ft_bzero((void *)key, sizeof(t_rsa_key_64));
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
	t_rsa_key		key = RSA_KEY_DEFAULT;
	int				outfd = 1;
	int				ret = 0;
	t_rsa_key_64	key64;
	t_des_getkey	gk;

	(void)args;
	INIT_RSA_KEY(key);
	genrsa_init_getkey(&gk, opt);
	if (opt[GENRSA_INPUT].is_set && !fill_rand_buf(opt[GENRSA_INPUT].value, 0))
		return (!!ft_dprintf(2, "ft_ssl: %s: %s: %s\n",
			cmd->name, opt[GENRSA_INPUT].value, strerror(errno)));
	if (opt[GENRSA_OUTPUT].is_set
		&& (outfd = output_option(opt[GENRSA_OUTPUT].value, cmd->name)) < 0)
		ret = 1;
	if (!ret && ((ret = rsa_keygen(&key64))
		|| rsa_key_64_to_bint(&key, &key64)))
		ft_dprintf(2, "ft_ssl: %s: failed to generate key\n", cmd->name);
	key64.is_enc = opt[GENRSA_DES].is_set;
	if (!ret && (ret = print_rsa_key(outfd, &key, cmd->name, &gk)))
		ft_dprintf(2, "ft_ssl: %s: failed to print key\n", cmd->name);
	if (outfd > 1)
		close(outfd);
	fill_rand_buf(NULL, 1);
	return (ret);
}
