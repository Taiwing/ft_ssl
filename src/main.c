/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 16:10:24 by yforeau           #+#    #+#             */
/*   Updated: 2020/12/29 19:00:41 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_opt	long_options[] = {
	{"add",     REQUIRED_ARG, NULL, 0 },
	{"append",  NO_ARG, NULL, 0 },
	{"delete",  REQUIRED_ARG, NULL, 0 },
	{"verbose", NO_ARG, NULL, 0 },
	{"create",  REQUIRED_ARG, NULL, 0 },
	{"file",    REQUIRED_ARG, NULL, 0 },
	{NULL,      NO_ARG, NULL, 0 }
};

int		main(int argc, char **argv)
{
	int c;
	int digit_optind = 0;
	int option_index = 0;
	t_optdata opt;

	ft_bzero((void *)&opt, sizeof(t_optdata));
	init_getopt(&opt, "", long_options, &option_index);
	while (1) {
		int this_option_optind = opt.optind ? opt.optind : 1;
		option_index = 0;
		c = ft_getopt_long_only(argc, argv, &opt);
		if (c == -1)
			break;

		switch (c) {
			case 0:
				ft_printf("option %s", long_options[option_index].name);
				if (opt.optarg)
					ft_printf(" with arg %s", opt.optarg);
				ft_printf("\n");
				break;

			case '0':
			case '1':
			case '2':
				if (digit_optind != 0 && digit_optind != this_option_optind)
					ft_printf("digits occur in two different argv-elements.\n");
				digit_optind = this_option_optind;
				ft_printf("option %c\n", c);
				break;

			case 'a':
				ft_printf("option a\n");
				break;

			case 'b':
				ft_printf("option b\n");
				break;

			case 'c':
				ft_printf("option c with value '%s'\n", opt.optarg);
				break;

			case 'd':
				ft_printf("option d with value '%s'\n", opt.optarg);
				break;

			case '?':
				break;

			default:
				ft_printf("?? getopt returned character code 0%o ??\n", c);
		}
	}

	if (opt.optind < argc) {
		ft_printf("non-option ARGV-elements: ");
		while (opt.optind < argc)
			ft_printf("%s ", argv[opt.optind++]);
		ft_printf("\n");
	}

	exit(EXIT_SUCCESS);
}
