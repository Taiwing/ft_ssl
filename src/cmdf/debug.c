/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 15:19:24 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 20:14:10 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "des.h"

#define LINE_LEN 8

int	print_bytes(unsigned char *buf, int size)
{
	int	ret;
	int	tmp;

	ret = 0;
	while (size > 0)
	{
		if ((tmp = ft_printf("%*t %02hhx\n", size > LINE_LEN
			? LINE_LEN : size, buf)))
			return (-1);
		ret += tmp;
		buf += size > LINE_LEN ? LINE_LEN : size;
		size -= size > LINE_LEN ? LINE_LEN : size;
	}
	return (ret);
}

int	print_hex(uint64_t n)
{
	return (ft_printf("%016lx\n", n));
}

# define BIT(x, n) (((x) >> (n)) & 0x01)

int	print_bits(uint64_t n)
{
	int		ret;
	char	byte[9];

	ret = 0;
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
			byte[j] = BIT(n, ((8-i) * 8) - (j+1)) + 48;
		byte[8] = 0;
		ret += ft_printf("%s%c", byte, i < 7 ? ' ' : '\n');
	}
	return (ret);
}

int	print_des_ctx(t_des_ctx *ctx)
{
	ft_printf("salt = %016lx\n", ctx->salt);
	ft_printf("key  = %016lx\n", ctx->key);
	if (ctx->process_block == des_cbc)
		ft_printf("iv   = %016lx\n", ctx->iv);
	return (0);
}
