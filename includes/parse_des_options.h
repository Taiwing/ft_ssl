/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_des_options.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 11:11:05 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/29 11:13:14 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_DES_OPTIONS_H
# define PARSE_DES_OPTIONS_H

# include "commands.h"
# include "des.h"

int	parse_des_options(t_des_ctx *ctx, const t_command *cmd, t_cmdopt *opt);

#endif
