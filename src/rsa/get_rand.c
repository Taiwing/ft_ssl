/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_rand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 14:59:09 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/10 15:05:18 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE	4096
#define	BUF_BYTES	(BUF_SIZE * sizeof(uint64_t))

static char	*flush_fd(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
	return (NULL);
}

/*
** fill_rand_buf: fill get_rand buffer with random data
**
** src: source file, "/dev/urandom" if NULL
** flush: close opened fd if any (boolean 1 == true)
**
** always call fill_rand_buf with flush == 1 one or multiple calls
** to get_rand if fill_rand_buf has been called outside of get_rand
** to specify a custom source file for randomness
*/

char		*fill_rand_buf(const char *src, int flush)
{
	int			rd;
	static int	fd = -1;
	static char	buf[BUF_BYTES];
	
	if (flush)
		return (flush_fd(&fd));
	rd = 0;
	if (fd == -1 && src && (fd = open(src, O_RDONLY)) < 0)
		return (flush_fd(&fd));
	else if (fd >= 0 && (rd = read(fd, (char *)buf, BUF_BYTES)) < 0)
		return (flush_fd(&fd));
	else if (rd < (int)BUF_BYTES)
	{
		flush_fd(&fd);
		if ((fd = open("/dev/urandom", O_RDONLY)) < 0)
			return (NULL);
		rd += read(fd, (char *)buf + rd, BUF_BYTES - rd);
		flush_fd(&fd);
	}
	return (rd == (int)BUF_BYTES ? (char *)buf : NULL);
}

/*
** get_rand: get random 64-bit unsinged integer
**
** n: uint64_t destination pointer
** min: lowest possible value (inclusive)
** max: highest possible value (inclusive)
**
** return: store the value in the given n pointer
** or NULL on error or on bad input
*/

uint64_t		*get_rand(uint64_t *n, uint64_t min, uint64_t max)
{
	uint64_t		dist;
	static uint64_t	*buf = NULL;
	static int		i = BUF_SIZE;

	if (!n || max < min)
		return (NULL);
	else if (!(dist = max - min))
	{
		*n = min;
		return (n);
	}
	if (i == BUF_SIZE)
	{
		if (!(buf = (uint64_t *)fill_rand_buf(NULL, 0)))
			return (NULL);
		i = 0;
	}
	*n = buf[i++];
	if (dist < UINT64_MAX)
		*n %= dist + 1;
	*n += min;
	return (n);
}
