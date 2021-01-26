/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_base64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:17:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/26 19:35:56 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "commands.h"
#include "readfile.h"
#include "base64.h"

char *g_base64_str =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

int	base64_encrypt(char out[BASE64_OUTBUF_SIZE], char inbuf[BASE64_INBUF_SIZE],
	int inlen)
{
	int				i;
	int				j;
	int				c;
	unsigned char	*in;

	i = 0;
	j = 0;
	in = (unsigned char *)inbuf;
	while (i < inlen)
	{
		c = in[i] >> 2;
		out[j++] = g_base64_str[c];
		c = (in[i] & 0x03) << 4;
		if (++i < inlen)
			c |= in[i] >> 4;
		out[j++] = g_base64_str[c];
		c = i < inlen ? (in[i] & 0x0f) << 2 : 64;
		if (++i < inlen)
			c |= (in[i] >> 6);
		out[j++] = g_base64_str[c];
		c = i < inlen ? (in[i++] & 0x3f) : 64;
		out[j++] = g_base64_str[c];
	}
	return (j);
}

int	base64_file_encrypt(int outfd, const char *input_file, const char *cmd)
{
	int		rd;
	int		len;
	char	inbuf[BASE64_INBUF_SIZE];
	char	outbuf[BASE64_OUTBUF_SIZE];

	while ((rd = readfile(input_file, (char *)inbuf, BASE64_INBUF_SIZE)) > 0)
	{
		len = base64_encrypt(outbuf, inbuf, rd);
		ft_dprintf(outfd, "%.*s\n", len, outbuf);
	}
	if (rd < 0)
		print_readfile_error(cmd, input_file ? input_file : "stdin");
	return (rd < 0);
}

int	cmd_base64(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int		ret;
	int		outfd;

	(void)cmd;
	(void)args;
	ret = 1;
	outfd = 1;
	if (opt[BASE64_OUTPUT].is_set
		&& (outfd = open(opt[BASE64_OUTPUT].value, O_WRONLY) == -1))
	{
		ft_dprintf(2, "ft_ssl: %s: %s: %s\n", cmd->name,
			opt[BASE64_OUTPUT].value, strerror(errno));
		return (ret);
	}
	if (!opt[BASE64_DECRYPT].is_set
		|| (opt[BASE64_ENCRYPT].is_set
		&& opt[BASE64_DECRYPT].ind < opt[BASE64_ENCRYPT].ind))
		ret = base64_file_encrypt(outfd, opt[BASE64_INPUT].value, cmd->name);
	//else
		//ret = base64_file_decrypt(outfd, opt[BASE64_INPUT].value, cmd->name);
	if (outfd > 1)
		close(outfd);
	return (ret);
}
