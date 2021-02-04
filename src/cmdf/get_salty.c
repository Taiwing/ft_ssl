/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_salty.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:15:53 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/04 19:27:22 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "readfile.h"
#include "cmd_des_utils.h"

static int	read_salt(uint64_t *salt, const char *cmd, t_cmdopt *opt)
{
	int			rd;
	char		magic[8];
	const char	*input_file;

	input_file = opt[CC_INPUT].value;
	if ((rd = des_readfile(input_file, magic, 8, opt)) < 0)
		print_readfile_error(cmd, input_file);
	else if (rd < 8 || ft_strncmp(magic, "Salted__", 8))
		return (!!ft_dprintf(2, "ft_ssl: %s: bad magic number\n", cmd));
	else if ((rd = des_readfile(input_file, (char *)salt,
		sizeof(uint64_t), opt)) < 0)
		print_readfile_error(cmd, input_file);
	else if (rd < 8)
		return (!!ft_dprintf(2, "ft_ssl: %s: bad salt\n", cmd));
	else if (rd == sizeof(uint64_t))
		ft_memswap((void *)salt, sizeof(uint64_t));
	return (rd != 8);
}

static int	generate_salt(uint64_t *salt, const char *cmd)
{
	int	fd;
	int	rd;

	if ((fd = open("/dev/urandom", O_RDONLY)) < 0)
		return (!!ft_dprintf(2, "ft_ssl: %s: open: %s\n",
			cmd, strerror(errno)));
	if ((rd = read(fd, (void *)salt, sizeof(uint64_t))) < 0)
		ft_dprintf(2, "ft_ssl: %s: read: %s\n", cmd, strerror(errno));
	else if (rd != sizeof(uint64_t))
		ft_dprintf(2, "ft_ssl: %s: read: could not read %d bytes\n",
			cmd, sizeof(uint64_t));
	close(fd);
	return (rd != sizeof(uint64_t));
}

int			write_salt(int fd, t_des_ctx *ctx, t_cmdopt *opt)
{
	int			ret;
	uint64_t	salt;

	ret = 0;
	salt = ctx->salt;
	if (opt[CC_ENCRYPT].is_set && opt[CC_PASSWORD].is_set)
	{
		if ((ret = des_writefile(fd, "Salted__", 8, opt)) != -1)
		{
			ft_memswap((void *)&salt, sizeof(uint64_t));
			ret = des_writefile(fd, (char *)&salt, sizeof(uint64_t), opt);
		}
	}
	if (ret == -1 && fd > 1)
		close(fd);
	return (ret == -1);
}

int			get_salty(t_des_ctx *ctx, const t_command *cmd, t_cmdopt *opt)
{
	if (opt[CC_ENCRYPT].is_set && opt[CC_SALT].is_set)
		return (parse_hex(&ctx->salt, opt[CC_SALT].value, cmd->name));
	else if (opt[CC_ENCRYPT].is_set)
		return (generate_salt(&ctx->salt, cmd->name));
	else
		return (read_salt(&ctx->salt, cmd->name, opt));
}
