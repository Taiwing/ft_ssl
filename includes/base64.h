/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 16:53:03 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/02 18:46:17 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_H
# define BASE64_H

# define BASE64_INBUF_SIZE	(16 * 3)
# define BASE64_OUTBUF_SIZE	(16 * 4)

# include <stddef.h>

int	base64_encrypt(char *out, unsigned char *in, int inlen);
int	base64_decrypt(unsigned char *out, char *in, int inlen, int flush);
int	base64_file_encrypt(int outfd, const char *input_file, const char *cmd);
int	base64_file_decrypt(int outfd, const char *input_file, const char *cmd);

int	base64_buffer_parse(char *buf, int len);
int	base64_readfile(const char *file_name, char *buf,
	size_t bufsize, int flush);

#endif
