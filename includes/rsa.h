/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:39:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/13 12:22:43 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RSA_H
# define RSA_H

# include "des.h"
# include "bint.h"

/*
** Util macros
*/

# define MC_STRLEN(s)		(sizeof(s) - 1)
//TODO: move this to libft.h as it could be useful everywhere tbh
# define		NBITS2(n)			((n & 2) ? 1 : 0)
# define		NBITS4(n)			((n & 0xC) ? (2 + NBITS2(n >> 2))\
		: (NBITS2(n)))
# define		NBITS8(n)			((n & 0xF0) ? (4 + NBITS4(n >> 4))\
		: (NBITS4(n)))
# define		NBITS16(n)			((n & 0xFF00) ? (8 + NBITS8(n >> 8))\
		: (NBITS8(n)))
# define		NBITS32(n)			((n & 0xFFFF0000) ? (16 + NBITS16(n >> 16))\
		: (NBITS16(n)))
# define		NBITS64(n)			((n & 0xFFFFFFFF00000000) ?\
		(32 + NBITS32(n >> 32)) : (NBITS32(n)))
# define		NBITS(n)			(n == 0 ? 0 : NBITS64(n) + 1)

/*
** RSA constants
*/

# define RSA_KEY_SIZE_MAX		1024
# define GENRSA_KEY_SIZE_MAX	64
# define E_VALUE				0x00010001

# define MODS_SIZE_MAX			(RSA_KEY_SIZE_MAX + 1)
# define FACTS_SIZE_MAX			((RSA_KEY_SIZE_MAX / 2) + 1)
# define DER_INT_LEN_SIZE(l)	(l < 128 ? 1\
		: (NBITS(l) / 4) + !!(NBITS(l) % 4) + 1)
# define DER_SEQ_SIZE_MAX		((3 * MODS_SIZE_MAX) \
		+ (5 * FACTS_SIZE_MAX) + RSA_KEY_BINTS\
		+ (3 * DER_INT_LEN_SIZE(MODS_SIZE_MAX))\
		+ (5 * DER_INT_LEN_SIZE(FACTS_SIZE_MAX)))
# define KEY_BUFLEN				(DER_SEQ_SIZE_MAX + 3)
# define RSA_KEY_BINTS			8
# define RSA_PUB_BINTS			2

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

//TODO: delete these lines (from DER_SEQ to DER_LEN_III)
# define DER_SEQ			"\x30\xFF\x30\x0D\x06\x09\x2A\x86"\
							"\x48\x86\xF7\x0D\x01\x01\x01\x05"\
							"\x00\x03\xFF\x00\x30\xFF"
# define DER_SEQ_MAXLEN		(MC_STRLEN(DER_SEQ))
# define DER_SEQ_MINLEN		0x02
# define DER_LEN_I			0x01
# define DER_LEN_II			0x12
# define DER_LEN_III		0x15

# define DER_OID_SEQ		"\x30\x0D\x06\x09\x2A\x86\x48\x86"\
							"\xF7\x0D\x01\x01\x01\x05\x00"
# define DER_OID_SEQ_LEN	(MC_STRLEN(DER_OID_SEQ))

# define RETARDED_OPENSSL_PASSWORD_MODE_IS_ON
# define RETARDED_PREFIX		"pass:"
# define RETARDED_PREFIX_LEN	(MC_STRLEN(RETARDED_PREFIX))
# define MIN_PASSLEN			4

# define RSA_KEY_ERR		"RSA key error: "

/*
** s_rsa_key_64: rsa key structure (represent 64-bit private and public key)
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
** size: size of the key (n) in bits
** des: des context if the key is encrypted
*/

typedef struct	s_rsa_key_64
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
	uint32_t	size;
	t_des_ctx	des;
}				t_rsa_key_64;

/*
** s_rsa_key: rsa key structure (represent private and public key)
**
** n: p * q (modulus)
** e: encryption, 1 < e < T(n) and gcd(e, T(n)) == 1 (public exponent)
** d: decryption, modinv(e, T(n))
** p: random unsigned prime (p != q)
** q: random unsigned prime (p != q)
** exp1: d mod (p - 1)
** exp2: d mod (q - 1)
** coeff: modinv(q, p)
** is_pub: boolean, the key is public if true
** is_enc: boolean, the key is encrypted if true
** size: size of the key (n) in bits
** des: des context if the key is encrypted
*/

typedef struct	s_rsa_key
{
	uint32_t	rsa_bints[RSA_KEY_BINTS][BINT_SIZE_DEF];
	t_bint		n;
	t_bint		e;
	t_bint		d;
	t_bint		p;
	t_bint		q;
	t_bint		exp1;
	t_bint		exp2;
	t_bint		coeff;
	int			is_pub;
	int			is_enc;
	uint32_t	size;
	t_des_ctx	des;
}				t_rsa_key;

enum			e_rsa_key_bints {
	RSA_BINT_N = 0,
	RSA_BINT_E,
	RSA_BINT_D,
	RSA_BINT_P,
	RSA_BINT_Q,
	RSA_BINT_EXP1,
	RSA_BINT_EXP2,
	RSA_BINT_COEFF,
};


# define		RSA_KEY_DEFAULT		{\
	{\
		BINT_DEFAULT(0),\
		BINT_DEFAULT(0),\
		BINT_DEFAULT(0),\
		BINT_DEFAULT(0),\
		BINT_DEFAULT(0),\
		BINT_DEFAULT(0),\
		BINT_DEFAULT(0),\
		BINT_DEFAULT(0),\
	},\
	NULL,\
	NULL,\
	NULL,\
	NULL,\
	NULL,\
	NULL,\
	NULL,\
	NULL,\
	0,\
	0,\
	0,\
	{ 0, 0, 0, 0, { 0 }, 0, NULL, 0 },\
}

# define		INIT_RSA_KEY(key) \
	key.n = key.rsa_bints[RSA_BINT_N];\
	key.e = key.rsa_bints[RSA_BINT_E];\
	key.d = key.rsa_bints[RSA_BINT_D];\
	key.p = key.rsa_bints[RSA_BINT_P];\
	key.q = key.rsa_bints[RSA_BINT_Q];\
	key.exp1 = key.rsa_bints[RSA_BINT_EXP1];\
	key.exp2 = key.rsa_bints[RSA_BINT_EXP2];\
	key.coeff = key.rsa_bints[RSA_BINT_COEFF];

/*
** rsa functions
*/

uint64_t	*der_decode_uint64(uint64_t *dst, uint8_t *der,
	uint8_t *i, uint8_t len);
int			parse_der_key(t_rsa_key_64 *key, uint8_t *der, uint8_t len);
void		encode_der_key(uint8_t *der, uint64_t *len, t_rsa_key *key);
int			print_rsa_key(int fd, t_rsa_key *key,
	const char *cmd, t_des_getkey *gk);
int			rsa_keygen(t_rsa_key_64 *key);
int			parse_rsa_key_64(t_rsa_key_64 *key,
	const char *cmd, t_des_getkey *gk);
void		rsa_hexdump(int fd, uint8_t *data, size_t len);
int			check_header(int fd, int is_pub);
int			check_encryption_headers(t_rsa_key_64 *key, int fd, char **line,
	const char *cmd);
int			rsa_des_getkey(t_des_ctx *des, const char *cmd, t_des_getkey *gk);
int			rsa_des_decrypt(uint8_t *der, uint8_t *len,
	t_des_ctx *des, const char *cmd);
void		rsa_des_encrypt(uint8_t *der, uint64_t *len, t_des_ctx *des);
int			rsa_check_key_size(t_rsa_key *key);
int			rsa_check_key_bint(int outfd, t_rsa_key *key, const char *cmd);
int			rsa_check_key_64(int outfd, t_rsa_key_64 *key);

//TODO: remove this (should only be in cmd_genrsa)
int			rsa_key_64_to_bint(t_rsa_key *dst, t_rsa_key_64 *src);

#endif
