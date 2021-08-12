/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rsa_key_headers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 17:04:48 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/12 18:55:15 by yforeau          ###   ########.fr       */
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

int	check_encryption_headers(t_rsa_key_64 *key, int fd, char **line,
	const char *cmd)
{
	int	ret;

	if (!(ret = get_next_line(fd, line) <= 0)
		&& ft_strlen(*line) == PROC_TYPE_LEN && !ft_strcmp(PROC_TYPE, *line))
	{
		ft_bzero((void *)&key->des, sizeof(t_des_ctx));
		key->is_enc = 1;
		key->des.reverse = 1;
		key->des.process_block = des_cbc;
		ft_memdel((void *)line);
		if ((ret = get_next_line(fd, line) <= 0)
			|| ft_strlen(*line) != DEK_INFO_LEN + IV_LEN
			|| ft_strncmp(DEK_INFO, *line, DEK_INFO_LEN)
			|| parse_hex_64(&key->des.iv, *line + DEK_INFO_LEN, cmd))
			ret = !!ft_dprintf(2,
				"ft_ssl: %s: invalid encryption headers\n", cmd);
		key->des.salt = key->des.iv;
		ft_memdel((void *)line);
	}
	return (ret);
}
