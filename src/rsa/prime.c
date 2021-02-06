/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prime.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 17:42:56 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/06 17:45:08 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <fcntl.h>
#include "libft.h"

#define BUF_SIZE	4096
#define	BUF_BYTES	(BUF_SIZE * sizeof(uint64_t))
#define uint128_t	__uint128_t
#define K_MAX		100

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

uint64_t		modexp(uint64_t raw_a, uint64_t raw_b, uint64_t raw_c)
{
	uint128_t	res;
	uint128_t	a;
	uint128_t	b;
	uint128_t	c;

	a = raw_a;
	b = raw_b;
	c = raw_c;
	a %= c;
	res = 1;
	while (b)
	{
		if (b & 0x01)
			res = (res * a) % c;
		a = (a * a) % c;
		b = b >> 1;
	}
	return ((uint64_t)res);
}

static int		miller_test(uint64_t n, uint64_t d)
{
	uint64_t	a;
	uint64_t	x;

	if (!get_rand(&a, 2, n-2))
		return (-1);
	x = modexp(a, d, n);
	if (x == 1 || x == n-1)
		return (0);
	while (d != n-1)
	{
		x = modexp(x, 2, n);
		if (x == 1)
			return (1);
		else if (x == n-1)
			return (0);
		d = d << 1;
	}
	return (1);
}

/*
** is_prime: test if 64-bit unsinged integer is probably prime (Miller-Rabin)
**
** n: integer to test
** k: number of time to repeat the test (the bigger, the more probable
**    it is that n is prime, with a maximum of 100)
**
** return: k + 1 if n does not pass initial tests, k otherwise, if k == 0
**  it means that n passed k tests successfully and is considered prime
**  according to the Miller-Rabin algorithm. If k > 0 then n is not prime
**  (also the number of passed tests can be obtained by substracting actual
**  k to original k, ex: if orig_k == 12 and k == 3 then n passed
**  orig_k - k = 12 - 3 = 9 tests successfully before failing)
*/

int				is_prime(uint64_t n, int k)
{
	uint64_t	d;
	int			ret;

	k = k > K_MAX ? K_MAX : k;
	if (n < 4)
		return (n < 2 ? k + 1 : 0);
	else if (!(n & 0x01))
		return (k + 1);
	d = n - 1;
	while (!(d & 0x01))
		d = d >> 1;
	ret = 0;
	while (k)
	{
		if ((ret = miller_test(n, d)))
			break ;
		--k;
	}
	return (ret < 0 ? ret : k);
}

uint64_t		find_prime(int k)
{
	uint64_t	n;
	int			ret;

	if (!get_rand(&n, 0, UINT64_MAX))
		return (0);
	if (!(n & 0x01))
		--n;
	ret = 1;
	while (n > 2)
	{
		if ((ret = is_prime(n, k)) < 0)
			break ;
		else if (ret <= k)
			ft_printf(".%.*s", (k - ret) + !ret, "++++++++++++\n");
		if (!ret)
			break ;
		n -= 2;
	}
	return (ret ? 0 : n);
}
