/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 01:44:21 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/12 17:50:10 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base64.h"
#include "libft.h"

char *g_base64_str =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

/*
** The output buffer must be larger than the input by at least a third.
**
** More precisely, there must be 4 bytes available for every group of 3
** input bytes, plus 4 for the rest if any:
** sizeof(output) >= (4 * sizeof(input) / 3) + (sizeof(input) % 3 ? 4 : 0)
*/
int	base64_encrypt(char *out, unsigned char *in, int inlen)
{
	int				i;
	int				j;
	int				c;

	i = 0;
	j = 0;
	while (i < inlen)
	{
		c = in[i] >> 2;
		out[j++] = g_base64_str[c];
		c = (in[i] & 0x03) << 4;
		if (++i < inlen)
			c |= in[i] >> 4;
		out[j++] = g_base64_str[c];
		c = i < inlen ? (in[i] & 0x0f) << 2 : 64;
		if (++i < inlen)
			c |= (in[i] >> 6);
		out[j++] = g_base64_str[c];
		c = i < inlen ? (in[i++] & 0x3f) : 64;
		out[j++] = g_base64_str[c];
	}
	return (j);
}

static unsigned char	digit_op(unsigned char *out, int cur_digit,
	unsigned int c, unsigned char byte)
{
	if (cur_digit == 0)
		byte = c << 2;
	else if (cur_digit == 1)
	{
		byte |= (c >> 4) & 0x03;
		*out = byte;
		byte = (c & 0x0f) << 4;
	}
	else if (cur_digit == 2)
	{
		byte |= c >> 2;
		*out = byte;
		byte = (c & 0x03) << 6;
	}
	else if (cur_digit == 3)
	{
		byte |= c & 0x3f;
		*out = byte;
	}
	return (byte);
}

/* The output buffer can be smaller than the input by at most a quarter */
int	base64_decrypt(unsigned char *out, char *in, int inlen, int flush)
{
	unsigned int		c;
	int					j;
	unsigned char		byte;
	static int			cur_digit = -1;
	static unsigned int	part_byte = 0xf00;

	byte = part_byte < 0xf00 ? part_byte : 0;
	part_byte = 0xf00;
	c = 0;
	j = 0;
	for (int i = 0; i < inlen; ++i)
	{
		cur_digit = ((cur_digit + 1) & 0x03);
		c = ft_strchr(g_base64_str, in[i]) - g_base64_str;
		byte = digit_op(out + j, cur_digit, c, byte);
		if (cur_digit)
			++j;
	}
	if (flush)
		cur_digit = -1;
	if (cur_digit > -1 && cur_digit < 3)
		part_byte = byte;
	return (j);
}
