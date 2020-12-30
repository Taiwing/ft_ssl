/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 16:10:24 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/30 16:27:56 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

int		main(int argc, char **argv)
{
	ft_exitmsg("ft_ssl");
	if (argc == 1)
		input_cmd();
	else
		return (exec_cmd(argv + 1));
	return (EXIT_SUCCESS);
}
