/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   der.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 09:41:32 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/13 10:04:36 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rsa.h"

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

void		encode_der_key(uint8_t *derkey, uint8_t *len, t_rsa_key *key)
{
	*len = 2;
	derkey[0] = 0x30;
	der_encode_uint64(derkey + *len, len, 0);
	der_encode_uint64(derkey + *len, len, key->n);
	der_encode_uint64(derkey + *len, len, key->e);
	der_encode_uint64(derkey + *len, len, key->d);
	der_encode_uint64(derkey + *len, len, key->p);
	der_encode_uint64(derkey + *len, len, key->q);
	der_encode_uint64(derkey + *len, len, key->exp1);
	der_encode_uint64(derkey + *len, len, key->exp2);
	der_encode_uint64(derkey + *len, len, key->coeff);
	derkey[1] = *len - 2;
}
