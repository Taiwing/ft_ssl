/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_value_options.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 11:39:57 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/05 13:49:13 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rsa.h"
#include "cmd_des_utils.h"

static void	set_hexbuf(char hexbuf[HEX_LEN + 1], char *str)
{
	int	len;
	int	to_copy;

	ft_memset((void *)hexbuf, '0', HEX_LEN);
	hexbuf[HEX_LEN] = 0;
	len = ft_strlen(str);
	to_copy = len > HEX_LEN ? HEX_LEN : len;
	ft_memcpy((void *)hexbuf + HEX_LEN - to_copy,
		(void *)str + len - to_copy, to_copy);
}

void		rsa_value_options(t_rsa_key_64 *key, t_cmdopt *opt, const char *cmd)
{
	char					hexbuf[HEX_LEN + 1];
	uint64_t				*value;
	enum e_rsa_option_codes	opcode;

	opcode = RSA_PUBOUT;
	while (opcode != RSA_COEFF)
		if (opt[++opcode].is_set)
		{
			switch (opcode)
			{
				case RSA_N:			value = &key->n;		break;
				case RSA_E:			value = &key->e;		break;
				case RSA_D:			value = &key->d;		break;
				case RSA_P:			value = &key->p;		break;
				case RSA_Q:			value = &key->q;		break;
				case RSA_EXP1:		value = &key->exp1;		break;
				case RSA_EXP2:		value = &key->exp2;		break;
				case RSA_COEFF:		value = &key->coeff;	break;
				default:									break;
			}
			set_hexbuf(hexbuf, opt[opcode].value);
			if (parse_hex(value, hexbuf, cmd))
				ft_dprintf(2, "ft_ssl: %s: invalid value for \"%s\" option is "
					"going to be ignored\n", cmd, g_rsa_options[opcode].name);
		}
}
