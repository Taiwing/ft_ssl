/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_genrsa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 23:59:02 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/09 12:22:29 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include "commands.h"
#include "rsa_math.h"
#include "rsa.h"
#include "base64.h"

uint64_t	*der_decode(uint8_t *derkey, uint8_t *i, uint8_t len, uint64_t *dst)
{
	uint8_t	cur_len;

	*dst = 0;
	if (*i >= len || derkey[(*i)++] != 0x02 || *i >= len
		|| (cur_len = derkey[(*i)++]) > RSA_DER_INT_MAXLEN
		|| (*i) + cur_len > len
		|| (cur_len < RSA_DER_INT_MAXLEN && (derkey[*i] & 0x80)))
		return (NULL);
	if (!derkey[*i])
	{
		++(*i);
		--cur_len;
	}
	ft_memcpy((void *)dst, derkey + *i, cur_len);
	ft_memswap((void *)dst, cur_len);
	*i += cur_len;
	return (dst);
}

int			parse_der_key(t_rsa_key *key, uint8_t *derkey, uint8_t len)
{
	uint8_t		i;
	int			ret;
	int			is_pub;
	uint64_t	version;

	i = 0;
	ret = 0;
	version = 0;
	is_pub = key->is_pub;
	if (!derkey || !len || derkey[i++] != 0x30
		|| i >= len || derkey[i++] != len - 2)
		ret = 1;
	ret = !ret && !is_pub ? !der_decode(derkey, &i, len, &version) : ret;
	ret = !ret ? !der_decode(derkey, &i, len, &key->n) : ret;
	ret = !ret ? !der_decode(derkey, &i, len, &key->e) : ret;
	ret = !ret && !is_pub ? !der_decode(derkey, &i, len, &key->d) : ret;
	ret = !ret && !is_pub ? !der_decode(derkey, &i, len, &key->p) : ret;
	ret = !ret && !is_pub ? !der_decode(derkey, &i, len, &key->q) : ret;
	ret = !ret && !is_pub ? !der_decode(derkey, &i, len, &key->exp1) : ret;
	ret = !ret && !is_pub ? !der_decode(derkey, &i, len, &key->exp2) : ret;
	ret = !ret && !is_pub ? !der_decode(derkey, &i, len, &key->coeff) : ret;
	return (!ret && is_pub ? i != len : ret);
}

void		der_encode_uint64(uint8_t *derkey, uint8_t *len, uint64_t n)
{
	uint8_t	*ptr;
	uint8_t	cur_len;
	int		first_bit;

	ft_memswap((void *)&n, sizeof(uint64_t));
	ptr = (uint8_t *)&n;
	cur_len = sizeof(uint64_t);
	while (cur_len && !ptr[sizeof(uint64_t) - cur_len])
		--cur_len;
	cur_len = !cur_len ? 1 : cur_len;
	first_bit = !!(ptr[sizeof(uint64_t) - cur_len] & 0x80);
	derkey[0] = 0x02;
	derkey[1] = cur_len + first_bit;
	if (first_bit)
		derkey[2] = 0;
	*len += 2 + cur_len + first_bit;
	derkey += 2 + first_bit;
	for (uint8_t i = sizeof(uint64_t) - cur_len; i < sizeof(uint64_t); ++i)
		*derkey++ = ptr[i];
}

int			print_rsa_key(int fd, t_rsa_key *key)
{
	int		ret;
	uint8_t	len;
	uint8_t	derkey[PRIVKEY_MAXLEN];

	len = 2;
	derkey[0] = 0x30;
	der_encode_uint64(derkey + len, &len, 0);
	der_encode_uint64(derkey + len, &len, key->n);
	der_encode_uint64(derkey + len, &len, key->e);
	der_encode_uint64(derkey + len, &len, key->d);
	der_encode_uint64(derkey + len, &len, key->p);
	der_encode_uint64(derkey + len, &len, key->q);
	der_encode_uint64(derkey + len, &len, key->exp1);
	der_encode_uint64(derkey + len, &len, key->exp2);
	der_encode_uint64(derkey + len, &len, key->coeff);
	derkey[1] = len - 2;
	if ((ret = ft_dprintf(fd, BEGIN_PRIV"\n")) > 0
		&& (ret = base64_writefile(fd, (char *)derkey, (size_t)len, 1)) >= 0)
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
	else if ((ret = print_rsa_key(outfd, &key)))
		ft_dprintf(2, "ft_ssl: %s: failed to print key\n", cmd->name);
	if (outfd > 1)
		close(outfd);
	fill_rand_buf(NULL, 1);
	return (ret);
}