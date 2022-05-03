# ft\_ssl

This program implements functions related to cryptographic security. It takes
data, it hashes it, passes it through a cipher or an encoding function and
returns the result.

<br />
<p align="center">
  <img src="https://github.com/Taiwing/ft_ssl/blob/master/resources/cipher.jpg?raw=true" alt="cipher" style="width: 50%;"/>
</p>

## Setup

```shell
# clone it with the libft submodule
git clone --recurse-submodules https://github.com/Taiwing/ft_ssl
# build it
cd ft_ssl/ && make
# run it
./ft_ssl
```

## Message Digest commands

These commands are hashing algorithms. They implement a one-way compression
function. This means that it is difficult to compute an input from a given
output of the function. Ideally they would be collision-proof, meaning that it
would be extremely rare for two inputs to have the same output, but md5 and
sha256 are known to be broken in that regard. Nowadays they are mostly used to
check file integrity as they are fast and cheap to execute.

### md5

The md5 algorithm takes an arbitrary long input and turns it into a 128bit hash.

```
Usage: md5 [options] [file...]
	-p		echo STDIN to STDOUT and append the checksum to STDOUT
	-q		quiet mode
	-r		reverse the format of the output
	-s val		print the sum of the given string
	-help		print this
```

#### example:

```shell
# run md5 hash on Makefile
./ft_ssl md5 Makefile
# prints --> MD5 (Makefile) = 78f0a46e4140abb6d12afccf3b68bbc7
```

### sha256

The sha256 algorithm takes an arbitrary long input and turns it into a 256bit
hash.

```
Usage: sha256 [options] [file...]
	-p		echo STDIN to STDOUT and append the checksum to STDOUT
	-q		quiet mode
	-r		reverse the format of the output
	-s val		print the sum of the given string
	-help		print this
```

#### example:

```shell
# run sha256 hash with a string
./ft_ssl sha256 -q -s 'Hello sha256!'
# prints --> 5b83d7b46ad7678f9fdd416309e9ff324edb30e424a409f751ebe39c313b05c5
```

## Cipher commands

These commands perform encryption/decryption and encoding/decoding functions.
They take an arbitrary long input and return encoded/crypted data. The input
can be retrieved by performing the inverse operation on the output.

### base64

This is an encoding scheme that simply takes binary data to represent it in a
65 characters format. Each character encodes 6bits of data. So the input is
encoded 3 bytes at a time wich is 3 * 8 = 24 bits of data for 24 / 6 = 4
base64 characters. The first 64 characters in the set are the digits in base
base 64, like '0123456789abcdef' are the digits in base 16, and the last one
is the '=' sign wich is only there to pad the output to make sure its length
is a multiple of 4.

```
Usage: base64 [options]
	-d			decode mode
	-e			encode mode (default)
	-i file			input file
	-o file			output file
	-help			print this
```

The valid base64 digits for this program are (from 0 to 63):
> ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/

#### example:

```shell
# turn stdin data to base64
echo 'toto' | ./ft_ssl base64 -e
# prints --> dG90bwo=
```

### des

DES (for 'Data Encryption System') is symmetric-key cipher for encrypting data.
A key is provided or generated at start. This key will be used for encryption
and decryption of the input data. As for the md commands the input is handled
block per block. Each block is 8 bytes long and is processed via a specific
process block function. That's actually the only difference between the each
version of DES included in this program. Running `./ft_ssl des` is equivalent to
`./ft_ssl des-cbc`.

```
Usage: des [options]
	-a		decode/encode in base64 depending on the encrypt mode
	-d		decrypt mode
	-e		encypt mode (default)
	-i file		input file for message
	-k val		key in hex is the next arguement
	-o file		output file for message
	-p val		password in ascii is the next argument
	-s val		the salt in hex is the next argument
	-v val		initialization vector in hex is the next argument
	-help		print this
```

#### password option:

Encrypt with DES by providing a password. A salt is generated if none is given.
The password string is appended to it and a 56bit key (64bit if counting parity
bits) is generated for the DES algorithm. The salt is prepended to the cipher
text (the output of DES) so only the password is required for decryption.

```shell
# data to be encrypted
echo 'toto' > clear_text_file
# encryption with DES
./ft_ssl des -p 'verySecurePassword' -i clear_text_file -o encrypted_file
# decryption with DES
./ft_ssl des -d -p 'verySecurePassword' -i encrypted_file -o decrypted_file
# diff shows no difference
diff clear_text_file decrypted_file
```

#### key option:

This skips the key generation step by directly giving the key to the DES
algorithm. Since there is no password no salt is generated. The output is simply
the cipher text. An initialization vector can and should be provided for every
process block mode other than ecb (cbc is the default). It will be set to 0
otherwise.

```shell
# data to be encrypted
echo 'super secret stuff' > clear_text_file
# encryption with DES
./ft_ssl des -k 'C0FFEE' -v '1234ABCD' -i clear_text_file -o encrypted_file
# decryption with DES
./ft_ssl des -d -k 'C0FFEE' -v '1234ABCD' -i encrypted_file -o decrypted_file
# diff shows no difference
diff clear_text_file decrypted_file
```

## Standard commands (RSA)

RSA is a public-key cryptosystem. The public key is used to encrypt messages and
can be freely shared whereas the private key is used for decryption and is
supposed to stay secret. The security of the RSA system is based on how
difficult it is to factor the product of two big prime numbers.

An RSA private key is generated as follows:
* select a public exponent _e_ that will be used for decryption
* generate two prime numbers _p_ and _q_
* multiply them to compute the modulus _n_
* compute the private exponent _d_ from _p_, _q_ and _e_

The public key will only hold _n_ and _e_, whereas the private key will also
contain _p_ , _q_ and _d_ which is used for decryption.

#### Encryption:
<p align="center">
  <img src="https://github.com/Taiwing/ft_ssl/blob/master/resources/rsa-encryption-equation-light.png?raw=true" alt="cipher" style="width: 50%;"/>
</p>

#### Decryption:
<p align="center">
  <img src="https://github.com/Taiwing/ft_ssl/blob/master/resources/rsa-decryption-equation-light.png?raw=true" alt="cipher" style="width: 50%;"/>
</p>

An important part of the RSA cryptosytem is the prime numbers generator. It
randomly selects an odd number _n_ which is our potential prime. Then it applies
the Miller-Rabin test algorithm _k_ number of times (_k_ being an arbitrary
pre-selected constant).

The test verifies some mathematical properties on _n_. It computes an odd factor
_d_ which satisfies this formula `n = 2^s * d`. It selects a random integer _a_
which is called the 'base'. From there, it computes `x = a^d % n`. If
_x_ == _n_-1 or _x_ == 1, then the base _a_ is said to be a strong probable
prime to n. Otherwise _a_ is a Miller witness, which means that _n_ cannot be
prime. This test is probabilistic because it can't say that _n_ is prime with
100% certainty, it checks how improbable it is to be composite. This is because
deterministic tests are really expensive to perform, especially for big numbers
which is how the RSA cryptosystem should be used.

### genrsa

Generate an rsa private key from random primes. The key can be of arbitrary size
from 3 to 64 (inclusive) and defaults to 64. The prime numbers are found with
the Miller-Rabin algorithm. This applies a pre-defined number of tests to a
randomly selected integer to probabilistically check if it is prime. The key can
be password protected if needed.

```
Usage: genrsa [options] [keysize]
	-i file		load the file into the random number generator
	-o file		output the key to specified file
	-des		encrypt the output with DES-CBC
	-passout val	output file pass phrase source
	-help		print this

	keysize: The length in bits of the key to be generated.
	Every value from 3 to 64 (inclusive) is valid. keysize
	will be equal to 64 if it is not given.
```

> This command can only generate up to 64 bit rsa keys because of the Miller
> Rabin algorithm. To test the primality of a number modular exponentiation
> needs to be executed many times. This computation itself is basically a
> division loop on big numbers. This can be done fairly fast with C's native
> integer types but it's harder to do with arbitrarily long integers (big ints).

#### example:

```shell
# generate a 64-bit key
./ft_ssl genrsa
```

possible output:
```
Generating RSA private key, 64 bit long modulus (2 primes)
..++++++++++++
..............++++++++++++
e is 65537 (0x010001)
-----BEGIN RSA PRIVATE KEY-----
MEACAQACCQDHoWnL4oGI/QIDAQABAghMszvOJzpBAQIFAP6jLFECBQDIsuLtAgUA
lfb5kQIFAMAPUFECBQDeSP0E
-----END RSA PRIVATE KEY-----
```

> Do not actually use this key for anything ever. A 64-bit key is really weak by
> today standards. Any information encrypted with it can be retrieved without
> direct access to the private key.

### rsa

This command handles public and private key edition. It can modify the input
key, encrypt it and add a password or generate a public key from a private key.
It can also simply print the key in a textual format.

```
Usage: rsa [options]
	-inform val		input format - PEM (already default)
	-outform val		output format - PEM (already default)
	-in file		input file
	-passin val		input file pass phrase source
	-out file		output file
	-passout val		output file pass phrase source
	-des			use des as a cypher
	-text			print the key in text
	-noout			don't print key out
	-modulus		print the RSA key modulus
	-check			verify key consistency
	-pubin			expect a public key in input file
	-pubout			output a public key
	-help			print this

Manually modify the output key:

	These options allow the user to set arbirary values
	for the output key. This works with private and public
	keys. However it will not have any effect to modify
	a private key only value for a public key output.
	Values are in hexadecimal without prefix and case
	agnostic (example: F86cE09dDe).

	-n	hex		set modulus
	-e	hex		set public exponent
	-d	hex		set private exponent
	-p	hex		set first prime
	-q	hex		set second prime
	-exp1	hex		set exponent1
	-exp2	hex		set exponent2
	-coeff	hex		set coefficient
```

> This command can handle keys of any size because no hard computation is
> required on the input it is given. Except for the -check option wich only
> takes keys up to 64 bit (for the same reason as genrsa).

#### generate a public key from a private key:

```shell
# create a private key
./ft_ssl genrsa -o key.priv
# generate its public key
./ft_ssl rsa -in key.priv -pubout -out key.pub
```

key.priv:
```
-----BEGIN RSA PRIVATE KEY-----
MD4CAQACCQCYd7qDu+OTawIDAQABAggjxNr1lhDkaQIFAMrBDJ8CBQDAgfm1AgQz
I7IdAgUAn9aLjQIEd0vWNg==
-----END RSA PRIVATE KEY-----
```

key.pub:
```
-----BEGIN PUBLIC KEY-----
MCQwDQYJKoZIhvcNAQEBBQADEwAwEAIJAJh3uoO745NrAgMBAAE=
-----END PUBLIC KEY-----
```

#### change a key's modulus:

```shell
# set key.pub modulus to 1234abcd
./ft_ssl rsa -in key.pub -pubin -n '1234abcd' -out new-key.pub
```

new-key.pub:
```
-----BEGIN PUBLIC KEY-----
MB8wDQYJKoZIhvcNAQEBBQADDgAwCwIEEjSrzQIDAQAB
-----END PUBLIC KEY-----
```

#### show a key in textual format:

```shell
# print new-key.pub in clear text
./ft_ssl rsa -in new-key.pub -pubin -text -noout
```

output:
```
RSA Public-Key: (29 bit)
Modulus: 305441741 (0x1234abcd)
Exponent: 65537 (0x10001)
```

#### check private key:

```shell
# check key.priv
./ft_ssl rsa -in key.priv -check -noout
# prints --> RSA key ok
# set key.priv modulus to 1234abcd
./ft_ssl rsa -in key.priv -n '1234abcd' -out new-key.priv
# check new-key.priv
./ft_ssl rsa -in new-key.priv -check -noout
# prints --> RSA key error: n does not equal p q
```

### rsautl

Use rsa keys to encrypt and decrypt data. This actually applies the encryption
and decryption formulas on the input it is given. Data can be encrypted with a
public or a private key but can only be decrypted with a private key.

```
Usage: rsautl [options]
	-in file		input file
	-out file		output file
	-inkey val		input key
	-pubin			input is an RSA public
	-encrypt		encrypt with public key
	-decrypt		decrypt with private key
	-hexdump		hex dump output
	-raw			no padding (already default)
	-passin val		input file pass phrase source
	-help			print this
```

> This command can handle keys up to 1024 bits. Decryption gets pretty slow from
> there and it only gets longer and harder after that. Also it does not support
> any padding scheme because it mostly deals with 64 bit keys for which padding
> cannot be implemented (the padding overhead is actually bigger than the data
> it can handle). So input data needs to be rigorously equal in length to the
> key in bytes (ex: 1024/8=128 bytes for a 1024 bit key) and also be less than
> the modulus of the key value-wise (it would get cropped otherwise).

#### encrypt and decrypt data:

```shell
# create an 8 byte long input file
echo 'abcd123' > plaintext
# encrypt it with a 64 bit key
./ft_ssl rsautl -in plaintext -pubin -inkey key.pub -encrypt -o ciphertext
# decrypt with the private key
./ft_ssl rsautl -in ciphertext -inkey key.priv -decrypt
# prints --> abcd123
```
