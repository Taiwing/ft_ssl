/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_genrsa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 23:59:02 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/20 11:55:23 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include "commands.h"
#include "rsa_math.h"
#include "rsa.h"

#define MAX_TRY 5

//TODO: fix small key generation (create dedicated function for that)
// and maybe precompute some of it
// Also maybe keep one of the generated primes and double the MAX_TRY
// value so that retries are more efficient.
int			rsa_keygen(t_rsa_key_64 *key)
{
	uint64_t	ps, gcd, totient;

	ft_dprintf(2, "Generating RSA private key, %llu bit long modulus "
		"(2 primes)\n", key->size);
	key->e = E_VALUE;
	for (int i = 0; i < MAX_TRY
		&& (key->p == key->q || NBITS(key->n) != key->size) ; ++i)
	{
		ps = (key->size / 2) + !!(key->size % 2);
		if (!(key->p = find_prime(12, ps)) || !(key->q = find_prime(12, ps)))
			return (!!ft_printf("\n"));
		key->n = key->p * key->q;
	}
	if (key->p == key->q || NBITS(key->n) != key->size)
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

static int	init_genrsa(t_des_getkey *gk, t_cmdopt *opt,
		int *outfd, t_rsa_key_64 *key)
{
	ft_bzero((void *)gk, sizeof(t_des_getkey));
	gk->pass = opt[GENRSA_PASSOUT].value;
	gk->prompt = "Enter pass phrase:";
	gk->verify = 1;
	if (opt[GENRSA_INPUT].is_set && !fill_rand_buf(opt[GENRSA_INPUT].value, 0))
		return (!!ft_dprintf(2, "ft_ssl: genrsa: %s: %s\n",
			opt[GENRSA_INPUT].value, strerror(errno)));
	if (opt[GENRSA_OUTPUT].is_set
		&& (*outfd = output_option(opt[GENRSA_OUTPUT].value, "genrsa")) < 0)
		return (1);
	ft_bzero((void *)key, sizeof(t_rsa_key_64));
	return (0);
}

int			cmd_genrsa(const t_command *cmd, t_cmdopt *opt, char **args)
{
	t_rsa_key		key = RSA_KEY_DEFAULT;
	int				outfd = 1;
	int				ret = 0;
	t_rsa_key_64	key64;
	t_des_getkey	gk;

	INIT_RSA_KEY(key);
	ret = init_genrsa(&gk, opt, &outfd, &key64);
	if (!ret && !*args)
		key64.size = GENRSA_KEY_SIZE_MAX;
	else if (!ret && (!(key64.size = ft_atoi(*args)) || key64.size
		> GENRSA_KEY_SIZE_MAX || key64.size < GENRSA_KEY_SIZE_MIN))
		ret = !!ft_dprintf(2, "ft_ssl: '%s' is not a valid key size\n"
			"ft_ssl: rsa key length must be between %u and %u\n",
			*args, GENRSA_KEY_SIZE_MIN, GENRSA_KEY_SIZE_MAX);
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
