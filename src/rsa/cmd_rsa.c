/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_rsa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 06:54:37 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/20 11:48:47 by yforeau          ###   ########.fr       */
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
