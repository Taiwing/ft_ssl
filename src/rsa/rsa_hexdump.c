/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_hexdump.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 15:21:12 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/06 17:29:58 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define HXDMP_SIZE	16
#define LINE_SIZE	57

const char g_space_buf[64] = { [0 ... 63] = ' ' };

void	rsa_hexdump(int fd, uint8_t *data, size_t len)
{
	size_t	i;
	int		ret;
	size_t	to_print;

	i = 0;
	while (i < len)
	{
		to_print = (len - i) / HXDMP_SIZE ? HXDMP_SIZE : len % HXDMP_SIZE;
		ret = ft_dprintf(fd, "%04lx -%*t %02hhx%s", i,
			to_print > 8 ? 8 : to_print, data + i, to_print >= 8 ? "-" : "" );
		if (to_print > 8)
			ret += ft_dprintf(fd, "%#*t%02hhx   ", to_print - 8, data + i + 8);
		ft_dprintf(2, "%.*s", LINE_SIZE - ret, g_space_buf);
		for (size_t j = 0; j < to_print; ++j)
			write(fd, ft_isprint(data[i + j]) ? (char *)data + i + j : ".", 1);
		write(fd, "\n", 1);
		i += HXDMP_SIZE;
	}
}
