/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_readfile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 14:44:21 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/02 18:54:27 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readfile.h"
#include "base64.h"
#include "libft.h"

/*
**
** t_buf: local data buffer
**
** buf: data
** size: size of the buffer
** len: length of data inside the buffer
*/

typedef struct	s_buf
{
	char		*buf;
	size_t		size;
	size_t		len;
}				t_buf;

static int		flush_base64_readfile(int *rd, t_buf *rawbytes)
{
	*rd = BASE64_OUTBUF_SIZE;
	rawbytes->len = 0;
	ft_bzero((void *)rawbytes->buf, rawbytes->size);
	base64_decrypt(NULL, NULL, 0, 1);
	return (0);
}

static size_t	fill_buf(t_buf *dst, t_buf *src)
{
	size_t	to_copy;

	if (dst->size > dst->len && src->len)
	{
		to_copy = dst->size - dst->len > src->len ? src->len
			: dst->size - dst->len;
		ft_memcpy((void *)dst->buf, (void *)src->buf, to_copy);
		dst->len += to_copy;
		ft_memmove((void *)src->buf, (void *)src->buf + to_copy,
			src->len - to_copy);
		src->len -= to_copy;
	}
	return (dst->size - dst->len);
}

static int		read_and_decrypt(t_buf *out, const char *file_name)
{
	int		rd;
	int		valid;
	char	base64_buf[BASE64_OUTBUF_SIZE];

	if ((rd = readfile(file_name, (char *)base64_buf, BASE64_OUTBUF_SIZE)) > 0)
	{
		valid = base64_buffer_parse(base64_buf, rd);
		out->len += base64_decrypt((unsigned char *)out->buf,
			base64_buf, valid, 0);
	}
	return (rd);
}

int				base64_readfile(const char *file_name, char *buf,
	size_t bufsize, int flush)
{
	t_buf			out;
	static int		rd = BASE64_OUTBUF_SIZE;
	static char		rawbytes_buf[BASE64_INBUF_SIZE];
	static t_buf	rawbytes = { rawbytes_buf, BASE64_INBUF_SIZE, 0 };

	if (flush)
		return (flush_base64_readfile(&rd, &rawbytes));
	out.buf = buf;
	out.size = bufsize;
	out.len = 0;
	while (fill_buf(&out, &rawbytes) && rd == BASE64_OUTBUF_SIZE)
		rd = read_and_decrypt(&rawbytes, file_name);
	return (rd < 0 ? rd : (int)out.len);
}
