/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:53:03 by yforeau           #+#    #+#             */
/*   Updated: 2021/01/26 19:33:22 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_H
# define BASE64_H

# define BASE64_INBUF_SIZE	(16 * 3)
# define BASE64_OUTBUF_SIZE	(16 * 4)

int	base64_encrypt(char out[BASE64_OUTBUF_SIZE], char in[BASE64_INBUF_SIZE],
	int inlen);
int	base64_file_encrypt(int outfd, const char *input_file, const char *cmd);

#endif
