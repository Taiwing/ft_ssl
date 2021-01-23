/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_md5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 13:51:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/23 12:07:22 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "readfile.h"
#include "md5.h"

static void	print_md5_hash(const char *name, t_cmdopt *opt, uint32_t *regs) 
{
	if (name && !opt[MDC_QUIET].is_set && !opt[MDC_REVERSE].is_set)
		ft_printf("MD5 (%2$s%1$s%2$s) = ", name,
			opt[MDC_STRING].value == name ? "\"" : "");
	ft_printf("%16t%02hhx", regs);
	if (name && !opt[MDC_QUIET].is_set && opt[MDC_REVERSE].is_set)
		ft_printf(" %2$s%1$s%2$s", name,
			opt[MDC_STRING].value == name ? "\"" : "");
	ft_printf("\n");
}

static int	md5_from_file(const char *cmd, const char *file_name, t_cmdopt *opt)
{
	uint64_t	size;
	uint32_t	regs[4];
	char		buf[MD5_BUF_SIZE];
	int			rd;

	size = 0;
	init_regs(regs);
	while ((rd = readfile(file_name, buf, MD5_BUF_SIZE)) == MD5_BUF_SIZE)
	{
		if (!file_name && opt[MDC_PRINT].is_set)
			ft_printf("%.*s", rd, buf);
		exec_md5(regs, (uint32_t *)buf);
		size += rd * 8;
	}
	if (rd < 0)
	{
		print_readfile_error(cmd, file_name ? file_name : "stdin");
		return (1);
	}
	add_md5_padding(regs, buf, rd, size);
	if (rd > 0 && rd < MD5_BUF_SIZE && !file_name && opt[MDC_PRINT].is_set)
		ft_printf("%.*s", rd, buf);
	print_md5_hash(file_name, opt, regs);
	return (0);
}

static int	md5_from_string(const char *str, t_cmdopt *opt)
{
	int			len;
	const char	*ptr;
	uint32_t	regs[4];
	char		buf[MD5_BUF_SIZE];

	len = ft_strlen(str);
	ptr = str;
	init_regs(regs);
	while (len >= MD5_BUF_SIZE)
	{
		ft_memcpy((void *)buf, (void *)ptr, MD5_BUF_SIZE);
		exec_md5(regs, (uint32_t *)buf);
		ptr += MD5_BUF_SIZE;
		len -= MD5_BUF_SIZE;
	}
	ft_memcpy((void *)buf, (void *)ptr, len);
	add_md5_padding(regs, buf, len, (ft_strlen(str) - len) * 8);
	print_md5_hash(str, opt, regs);
	return (0);
}

int	cmd_md5(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int	ret;

	ret = 0;
	if (opt[MDC_PRINT].is_set || (!opt[MDC_STRING].is_set && !*args))
		ret = md5_from_file(cmd->name, NULL, opt);
	if (opt[MDC_STRING].is_set)
		md5_from_string(opt[MDC_STRING].value, opt);
	while (*args)
		ret += md5_from_file(cmd->name, *args++, opt);
	return (ret);
}
