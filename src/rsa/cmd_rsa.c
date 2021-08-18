/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_rsa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 06:54:37 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 13:50:52 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "rsa_value_options.h"
#include "rsa_math.h"

static int	parse_options(t_rsa_key *key, int *outfd,
		t_cmdopt *opt, const t_command *cmd)
{
	t_des_getkey	gk = { opt[RSA_IN].value, opt[RSA_PASSIN].value,
		NULL, "Enter pass phrase:", 0 };

	key->is_pub = opt[RSA_PUBIN].is_set;
	if (key->is_pub && opt[RSA_CHECK].is_set)
		return (!!ft_dprintf(2,
			"ft_ssl: %s: Only private keys can be checked\n", cmd->name));
	if (opt[RSA_INFORM].is_set
			&& ft_ignore_case_strcmp(opt[RSA_INFORM].value, "PEM"))
		return (!!ft_dprintf(2, "ft_ssl: %s: Invalid format \"%s\" "
			"for -inform\n", cmd->name, opt[RSA_INFORM].value));
	if (opt[RSA_OUTFORM].is_set
			&& ft_ignore_case_strcmp(opt[RSA_OUTFORM].value, "PEM"))
		return (!!ft_dprintf(2, "ft_ssl: %s: Invalid format \"%s\" "
			"for -outform\n", cmd->name, opt[RSA_OUTFORM].value));
	if (parse_rsa_key(key, cmd->name, &gk))
		return (!!ft_dprintf(2, "ft_ssl: %s: unable to load %s Key\n",
			cmd->name, key->is_pub ? "Public" : "Private"));
	if (opt[RSA_OUT].is_set)
		*outfd = output_option(opt[RSA_OUT].value, cmd->name);
	return (*outfd < 0);
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

static void	print_text_rsa_key(int outfd, t_rsa_key *key)
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

static void	print_rsa_modulus(int outfd, t_rsa_key *key)
{
	int	i = BINT_LEN(key->n);

	ft_dprintf(outfd, "Modulus=%llX", i ? key->n[i--] : 0);
	while (i)
		ft_dprintf(outfd, "%08llX", key->n[i--]);
	ft_dprintf(outfd, "\n");
}

static int	create_output_key(int outfd, t_rsa_key *key_in,
	t_cmdopt *opt, const t_command *cmd)
{
	t_rsa_key		key_out;
	t_des_getkey	gk = { NULL, opt[RSA_PASSOUT].value,
		NULL, "Enter PEM pass phrase:", 1 };

	ft_memcpy((void *)&key_out, (void *)key_in, sizeof(t_rsa_key));
	INIT_RSA_KEY(key_out);
	ft_dprintf(2, "writing RSA key\n");
	ft_bzero((void *)&key_out.des, sizeof(t_des_ctx));
	key_out.is_pub = !key_out.is_pub ? opt[RSA_PUBOUT].is_set : key_out.is_pub;
	key_out.is_enc = !key_out.is_pub && opt[RSA_DES].is_set;
	rsa_value_options(&key_out, opt, cmd->name);
	return (print_rsa_key(outfd, &key_out, cmd->name, &gk));
}

static int	rsa_key_bint_to_64(t_rsa_key_64 *key64, t_rsa_key *key)
{
	ft_bzero((void *)key64, sizeof(t_rsa_key_64));
	if (bint_to_u64(&key64->n, key->n) == BINT_FAILURE
		|| bint_to_u64(&key64->e, key->e) == BINT_FAILURE)
		return (1);
	if (!key->is_pub && (bint_to_u64(&key64->d, key->d) == BINT_FAILURE
		|| bint_to_u64(&key64->p, key->p) == BINT_FAILURE
		|| bint_to_u64(&key64->q, key->q) == BINT_FAILURE
		|| bint_to_u64(&key64->exp1, key->exp1) == BINT_FAILURE
		|| bint_to_u64(&key64->exp2, key->exp2) == BINT_FAILURE
		|| bint_to_u64(&key64->coeff, key->coeff) == BINT_FAILURE))
		return (1);
	key64->is_pub = key->is_pub;
	key64->is_enc = key->is_enc;
	key64->size = key->size;
	ft_memcpy((void *)&key64->des, (void *)&key->des, sizeof(t_des_ctx));
	return (0);
}

int			cmd_rsa(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int				ret;
	t_rsa_key_64	key64;
	int				outfd = 1;
	t_rsa_key		key = RSA_KEY_DEFAULT;

	(void)args;
	INIT_RSA_KEY(key);
	ret = parse_options(&key, &outfd, opt, cmd);
	if (!ret && opt[RSA_TEXT].is_set)
		print_text_rsa_key(outfd, &key);
	if (!ret && opt[RSA_MODULUS].is_set)
		print_rsa_modulus(outfd, &key);
	if (!ret && opt[RSA_CHECK].is_set
		&& rsa_check_key_size(&key, GENRSA_KEY_SIZE_MAX))
		ft_dprintf(2, "ft_ssl: %1$s: rsa key is too long (%2$u bit)\n"
			"ft_ssl: %1$s: -check option handles keys up to %3$u bit long\n",
			cmd->name, key.size, GENRSA_KEY_SIZE_MAX);
	else if (!ret && opt[RSA_CHECK].is_set && !rsa_key_bint_to_64(&key64, &key))
		rsa_check_key_64(outfd, &key64);
	if (!ret && !opt[RSA_NOOUT].is_set)
		ret = create_output_key(outfd, &key, opt, cmd);
	if (outfd > 1)
		close(outfd);
	return (ret);
}
