/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prime.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 17:43:24 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/10 14:44:01 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIME_H
# define PRIME_H

# include <stdint.h>
# define uint128_t	__uint128_t

char		*fill_rand_buf(const char *src, int flush);
uint64_t	*get_rand(uint64_t *n, uint64_t min, uint64_t max);
uint64_t	modexp(uint64_t raw_a, uint64_t raw_b, uint64_t raw_c);
int			is_prime(uint64_t n, int k);
uint64_t	find_prime(int k, size_t size);

#endif
