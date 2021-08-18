/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_der_key.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 19:26:36 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 15:00:09 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rsa.h"

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

int				bintset_bytes(t_bint b, uint8_t *bytes, uint64_t len)
{
	uint64_t	bint_len;

	bint_len = len / sizeof(uint32_t) + !!(len % sizeof(uint32_t));
	if (bint_len >= BINT_SIZE(b))
		return (BINT_FAILURE);
	SET_BINT_LEN(b, bint_len);
	ft_bzero((void *)(b + 1), bint_len * sizeof(uint32_t));
	for (uint8_t *d = (uint8_t *)(b + 1); len; ++d)
		*d = bytes[--len];
	return (BINT_SUCCESS);
}

static t_bint	der_decode_bint(t_bint dst, uint8_t *der,
		uint64_t *i, uint64_t len)
{
	uint64_t	cur_len;

	if (bintcpy(dst, BINT_ZERO) == BINT_FAILURE)
		return (NULL);
	if (*i > len || der[(*i)++] != 0x02
		|| der_decode_length(&cur_len, der, i, len) || *i + cur_len > len)
		return (NULL);
	if (!der[*i])
	{
		++(*i);
		--cur_len;
	}
	else if (der[*i] & 0x80)
		return (NULL);
	der += *i;
	*i += cur_len;
	if (bintset_bytes(dst, der, cur_len) == BINT_FAILURE)
		return (NULL);
	return (dst);
}

static int	check_start_sequence(uint8_t *der, uint64_t *i,
	uint64_t len, int is_pub)
{
	uint64_t	total_key_length = 0;
	uint64_t	bit_string_length = 0;
	uint64_t	key_seq_length = 0;

	if (!len || der[(*i)++] != 0x30
		|| der_decode_length(&total_key_length, der, i, len)
		|| total_key_length != len - *i)
		return (1);
	if (!is_pub)
		return (0);
	if (*i >= len || len - *i <= DER_OID_SEQ_LEN
		|| ft_memcmp((void *)(der + *i), (void *)DER_OID_SEQ, DER_OID_SEQ_LEN))
		return (1);
	*i += DER_OID_SEQ_LEN;
	if (der[(*i)++] != 0x03 || *i >= len
		|| der_decode_length(&bit_string_length, der, i, len)
		|| bit_string_length != len - *i || *i >= len
		|| der[(*i)++] != 0x00 || *i >= len || der[(*i)++] != 0x30
		|| der_decode_length(&key_seq_length, der, i, len)
		|| key_seq_length != len - *i)
		return (1);
	return (0);
}

int			decode_der_key(t_rsa_key *key, uint8_t *der, uint64_t len)
{
	uint64_t	i = 0;
	uint32_t	version[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint64_t	stop = key->is_pub ? RSA_PUB_BINTS : RSA_KEY_BINTS;

	if (check_start_sequence(der, &i, len, key->is_pub))
		return (1);
	if (!key->is_pub && (!der_decode_bint(version, der, &i, len)
		|| bintcmp(version, BINT_ZERO)))
		return (1);
	for (uint64_t j = 0; j < stop; ++j)
		if (!der_decode_bint(key->rsa_bints[j], der, &i, len))
			return (1);
	return (i != len);
}
