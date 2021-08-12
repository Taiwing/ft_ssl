/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_value_options.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 11:52:16 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/12 19:33:28 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RSA_VALUE_OPTIONS_H
# define RSA_VALUE_OPTIONS_H

# include "rsa.h"
# include "options.h"
	
void	rsa_value_options(t_rsa_key *key, t_cmdopt *opt, const char *cmd);

#endif
