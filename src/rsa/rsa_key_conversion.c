/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_key_conversion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 11:48:11 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/20 11:50:54 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rsa.h"
#include "libft.h"

int	rsa_key_64_to_bint(t_rsa_key *dst, t_rsa_key_64 *src)
{
	if (bintset_u64(dst->n, src->n) == BINT_FAILURE
		|| bintset_u64(dst->e, src->e) == BINT_FAILURE
		|| bintset_u64(dst->d, src->d) == BINT_FAILURE
		|| bintset_u64(dst->p, src->p) == BINT_FAILURE
		|| bintset_u64(dst->q, src->q) == BINT_FAILURE
		|| bintset_u64(dst->exp1, src->exp1) == BINT_FAILURE
		|| bintset_u64(dst->exp2, src->exp2) == BINT_FAILURE
		|| bintset_u64(dst->coeff, src->coeff) == BINT_FAILURE)
		return (1);
	dst->is_pub = src->is_pub;
	dst->is_enc = src->is_enc;
	dst->size = bintlog2(dst->n);
	ft_memcpy((void *)&dst->des, (void *)&src->des, sizeof(t_des_ctx));
	return (0);
}

int	rsa_key_bint_to_64(t_rsa_key_64 *key64, t_rsa_key *key)
{
	ft_bzero((void *)key64, sizeof(t_rsa_key_64));
	if (bint_to_u64(&key64->n, key->n) == BINT_FAILURE
		|| bint_to_u64(&key64->e, key->e) == BINT_FAILURE)
		return (1);
	if (!key->is_pub && (bint_to_u64(&key64->d, key->d) == BINT_FAILURE
		|| bint_to_u64(&key64->p, key->p) == BINT_FAILURE
		|| bint_to_u64(&key64->q, key->q) == BINT_FAILURE
		|| bint_to_u64(&key64->exp1, key->exp1) == BINT_FAILURE
		|| bint_to_u64(&key64->exp2, key->exp2) == BINT_FAILURE
		|| bint_to_u64(&key64->coeff, key->coeff) == BINT_FAILURE))
		return (1);
	key64->is_pub = key->is_pub;
	key64->is_enc = key->is_enc;
	key64->size = key->size;
	ft_memcpy((void *)&key64->des, (void *)&key->des, sizeof(t_des_ctx));
	return (0);
}
