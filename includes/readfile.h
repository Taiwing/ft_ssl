/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfile.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 12:16:20 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/23 12:08:16 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READFILE_H
# define READFILE_H

# include <fcntl.h>
# include <stddef.h>

void print_readfile_error(const char *cmd, const char *file_name);
int	readfile(const char *file_name, char *buf, size_t bufsize);

#endif
