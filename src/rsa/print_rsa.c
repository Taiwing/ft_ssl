/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_rsa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 11:38:40 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/20 11:52:30 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rsa.h"
#include "libft.h"
#include "base64.h"
#include "rsa_math.h"

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

void		print_rsa_modulus(int outfd, t_rsa_key *key)
{
	int	i = BINT_LEN(key->n);

	ft_dprintf(outfd, "Modulus=%llX", i ? key->n[i--] : 0);
	while (i)
		ft_dprintf(outfd, "%08llX", key->n[i--]);
	ft_dprintf(outfd, "\n");
}

static void	print_rsa_bint(int outfd, const t_bint b)
{
	uint32_t	j = 0;
	uint8_t		byte, *digit;

	ft_dprintf(outfd, "    ");
	for (int i = BINT_LEN(b); i; --i)
	{
		digit = (uint8_t *)(b + i);
		for (uint32_t k = 1; k <= sizeof(uint32_t); ++k)
		{
			byte = digit[sizeof(uint32_t) - k];
			if (!j && (byte & 0x80))
			{
				ft_dprintf(outfd, "00");
				++j;
			}
			if (j || byte)
			{
				ft_dprintf(outfd, "%s%s%02hhx", j ? ":" : "",
					j && !(j % 15) ? "\n    " : "", byte);
				++j;
			}
		}
	}
	if (j % 15)
		ft_dprintf(outfd, "\n");
}

void		print_text_rsa_key(int outfd, t_rsa_key *key)
{
	uint64_t	u;
	int			stop = key->is_pub ? RSA_PUB_BINTS : RSA_KEY_BINTS;
	char		*pub_names[RSA_PUB_BINTS] = { "Modulus", "Exponent" };
	char		*priv_names[RSA_KEY_BINTS] = {
		"modulus", "publicExponent", "privateExponent", "prime1",
		"prime2", "exponent1", "exponent2", "coefficient"
	};
	char		**tab = key->is_pub ? pub_names : priv_names;

	ft_dprintf(outfd, "RSA %s-Key: (%u bit%s)\n",
		key->is_pub ? "Public" : "Private", key->size,
		key->is_pub ? "" : ", 2 primes");
	for (int i = 0; i < stop; ++i)
	{
		ft_dprintf(outfd, "%s:", tab[i]);
		if (!BINT_LEN(key->rsa_bints[i]))
			ft_dprintf(outfd, " 0\n");
		else if (bint_to_u64(&u, key->rsa_bints[i]) == BINT_SUCCESS)
			ft_dprintf(outfd, " %1$llu (%1$#llx)\n", u);
		else
		{
			ft_dprintf(outfd, "\n");
			print_rsa_bint(outfd, key->rsa_bints[i]);
		}
	}
}
