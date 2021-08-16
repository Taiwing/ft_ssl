/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encode_der_key.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 17:58:00 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/16 19:44:44 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rsa.h"

static uint64_t	der_key_lengths(uint64_t lengths[RSA_KEY_BINTS], t_rsa_key *key)
{
	uint64_t	key_seq_length;

	key_seq_length = key->is_pub ? 0 : 3;
	for (int i = 0; i < (key->is_pub ? RSA_PUB_BINTS : RSA_KEY_BINTS); ++i)
	{
		lengths[i] = (bintlog2(key->rsa_bints[i]) / 8) + 1;
		key_seq_length += 1 + der_lenlen(lengths[i]);
		key_seq_length += lengths[i];
	}
	return (key_seq_length);
}

static void		der_encode_length(uint8_t *der, uint64_t *len, uint64_t length)
{
	uint8_t		*ptr;
	uint64_t	lenlen;

	lenlen = NBITS(length);
	ptr = (uint8_t *)&length;
	if (lenlen < 8)
		der[(*len)++] = ptr[0];
	else
	{
		lenlen = (lenlen / 8) + !!(lenlen % 8);
		der[(*len)++] = 128 + (uint8_t)lenlen;
		while (lenlen)
			der[(*len)++] = ptr[--lenlen];
	}
}

static void		set_pubkey_header(uint8_t *der, uint64_t *len,
		uint64_t key_seq_length)
{
	uint64_t	bit_string_length;
	uint64_t	total_pubkey_length;

	bit_string_length = 2 + der_lenlen(key_seq_length);
	bit_string_length += key_seq_length;
	total_pubkey_length = 1 + der_lenlen(bit_string_length);
	total_pubkey_length += bit_string_length + DER_OID_SEQ_LEN;
	der_encode_length(der, len, total_pubkey_length);
	ft_memcpy((void *)(der + *len), (void *)DER_OID_SEQ, DER_OID_SEQ_LEN);
	*len += DER_OID_SEQ_LEN;
	der[(*len)++] = 0x03;
	der_encode_length(der, len, bit_string_length);
	der[(*len)++] = 0x00;
	der[(*len)++] = 0x30;
}

static void		der_encode_bint(uint8_t *der, uint64_t *len,
		t_bint b, uint64_t length)
{
	uint8_t	*ptr;

	der[(*len)++] = 0x02;
	der_encode_length(der, len, length);
	der[*len] = 0;
	*len += length;
	for (uint64_t i = 1; i <= BINT_LEN(b) && length; ++i)
	{
		ptr = (uint8_t *)(b + i);
		for (uint64_t j = 0; j < sizeof(uint32_t) && length; ++j, --length)
			der[(*len) - ((i - 1) * sizeof(uint32_t)) - (j + 1)] = ptr[j];
	}
}

void			encode_der_key(uint8_t *der, uint64_t *len, t_rsa_key *key)
{
	uint64_t	key_seq_length;
	uint64_t	lengths[RSA_KEY_BINTS] = { 0 };

	*len = 0;
	key_seq_length = der_key_lengths(lengths, key);
	der[(*len)++] = 0x30;
	if (key->is_pub)
		set_pubkey_header(der, len, key_seq_length);
	der_encode_length(der, len, key_seq_length);
	if (!key->is_pub)
	{
		ft_memcpy((void *)(der + *len), "\x02\x01\x00", 3);
		*len += 3;
	}
	for (int i = 0; i < (key->is_pub ? RSA_PUB_BINTS : RSA_KEY_BINTS); ++i)
		der_encode_bint(der, len, key->rsa_bints[i], lengths[i]);
}
