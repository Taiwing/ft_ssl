/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:38:30 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 17:59:25 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

/* TODO: do some real parsing beside field splitting */
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
		ft_exit("read_input: get_next_line error\n", EXIT_FAILURE);
	else if (!ret)
		ft_exit(NULL, EXIT_SUCCESS);
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
