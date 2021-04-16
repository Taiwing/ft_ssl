/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_math.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 14:55:06 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/16 09:49:22 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RSA_MATH_H 
# define RSA_MATH_H

# include <stdint.h>
# include <stddef.h>
# define uint128_t	__uint128_t
# define int128_t	__int128_t

# define K_MAX		100

char		*fill_rand_buf(const char *src, int flush);
uint64_t	*get_rand(uint64_t *n, uint64_t min, uint64_t max);
uint64_t	modexp(uint128_t a, uint128_t b, uint128_t c);
uint64_t	modinv(int128_t newr, int128_t n, uint64_t *gcd);
uint64_t	modmul(uint128_t a, uint128_t b, uint128_t c);
int			is_prime(uint64_t n, unsigned int k);
uint64_t	find_prime(unsigned int k, size_t size);

#endif
