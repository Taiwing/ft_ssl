/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:39:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/02/12 18:08:46 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RSA_H
# define RSA_H

# define MC_STRLEN(s)		(sizeof(s) - 1)

# define E_VALUE			0x00010001
# define PRIVKEY_MAXLEN		87
# define PUBKEY_MAXLEN		87
# define KEY_MAXLEN			87
# define KEY_BUFLEN			128
# define BEGIN_PRIV			"-----BEGIN RSA PRIVATE KEY-----"
# define END_PRIV			"-----END RSA PRIVATE KEY-----"
# define BEGIN_PUB			"-----BEGIN PUBLIC KEY-----"
# define END_PUB			"-----END PUBLIC KEY-----"
# define BEGIN_PRIV_LEN		(MC_STRLEN(BEGIN_PRIV))
# define END_PRIV_LEN		(MC_STRLEN(END_PRIV))
# define BEGIN_PUB_LEN		(MC_STRLEN(BEGIN_PUB))
# define END_PUB_LEN		(MC_STRLEN(END_PUB))

/*
** s_rsa_key: rsa key structure (represent private and public key)
**
** n: p * q (modulus)
** e: encryption, 1 < e < T(n) and gcd(e, T(n)) == 1 (public exponent)
** d: decryption, modinv(e, T(n))
** p: random 32bit unsigned prime (p != q)
** q: random 32bit unsigned prime (p != q)
** exp1: d mod (p -1)
** exp2: d mod (q -1)
** coeff: modinv(q, p)
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
}				t_rsa_key;

void	der_encode_uint64(uint8_t *derkey, uint8_t *len, uint64_t n);
int		print_rsa_key(int fd, t_rsa_key *key);
int		rsa_keygen(t_rsa_key *key);
int		parse_rsa_key(t_rsa_key *key, const char *inkey,
	int is_pub, const char *cmd);

#endif
