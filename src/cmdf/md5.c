/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 14:39:52 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/22 17:39:11 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.h"
#include "libft.h"

/* DEBUG */

void print_block(uint32_t *block)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)block;
	for (int i = 0; i < 64; ++i)
	{
		ft_printf("%02x%c", ptr[i], !((i + 1) % 4) ? ' ' : 0);
		if (!((i + 1) % 32))
			ft_printf("\n");
	}
}

/* DEBUG */

/*
void	decode_block(uint32_t *block)
{
	for (int i = 0; i < 16; ++i)
		block[i] = (block[i] & 0xff000000) >> 24
			| (block[i] & 0x00ff0000) >> 8
			| (block[i] & 0x0000ff00) << 8
			| (block[i] & 0x000000ff) << 24;
}
*/

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

	/* DEBUG */
	ft_printf("SIZE: %08x (%u)\n", size, size);
	/* DEBUG */

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

	//decode_block(buf);
	print_block(buf);
	ft_memcpy((void *)tmp_regs, (void *)regs, sizeof(uint32_t) * 4);
	md5(tmp_regs, buf);
	for (int i = 0; i < 4; ++i)
		regs[i] += tmp_regs[i];
}
