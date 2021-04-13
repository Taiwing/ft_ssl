/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_des_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 11:11:05 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/13 18:45:59 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_DES_UTILS_H
# define CMD_DES_UTILS_H

# include "commands.h"
# include "des.h"

int	parse_hex(uint64_t *value, const char *str, const char *cmd);
int	get_salty(t_des_ctx *ctx, const t_command *cmd, t_cmdopt *opt);
int	write_salt(int fd, t_des_ctx *ctx, t_cmdopt *opt);
int	pbkdf(t_des_ctx *des_ctx, const char *pass,
	int iv_is_set, const char *md_name);
int	parse_des_options(t_des_ctx *ctx, const t_command *cmd, t_cmdopt *opt);
int	des_readfile(const char *file_name, char *buf,
	size_t bufsize, t_cmdopt *opt);
int	des_writefile(int fd, char *buf, size_t n, t_cmdopt *opt);
uint64_t	exec_cypher(t_des_ctx *ctx);
int	des_decrypt(int outfd, t_des_ctx *ctx, t_cmdopt *opt, const char *cmd);
int	des_encrypt(int outfd, t_des_ctx *ctx, t_cmdopt *opt, const char *cmd);
int	read_des_password(char *value, const char *cmd, t_des_getkey *gk);

#endif
