/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_des.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 19:12:40 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/11 21:50:28 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rsa.h"
#include "cmd_des_utils.h"

const char	*rsa_check_pass(const char *pass, const char *cmd)
{
	if (pass)
	{
#ifdef RETARDED_OPENSSL_PASSWORD_MODE_IS_ON
		if (ft_strncmp("pass:", pass, RETARDED_PREFIX_LEN))
		{
			ft_dprintf(2, "ft_ssl: %s: Invalid password argument "
				"\"%s\"\n", cmd, pass);
			return (NULL);
		}
		pass += RETARDED_PREFIX_LEN;
#endif
		if (ft_strlen(pass) < MIN_PASSLEN && pass[0])
		{
			ft_dprintf(2, "ft_ssl: %s: password too small (you must type in "
				"%d to %d characters)\n", cmd, MIN_PASSLEN, _SC_PASS_MAX);
			return (NULL);
		}
		else if (!pass[0])
			pass = NULL;
	}
	return (pass);
}

int			rsa_des_getkey(t_des_ctx *des, const char *cmd, t_des_getkey *gk)
{
	char		passbuf[_SC_PASS_MAX + 1];
	const char	*pass;

	if (!(pass = rsa_check_pass(gk->pass, cmd)))
	{
		if (read_des_password(passbuf, cmd, gk))
			return (1);
		pass = passbuf;
	}
	return (pbkdf(des, pass, 1, "md5"));
}

int	rsa_des_decrypt(uint8_t *der, uint8_t *len,
	t_des_ctx *des, const char *cmd)
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
	des_keygen(des);
	while (old_len--)
	{
		des->plaintext = *derptr;
		block = exec_cypher(des);
		*derptr++ = block;
	}
	padl = ((uint8_t *)&block)[sizeof(uint64_t) - 1];
	*len -= padl < sizeof(uint64_t) ? padl : sizeof(uint64_t);
	return (0);
}

void rsa_des_encrypt(uint8_t *der, uint64_t *len, t_des_ctx *des)
{
	uint64_t	*derptr;
	uint64_t	old_len;
	uint64_t	block;
	int			c;

	c = sizeof(uint64_t) - (*len % sizeof(uint64_t));
	derptr = (uint64_t *)der;
	old_len = *len;
	*len += c;
	des_keygen(des);
	while (c)
	{
		ft_memcpy((void *)&des->plaintext,
			(void *)derptr, sizeof(uint64_t));
		if (old_len < sizeof(uint64_t))
		{
			ft_memset((void *)&des->plaintext + old_len, c, c);
			c = 0;
		}
		block = exec_cypher(des);
		*derptr++ = block;
		old_len -= sizeof(uint64_t);
	}
}
