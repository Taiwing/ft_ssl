/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_check_key.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 20:13:56 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/13 12:06:07 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rsa.h"
#include "rsa_math.h"

int			rsa_check_key_size(t_rsa_key *key)
{
	if (key->size > RSA_KEY_SIZE_MAX || bintlog2(key->e) > RSA_KEY_SIZE_MAX)
		return (1);
	if (!key->is_pub && (bintlog2(key->d) > RSA_KEY_SIZE_MAX
		|| (bintlog2(key->p) + bintlog2(key->q)) > (RSA_KEY_SIZE_MAX + 1)))
		return (1);
	return (0);
}

static int	rsa_check_key_1(t_rsa_key *key)
{
	int			ret = 0;
	uint64_t	is_prime = 0;
	uint32_t	tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (!bintcmp(key->e, BINT_ONE) || bint_is_even(key->e))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"bad e value\n");
	if (bint_is_prime(key->p, 12, &is_prime) == BINT_FAILURE)
		return (-1);
	else if (is_prime)
		ret = !!ft_dprintf(2, RSA_KEY_ERR"p not prime\n");
	if (bint_is_prime(key->q, 12, &is_prime) == BINT_FAILURE)
		return (-1);
	else if (is_prime)
		ret = !!ft_dprintf(2, RSA_KEY_ERR"q not prime\n");
	if (bint_mult(tmp, key->p, key->q) == BINT_FAILURE)
		return (-1);
	else if (bintcmp(tmp, key->n))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"n does not equal p q\n");
	return (ret);
}

static int	rsa_check_key_2(t_rsa_key *key, t_bint p1,
	t_bint q1, t_bint totient)
{
	int			ret = 0;
	uint32_t	tmp1[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	tmp2[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	tmp3[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (bint_modinv(tmp1, tmp3, p1, q1) == BINT_FAILURE || (BINT_LEN(tmp3)
		&& bint_divide(tmp1, NULL, totient, tmp3) == BINT_FAILURE)
		|| bint_modmul(tmp2, key->d, key->e, tmp1) == BINT_FAILURE)
		return (-1);
	if (!BINT_LEN(tmp3) || !BINT_LEN(tmp1) || bintcmp(tmp2, BINT_ONE))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"d e not congruent to 1\n");
	if ((BINT_LEN(p1) && bint_divide(NULL, tmp1, key->d, p1) == BINT_FAILURE)
		|| (BINT_LEN(q1) && bint_divide(NULL, tmp2, key->d, q1) == BINT_FAILURE)
		|| bint_modinv(tmp3, NULL, key->q, key->p) == BINT_FAILURE)
		return (-1);
	if (BINT_LEN(p1) && bintcmp(key->exp1, tmp1))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"exp1 not congruent to d\n");
	if (BINT_LEN(q1) && bintcmp(key->exp2, tmp2))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"exp2 not congruent to d\n");
	if (bintcmp(key->coeff, tmp3))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"coeff not inverse of q\n");
	return (ret);
}

/*
** TODO: When/if bint_mult and modexp are optimized and fast, replace
** rsa_check_key_64 by rsa_check_key_bint and let rsa -check accept keys
** bigger than 64 bits.
**
** Until then this function cannot be used because the primality check
** can be way too long (that's also why genrsa cant go beyond 64 bit).
*/
int			rsa_check_key_bint(int outfd, t_rsa_key *key, const char *cmd)
{
	int			ret = 0;
	uint32_t	p1[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	q1[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	totient[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if ((ret = rsa_check_key_size(key)) > 0)
		return (!!ft_dprintf(2,
			"ft_ssl: %1$s: rsa key is too long (%2$u bit)\n"
			"ft_ssl: %1$s: ft_ssl handles keys up to %3$u bit long\n",
			cmd, key->size, RSA_KEY_SIZE_MAX));
	if ((ret = rsa_check_key_1(key)) < 0
		|| bint_sub(p1, key->p, BINT_ONE) == BINT_FAILURE
		|| bint_sub(q1, key->q, BINT_ONE) == BINT_FAILURE
		|| bint_mult(totient, p1, q1) == BINT_FAILURE
		|| (ret = rsa_check_key_2(key, p1, q1, totient) || ret) < 0)
		return (!!ft_dprintf(2, "ft_ssl: %s: error while checking key\n", cmd));
	return (!ret ? !ft_dprintf(outfd, "RSA key ok\n") : ret);
}

int			rsa_check_key_64(int outfd, t_rsa_key_64 *key)
{
	int			ret;
	uint64_t	gcd;
	uint128_t	totient;

	ret = 0;
	totient = (key->p - 1) * (key->q - 1);
	if (key->e == 1 || (key->e % 2) == 0)
		ret = !!ft_dprintf(2, RSA_KEY_ERR"bad e value\n");
	if (is_prime(key->p, K_MAX))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"p not prime\n");
	if (is_prime(key->q, K_MAX))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"q not prime\n");
	if (((!key->p || !key->q) && key->n)
		|| (key->p && key->q && key->n / key->p != key->q))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"n does not equal p q\n");
	modinv((int128_t)key->p - 1, (int128_t)key->q - 1, &gcd);
	if (!gcd || !(totient/gcd) || modmul(key->d, key->e, totient/gcd) != 1)
		ret = !!ft_dprintf(2, RSA_KEY_ERR"d e not congruent to 1\n");
	if (key->p != 1 && key->exp1 != (key->d % (key->p - 1)))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"exp1 not congruent to d\n");
	if (key->q != 1 && key->exp2 != (key->d % (key->q - 1)))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"exp2 not congruent to d\n");
	if (key->coeff != modinv((int128_t)key->q, (int128_t)key->p, &gcd))
		ret = !!ft_dprintf(2, RSA_KEY_ERR"coeff not inverse of q\n");
	return (!ret ? !ft_dprintf(outfd, "RSA key ok\n") : ret);
}
