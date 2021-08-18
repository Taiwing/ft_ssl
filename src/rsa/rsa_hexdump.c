/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_hexdump.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 15:21:12 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 22:48:32 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define HXDMP_SIZE	16
#define LINE_SIZE	57

const char g_space_buf[64] = { [0 ... 63] = ' ' };

int	rsa_hexdump(int fd, uint8_t *data, size_t len)
{
	int		p;
	size_t	i = 0;
	int		ret = 0;
	size_t	to_print;

	while (i < len && ret >= 0)
	{
		to_print = (len - i) / HXDMP_SIZE ? HXDMP_SIZE : len % HXDMP_SIZE;
		ret = ft_dprintf(fd, "%04lx -%*t %02hhx%s", i,
			to_print > 8 ? 8 : to_print, data + i, to_print >= 8 ? "-" : "" );
		p = ret;
		if (p >= 0 && to_print > 8)
			ret = ft_dprintf(fd, "%#*t%02hhx   ", to_print - 8, data + i + 8);
		p += ret;
		if (ret >= 0)
			ret = ft_dprintf(fd, "%.*s", LINE_SIZE - p, g_space_buf);
		for (size_t j = 0; j < to_print && ret >= 0; ++j)
			ret = write(fd, ft_isprint(data[i + j]) ?
				(char *)data + i + j : ".", 1);
		if (ret >= 0)
			ret = write(fd, "\n", 1);
		i += HXDMP_SIZE;
	}
	return (ret);
}
