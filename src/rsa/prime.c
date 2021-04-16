/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prime.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 17:42:56 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/16 09:51:26 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "libft.h"
#include "rsa_math.h"

const char		g_plus[K_MAX] = { [ 0 ... K_MAX - 1 ] = '+' };

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

int				is_prime(uint64_t n, unsigned int k)
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
	return (ret < 0 ? ret : (int)k);
}

uint64_t		find_prime(unsigned int k, size_t size)
{
	uint64_t	n;
	int			ret;
	uint64_t	max;

	max = UINT64_MAX;
	k = k > K_MAX ? K_MAX : k;
	size = !size || size > 64 ? 64 : size;
	max = max >> (64 - size);
	if (!get_rand(&n, 0, max))
		return (0);
	n |= (0x01UL << (size - 1)) | 0x01UL;
	ret = 1;
	while (n > 2)
	{
		if ((ret = is_prime(n, k)) < 0)
			break ;
		else if (ret <= (int)k)
			ft_dprintf(2, ".%.*s%s", ((int)k - ret), g_plus, !ret ? "\n" : "");
		if (!ret)
			break ;
		n -= 2;
	}
	return (ret ? 0 : n);
}
