/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 15:28:07 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/03 19:51:54 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include <stddef.h>

int	print_bytes(unsigned char *buf, int size);
int	print_hex(uint64_t n);
int	print_bits(uint64_t n);

#endif
