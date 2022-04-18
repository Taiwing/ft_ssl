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

### DES

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

### RSA

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

