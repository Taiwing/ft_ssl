/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 15:28:07 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/04 16:08:26 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include <stddef.h>
# include "des.h"

int	print_bytes(unsigned char *buf, int size);
int	print_hex(uint64_t n);
int	print_bits(uint64_t n);
int	print_des_ctx(t_des_ctx *ctx);

#endif
