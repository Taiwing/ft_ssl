/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 11:23:37 by yforeau           #+#    #+#             */
/*   Updated: 2022/04/12 18:12:16 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DES_H
# define DES_H

# include <stddef.h>
# include <stdint.h>

# define DES_CONST_MAX_SIZE	64
# define DES_ROUNDS			16
# define SBOX_NB			8
# define SBOX_ROW			4
# define SBOX_COL			16

/*
** s_des_ctx: DES context
**
** plaintext: raw 64-bit input block
** last_plaintext: last input block (for pcbc)
** salt: not directly useful in des but needed in context
** iv: initialization vector (for cbc)
** key: generated or given key of 64-bit (56 without parity bits)
** subkeys: subkeys for each round (generated from key)
** cyphertext: raw 64-bit output block
** process_block: block cypher encryption mode (ecb, cbc, etc...)
** reverse: boolean indicating if executing in decryption mode
** first_block: boolean set to true if is the first block (for pcbc)
*/

typedef struct	s_des_ctx
{
	uint64_t	plaintext;
	uint64_t	last_plaintext;
	uint64_t	salt;
	uint64_t	iv;
	uint64_t	key;
	uint64_t	subkey[DES_ROUNDS];
	uint64_t	cyphertext;
	uint64_t	(*process_block)(struct s_des_ctx *ctx);
	int			reverse;
	int			first_block;
}				t_des_ctx;

/*
** s_des_cmd: map command string to process_block function
**
** name: command name
** process_block: block cypher encryption mode (ecb, cbc, etc...)
*/

typedef struct	s_des_cmd
{
	const char	*name;
	uint64_t	(*process_block)(struct s_des_ctx *ctx);
}				t_des_cmd;

/*
** s_des_const: des constants for permutation function
**
** input_size: size of the input integer
** output_size: size of the output integer
** bit: map of bits to move
*/

typedef struct	s_des_const
{
	size_t		input_size;
	size_t		output_size;
	int			bit[DES_CONST_MAX_SIZE];
}				t_des_const;

/*
** list of des constants
*/

extern const t_des_const	g_ip;
extern const t_des_const	g_reverse_ip;
extern const t_des_const	g_e;
extern const t_des_const	g_p;
extern const t_des_const	g_left_pc1;
extern const t_des_const	g_right_pc1;
extern const t_des_const	g_pc2;
extern const uint64_t		g_sbox[SBOX_NB][SBOX_ROW][SBOX_COL];

/*
** des_cem (cypher encryption mode) functions
*/

uint64_t		des_ecb(struct s_des_ctx *ctx);
uint64_t		des_cbc(struct s_des_ctx *ctx);
uint64_t		des_pcbc(struct s_des_ctx *ctx);

/*
** des functions
*/

void			des_keygen(t_des_ctx *ctx);
uint64_t		des_sbox(uint64_t x);
uint64_t		des_permute(uint64_t x, const t_des_const *p);
uint64_t		des_f(uint64_t x, uint64_t key);
uint64_t		des(t_des_ctx *ctx);

/*
** s_des_getkey: is used for rsa_des_getkey
**
** inkey: name of the key file if any
** pass: pass argument if any
** arg: argument for printf fmt string
** prompt: printf format string for prompt
** verify: boolean indicating if there is a double prompt
**
** note: prompt cant be NULL (use "" instead)
*/

typedef struct	s_des_getkey
{
	const char	*inkey;
	const char	*pass;
	const char	*arg;
	const char	*prompt;
	int			verify;
}				t_des_getkey;

#endif
