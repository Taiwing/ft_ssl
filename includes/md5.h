/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 14:41:21 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/21 15:59:30 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MD5_H
# define MD5_H

# include <stdint.h>

# define MD5_BUF_SIZE 64

void	init_regs(uint32_t regs[4]);
void	add_md5_padding(uint32_t regs[4], char buf[MD5_BUF_SIZE],
	int rd, uint64_t size);
void	exec_md5(uint32_t regs[4], uint32_t *buf);
void	md5(uint32_t regs[4], uint32_t *buf);

#endif
