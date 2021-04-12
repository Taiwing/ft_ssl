/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_des.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 19:12:40 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/12 16:18:43 by yforeau          ###   ########.fr       */
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

int	rsa_des_decrypt(uint8_t derkey[KEY_MAXLEN], uint8_t len,
	t_rsa_key *key, const char *cmd)
{
	uint64_t	*derptr;

	if (!len || len % sizeof(uint64_t))
		return (!!ft_dprintf(2, "\nft_ssl: %s: error: incomplete %u bytes"
			" input block\n", cmd, sizeof(uint64_t)));
	derptr = (uint64_t *)derkey;
	len /= sizeof(uint64_t);
	des_keygen(&key->des);
	while (len--)
	{
		key->des.plaintext = *derptr;
		*derptr++ = exec_cypher(&key->des);
	}
	return (0);
}
