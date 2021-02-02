/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 11:23:37 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/02 21:50:17 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DES_H
# define DES_H

# include <stdint.h>

# define DES_ROUNDS	16

/*
** s_des_ctx: DES context
**
** plaintext: raw 64-bit input block
** salt: not directly useful in des but needed in context
** iv: initialization vector (for cbc)
** key: generated or given key of 64-bit (56 without parity bits)
** subkeys: subkeys for each round (generated from key)
** cyphertext: raw 64-bit output block
** process_block: block cypher encryption mode (ecb, cbc, etc...)
*/

typedef struct	s_des_ctx
{
	uint64_t	plaintext;
	uint64_t	salt;
	uint64_t	iv;
	uint64_t	key;
	uint64_t	subkey[DES_ROUNDS];
	uint64_t	cyphertext;
	uint64_t	(*process_block)(struct s_des_ctx *ctx);
}				t_des_ctx;

/*
** s_des_cmd: map command string to process_block function
** name: command name
** process_block: block cypher encryption mode (ecb, cbc, etc...)
*/

typedef struct	s_des_cmd
{
	const char	*name;
	uint64_t	(*process_block)(struct s_des_ctx *ctx);
}				t_des_cmd;

/*
** des_cem (cypher encryption mode) functions
*/

uint64_t		des_ecb(struct s_des_ctx *ctx);
uint64_t		des_cbc(struct s_des_ctx *ctx);

void			des_keygen(t_des_ctx *ctx);

#endif
