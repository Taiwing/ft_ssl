# ft\_ssl

This program implements functions related to cryptographic security. It takes
data, it hashes it, passes it through a cypher or an encoding function and
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

This is an encoding scheme that simply takes binary data and represents it in
a 65 characters format. Each character encodes 6bits of data. So the input is
encoded 3 bytes at a time wich is 3 * 8 = 24 bits of data and 24 / 6 = 4
base64 characters. The first 64 characters in the set are the digits in base
base 64, like '0123456789abcdef' are the digits in base 16, and the last one
is the '=' sign with is only there to pad the output to make sure its length
is a multiple of 4.

The valid base64 digits for this program are (from 0 to 63):
> ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/

```
Usage: base64 [options]
	-d			decode mode
	-e			encode mode (default)
	-i file			input file
	-o file			output file
	-help			print this
```

#### example:

```shell
# turn stdin data to base64
echo 'toto' | ./ft_ssl base64 -e
# prints --> dG90bwo=
```
