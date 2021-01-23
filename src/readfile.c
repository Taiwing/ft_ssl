/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 12:15:12 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/23 01:10:51 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "readfile.h"
#include "libft.h"

void print_readfile_error(const char *file_name)
{
	ft_dprintf(2, "%s: %s\n", file_name, strerror(errno));
}

int	readfile(const char *file_name, char *buf, size_t bufsize)
{
	static int	fd = -1;	
	int			rd;

	rd = -1;
	if (fd < 0)
		fd = file_name ? open(file_name, O_RDONLY) : 0;
	if (fd >= 0)
		rd = read(fd, buf, bufsize);
	if (rd < 0 || (size_t)rd < bufsize)
	{
		if (fd > 0)
			close(fd);
		fd = -1;
	}
	return (rd);
}
