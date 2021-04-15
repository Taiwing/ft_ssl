/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:39:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/15 17:58:31 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RSA_H
# define RSA_H

# include "des.h"

# define MC_STRLEN(s)		(sizeof(s) - 1)

# define E_VALUE			0x00010001
# define KEY_BUFLEN			128
# define RSA_DER_INT_MAXLEN	9
# define BEGIN_PRIV			"-----BEGIN RSA PRIVATE KEY-----"
# define END_PRIV			"-----END RSA PRIVATE KEY-----"
# define BEGIN_PUB			"-----BEGIN PUBLIC KEY-----"
# define END_PUB			"-----END PUBLIC KEY-----"
# define PROC_TYPE			"Proc-Type: 4,ENCRYPTED"
# define DEK_INFO			"DEK-Info: DES-CBC,"
# define BEGIN_PRIV_LEN		(MC_STRLEN(BEGIN_PRIV))
# define END_PRIV_LEN		(MC_STRLEN(END_PRIV))
# define BEGIN_PUB_LEN		(MC_STRLEN(BEGIN_PUB))
# define END_PUB_LEN		(MC_STRLEN(END_PUB))
# define PROC_TYPE_LEN		(MC_STRLEN(PROC_TYPE))
# define DEK_INFO_LEN		(MC_STRLEN(DEK_INFO))
# define IV_LEN				16
# define RSABUF_SIZE		(8+1)

# define DER_SEQ			"\x30\xFF\x30\x0D\x06\x09\x2A\x86"\
							"\x48\x86\xF7\x0D\x01\x01\x01\x05"\
							"\x00\x03\xFF\x00\x30\xFF"
# define DER_SEQ_MAXLEN		(MC_STRLEN(DER_SEQ))
# define DER_SEQ_MINLEN		0x02
# define DER_LEN_I			0x01
# define DER_LEN_II			0x12
# define DER_LEN_III		0x15

# define RETARDED_OPENSSL_PASSWORD_MODE_IS_ON
# define RETARDED_PREFIX		"pass:"
# define RETARDED_PREFIX_LEN	(MC_STRLEN(RETARDED_PREFIX))
# define MIN_PASSLEN			4

# define RSA_KEY_ERR		"RSA key error: "

/*
** s_rsa_key: rsa key structure (represent private and public key)
**
** n: p * q (modulus)
** e: encryption, 1 < e < T(n) and gcd(e, T(n)) == 1 (public exponent)
** d: decryption, modinv(e, T(n))
** p: random 32bit unsigned prime (p != q)
** q: random 32bit unsigned prime (p != q)
** exp1: d mod (p - 1)
** exp2: d mod (q - 1)
** coeff: modinv(q, p)
** is_pub: boolean, the key is public if true
** is_enc: boolean, the key is encrypted if true
** des: des context if the key is encrypted
*/

typedef struct	s_rsa_key
{
	uint64_t	n;
	uint64_t	e;
	uint64_t	d;
	uint64_t	p;
	uint64_t	q;
	uint64_t	exp1;
	uint64_t	exp2;
	uint64_t	coeff;
	int			is_pub;
	int			is_enc;
	t_des_ctx	des;
}				t_rsa_key;

/*
** rsa functions
*/

uint64_t	*der_decode_uint64(uint64_t *dst, uint8_t *der,
	uint8_t *i, uint8_t len);
void		der_encode_uint64(uint8_t *der, uint8_t *len, uint64_t n);
int			parse_der_key(t_rsa_key *key, uint8_t *der, uint8_t len);
void		encode_der_key(uint8_t *der, uint8_t *len, t_rsa_key *key);
int			print_rsa_key(int fd, t_rsa_key *key,
	const char *cmd, t_des_getkey *gk);
int			rsa_keygen(t_rsa_key *key);
int			parse_rsa_key(t_rsa_key *key, const char *cmd, t_des_getkey *gk);
void		rsa_hexdump(int fd, uint8_t *data, size_t len);
int			check_header(int fd, int is_pub);
int			check_encryption_headers(t_rsa_key *key, int fd, char **line,
	const char *cmd);
int			rsa_des_getkey(t_rsa_key *key, const char *cmd, t_des_getkey *gk);
int			rsa_des_decrypt(uint8_t *der, uint8_t *len,
	t_rsa_key *key, const char *cmd);
void		rsa_des_encrypt(uint8_t *der, uint8_t *len, t_rsa_key *key);

#endif
