/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_base64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:17:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/19 17:58:19 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "readfile.h"
#include "base64.h"

int	base64_file_encrypt(int outfd, const char *input_file, const char *cmd)
{
	int		rd;
	int		len;
	char	inbuf[BASE64_INBUF_SIZE];
	char	outbuf[BASE64_OUTBUF_SIZE];

	while (1)
	{
		if ((rd = readfile(input_file, (char *)inbuf, BASE64_INBUF_SIZE)) < 0)
		{
			print_readfile_error(cmd, input_file ? input_file : "stdin");
			return (1);
		}
		if ((len = base64_encrypt(outbuf, (unsigned char *)inbuf, rd)))
			ft_dprintf(outfd, "%.*s\n", len, outbuf);
		if (rd < BASE64_INBUF_SIZE)
			break;
	}
	return (0);
}

int	base64_buffer_parse(char *buf, int len)
{
	int	i;
	int	invalid;

	invalid = 0;
	for (i = 0; i < len; ++i)
	{
		if (buf[i] == '+' || buf[i] == '/' || ft_isalnum(buf[i]))
		{
			if (invalid)
			{
				ft_memmove((void *)buf + i - invalid, (void *)buf + i, len - i);
				len -= invalid;
				i -= invalid;
				invalid = 0;
			}
		}
		else
			++invalid;
	}
	return (i - invalid);
}

int	base64_file_decrypt(int outfd, const char *input_file, const char *cmd)
{
	int		rd;
	int		len;
	int		valid;
	char	inbuf[BASE64_OUTBUF_SIZE];
	char	outbuf[BASE64_INBUF_SIZE];

	base64_decrypt(NULL, NULL, 0, 1);
	while (1)
	{
		if ((rd = readfile(input_file, (char *)inbuf, BASE64_OUTBUF_SIZE)) < 0)
		{
			print_readfile_error(cmd, input_file ? input_file : "stdin");
			return (1);
		}
		valid = base64_buffer_parse(inbuf, rd);
		if ((len = base64_decrypt((unsigned char *)outbuf, inbuf, valid, 0)))
			ft_dprintf(outfd, "%*t%c", len, outbuf);
		if (rd < BASE64_OUTBUF_SIZE)
			break;
	}
	return (0);
}

int	cmd_base64(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int		ret;
	int		outfd;

	(void)args;
	ret = 1;
	outfd = 1;
	if (opt[BASE64_OUTPUT].is_set
		&& (outfd = output_option(opt[BASE64_OUTPUT].value, cmd->name)) == -1)
		return (ret);
	if (!opt[BASE64_DECRYPT].is_set
		|| (opt[BASE64_ENCRYPT].is_set
		&& opt[BASE64_DECRYPT].ind < opt[BASE64_ENCRYPT].ind))
		ret = base64_file_encrypt(outfd, opt[BASE64_INPUT].value, cmd->name);
	else
		ret = base64_file_decrypt(outfd, opt[BASE64_INPUT].value, cmd->name);
	if (outfd > 1)
		close(outfd);
	return (ret);
}
