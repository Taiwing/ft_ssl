/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_des.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 19:12:40 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/13 10:30:41 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rsa.h"
#include "cmd_des_utils.h"

int	rsa_des_getkey(t_rsa_key *key, const char *passin,
	const char *cmd, const char *inkey)
{
	char	passbuf[_SC_PASS_MAX + 1];

	if (passin)
	{
#ifdef RETARDED_OPENSSL_PASSWORD_MODE_IS_ON
		if (ft_strncmp("pass:", passin, RETARDED_PREFIX_LEN))
			return (!!ft_dprintf(2, "ft_ssl: %s: Invalid password argument "
				"\"%s\"\n", cmd, passin));
		passin += RETARDED_PREFIX_LEN;
#endif
		if (ft_strlen(passin) < MIN_PASSLEN && passin[0])
			return (!!ft_dprintf(2, "ft_ssl: %s: password too small (you must "
				"type in %d to %d characters)\n",
				cmd, MIN_PASSLEN, _SC_PASS_MAX));
		else if (!passin[0])
			passin = NULL;
	}
	if (!passin)
	{
		ft_printf("Enter pass phrase for %s:", inkey);
		if (read_des_password(passbuf, cmd, 0, 0))
			return (1);
		passin = passbuf;
	}
	return (pbkdf(&key->des, passin, 1, "md5"));
}

int	rsa_des_decrypt(uint8_t *der, uint8_t *len,
	t_rsa_key *key, const char *cmd)
{
	uint64_t	*derptr;
	uint64_t	old_len;
	uint64_t	block;
	uint8_t		padl;

	old_len = *len;
	if (!old_len || old_len % sizeof(uint64_t))
		return (!!ft_dprintf(2, "\nft_ssl: %s: error: incomplete %u bytes"
			" input block\n", cmd, sizeof(uint64_t)));
	derptr = (uint64_t *)der;
	old_len /= sizeof(uint64_t);
	des_keygen(&key->des);
	while (old_len--)
	{
		key->des.plaintext = *derptr;
		block = exec_cypher(&key->des);
		*derptr++ = block;
	}
	padl = ((uint8_t *)&block)[sizeof(uint64_t) - 1];
	*len -= padl ? padl : sizeof(uint64_t);
	return (0);
}
