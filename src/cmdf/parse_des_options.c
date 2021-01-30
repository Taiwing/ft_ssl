/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_des_options.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:20:12 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/30 20:13:50 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "libft.h"
#include "cmd_des_utils.h"
#include "md.h"

#define SALT_LEN	8
#define HEX_LEN		16
#define FLIP(x, n)	((x) >> n | ((x) << n))

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

static int		pbkdf(t_des_ctx *des_ctx, t_cmdopt *opt)
{
	t_md_ctx	md_ctx;
	void		*pass;
	size_t		pass_len;
	size_t		len;

	if (init_md_context("sha256", &md_ctx))
		return (1);
	init_registers(&md_ctx);
	ft_memswap((void *)&des_ctx->salt, SALT_LEN);
	pass = opt[CC_PASSWORD].value;
	pass_len = ft_strlen(pass);
	len = md_block_exec(&md_ctx, pass, pass_len);
	ft_memcpy((void *)md_ctx.buf, (void *)pass + pass_len - len, len);
	ft_memcpy((void *)md_ctx.buf + len, (void *)&des_ctx->salt,
		len + SALT_LEN > MD_BUF_SIZE ? MD_BUF_SIZE - len : SALT_LEN);
	if ((len = md_block_exec(&md_ctx, NULL, len + SALT_LEN)) < SALT_LEN)
		ft_memcpy((void *)md_ctx.buf, (void *)&des_ctx->salt + SALT_LEN - len,
			len);
	add_md_padding(&md_ctx, len, (pass_len + SALT_LEN - len) * 8);
	des_ctx->key = FLIP(*(uint64_t *)md_ctx.regs, 32);
	if (!opt[CC_INIT_VECTOR].is_set)
		des_ctx->iv = FLIP(*(uint64_t *)md_ctx.regs + sizeof(uint64_t), 32);
	//TEST
	ft_printf("KEY = %016lx\n", des_ctx->key);
	ft_printf("IV = %016lx\n", des_ctx->iv);
	ft_printf("raw bytes key = %*t%02hhx\n", sizeof(uint64_t),
		(unsigned char *)&des_ctx->key); //TEST
	ft_printf("raw bytes iv = %*t%02hhx\n", sizeof(uint64_t),
		(unsigned char *)&des_ctx->iv); //TEST
	//TEST
	return (0);
}

static int		read_password(char *value, const char *cmd)
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

int				parse_hex(uint64_t *value, const char *str, const char *cmd)
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
	char	pass[_SC_PASS_MAX + 1];

	opt[CC_PASSWORD].is_set = !opt[CC_KEY].is_set;
	opt[CC_ENCRYPT].is_set = !opt[CC_DECRYPT].is_set
		|| opt[CC_DECRYPT].ind < opt[CC_ENCRYPT].ind;
	if (ctx->process_block == des_cbc && opt[CC_KEY].is_set
		&& !opt[CC_INIT_VECTOR].is_set)
		return (!!ft_dprintf(2, "ft_ssl: %s: iv undefined\n", cmd->name));
	else if (opt[CC_INIT_VECTOR].is_set
		&& parse_hex(&ctx->iv, opt[CC_INIT_VECTOR].value, cmd->name))
		return (1);
	if (opt[CC_KEY].is_set)
		return (parse_hex(&ctx->key, opt[CC_KEY].value, cmd->name));
	if (!opt[CC_PASSWORD].value)
	{
		if (read_password(pass, cmd->name))
			return (1);
		opt[CC_PASSWORD].value = (char *)pass;
		ft_printf("password: %s\n", opt[CC_PASSWORD].value);
	}
	return (get_salty(ctx, cmd, opt) || pbkdf(ctx, opt));
}
