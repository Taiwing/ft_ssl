/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modular_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 14:57:21 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/10 15:38:01 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rsa_math.h"

/*
** modexp and modinv only work with values up to UINT64_MAX as input
*/

uint64_t		modexp(uint128_t a, uint128_t b, uint128_t c)
{
	uint128_t	res;

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

uint64_t		modinv(int128_t newr, int128_t n, uint64_t *gcd)
{
	int128_t	q;
	int128_t	t;
	int128_t	r;
	int128_t	tmp;
	int128_t	newt;

	t = 0;
	r = n;
	newt = 1;
	while (newr != 0)
	{
		q = r / newr;
		tmp = newt;
		newt = t - (q * newt);
		t = tmp;
		tmp = newr;
		newr = r - (q * newr);
		r = tmp;
	}
	*gcd = (uint64_t)r;
	t = t < 0 ? t + n : t;
	return ((uint64_t)t);
}
