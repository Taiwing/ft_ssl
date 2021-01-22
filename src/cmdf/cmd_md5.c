/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_md5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 13:51:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/22 17:47:43 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "readfile.h"
#include "md5.h"

static int	md5_from_file(const char *file_name)
{
	uint64_t	size;
	uint32_t	regs[4];
	char		buf[MD5_BUF_SIZE];
	int			rd;

	size = 0;
	init_regs(regs);
	while ((rd = readfile(file_name, buf, MD5_BUF_SIZE)) == MD5_BUF_SIZE)
	{
		exec_md5(regs, (uint32_t *)buf);
		size += rd * 8;
	}
	if (rd < 0)
	{
		print_readfile_error(file_name ? file_name : "stdin");
		return (1);
	}
	add_md5_padding(regs, buf, rd, size);
	//ft_printf("RESULT: %x%x%x%x\n", regs[0], regs[1], regs[2], regs[3]);
	ft_printf("%16t%02hhx\n", (unsigned char *)regs);
	return (0);
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
