/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_option.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 04:00:07 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/03 05:34:34 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <errno.h>
#include "libft.h"

int	output_option(const char *file_name, const char *cmd)
{
	int	fd;

	fd = -1;
	if ((fd = open(file_name, O_WRONLY | O_CREAT, 0664)) == -1)
		ft_dprintf(2, "ft_ssl: %s: %s: %s\n", cmd, file_name, strerror(errno));
	return (fd);
}
