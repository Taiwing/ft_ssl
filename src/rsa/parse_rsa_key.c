/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rsa_key.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 17:36:51 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/20 09:19:34 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <errno.h>
#include "base64.h"
#include "libft.h"
#include "rsa.h"

static void		flush_gnl(int fd)
{
	char	*line;

	if (get_next_line(fd, &line) > 0)
	{
		ft_memdel((void *)&line);
		if (fd > 0)
			close(fd);
		get_next_line(fd, &line);
	}
}

static int		read_base64(uint8_t der[KEY_BUFLEN], int *len,
	int ret, char *line)
{
	int	valid;

	valid = base64_buffer_parse(line, ret);
	if ((int)(KEY_BUFLEN - *len)
		< (((valid *  3) / 4) + 3 * !!((valid * 3) % 4)))
	{
		ft_dprintf(2, "ft_ssl: DER key too long\n");
		return (-1);
	}
	*len += base64_decrypt(der + *len, line, valid, 0);
	return (0);
}

static int		read_key(uint8_t der[KEY_BUFLEN], int fd,
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
			ret = read_base64(der, &len, ret, line);
		ft_memdel((void *)&line);
	}
	if (footer)
		ft_dprintf(2, "ft_ssl: %s: invalid footer\n", cmd);
	else if (ret)
		flush_gnl(fd);
	return (footer || ret < 0 ? -1 : len);
}

int				parse_rsa_key(t_rsa_key *key, const char *cmd, t_des_getkey *gk)
{
	int			fd;
	int			ret;
	uint64_t	len = 0;
	uint8_t		der[KEY_BUFLEN];

	if ((fd = gk->inkey ? open(gk->inkey, O_RDONLY) : 0) < 0)
		return (1);
	if ((ret = check_header(fd, key->is_pub)) < 0)
		return (!!ft_dprintf(2, "ft_ssl: %s: get_next_line error\n", cmd));
	else if (ret)
		return (!!ft_dprintf(2, "ft_ssl: %s: invalid header\n", cmd));
	else if (!ret && (ret = read_key(der, fd, cmd, key)) < 0)
		return (1);
	else
	{
		len = (uint64_t)ret;
		ret = 0;
		if (key->is_enc)
			ret = rsa_des_getkey(&key->des, cmd, gk)
				|| rsa_des_decrypt(der, &len, &key->des, cmd);
		if (!ret)
			ret = decode_der_key(key, der, len);
		key->size = !ret && BINT_LEN(key->n) ? bintlog2(key->n) : key->size;
	}
	return (ret);
}
