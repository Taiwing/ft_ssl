/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rsa_key.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 17:36:51 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/09 13:32:50 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <errno.h>
#include "base64.h"
#include "libft.h"
#include "rsa.h"
#include "cmd_des_utils.h"

static void	flush_gnl(int fd)
{
	char	*line;

	if (get_next_line(fd, &line) > 0)
	{
		ft_memdel((void *)&line);
		close(fd);
		get_next_line(fd, &line);
	}
}

static int	read_base64(uint8_t derkey[KEY_MAXLEN], int *len,
	int ret, char *line)
{
	int	valid;

	valid = base64_buffer_parse(line, ret);
	if ((KEY_MAXLEN - *len) < (((valid *  3) / 4) + 3 * !!((valid * 3) % 4)))
	{
		ft_dprintf(2, "ft_ssl: DER key too long\n");
		return (-1);
	}
	*len += base64_decrypt(derkey + *len, line, valid, 0);
	return (0);
}

static int	check_encryption_headers(t_rsa_key *key, int fd, char **line,
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

static int	read_key(uint8_t derkey[KEY_MAXLEN], int fd,
	const char *cmd, t_rsa_key *key)
{
	char	*line;
	char	*footer;
	int		len;
	int		ret;
	int		footer_len;

	footer = key->is_pub ? END_PUB : END_PRIV;
	footer_len = key->is_pub ? END_PUB_LEN : END_PRIV_LEN;
	line = NULL;
	len = 0;
	ret = check_encryption_headers(key, fd, &line, cmd);
	while (!ret && (line || (ret = get_next_line(fd, &line)) > 0))
	{
		ret = ft_strlen(line);
		if (ret == footer_len && !ft_strcmp(footer, line))
			footer = NULL;
		else
			ret = read_base64(derkey, &len, ret, line);
		ft_memdel((void *)&line);
	}
	if (footer)
		ft_dprintf(2, "ft_ssl: %s: invalid footer\n", cmd);
	else if (ret)
		flush_gnl(fd);
	return (footer || ret < 0 ? -1 : len);
}

static int	check_header(int fd, int is_pub)
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

//TODO: parse encryption headers and get IV to decrypt data with des
//before using parse_der_key (IV is also the salt)
int			parse_rsa_key(t_rsa_key *key, const char *inkey,
	const char *cmd)
{
	int		fd;
	int		ret;
	uint8_t	len;
	uint8_t	derkey[KEY_MAXLEN];

	len = 0;
	if ((fd = open(inkey, O_RDONLY)) < 0)
		return (1);
	if ((ret = check_header(fd, key->is_pub)) < 0)
		return (!!ft_dprintf(2, "ft_ssl: %s: get_next_line error\n", cmd));
	else if (ret)
		return (!!ft_dprintf(2, "ft_ssl: %s: invalid header\n", cmd));
	else if (!ret && (ret = read_key(derkey, fd, cmd, key)) < 0)
		return (1);
	else
	{
		len = (uint8_t)ret;
		ret = parse_der_key(key, derkey, len);
	}
	return (ret);
}
