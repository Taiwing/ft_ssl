/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 12:15:12 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/05 00:51:18 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "readfile.h"
#include "libft.h"

static int	readstdin(char *buf, size_t bufsize)
{
	int	rd;
	int	ret;

	rd = 0;
	ret = 0;
	while (bufsize && (rd = read(0, buf + ret, bufsize)) > 0)
	{
		bufsize -= (size_t)rd;
		ret += rd;
	}
	return (rd < 0 ? rd : ret);
}

int	readfile(const char *file_name, char *buf, size_t bufsize)
{
	static int	fd = -1;
	int			rd;

	rd = -1;
	if (fd < 0)
		fd = file_name ? open(file_name, O_RDONLY) : 0;
	if (fd > 0)
		rd = read(fd, buf, bufsize);
	else if (fd == 0)
		rd = readstdin(buf, bufsize);
	if (rd < 1 || (!file_name && !buf && !bufsize))
	{
		if (fd > 0)
			close(fd);
		fd = -1;
	}
	return (rd);
}

void	print_readfile_error(const char *cmd, const char *file_name)
{
	ft_dprintf(2, "ft_ssl: %s: %s: %s\n", cmd, file_name, strerror(errno));
}
