/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   der.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 09:41:32 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/13 20:12:53 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rsa.h"

uint64_t	*der_decode_uint64(uint64_t *dst, uint8_t *der,
	uint8_t *i, uint64_t len)
{
	uint64_t	cur_len;

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

static int	der_decode_length(uint64_t *dstlen, uint8_t *der,
	uint64_t *i, uint64_t len)
{
	uint8_t		*ptr;
	uint64_t	lenlen;

	if (*i >= len)
		return (1);
	if (der[*i] & 0x80)
	{
		lenlen = der[(*i)++] & 0x7f;
		if (lenlen > sizeof(uint64_t) || *i + lenlen > len)
			return (1);
		*dstlen = 0;
		ptr = (uint8_t *)dstlen;
		while (lenlen && *i < len)
			ptr[--lenlen] = der[(*i)++];
		return (!!lenlen);
	}
	*dstlen = der[(*i)++];
	return (0);
}

static int	check_start_sequence(uint8_t *der, uint64_t *i,
	uint64_t len, int is_pub)
{
	if (!len || der[(*i)++] != 0x30)
		return (1);
}

int			parse_der_key(t_rsa_key *key, uint8_t *der, uint64_t len)
{
	uint64_t	i;
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
