/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_genrsa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 23:59:02 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/07 00:20:38 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include "commands.h"
#include "prime.h"

int	cmd_genrsa(const t_command *cmd, t_cmdopt *opt, char **args)
{
	int	ret;
	int	outfd;

	(void)args;
	ret = 0;
	outfd = 1;
	if (opt[GENRSA_INPUT].is_set && !fill_rand_buf(opt[GENRSA_INPUT].value, 0))
		return (!!ft_printf("ft_ssl: %s: %s\n", cmd->name, strerror(errno)));
	if (opt[GENRSA_OUTPUT].is_set
		&& (outfd = output_option(opt[GENRSA_OUTPUT].value, cmd->name)) < 0)
		ret = 1;
	if (outfd > 1)
		close(outfd);
	fill_rand_buf(NULL, 1);
	return (ret);
}
