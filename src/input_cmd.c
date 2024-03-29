/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:38:30 by yforeau           #+#    #+#             */
/*   Updated: 2022/04/11 06:51:34 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

static char **parse_input(char *line)
{
	return (ft_split_whitespaces(line));
}

static char	**read_input(void)
{
	int		ret;
	char	*line;
	char	**argv;

	line = NULL;
	if ((ret = get_next_line(0, &line)) < 0)
		ft_exit(EXIT_FAILURE, "%s: get_next_line error\n", __func__);
	else if (!ret)
		ft_exit(EXIT_SUCCESS, NULL);
	argv = parse_input(line);
	ft_memdel((void **)&line);
	return (argv);
}

void		input_cmd(void)
{
	char	**argv;

	while (1)
	{
		ft_printf("ft_ssl> ");
		argv = read_input();
		exec_cmd(argv);
		ft_wtfree(argv);
	}
}
