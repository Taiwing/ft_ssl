/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:39:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/10 16:26:24 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RSA_H
# define RSA_H

# define E_VALUE	0x00010001

/*
** s_rsa_key: rsa key structure (represent private and public key)
**
** n: p * q (modulus)
** e: encryption, 1 < e < T(n) and gcd(e, T(n)) == 1 (public exponent)
** d: decryption, modinv(e, T(n))
** p: random 32bit unsigned prime (p != q)
** q: random 32bit unsigned prime (p != q)
** exp1: d mod (p -1)
** exp2: d mod (q -1)
** coeff: modinv(q, p)
*/

typedef struct	s_rsa_key
{
	uint64_t	n;
	uint64_t	e;
	uint64_t	d;
	uint64_t	p;
	uint64_t	q;
	uint64_t	exp1;
	uint64_t	exp2;
	uint64_t	coeff;
}				t_rsa_key;

int	rsa_keygen(t_rsa_key *key);

#endif
