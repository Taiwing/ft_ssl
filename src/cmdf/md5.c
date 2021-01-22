/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 14:39:52 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/22 19:33:35 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.h"
#include "libft.h"

/* DEBUG */
/*
static void print_block(unsigned char *block)
{
	for (int i = 0; i < 64; ++i)
	{
		ft_printf("%02x%s", block[i], !((i + 1) % 4) ? " " : "");
		if (!((i + 1) % 32))
			ft_printf("\n");
	}
}
*/
/* DEBUG */

void	init_regs(uint32_t regs[4])
{
	regs[0] = 0x67452301;
	regs[1] = 0xEFCDAB89;
	regs[2] = 0x98BADCFE;
	regs[3] = 0x10325476;
}

void	add_md5_padding(uint32_t regs[4], char buf[MD5_BUF_SIZE],
	int rd, uint64_t size)
{
	size += rd * 8;
	ft_bzero((void *)buf + rd, MD5_BUF_SIZE - rd);
	buf[rd++] = 0x80;
	if ((size_t)(MD5_BUF_SIZE - rd) < sizeof(size))
	{
		exec_md5(regs, (uint32_t *)buf);
		ft_bzero((void *)buf, MD5_BUF_SIZE);
	}
	ft_memcpy((void *)buf + MD5_BUF_SIZE - sizeof(size),
		(void *)&size, sizeof(size));
	exec_md5(regs, (uint32_t *)buf);
}

void	exec_md5(uint32_t regs[4], uint32_t *buf)
{
	uint32_t	tmp_regs[4];

	//print_block((unsigned char *)buf);
	ft_memcpy((void *)tmp_regs, (void *)regs, sizeof(uint32_t) * 4);
	md5(tmp_regs, buf);
	for (int i = 0; i < 4; ++i)
		regs[i] += tmp_regs[i];
}
