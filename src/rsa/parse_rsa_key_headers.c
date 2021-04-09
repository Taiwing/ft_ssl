/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rsa_key_headers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 17:04:48 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/09 17:08:14 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rsa.h"
#include "cmd_des_utils.h"

int	check_header(int fd, int is_pub)
{
	int			ret;
	int			len;
	char		*line;
	char		*header;

	ret = 0;
	line = NULL;
	header = is_pub ? BEGIN_PUB : BEGIN_PRIV;
	len = is_pub ? BEGIN_PUB_LEN : BEGIN_PRIV_LEN;
	while (!ret && (ret = get_next_line(fd, &line)) > 0)
	{
		ret = ft_strlen(line);
		if (ret != len || ft_strcmp(header, line))
			ret = 0;
		ft_memdel((void *)&line);
	}
	return (ret < 0 ? -1 : ret != len);
}

int	check_encryption_headers(t_rsa_key *key, int fd, char **line,
	const char *cmd)
{
	int	ret;

	if (!(ret = get_next_line(fd, line) <= 0)
		&& ft_strlen(*line) == PROC_TYPE_LEN && !ft_strcmp(PROC_TYPE, *line))
	{
		key->is_enc = 1;
		ft_memdel((void *)line);
		if ((ret = get_next_line(fd, line) <= 0)
			|| ft_strlen(*line) != DEK_INFO_LEN + IV_LEN
			|| ft_strncmp(DEK_INFO, *line, DEK_INFO_LEN)
			|| parse_hex(&key->iv, *line + DEK_INFO_LEN, cmd))
			ret = !!ft_dprintf(2,
				"ft_ssl: %s: invalid encryption headers\n", cmd);
		ft_memdel((void *)line);
	}
	return (ret);
}
