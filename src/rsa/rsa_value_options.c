/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_value_options.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 11:39:57 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 20:19:10 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rsa.h"
#include "cmd_des_utils.h"

static int	bint_parse_hex(t_bint dst, const char *hex)
{
	uint32_t	tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	len = ft_strlen(hex);
	uint32_t	k, c, d, n;

	for (uint32_t i = 1; len && i < BINT_SIZE(tmp); ++i, len -= (k - 1))
	{
		for (k = 1, n = 0; k <= 8 && k <= len; ++k)
		{
			c = hex[len - k] >= 'A' && hex[len - k] <= 'F' ?
				hex[len - k] + 32 : hex[len - k];
			if (c >= '0' && c <= '9')
				d = (c - '0') & 0x0f;
			else if (c >= 'a' && c <= 'f')
				d = (c - 'a' + 10) & 0x0f;
			else
				return (BINT_FAILURE);
			d = k > 1 ? d << ((k - 1) * 4) : d;
			n |= d;
		}
		tmp[i] = n;
		if (n)
			SET_BINT_LEN(tmp, i);
	}
	if (!len)
		return (bintcpy(dst, tmp));
	return (BINT_FAILURE);
}

void		rsa_value_options(t_rsa_key *key, t_cmdopt *opt, const char *cmd)
{
	t_bint					val;
	enum e_rsa_option_codes	opcode;

	opcode = RSA_PUBOUT;
	while (opcode != RSA_COEFF)
		if (opt[++opcode].is_set)
		{
			val = NULL;
			switch (opcode)
			{
				case RSA_N:			val = key->n;			break;
				case RSA_E:			val = key->e;			break;
				case RSA_D:			val = key->d;			break;
				case RSA_P:			val = key->p;			break;
				case RSA_Q:			val = key->q;			break;
				case RSA_EXP1:		val = key->exp1;		break;
				case RSA_EXP2:		val = key->exp2;		break;
				case RSA_COEFF:		val = key->coeff;		break;
				default:									break;
			}
			if (!val || bint_parse_hex(val, opt[opcode].value) == BINT_FAILURE)
				ft_dprintf(2, "ft_ssl: %s: invalid value for \"%s\" option is "
					"going to be ignored\n", cmd, g_rsa_options[opcode].name);
		}
}
