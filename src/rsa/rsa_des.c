/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_des.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 19:12:40 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/12 17:11:01 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rsa.h"
#include "cmd_des_utils.h"

int	rsa_des_getkey(t_rsa_key *key, const char *passin, const char *cmd)
{
	char	passbuf[_SC_PASS_MAX + 1];

	if (!passin && read_des_password(passbuf, cmd))
		return (1);
	else if (!passin)
		passin = passbuf;
	return (pbkdf(&key->des, passin, 1, "md5"));
}

int	rsa_des_decrypt(uint8_t derkey[KEY_MAXLEN], uint8_t *len,
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
	derptr = (uint64_t *)derkey;
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
