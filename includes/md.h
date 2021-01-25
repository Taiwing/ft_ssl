/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 14:41:21 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/25 17:53:53 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MD_H
# define MD_H

# include <stdint.h>
# include <stddef.h>

# define MD_BUF_SIZE	64
# define REGS_MAX_SIZE	8

/*
 * s_md_ctx: context structure for message digest commands
 *
 * name: copy of the command string
 * buf: raw character buffer (from read function or string argument)
 * block: cast of buf as a uint32_t* for processing
 * regs: registers, where the hash is generated
 * regs_start_state: used for the init_registers function
 * regs_size: number of 32-bit registers
 * process_block: function executing the hashing algorithm
*/

typedef struct	s_md_ctx
{
	const char	*name;
	char		buf[MD_BUF_SIZE];
	uint32_t	*block;
	uint32_t	regs[REGS_MAX_SIZE];
	uint32_t	*regs_start_state;
	size_t		regs_size;
	void		(*process_block)(uint32_t *, uint32_t *);
}				t_md_ctx;

/*
 * t_md_cmd: command structure for setting the context
 *
 * name: command string
 * process_block: function executing the hashing algorithm
 * regs_start_state: stores initial registers state
*/

typedef struct	s_md_cmd
{
	const char	*name;
	void		(*process_block)(uint32_t *, uint32_t *);
	size_t		regs_size;
	uint32_t	regs_start_state[REGS_MAX_SIZE];
}				t_md_cmd;

/*
 * cmd_md functions (called whatever the algorithm is
*/

void	init_registers(t_md_ctx *ctx);
void	exec_md(t_md_ctx *ctx);
void	add_md_padding(t_md_ctx *ctx, int rd, uint64_t size);

/*
 * algorithm specific functions (process_block)
*/

void	md5(uint32_t regs[REGS_MAX_SIZE], uint32_t *block);
void	sha256(uint32_t regs[REGS_MAX_SIZE], uint32_t *block);

#endif
