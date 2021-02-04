/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_des_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 11:11:05 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/04 16:14:32 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_DES_UTILS_H
# define CMD_DES_UTILS_H

# include "commands.h"
# include "des.h"

int	parse_hex(uint64_t *value, const char *str, const char *cmd);
int	get_salty(t_des_ctx *ctx, const t_command *cmd, t_cmdopt *opt);
int	write_salt(int fd, t_des_ctx *ctx, t_cmdopt *opt);
int	parse_des_options(t_des_ctx *ctx, const t_command *cmd, t_cmdopt *opt);
int	des_readfile(const char *file_name, char *buf,
	size_t bufsize, t_cmdopt *opt);
int	des_writefile(int fd, char *buf, size_t n, t_cmdopt *opt);

#endif
