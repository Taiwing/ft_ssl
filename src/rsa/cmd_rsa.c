/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_rsa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 06:54:37 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/14 09:19:27 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "rsa.h"

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

static int	print_text_rsa_key(int outfd, t_rsa_key *key)
{
	if (key->is_pub)
		ft_dprintf(outfd, "RSA Public-Key: (64 bit)\n"
			"Modulus: %1$llu (%1$#llx)\n"
			"Exponent: %2$llu (%2$#llx)\n",
			key->n, key->e);
	else
		ft_dprintf(outfd, "RSA Private-Key: (64 bit, 2 primes)\n"
			"modulus: %1$llu (%1$#llx)\n"
			"publicExponent: %2$llu (%2$#llx)\n"
			"privateExponent: %3$llu (%3$#llx)\n"
			"prime1: %4$llu (%4$#llx)\n"
			"prime2: %5$llu (%5$#llx)\n"
			"exponent1: %6$llu (%6$#llx)\n"
			"exponent2: %7$llu (%7$#llx)\n"
			"coefficient: %8$llu (%8$#llx)\n",
			key->n, key->e, key->d, key->p, key->q,
			key->exp1, key->exp2, key->coeff);
	return (0);
}

static int	rsa_check_key(int outfd, t_rsa_key *key)
{
	(void)key;
	//TODO: actually check key
	ft_dprintf(outfd, "RSA key ok\n");
	return (0);
}

static int	create_output_key(int outfd, t_rsa_key *key_in,
	t_cmdopt *opt, const t_command *cmd)
{
	t_rsa_key		key_out;
	t_des_getkey	gk = { NULL, opt[RSA_PASSOUT].value,
		NULL, "Enter PEM pass phrase:", 1 };

	ft_dprintf(2, "writing RSA key\n");
	ft_memcpy((void *)&key_out, (void *)key_in, sizeof(t_rsa_key));
	ft_bzero((void *)&key_out.des, sizeof(t_des_ctx));
	key_out.is_pub = !key_out.is_pub ? opt[RSA_PUBOUT].is_set : key_out.is_pub;
	key_out.is_enc = !key_out.is_pub && opt[RSA_DES].is_set;
	return (print_rsa_key(outfd, &key_out, cmd->name, &gk));
}

int			cmd_rsa(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int			ret;
	int			outfd;
	t_rsa_key	key;

	(void)args;
	outfd = 1;
	ft_bzero((void *)&key, sizeof(t_rsa_key));
	ret = parse_options(&key, &outfd, opt, cmd);
	if (!ret && opt[RSA_TEXT].is_set)
		print_text_rsa_key(outfd, &key);
	if (!ret && opt[RSA_MODULUS].is_set)
		ft_dprintf(outfd, "Modulus=%llX\n", key.n);
	if (!ret && opt[RSA_CHECK].is_set)
		ret = rsa_check_key(outfd, &key);
	if (!ret && !opt[RSA_NOOUT].is_set)
		ret = create_output_key(outfd, &key, opt, cmd);
	if (outfd > 1)
		close(outfd);
	return (ret);
}
