/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_des_options.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:20:12 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/29 11:33:02 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "libft.h"
#include "commands.h"
#include "des.h"

#define HEX_LEN	16

int	parse_hex(uint64_t *value, char *str)
{
	uint8_t		d;
	uint64_t	n;
	int			len;
	char		hex[HEX_LEN];
	
	ft_memset((void *)hex, '0', HEX_LEN);
	if ((len = ft_strlen(str)) != HEX_LEN)
		ft_dprintf(2, "hex string is too %s\n",
			len > HEX_LEN ? "long, ignoring excess"
			: "short, padding with zero bytes to length");
	ft_strncpy(hex, str, len > HEX_LEN ? HEX_LEN : len);
	n = 0;
	for (int i = 0; i < HEX_LEN; ++i)
	{
		hex[i] = hex[i] >= 'A' && hex[i] <= 'F' ? hex[i] + 32 : hex[i];
		if (hex[i] >= '0' && hex[i] <= '9')
			d = hex[i] - '0';
		else if (hex[i] >= 'a' && hex[i] <= 'f')
			d = hex[i] - 'a' + 10;
		else
			return (!!ft_dprintf(2, "non-hex digit\n"));
		n = (n << 4) | (d & 0x0f);
	}
	*value = n;
	return (0);
}

static int	read_password(char *value, const char *cmd)
{
	char	*pass;

	(void)value;
	ft_printf("enter %s ", cmd);
	if (!(pass = getpass("encryption password:")))
		return (!!ft_dprintf(2, "ft_ssl: %s: %s\n", cmd, strerror(errno)));
	ft_strncpy(value, pass, _SC_PASS_MAX);
	value[_SC_PASS_MAX] = 0;
	if (ft_strlen(value) == _SC_PASS_MAX)
		ft_dprintf(2, "ft_ssl: %s: warning: the password can't be longer than "
			"%d characters\n\t(any character above this limit will be "
			"discarded)\n", cmd, _SC_PASS_MAX);
	ft_printf("Verifying - enter %s ", cmd);
	ft_bzero((void *)pass, ft_strlen(pass));
	if (!(pass = getpass("encryption password:")))
		return (!!ft_dprintf(2, "ft_ssl: %s: %s\n", cmd, strerror(errno)));
	if (ft_strncmp(value, pass, _SC_PASS_MAX))
		return (!!ft_dprintf(2, "ft_ssl: %s: passwords dont match\n", cmd));
	return (0);
}

int			parse_des_options(t_des_ctx *ctx, const t_command *cmd,
	t_cmdopt *opt)
{
	char	pass[_SC_PASS_MAX + 1];

	opt[CC_PASSWORD].is_set = !opt[CC_KEY].is_set;
	opt[CC_ENCRYPT].is_set = !opt[CC_DECRYPT].is_set
		|| opt[CC_DECRYPT].ind < opt[CC_ENCRYPT].ind;
	/* TODO: replace '!= des_ecb' by '== des_cbc' when implemented */
	if (ctx->process_block != des_ecb && opt[CC_KEY].is_set
		&& !opt[CC_INIT_VECTOR].is_set)
		return (!!ft_dprintf(2, "ft_ssl: %s: iv undefined\n", cmd->name));
	else if (opt[CC_INIT_VECTOR].is_set
		&& parse_hex(&ctx->iv, opt[CC_INIT_VECTOR].value))
		return (1);
	if (opt[CC_KEY].is_set)
		return (parse_hex(&ctx->key, opt[CC_KEY].value));
	if (!opt[CC_PASSWORD].value)
	{
		if (read_password(pass, cmd->name))
			return (1);
		opt[CC_PASSWORD].value = (char *)pass;
		ft_printf("password: %s\n", opt[CC_PASSWORD].value);
	}
	// get salty or generate it: salt = opt[CC_SALT].value;
	// parse password
	return (0);
}
