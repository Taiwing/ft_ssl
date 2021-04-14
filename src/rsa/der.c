/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   der.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 09:41:32 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/14 09:41:08 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rsa.h"

uint64_t	*der_decode_uint64(uint64_t *dst, uint8_t *der,
	uint8_t *i, uint8_t len)
{
	uint8_t	cur_len;

	*dst = 0;
	if (*i >= len || der[(*i)++] != 0x02 || *i >= len
		|| (cur_len = der[(*i)++]) > RSA_DER_INT_MAXLEN
		|| (*i) + cur_len > len
		|| (cur_len < RSA_DER_INT_MAXLEN && (der[*i] & 0x80)))
		return (NULL);
	if (!der[*i])
	{
		++(*i);
		--cur_len;
	}
	ft_memcpy((void *)dst, der + *i, cur_len);
	ft_memswap((void *)dst, cur_len);
	*i += cur_len;
	return (dst);
}

void		der_encode_uint64(uint8_t *der, uint8_t *len, uint64_t n)
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
	der[0] = 0x02;
	der[1] = cur_len + first_bit;
	if (first_bit)
		der[2] = 0;
	*len += 2 + cur_len + first_bit;
	der += 2 + first_bit;
	for (uint8_t i = sizeof(uint64_t) - cur_len; i < sizeof(uint64_t); ++i)
		*der++ = ptr[i];
}

static int	check_start_sequence(uint8_t *der, uint8_t *i,
	uint8_t len, int is_pub)
{
	uint8_t	seqbuf[DER_SEQ_MAXLEN + 1];

	ft_memcpy((void *)seqbuf, (void *)DER_SEQ, DER_SEQ_MAXLEN + 1);
	seqbuf[DER_LEN_I] = len - (DER_LEN_I + 1);
	seqbuf[DER_LEN_II] = len - (DER_LEN_II + 1);
	seqbuf[DER_LEN_III] = len - (DER_LEN_III + 1);
	if (seqbuf[DER_LEN_I] > len || (is_pub && (seqbuf[DER_LEN_II] > len
		|| seqbuf[DER_LEN_III] > len || len <= DER_SEQ_MAXLEN)))
		return (1);
	*i = is_pub ? DER_SEQ_MAXLEN : DER_SEQ_MINLEN;
	return (ft_strncmp((char *)seqbuf, (char *)der,
		is_pub ? DER_SEQ_MAXLEN : DER_SEQ_MINLEN));
}

int			parse_der_key(t_rsa_key *key, uint8_t *der, uint8_t len)
{
	uint8_t		i;
	int			ret;
	int			is_pub;
	uint64_t	version;

	version = 0;
	is_pub = key->is_pub;
	ret = check_start_sequence(der, &i, len, is_pub);
	ret = !ret && !is_pub ? !der_decode_uint64(&version, der, &i, len) : ret;
	ret = !ret ? !der_decode_uint64(&key->n, der, &i, len) : ret;
	ret = !ret ? !der_decode_uint64(&key->e, der, &i, len) : ret;
	ret = !ret && !is_pub ? !der_decode_uint64(&key->d, der, &i, len) : ret;
	ret = !ret && !is_pub ? !der_decode_uint64(&key->p, der, &i, len) : ret;
	ret = !ret && !is_pub ? !der_decode_uint64(&key->q, der, &i, len) : ret;
	ret = !ret && !is_pub ? !der_decode_uint64(&key->exp1, der, &i, len) : ret;
	ret = !ret && !is_pub ? !der_decode_uint64(&key->exp2, der, &i, len) : ret;
	ret = !ret && !is_pub ? !der_decode_uint64(&key->coeff, der, &i, len) : ret;
	return (!ret && is_pub ? i != len : ret);
}

void		encode_der_key(uint8_t *der, uint8_t *len, t_rsa_key *key)
{
	*len = key->is_pub ? DER_SEQ_MAXLEN : DER_SEQ_MINLEN;
	ft_memcpy((void *)der, (void *)DER_SEQ, *len);
	if (!key->is_pub)
		der_encode_uint64(der + *len, len, 0);
	der_encode_uint64(der + *len, len, key->n);
	der_encode_uint64(der + *len, len, key->e);
	if (!key->is_pub)
	{
		der_encode_uint64(der + *len, len, key->d);
		der_encode_uint64(der + *len, len, key->p);
		der_encode_uint64(der + *len, len, key->q);
		der_encode_uint64(der + *len, len, key->exp1);
		der_encode_uint64(der + *len, len, key->exp2);
		der_encode_uint64(der + *len, len, key->coeff);
	}
	der[DER_LEN_I] = *len - (DER_LEN_I + 1);
	if (key->is_pub)
	{
		der[DER_LEN_II] = *len - (DER_LEN_II + 1);
		der[DER_LEN_III] = *len - (DER_LEN_III + 1);
	}
}
