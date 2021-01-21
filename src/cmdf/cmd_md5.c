/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_md5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 13:51:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/21 14:34:08 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "readfile.h"

#define MD5_BUF_SIZE 64

static int	md5_from_file(const char *file_name)
{
	char	buf[MD5_BUF_SIZE];
	int		rd;

	while ((rd = readfile(file_name, buf, MD5_BUF_SIZE)) > 0)
		ft_printf("'%.*s'", rd, buf);
	if (rd < 0)
		print_readfile_error(file_name ? file_name : "stdin");
	return (rd < 0);
}

int	cmd_md5(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int	ret;

	(void)cmd;
	(void)opt; //TODO: handle options
	ret = 0;
	if (!ft_wtlen(args))
		ret = md5_from_file(NULL);
	else
	{
		while (*args)
			ret += md5_from_file(*args++);
	}
	return (ret);
}
