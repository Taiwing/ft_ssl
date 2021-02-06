/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prime.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 17:43:24 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/06 17:46:16 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define PRIME_H
# ifndef PRIME_H

# include <stdint.h>

char		*fill_rand_buf(const char *src, int flush);
uint64_t	*get_rand(uint64_t *n, uint64_t min, uint64_t max);
uint64_t	modexp(uint64_t raw_a, uint64_t raw_b, uint64_t raw_c);
int			is_prime(uint64_t n, int k);
uint64_t	find_prime(int k);

#endif
