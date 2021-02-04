/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_readfile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 14:44:21 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/04 14:55:17 by yforeau          ###   ########.fr       */
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

static int		flush_base64_readfile(int *rd, t_buf *b)
{
	if (rd)
		*rd = BASE64_OUTBUF_SIZE;
	b->len = 0;
	ft_bzero((void *)b->buf, b->size);
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
		ft_memcpy((void *)dst->buf + dst->len, (void *)src->buf, to_copy);
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

static int		encrypt_and_write(int fd, t_buf *in)
{
	int		len;
	char	out[BASE64_OUTBUF_SIZE];

	len = base64_encrypt(out, (unsigned char *)in->buf, in->len);
	in->len = 0;
	return (write(fd, out, len));
}

int				base64_writefile(int fd, char *buf, size_t n, int flush)
{
	t_buf			in;
	int				wr;
	static char		rawbytes_buf[BASE64_INBUF_SIZE];
	static t_buf	rawbytes = { rawbytes_buf, BASE64_INBUF_SIZE, 0 };

	wr = 0;
	in.buf = buf;
	in.size = n;
	in.len = n;
	while (in.len && wr >= 0)
	{
		if (!fill_buf(&rawbytes, &in))
			wr = encrypt_and_write(fd, &rawbytes);
	}
	if (flush)
	{
		wr = encrypt_and_write(fd, &rawbytes);
		return (flush_base64_readfile(NULL, &rawbytes));
	}
	return (wr);
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
