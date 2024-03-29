/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_des_options.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:20:12 by yforeau           #+#    #+#             */
/*   Updated: 2022/04/18 10:04:30 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "libft.h"
#include "cmd_des_utils.h"
#include "md.h"

static size_t	md_block_exec(t_md_ctx *ctx, const char *src, size_t len)
{
	while (len >= MD_BUF_SIZE)
	{
		if (src)
			ft_memcpy((void *)ctx->buf, (void *)src, MD_BUF_SIZE);
		exec_md(ctx);
		len -= MD_BUF_SIZE;
		src += MD_BUF_SIZE;
	}
	return (len);
}

static uint64_t	set_u64_des_value(char *regs, int is_be)
{
	uint64_t	u;

	ft_memcpy((void *)&u, (void *)regs, sizeof(uint64_t));
	if (is_be)
		u = FLIP(u, 32);
	else
		ft_memswap((void *)&u, sizeof(uint64_t));
	return (u);
}

int				pbkdf(t_des_ctx *des, const char *pass,
	int iv_is_set, const char *md_name)
{
	t_md_ctx	md;
	size_t		len;
	uint64_t	salt = des->salt;
	size_t		pass_len = ft_strlen(pass);

	if (init_md_context(md_name, &md))
		return (1);
	init_registers(&md);
	ft_memswap((void *)&salt, SALT_LEN);
	len = md_block_exec(&md, pass, pass_len);
	ft_memcpy((void *)md.buf, (void *)pass + pass_len - len, len);
	ft_memcpy((void *)md.buf + len, (void *)&salt,
		len + SALT_LEN > MD_BUF_SIZE ? MD_BUF_SIZE - len : SALT_LEN);
	if ((len = md_block_exec(&md, NULL, len + SALT_LEN)) < SALT_LEN)
		ft_memcpy((void *)md.buf, (void *)&salt + SALT_LEN - len, len);
	add_md_padding(&md, len, (pass_len + SALT_LEN - len) * 8);
	des->key = set_u64_des_value((char *)md.regs, md.is_be);
	if (!iv_is_set)
		des->iv = set_u64_des_value((char *)(md.regs + 2), md.is_be);
	return (0);
}

int				read_des_password(char *value,
		const char *cmd, t_des_getkey *gk)
{
	char	*pass;

	ft_dprintf(2, gk->prompt, gk->arg);
	if (!(pass = getpass("")))
		return (!!ft_dprintf(2, "ft_ssl: %s: %s\n", cmd, strerror(errno)));
	ft_strncpy(value, pass, _SC_PASS_MAX);
	value[_SC_PASS_MAX] = 0;
	if (ft_strlen(value) == _SC_PASS_MAX)
		ft_dprintf(2, "ft_ssl: %s: warning: the password can't be longer than "
			"%d characters\n\t(any character above this limit will be "
			"discarded)\n", cmd, _SC_PASS_MAX);
	if (gk->verify)
	{
		ft_dprintf(2, "Verifying - ");
		ft_dprintf(2, gk->prompt, gk->arg);
		ft_bzero((void *)pass, ft_strlen(pass));
		if (!(pass = getpass("")))
			return (!!ft_dprintf(2, "ft_ssl: %s: %s\n", cmd, strerror(errno)));
		if (ft_strncmp(value, pass, _SC_PASS_MAX))
			return (!!ft_dprintf(2, "ft_ssl: %s: passwords dont match\n", cmd));
	}
	return (0);
}

int				parse_hex_64(uint64_t *value, const char *str, const char *cmd)
{
	uint8_t		d;
	uint64_t	n;
	int			len;
	char		hex[HEX_LEN];

	ft_memset((void *)hex, '0', HEX_LEN);
	if ((len = ft_strlen(str)) != HEX_LEN)
		ft_dprintf(2, "ft_ssl: %s: warning: hex string is too %s\n", cmd,
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
			return (!!ft_dprintf(2, "ft_ssl: %s: non-hex digit\n", cmd));
		n = (n << 4) | (d & 0x0f);
	}
	*value = n;
	return (0);
}

int				parse_des_options(t_des_ctx *ctx, const t_command *cmd,
	t_cmdopt *opt)
{
	t_des_getkey	gk = { 0, 0, 0, "enter des encryption password:", 1 };
	char			pass[_SC_PASS_MAX + 1];

	opt[CC_PASSWORD].is_set = !opt[CC_KEY].is_set;
	opt[CC_ENCRYPT].is_set = !opt[CC_DECRYPT].is_set
		|| opt[CC_DECRYPT].ind < opt[CC_ENCRYPT].ind;
	ctx->reverse = !opt[CC_ENCRYPT].is_set;
	if (opt[CC_INIT_VECTOR].is_set
		&& parse_hex_64(&ctx->iv, opt[CC_INIT_VECTOR].value, cmd->name))
		return (1);
	if (opt[CC_KEY].is_set)
		return (parse_hex_64(&ctx->key, opt[CC_KEY].value, cmd->name));
	if (!opt[CC_PASSWORD].value)
	{
		if (read_des_password(pass, cmd->name, &gk))
			return (1);
		opt[CC_PASSWORD].value = (char *)pass;
	}
	return (get_salty(ctx, cmd, opt) || pbkdf(ctx, opt[CC_PASSWORD].value,
		opt[CC_INIT_VECTOR].is_set, "sha256"));
}
