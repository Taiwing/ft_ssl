/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_const.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 19:34:07 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/02 20:07:35 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DES_CONST_H
# define DES_CONST_H

# include <stddef.h>

# define DES_CONST_MAX_SIZE	64
# define SBOX_NB			8
# define SBOX_ROW			4
# define SBOX_COL			16

typedef struct	s_des_const
{
	size_t		size;
	int			bit[DES_CONST_MAX_SIZE];
}				t_des_const;

extern const t_des_const	g_ip;
extern const t_des_const	g_reverse_ip;
extern const t_des_const	g_e;
extern const t_des_const	g_p;
extern const t_des_const	g_left_pc1;
extern const t_des_const	g_right_pc1;
extern const t_des_const	g_pc2;
extern const int			g_sbox[SBOX_NB][SBOX_ROW][SBOX_COL];

#endif
