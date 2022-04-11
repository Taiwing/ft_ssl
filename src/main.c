/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 16:10:24 by yforeau           #+#    #+#             */
/*   Updated: 2022/04/11 06:56:25 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

int	main(int argc, char **argv)
{
	int	ret;

	ret = EXIT_SUCCESS;
	ft_exitmsg("ft_ssl");
	if (argc == 1)
		input_cmd();
	else
		ret = exec_cmd(argv + 1);
	ft_exit(ret, NULL);
	return (EXIT_FAILURE);
}
